# coding=utf-8
r"""Quantized convolution modules."""

from typing import Optional, List, TypeVar

import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.nn.intrinsic as nni
import torch.nn.intrinsic.qat as nniqat
import torch
import torch.ao.nn.intrinsic
import torch.nn.intrinsic.qat
import torch.nn.functional as F
import torch.ao.nn.quantized as nnq


from torch._ops import ops
from torch.nn.common_types import _size_1_t
from torch.nn.modules.utils import _single, _pair, _triple
from torch.nn.utils import fuse_conv_bn_weights

from torch.ao.nn.quantized.modules.utils import _quantize_weight, WeightedQuantizedModule
import copy
import torch.nn.intrinsic.quantized as nniq
import torch.nn.intrinsic.quantized.dynamic as nniqd
import torch.ao.nn.quantized as nnq
import torch.ao.nn.quantized.reference as nnqr
import torch.ao.nn.quantized.dynamic as nnqd
import torch.ao.nn.qat as nnqat
import torch.ao.nn.qat.dynamic as nnqatd

from typing import Optional, Union, Dict, Set, Callable, Any

# Because `torch.ao.nn` uses lazy imports, we need to make
# sure we import the contents explicitly here.
import torch.ao.nn.sparse
import torch.ao.nn as ao_nn
from torch.ao.quantization.stubs import QuantStub, DeQuantStub
from torch.ao.quantization.fake_quantize import (
    default_fixed_qparams_range_0to1_fake_quant,
    default_fixed_qparams_range_neg1to1_fake_quant,
)
from torch.ao.quantization.utils import get_combined_dict
from torch.nn.utils.parametrize import type_before_parametrizations


_SUPPORTED_PADDING = {
    'zeros',
    'reflect'
}


def _reverse_repeat_padding(padding: List[int]) -> List[int]:
    _reversed_padding_repeated_twice: List[int] = []
    N = len(padding)
    for idx in range(N):
        for _ in range(2):
            _reversed_padding_repeated_twice.append(padding[N - idx - 1])
    return _reversed_padding_repeated_twice


