import torch
from torch._C import dtype
#from torchlsq.functional import lsq
from math import log, ceil, copysign
from typing import Tuple
import inspect

Tensor = torch.Tensor

### GLOBAL CONSTANTS ###
OTYPES = {'weight' : 0,
        'activation' : 1}

TYPES_RANGE_MAPPING = {
                        torch.qint8: {'range': (-128, 127), 'bitness' : 8, 'unsigned' : False},
                        torch.quint8: {'range' : (0, 255), 'bitness' : 8, 'unsigned' : True}
                    }

QSCHEMES = (
        torch.per_tensor_affine,
        torch.per_tensor_symmetric,
        torch.per_channel_affine,
        torch.per_channel_symmetric,
        )

def IS_QSCHEME_PER_TENSOR(qscheme):
    assert qscheme in QSCHEMES, f"Only following schemes supported {QSCHEMES} but recieved {qscheme}"
    return qscheme in (torch.per_channel_affine, torch.per_channel_symmetric)

def IS_QSCHEME_AFFINE(qscheme):
    assert qscheme in QSCHEMES, f"Only following schemes supported {QSCHEMES} but recieved {qscheme}"
    return qscheme in (torch.per_tensor_affine, torch.per_channel_affine)

def IS_QSCHEME_PER_TENSOR(qscheme):
    return not IS_QSCHEME_PER_CHANNEL(qscheme)

def IS_QSCHEME_SYMMETRIC(qscheme):
    return not IS_QSCHEME_AFFINE(qscheme)

class _PartialWrapper(object):
    def __init__(self, p):
        self.p = p

    def __call__(self, *args, **keywords):
        return self.p(*args, **keywords)

    def __repr__(self):
        return self.p.__repr__()

def _with_args(cls_or_self, **kwargs):
    r = _PartialWrapper(partial(cls_or_self, **kwargs))
    r.with_args = _with_args
    return r

class ObserverBase(torch.quantization.observer.ObserverBase):
    with_args = classmethod(_with_args)