class _slsq_ConvNd(WeightedQuantizedModule):
    def __init__(self, in_channels, out_channels, kernel_size, stride=1,
                 padding=0, dilation=1, groups=1, bias=True,
                 padding_mode='zeros', device=None, dtype=None):
        # All subclasses have this signature - See PR #49702s
        raise NotImplementedError

    def _init(self, in_channels, out_channels, kernel_size, stride,
              padding, dilation,
              transposed, output_padding,
              groups, bias,
              padding_mode='zeros',
              device=None,
              dtype=None) -> None:
        factory_kwargs = {'device': device, 'dtype': dtype}
        super(_slsq_ConvNd, self).__init__()

        if in_channels % groups != 0:
            raise ValueError('in_channels must be divisible by groups')
        if out_channels % groups != 0:
            raise ValueError('out_channels must be divisible by groups')
        self.in_channels = in_channels
        self.out_channels = out_channels
        self.kernel_size = kernel_size
        self.stride = stride
        self.padding = padding
        self.dilation = dilation
        self.transposed = transposed
        self.output_padding = output_padding
        self.groups = groups
        if padding_mode not in _SUPPORTED_PADDING:
            raise ValueError("'padding_mode' {} is not supported by quantized convolution".format(padding_mode))
        self.padding_mode = padding_mode
        # Initialize as NCHW. set_weight will internally transpose to NHWC.
        if self.transposed:
            weight_shape = [in_channels, out_channels // self.groups]
        else:
            weight_shape = [out_channels, in_channels // self.groups]
        qweight = torch._empty_affine_quantized(
            weight_shape + list(kernel_size),
            scale=1, zero_point=0, dtype=torch.qint8,
            **{k: v for k, v in factory_kwargs.items() if k != 'dtype'})
        bias_float = (
            torch.zeros(out_channels, dtype=torch.float,
                        **{k: v for k, v in factory_kwargs.items() if k != 'dtype'}) if bias else None)

        self.set_weight_bias(qweight, bias_float)
        self.scale = 1.0
        self.zero_point = 0

    def set_weight_bias(self, qweight, bias_float):
        raise NotImplementedError

    def bias(self):
        raise NotImplementedError

    def _weight_bias(self):
        raise NotImplementedError

    def extra_repr(self):
        s = ('{in_channels}, {out_channels}, kernel_size={kernel_size}'
             ', stride={stride}, scale={scale}, zero_point={zero_point}')
        if self.padding != (0,) * len(self.padding):
            s += ', padding={padding}'
        if self.dilation != (1,) * len(self.dilation):
            s += ', dilation={dilation}'
        if self.output_padding != (0,) * len(self.output_padding):
            s += ', output_padding={output_padding}'
        if self.groups != 1:
            s += ', groups={groups}'
        if self.bias() is None:
            s += ', bias=False'
        return s.format(**self.__dict__)

    # ===== Serialization methods =====
    # The special consideration here is that we have to unpack the weights into
    # their regular QTensor form for serialization. Packed weights should not
    # live outside the process in which they were created, rather they should be
    # derived from the QTensor weight.
    #   self
    #   |--- weight : Tensor
    #   |--- bias : Tensor
    #
    # TODO: maybe change to this when https://github.com/pytorch/pytorch/pull/32958 is landed
    #   self
    #   |--- _packed_params : Conv2dPackedParamsBase or Conv3dPackedParamsBase
    def _save_to_state_dict(self, destination, prefix, keep_vars):
        super(_slsq_ConvNd, self)._save_to_state_dict(destination, prefix, keep_vars)
        (w, b) = self._weight_bias()
        destination[prefix + 'weight'] = w
        destination[prefix + 'bias'] = b
        destination[prefix + 'scale'] = torch.tensor(self.scale)
        destination[prefix + 'zero_point'] = torch.tensor(self.zero_point)

    @torch.jit.export
    def __getstate__(self):
        (w, b) = self._weight_bias()
        return (
            self.in_channels,
            self.out_channels,
            self.kernel_size,
            self.stride,
            self.padding,
            self.dilation,
            self.transposed,
            self.output_padding,
            self.groups,
            self.padding_mode,
            w,
            b,
            self.scale,
            self.zero_point,
            self.training
        )

    # ===== Deserialization methods =====
    # Counterpart to the serialization methods, we must pack the serialized
    # QTensor weight into its packed format for use by the FBGEMM ops.
    def _load_from_state_dict(self, state_dict, prefix, local_metadata, strict,
                              missing_keys, unexpected_keys, error_msgs):
        self.set_weight_bias(
            state_dict[prefix + 'weight'], state_dict[prefix + 'bias'])
        state_dict.pop(prefix + 'weight')
        state_dict.pop(prefix + 'bias')
        self.scale = float(state_dict[prefix + 'scale'])
        state_dict.pop(prefix + 'scale')
        self.zero_point = int(state_dict[prefix + 'zero_point'])
        state_dict.pop(prefix + 'zero_point')
        super(_slsq_ConvNd, self)._load_from_state_dict(
            state_dict, prefix, local_metadata, False, missing_keys,
            unexpected_keys, error_msgs)

    @torch.jit.export
    def __setstate__(self, state):
        self.in_channels = state[0]
        self.out_channels = state[1]
        self.kernel_size = state[2]
        self.stride = state[3]
        self.padding = state[4]
        self.dilation = state[5]
        self.transposed = state[6]
        self.output_padding = state[7]
        self.groups = state[8]
        self.padding_mode = state[9]
        self.set_weight_bias(state[10], state[11])
        self.scale = state[12]
        self.zero_point = state[13]
        self.training = state[14]

    def __deepcopy__(self, memo):
        new_instance = type(self).__new__(type(self))
        torch.nn.Module.__init__(new_instance)
        state = self.__getstate__()
        new_instance.__setstate__(state)
        return new_instance

    def __copy__(self):
        return self.__deepcopy__({})

    @classmethod
    def get_qconv(cls, mod, activation_post_process, weight_post_process=None):
        r"""Creates a qconv object and returns it.
        """
        if weight_post_process is None:
            weight_post_process = mod.qconfig.weight()
        weight_post_process(mod.weight)
        assert weight_post_process.dtype == torch.qint8, \
            'Weight observer must have a dtype of qint8'
        qweight = _quantize_weight(weight_post_process(mod.weight).float(), weight_post_process)
        # the __init__ call used is the one from derived classes and not the one from _ConvNd
        qconv = cls(mod.in_channels, mod.out_channels, mod.kernel_size,
                    mod.stride, mod.padding, mod.dilation, mod.groups,
                    mod.bias is not None, mod.padding_mode)
        qconv.set_weight_bias(qweight, mod.bias)
        if activation_post_process is None or activation_post_process.dtype == torch.float:
            return qconv  # dynamic quantization doesn't need scale/zero_point
        else:
            act_scale, act_zp = activation_post_process.calculate_qparams()
            qconv.scale = float(act_scale)
            qconv.zero_point = int(act_zp)
            return qconv

    @staticmethod
    def from_float(cls, mod):
        if hasattr(mod, "weight_fake_quant"):
            # assert type(mod) == cls.__QAT_MODULE, " nnq." + cls.__name__ + \
            # ".from_float only works for " + cls.__QAT_MODULE.__name__
            if type(mod) == cls._NNIQAT_CONV_BN_MODULE:
                mod.weight, mod.bias = fuse_conv_bn_weights(
                    mod.weight, mod.bias, mod.bn.running_mean, mod.bn.running_var,
                    mod.bn.eps, mod.bn.weight, mod.bn.bias)
            assert hasattr(mod, "activation_post_process"), \
                "Input QAT module must have observer attached"
            weight_post_process = mod.weight_fake_quant
            activation_post_process = mod.activation_post_process
        else:
            assert type(mod) == cls._FLOAT_MODULE, \
                " nnq." + cls.__name__ + ".from_float only works for " + \
                cls._FLOAT_MODULE.__name__ + " but got:" + str(type(mod))
            assert hasattr(mod, "qconfig"), \
                "Input float module must have qconfig defined."
            activation_post_process = None if not hasattr(
                mod, "activation_post_process") else mod.activation_post_process
            if type(mod) == cls._NNI_CONV_RELU_MODULE:
                mod = mod[0]
            weight_post_process = mod.qconfig.weight()
        return cls.get_qconv(mod, activation_post_process, weight_post_process)

    @classmethod
    def from_reference(cls, ref_qconv, output_scale, output_zero_point):
        r"""Create a (fbgemm/qnnpack) quantized module from a reference quantized module
        Args:
            ref_module (Module): a reference quantized  module, either produced by torch.ao.quantization
                          utilities or provided by the user
            output_scale (float): scale for output Tensor
            output_zero_point (int): zero point for output Tensor
        """
        qconv = cls(
            ref_qconv.in_channels,
            ref_qconv.out_channels,
            ref_qconv.kernel_size,  # type: ignore[arg-type]
            ref_qconv.stride,  # type: ignore[arg-type]
            ref_qconv.padding,  # type: ignore[arg-type]
            ref_qconv.dilation,  # type: ignore[arg-type]
            ref_qconv.groups,
            ref_qconv.bias is not None,  # type: ignore[arg-type]
            ref_qconv.padding_mode,
            device=ref_qconv.weight.device,
            dtype=ref_qconv.weight.dtype)
        qweight = ref_qconv.get_quantized_weight()
        qconv.set_weight_bias(qweight, ref_qconv.bias)
        qconv.scale = float(output_scale)
        qconv.zero_point = int(output_zero_point)
        return qconv


class slsq_Conv1d(_slsq_ConvNd):
    r"""Applies a 1D convolution over a quantized input signal composed of
    several quantized input planes.

    For details on input arguments, parameters, and implementation see
    :class:`~torch.nn.Conv1d`.

    .. note::
        Only `zeros` is supported for the :attr:`padding_mode` argument.

    .. note::
        Only `torch.quint8` is supported for the input data type.


    Attributes:
        weight (Tensor):     packed tensor derived from the learnable weight
                             parameter.
        scale (Tensor):      scalar for the output scale
        zero_point (Tensor): scalar for the output zero point

    See :class:`~torch.nn.Conv1d` for other attributes.

    Examples::

        >>> m = nn.quantized.Conv1d(16, 33, 3, stride=2)
        >>> input = torch.randn(20, 16, 100)
        >>> # quantize input to quint8
        >>> # xdoctest: +SKIP
        >>> q_input = torch.quantize_per_tensor(input, scale=1.0, zero_point=0,
        ...                                     dtype=torch.quint8)
        >>> output = m(q_input)

    """

    _FLOAT_MODULE = nn.Conv1d
    _NNIQAT_CONV_BN_MODULE = nniqat.ConvBn1d
    _NNI_CONV_RELU_MODULE = nni.ConvReLU1d

    def __init__(self,
                 in_channels: int,
                 out_channels: int,
                 kernel_size: _size_1_t,
                 stride: _size_1_t = 1,
                 padding: _size_1_t = 0,
                 dilation: _size_1_t = 1,
                 groups: int = 1,
                 bias: bool = True,
                 padding_mode: str = 'zeros',
                 device=None,
                 dtype=None):
        factory_kwargs = {'device': device, 'dtype': dtype}
        kernel_size = _single(kernel_size)
        stride = _single(stride)
        padding = padding if isinstance(padding, str) else _single(padding)
        dilation = _single(dilation)

        # Subclasses of _ConvNd needs to call _init rather than __init__. See
        # discussion on PR #49702
        super(slsq_Conv1d, self)._init(
            in_channels, out_channels, kernel_size, stride, padding, dilation,
            False, _single(0), groups, bias, padding_mode, **factory_kwargs)

    def _get_name(self):
        return 'slsq_QuantizedConv1d'

    def set_weight_bias(self, w: torch.Tensor, b: Optional[torch.Tensor]) -> None:
        if self.padding_mode == 'zeros':
            self._packed_params = torch.ops.quantized.conv1d_prepack(
                w, b, self.stride, self.padding, self.dilation, self.groups)
        else:
            self._packed_params = torch.ops.quantized.conv1d_prepack(
                w, b, self.stride, _pair(0), self.dilation,
                self.groups)

    def _weight_bias(self):
        w, b = torch.ops.quantized.conv1d_unpack(self._packed_params)
        return w, b

    def weight(self):
        return self._weight_bias()[0]

    def bias(self):
        return self._weight_bias()[1]

    def forward(self, input):
        # Temporarily using len(shape) instead of ndim due to JIT issue
        # https://github.com/pytorch/pytorch/issues/23890
        if len(input.shape) != 3:
            raise ValueError("Input shape must be `(N, C, L)`!")
        if self.padding_mode != 'zeros':
            # Padding in Conv1d is stored as (p, p), need to get (p,)
            _reversed_padding_repeated_twice = _reverse_repeat_padding(self.padding[:1])
            input = F.pad(input, _reversed_padding_repeated_twice,
                          mode=self.padding_mode)
        return ops.quantized.conv1d(input, self._packed_params, self.scale, self.zero_point)

    @classmethod
    def from_float(cls, mod):
        r"""Creates a quantized module from a float module or qparams_dict.

        Args:
            mod (Module): a float module, either produced by torch.ao.quantization
              utilities or provided by the user
        """
        return _slsq_ConvNd.from_float(cls, mod)


class slsq_Conv2d(_slsq_ConvNd):
    r"""Applies a 2D convolution over a quantized input signal composed of
    several quantized input planes.

    For details on input arguments, parameters, and implementation see
    :class:`~torch.nn.Conv2d`.

    .. note::
        Only `zeros` is supported for the :attr:`padding_mode` argument.

    .. note::
        Only `torch.quint8` is supported for the input data type.


    Attributes:
        weight (Tensor):     packed tensor derived from the learnable weight
                             parameter.
        scale (Tensor):      scalar for the output scale
        zero_point (Tensor): scalar for the output zero point

    See :class:`~torch.nn.Conv2d` for other attributes.

    Examples::

        >>> # With square kernels and equal stride
        >>> m = nn.quantized.Conv2d(16, 33, 3, stride=2)
        >>> # non-square kernels and unequal stride and with padding
        >>> m = nn.quantized.Conv2d(16, 33, (3, 5), stride=(2, 1), padding=(4, 2))
        >>> # non-square kernels and unequal stride and with padding and dilation
        >>> m = nn.quantized.Conv2d(16, 33, (3, 5), stride=(2, 1), padding=(4, 2), dilation=(3, 1))
        >>> input = torch.randn(20, 16, 50, 100)
        >>> # quantize input to quint8
        >>> # xdoctest: +SKIP
        >>> q_input = torch.quantize_per_tensor(input, scale=1.0, zero_point=0, dtype=torch.quint8)
        >>> output = m(q_input)

    """
    _FLOAT_MODULE = nn.Conv2d
    _NNIQAT_CONV_BN_MODULE = nniqat.ConvBn2d
    _NNI_CONV_RELU_MODULE = nni.ConvReLU2d

    def __init__(self, in_channels, out_channels, kernel_size, stride=1,
                 padding=0, dilation=1, groups=1, bias=True,
                 padding_mode='zeros', device=None, dtype=None):
        factory_kwargs = {'device': device, 'dtype': dtype}
        kernel_size = _pair(kernel_size)
        stride = _pair(stride)
        padding = _pair(padding)
        dilation = _pair(dilation)
        # Subclasses of _ConvNd need to call _init rather than __init__. See
        # discussion on PR #49702
        super(slsq_Conv2d, self)._init(
            in_channels, out_channels, kernel_size, stride, padding, dilation,
            False, _pair(0), groups, bias, padding_mode, **factory_kwargs)

    def _get_name(self):
        return 'slsq_QuantizedConv2d'

    def set_weight_bias(self, w: torch.Tensor, b: Optional[torch.Tensor]) -> None:
        if self.padding_mode == 'zeros':
            self._packed_params = torch.ops.quantized.conv2d_prepack(
                w, b, self.stride, self.padding, self.dilation, self.groups)
        else:
            self._packed_params = torch.ops.quantized.conv2d_prepack(
                w, b, self.stride, _pair(0), self.dilation, self.groups)

    def _weight_bias(self):
        return self._packed_params.unpack()

    def weight(self):
        return self._weight_bias()[0]

    def bias(self):
        return self._weight_bias()[1]

    def forward(self, input):
        # Temporarily using len(shape) instead of ndim due to JIT issue
        # https://github.com/pytorch/pytorch/issues/23890
        if len(input.shape) != 4:
            raise ValueError("Input shape must be `(N, C, H, W)`!")
        if self.padding_mode != 'zeros':
            _reversed_padding_repeated_twice = _reverse_repeat_padding(self.padding)
            input = F.pad(input, _reversed_padding_repeated_twice,
                          mode=self.padding_mode)
        return ops.quantized.conv2d(
            input, self._packed_params, self.scale, self.zero_point)

    @classmethod
    def from_float(cls, mod):
        r"""Creates a quantized module from a float module or qparams_dict.

        Args:
            mod (Module): a float module, either produced by torch.ao.quantization
              utilities or provided by the user
        """
        return _slsq_ConvNd.from_float(cls, mod)


class slsq_Conv3d(_slsq_ConvNd):
    r"""Applies a 3D convolution over a quantized input signal composed of
    several quantized input planes.

    For details on input arguments, parameters, and implementation see
    :class:`~torch.nn.Conv3d`.

    .. note::
        Only `zeros` is supported for the :attr:`padding_mode` argument.

    .. note::
        Only `torch.quint8` is supported for the input data type.


    Attributes:
        weight (Tensor):     packed tensor derived from the learnable weight
                             parameter.
        scale (Tensor):      scalar for the output scale
        zero_point (Tensor): scalar for the output zero point

    See :class:`~torch.nn.Conv3d` for other attributes.

    Examples::

        >>> # With square kernels and equal stride
        >>> m = nn.quantized.Conv3d(16, 33, 3, stride=2)
        >>> # non-square kernels and unequal stride and with padding
        >>> m = nn.quantized.Conv3d(16, 33, (3, 5, 5), stride=(1, 2, 2), padding=(1, 2, 2))
        >>> # non-square kernels and unequal stride and with padding and dilation
        >>> m = nn.quantized.Conv3d(16, 33, (3, 5, 5), stride=(1, 2, 2), padding=(1, 2, 2), dilation=(1, 2, 2))
        >>> input = torch.randn(20, 16, 56, 56, 56)
        >>> # quantize input to quint8
        >>> # xdoctest: +SKIP
        >>> q_input = torch.quantize_per_tensor(input, scale=1.0, zero_point=0, dtype=torch.quint8)
        >>> output = m(q_input)

    """
    _FLOAT_MODULE = nn.Conv3d
    _NNIQAT_CONV_BN_MODULE = nniqat.ConvBn3d
    _NNI_CONV_RELU_MODULE = nni.ConvReLU3d

    def __init__(self, in_channels, out_channels, kernel_size, stride=1,
                 padding=0, dilation=1, groups=1, bias=True,
                 padding_mode='zeros', device=None, dtype=None):
        assert padding_mode != 'reflect', "Conv3d does not support reflection padding"
        factory_kwargs = {'device': device, 'dtype': dtype}
        kernel_size = _triple(kernel_size)
        stride = _triple(stride)
        padding = _triple(padding)
        dilation = _triple(dilation)
        # Subclasses of _ConvNd need to call _init rather than __init__. See
        # discussion on PR #49702
        super(slsq_Conv3d, self)._init(
            in_channels, out_channels, kernel_size, stride, padding, dilation,
            False, _triple(0), groups, bias, padding_mode, **factory_kwargs)

    def _get_name(self):
        return 'slsq_QuantizedConv3d'

    def set_weight_bias(self, w: torch.Tensor, b: Optional[torch.Tensor]) -> None:
        if self.padding_mode == 'zeros':
            self._packed_params = torch.ops.quantized.conv3d_prepack(
                w, b, self.stride, self.padding, self.dilation, self.groups)
        else:
            self._packed_params = torch.ops.quantized.conv3d_prepack(
                w, b, self.stride, _triple(0), self.dilation, self.groups)

    def _weight_bias(self):
        return self._packed_params.unpack()

    def weight(self):
        return self._weight_bias()[0]

    def bias(self):
        return self._weight_bias()[1]

    def forward(self, input):
        # Temporarily using len(shape) instead of ndim due to JIT issue
        # https://github.com/pytorch/pytorch/issues/23890
        if len(input.shape) != 5:
            raise ValueError("Input shape must be `(N, C, D, H, W)`!")
        if self.padding_mode != 'zeros':
            _reversed_padding_repeated_twice = _reverse_repeat_padding(self.padding)
            input = F.pad(input, _reversed_padding_repeated_twice,
                          mode=self.padding_mode)
        return ops.quantized.conv3d(
            input, self._packed_params, self.scale, self.zero_point)

    @classmethod
    def from_float(cls, mod):
        r"""Creates a quantized module from a float module or qparams_dict.

        Args:
            mod (Module): a float module, either produced by torch.ao.quantization
              utilities or provided by the user
        """
        return _slsq_ConvNd.from_float(cls, mod)

class slsq_ConvReLU1d(slsq_Conv1d):
    r"""
    A ConvReLU1d module is a fused module of Conv1d and ReLU

    We adopt the same interface as :class:`torch.ao.nn.quantized.Conv1d`.

    Attributes:
        Same as torch.ao.nn.quantized.Conv1d

    """
    _FLOAT_MODULE = torch.nn.intrinsic.ConvReLU1d  # type: ignore[assignment]

    def __init__(self, in_channels, out_channels, kernel_size, stride=1,
                 padding=0, dilation=1, groups=1, bias=True,
                 padding_mode='zeros', device=None, dtype=None):
        super(slsq_ConvReLU1d, self).__init__(
            in_channels, out_channels, kernel_size, stride=stride,
            padding=padding, dilation=dilation, groups=groups, bias=bias,
            padding_mode=padding_mode, device=device, dtype=dtype)

    def forward(self, input):
        # Temporarily using len(shape) instead of ndim due to JIT issue
        # https://github.com/pytorch/pytorch/issues/23890
        if len(input.shape) != 3:
            raise ValueError("Input shape must be `(N, C, L)`!")
        if self.padding_mode != 'zeros':
            # Padding in Conv1d is stored as (p, p), need to get (p,)
            _reversed_padding_repeated_twice = _reverse_repeat_padding(self.padding[:1])
            input = F.pad(input, _reversed_padding_repeated_twice,
                          mode=self.padding_mode)
        return torch.ops.quantized.conv1d_relu(
            input, self._packed_params, self.scale, self.zero_point)

    def _get_name(self):
        return 'slsq_QuantizedConvReLU1d'

    @classmethod
    def from_float(cls, mod):
        if type(mod) == torch.nn.intrinsic.qat.ConvBnReLU1d:
            mod.weight, mod.bias = fuse_conv_bn_weights(
                mod.weight, mod.bias, mod.bn.running_mean, mod.bn.running_var,
                mod.bn.eps, mod.bn.weight, mod.bn.bias)
        return super(slsq_ConvReLU1d, cls).from_float(mod)

    @classmethod
    def from_reference(cls, ref_qconv, output_scale, output_zero_point):
        assert type(ref_qconv) != torch.nn.intrinsic.ConvBnReLU1d, \
            "BatchNorm1d should be fused into Conv1d before converting to reference module"
        return super().from_reference(ref_qconv[0], output_scale, output_zero_point)

class slsq_ConvReLU2d(slsq_Conv2d):
    r"""
    A ConvReLU2d module is a fused module of Conv2d and ReLU

    We adopt the same interface as :class:`torch.ao.nn.quantized.Conv2d`.

    Attributes:
        Same as torch.ao.nn.quantized.Conv2d

    """
    _FLOAT_MODULE = torch.nn.intrinsic.ConvReLU2d  # type: ignore[assignment]

    def __init__(self, in_channels, out_channels, kernel_size, stride=1,
                 padding=0, dilation=1, groups=1, bias=True,
                 padding_mode='zeros', device=None, dtype=None):
        super(slsq_ConvReLU2d, self).__init__(
            in_channels, out_channels, kernel_size, stride=stride,
            padding=padding, dilation=dilation, groups=groups, bias=bias,
            padding_mode=padding_mode, device=device, dtype=dtype)

    def forward(self, input):
        # Temporarily using len(shape) instead of ndim due to JIT issue
        # https://github.com/pytorch/pytorch/issues/23890
        if len(input.shape) != 4:
            raise ValueError("Input shape must be `(N, C, H, W)`!")
        if self.padding_mode != 'zeros':
            _reversed_padding_repeated_twice = _reverse_repeat_padding(self.padding)
            input = F.pad(input, _reversed_padding_repeated_twice,
                          mode=self.padding_mode)
        return torch.ops.quantized.conv2d_relu(
            input, self._packed_params, self.scale, self.zero_point)

    def _get_name(self):
        return 'slsq_QuantizedConvReLU2d'

    @classmethod
    def from_float(cls, mod):
        if type(mod) == torch.nn.intrinsic.qat.ConvBnReLU2d:
            mod.weight, mod.bias = fuse_conv_bn_weights(
                mod.weight, mod.bias, mod.bn.running_mean, mod.bn.running_var,
                mod.bn.eps, mod.bn.weight, mod.bn.bias)
        return super(slsq_ConvReLU2d, cls).from_float(mod)

    @classmethod
    def from_reference(cls, ref_qconv, output_scale, output_zero_point):
        assert type(ref_qconv) != torch.nn.intrinsic.ConvBnReLU2d, \
            "BatchNorm2d should be fused into Conv2d before converting to reference module"
        return super().from_reference(ref_qconv[0], output_scale, output_zero_point)


class slsq_ConvReLU3d(slsq_Conv3d):
    r"""
    A ConvReLU3d module is a fused module of Conv3d and ReLU

    We adopt the same interface as :class:`torch.ao.nn.quantized.Conv3d`.

    Attributes: Same as torch.ao.nn.quantized.Conv3d

    """
    _FLOAT_MODULE = torch.nn.intrinsic.ConvReLU3d  # type: ignore[assignment]

    def __init__(self, in_channels, out_channels, kernel_size, stride=1,
                 padding=0, dilation=1, groups=1, bias=True,
                 padding_mode='zeros', device=None, dtype=None):
        assert padding_mode != 'reflect', "Conv3d does not support reflection padding"
        super(slsq_ConvReLU3d, self).__init__(
            in_channels, out_channels, kernel_size, stride=stride,
            padding=padding, dilation=dilation, groups=groups, bias=bias,
            padding_mode=padding_mode, device=device, dtype=dtype)

    def forward(self, input):
        # Temporarily using len(shape) instead of ndim due to JIT issue
        # https://github.com/pytorch/pytorch/issues/23890
        if len(input.shape) != 5:
            raise ValueError("Input shape must be `(N, C, D, H, W)`!")
        if self.padding_mode != 'zeros':
            _reversed_padding_repeated_twice = _reverse_repeat_padding(self.padding)
            input = F.pad(input, _reversed_padding_repeated_twice,
                          mode=self.padding_mode)
        return torch.ops.quantized.conv3d_relu(
            input, self._packed_params, self.scale, self.zero_point)

    def _get_name(self):
        return 'slsq_QuantizedConvReLU3d'

    @classmethod
    def from_float(cls, mod):
        if type(mod) == torch.nn.intrinsic.qat.ConvBnReLU3d:
            mod.weight, mod.bias = fuse_conv_bn_weights(
                mod.weight,
                mod.bias,
                mod.bn.running_mean,
                mod.bn.running_var,
                mod.bn.eps,
                mod.bn.weight,
                mod.bn.bias,
            )
        return super(slsq_ConvReLU3d, cls).from_float(mod)

    @classmethod
    def from_reference(cls, ref_qconv, output_scale, output_zero_point):
        assert type(ref_qconv) != torch.nn.intrinsic.ConvBnReLU3d, \
            "BatchNorm3d should be fused into Conv3d before converting to reference module"
        return super().from_reference(ref_qconv[0], output_scale, output_zero_point)

# Default map for swapping float module to quantized ones
SLSQ_STATIC_QUANT_MODULE_MAPPINGS : Dict[Callable, Any] = {
    QuantStub: nnq.Quantize,
    DeQuantStub: nnq.DeQuantize,
    nn.BatchNorm2d: nnq.BatchNorm2d,
    nn.BatchNorm3d: nnq.BatchNorm3d,
    nn.Dropout: nnq.Dropout,
    nn.Conv1d: slsq_Conv1d,
    nn.Conv2d: slsq_Conv2d,
    nn.Conv3d: slsq_Conv3d,
    nn.ConvTranspose1d: nnq.ConvTranspose1d,
    nn.ConvTranspose2d: nnq.ConvTranspose2d,
    nn.ConvTranspose3d: nnq.ConvTranspose3d,
    nn.ELU: nnq.ELU,
    nn.Embedding: nnq.Embedding,
    nn.EmbeddingBag: nnq.EmbeddingBag,
    nn.GroupNorm: nnq.GroupNorm,
    nn.Hardswish: nnq.Hardswish,
    nn.InstanceNorm1d: nnq.InstanceNorm1d,
    nn.InstanceNorm2d: nnq.InstanceNorm2d,
    nn.InstanceNorm3d: nnq.InstanceNorm3d,
    nn.LayerNorm: nnq.LayerNorm,
    nn.LeakyReLU: nnq.LeakyReLU,
    nn.modules.linear.NonDynamicallyQuantizableLinear: nnq.Linear,
    nn.Linear: nnq.Linear,
    nn.ReLU6: nnq.ReLU6,
    nn.Dropout: nnq.Dropout,
    # Wrapper Modules:
    nnq.FloatFunctional: nnq.QFunctional,
    # Intrinsic modules:
    nni.BNReLU2d: nniq.BNReLU2d,
    nni.BNReLU3d: nniq.BNReLU3d,
    nni.ConvReLU1d: slsq_ConvReLU1d,
    nni.ConvReLU2d: slsq_ConvReLU2d,
    nni.ConvReLU3d: slsq_ConvReLU3d,
    nni.LinearReLU: nniq.LinearReLU,
    nniqat.ConvBn1d: slsq_Conv1d,
    nniqat.ConvBn2d: slsq_Conv2d,
    nniqat.ConvBn3d: slsq_Conv3d,
    nniqat.ConvBnReLU1d: slsq_ConvReLU1d,
    nniqat.ConvBnReLU2d: slsq_ConvReLU2d,
    nniqat.ConvBnReLU3d: slsq_ConvReLU3d,
    nniqat.ConvReLU2d: slsq_ConvReLU2d,
    nniqat.ConvReLU3d: slsq_ConvReLU3d,
    nniqat.LinearReLU: nniq.LinearReLU,
    nniqat.LinearBn1d: nnq.Linear,
    # QAT modules:
    nnqat.Linear: nnq.Linear,
    nnqat.Conv2d: slsq_Conv2d,
    nnqat.Conv3d: slsq_Conv3d,
}

def get_slsq_static_quant_module_mappings() -> Dict[Callable, Any]:
    ''' Get module mapping for post training static quantization
    '''
    return copy.deepcopy(SLSQ_STATIC_QUANT_MODULE_MAPPINGS)