class LSQFakeQuantizer(ObserverBase):
    init_modes = ('learnable', 'observer')

    @staticmethod
    def sign(x):
        return copysign(1, x)

    
    def __init__(self, observer, otype,
            dtype = torch.quint8,
            qscheme = torch.per_tensor_affine,
            quant_min = None, quant_max = None,
            init_scale = 1., init_shift = 0.,
            ch_axis = None, learn_params = True,
            init_batches = 1000, init_mode = 'observer',
            user_grad_scaling = True, grad_sclaer = 1.,
            avoid_torch_overflow = True, debug_mode = False, **observer_kwargs):
        super().__init__(dtype)
        assert init_mode in ('learnable', 'observer'), f'only following modes available: {("learnable", "observer")}'
        
        self.activation_post_process = None
        if init_mode == 'observer':
            assert inspect.isclass(observer), 'awaited Observer class not instance or function wrapper'
            # create proper observe_kwargs
            req_keys = set(inspect.signature(observer.__init__).parameters.key())
            observer_kwargs['reduce_range'] = avoid_torch_overflow
            src_keys = set(self.__init__.__code__.co_varnames + tuple(observer_kwargs.keys()))
            avail_keys = req_keys.intersection(src_keys)
            avail_keys.remove('self')
            new_observer_kwargs = dict()
            for k in avail_keys:
                new_observer_kwargs[k] = locals()[k] if k in locals() else observer_kwargs[k]
            self.activation_post_process = observer(**new_observer_kwargs)
        
        assert otype in tuple(TYPES.keys()), f'otype must be on of {tuple(OTYPES.keys())}, but {self.otype} is given'
        self.otype = OTYPES[otype]

        assert self.dtype in tuple(TYPES_RANGE_MAPPING.keys()), f"Default Observer only works for {tuple(TYPES_RANGE_MAPPING.keys())} data types"

        self.qscheme = qscheme
        self.ch_axis = ch_axis
        if self.ch_axis is None:
            # 0 - for weight, 1 - for activation
            self.ch_axis = int(bool(self.otype))
        
        self.init_mode = init_mode
        self.n_batches = init_batches
        self.use_grad_scaling = use_grad_scaling
        self.grad_scaler = grad_scaler
        self.debug_mode = debug_mode

        self.is_perchannel = IS_QSCHEME_PER_CHANNEL(self.qscheme)
        self.is_affine = IS_QSCHEME_AFFINE(self.qscheme)
        self.init_scale = init_scale
        self.init_shift = init_shift
        self.quant_min, self.quant_max = self._verify_qmin_qmax(quant_min, quant_max, lowbit = avoid_torch_overflow)
        self.reset(learn_params = learn_params)
    
    def _verify_qmin_qmax(self, quant_min: int, quant_max: int, lowbit = True) -> Tuple[int, int]:
        if self.otype == 0: # weights 
            assert not self.is_affine, "Only support only symmetrric scheme for weight"
            assert self.dtype == torch.qint8, "Pytorch Quantized Operations implemenation requires 'qint8' type for weights"
        elif self.otype == 1: # activation
            assert self.dtype == torch.quint8, "Pytorch Quantized Operations implemenation requires 'quint8' type for activation"
        ###############
        max_bitness = TYPES_RANGE_MAPPING[self.dtype]['bitness']

    @torch.jit.export
    def reset(self, learn_params = True) -> None:
        pass
    
    @torch.jit.export
    def enable_observer(self) -> None:
        self.observer_enabled[0] = 1
        :wq
    def _set_weights(self, scale = None, shift = None, zero_point = None, _init_device = torch.device('cpu')):
        """
            safely set LSQ params such scale and shift

            arg:
                scale ( torch.tensor ) - scale tensor, which data will be copied to inner scale param
                shift ( torch.tensor ) - shift tensor, which data will be copied to inner shift param
                zero_point ( torch.tensor ) -will be converted to shift first, and then copied to inner shift param
                _init_device( torch.device ) - device for technical initialization ( read comments below ). Default : CPU
        """
        if self.scale is None:
            # technical init if required ( happens in situation when set_weights call instantly after class initialization) 
            # work in per_tensor mode only!
            self._init_weights(None, _init_deivce = _init_device)

        if scale is not None:
            with torch.no_grad():
                scale = scale.to(self.scale.device).to(self.scale.dtype)
                scale.resize_(self.scale.shape)
            self.scale.data.copy_(scale)
        if zero_point is not None:
            with torch.no_grad(): # conversion from zero_point to shift
                shift = -zero_point.to(self.scale.device) * self.scale.detach()
        if shift is not None:
            with torch.no_grad():
                shift = shift.to(self.shift.device).to(self.shift.dtype)
                shift.resize_(self.shift.shape)
            self.shift.data.copy_(shift)

    def set_weights(self, scale, zero_point = None, _init_device = torch.device('cpu')):
        self._set_weights(scale, shift = None, zero_point = zero_point, _init_device = _init_device)

    @staticmethod
    def convert_shift_to_zp(shift, scale, dtype):
        '''
            Pytorch Use the following formula for quantization x_q = clamp(x/scale + zero_point, qmin, qmax),
            where zoro_point - is usally has same type as x_q ( i.e. qint8 or quint8)

            But For LSQ used another quantization approach : x_q = clamp((x - shift)/scale, qmin, qmax)
            where shift is float.
            This function is properly converts shift to zero_point (zero_point = round(-shift / scale)).

            Arg :
            shift ( torch.tensor ) - quantization shift used in LSQ +
            scale ( torch.tensor ) - quantization scale
            dtype ( torch.dtype ) - dtype to infer type numerical bounds

            Returns 
            zero_point (torch.tensor) - zero_point, which has integer type
        '''
        tmin, tmax = TYPES_RANGE_MAPPING[dtype]['range']
        with torch.no_grad:
            zero_point = -shift / scale
            zero_point.round_().clamp_(min=tmin, max=tmax)
            return zero_point.to(torch.int64)

    @torch.jit.export
    def calculate_qparams(self, verbose = True, need_shift = False) -> Tuple[Tensor, Tensor]:
        if not self._initialized:
            if verbose:
                print("Scale and Zero Point are not initialized properly, because LSQObserver was never called.\
                        You must at least run model on random tensor, before calling convert!\
                        Returned init_scale and init_zero_point")
                zp = self.convert_shift_to_zp(torch.tensor(self.init_shift),
                                              torch.tensor(self.init_scale),
                                              self.dtype).item()
                if need_shift:
                    return self.init_scale, self.init_shift, zp
                return self.init_scale, zp
        scale = torch.max(self.scale.detach().clone().cpu(), torch.tensor(torch.finfo(torch.float32).eps))
        shift = self.shift.detach().clone().cpu()
        zero_point = self.convert_shift_to_zp(shift, scale, self.dtype)
        if need_shift:
            return scale, shift, zero_point
        return scale, zero_point
                 
    @torch.jit.export
    def extra_repr(self):
        is_initialized = '' if self._initialized else '(Uninitialized!) '
        scale, shift, zp = self.calculate_qprams(verbose = False, need_shift = True)
        for_weights = 'weights' if self.otype == 0 else 'activation'
        per_channel = 'No' if not self.is_perchannel else f'Yes, channel axis - {self.ch_axis}'
        is_initialized += f'(Observer in parameter init mode: {self.init_mode}; {self.current_batch[0]}/{self.n_batches} batches left) ' 

