from torch.ao.quantization.observer import ObserverBase, UniformQuantizationObserverBase
from torch.ao.quantization.fake_quantize import FakeQuantizeBase
from torch.ao.quantization import * 
import torch 
def IS_QSCHEME_PER_CHANNEL(qscheme):
    return qscheme in (torch.per_channel_affine, torch.per_channel_symmetric)
def IS_QSCHEME_AFFINE(qscheme):
    return qscheme in (torch.per_tensor_affine, torch.per_channel_affine)
def IS_QSCHEME_PER_TENSOR(qscheme):
    return not IS_QSCHEME_PER_CHANNEL(qscheme)
def IS_QSCHEME_SYMMETRIC(qscheme):
    return not IS_QSCHEME_AFFINE(qscheme)

def init_mode(mod):
    """
    Enable Init Mode quantization for this module, if applicable. Example usage::
        # model is any PyTorch Model
        model.apply(init_mode)
    """
    if isinstance(mod, LSQFakeQuantize) or isinstance(mod, QILFakeQuantize) or isinstance(mod, S_LSQFakeQuantize) or _is_fake_quant_script_module(mod):
        assert hasattr(mod, "init_mode"), "This module does not support init mode"
        mod.init_mode()

def training_mode(mod):
    if isinstance(mod, LSQFakeQuantize) or isinstance(mod, QILFakeQuantize) or isinstance(mod, S_LSQFakeQuantize) or _is_fake_quant_script_module(mod):
        assert hasattr(mod, "init_mode"), "This module does not support training mode"
        mod.training_mode()

def soft_pruning_mode(mod):
    if isinstance(mod, S_LSQFakeQuantize):
        assert hasattr(mod, "soft_pruning_mode"), "This module does not support soft pruning mode"
        mod.soft_pruning_mode()

def hard_pruning_mode(mod):
    if isinstance(mod, S_LSQFakeQuantize):
        assert hasattr(mod, "hard_pruning_mode"), "This module does not support hard pruning mode"
        mod.hard_pruning_mode()

def _is_fake_quant_script_module(mod):
    if isinstance(mod, torch.jit.RecursiveScriptModule):
        suffix = mod._c.qualified_name.split('.', 1)[1]
        name = re.sub(r'\.___torch_mangle_\d+', '', suffix)
        return name == 'torch.ao.quantization.fake_quantize.FakeQuantize' or \
                name == 'torch.ao.quantization.fake_quantize.FusedMovingAvgObsFakeQuantize'
    return False

class LSQObserver(UniformQuantizationObserverBase):
    scale : torch.Tensor
    
    def __init__(
            self,
            dtype = torch.qint8,
            qscheme = torch.per_tensor_affine,
            reduce_range = False,
            quant_min = None,
            quant_max = None,
            factory_kwargs = None,
            eps = torch.finfo(torch.float32).eps,
    ) -> None:
        super(LSQObserver, self).__init__(
                dtype = dtype,
                qscheme = qscheme,
                reduce_range = reduce_range,
                quant_min = quant_min,
                quant_max = quant_max,
                factory_kwargs = factory_kwargs,
                eps = eps,
        )

        factory_kwargs = torch.nn.factory_kwargs(factory_kwargs)
        self.register_buffer('scale', torch.tensor([1.0], dtype = torch.float))
        if (self.qscheme == torch.per_tensor_symmetric
                and self.reduce_range
                and self.dtype == torch.quint8
        ):
            raise NotImplementedError(
                    "Cannot reduce range for symmetric \
                            quantization for quint8"
            )

    def forward(self, x_orig):
        r"""Records Initialize value of scale """
        if x_orig.numel() == 0:
            return x_orig
        x = x_orig.detach()
        x = x.to(self.scale.dtype)
        #_scale = x.abs().mean() * 2 / (self.quant_max ** 0.5)
        _scale = x.abs().max() / self.quant_max
        self.scale.copy_(_scale)
        return x_orig

    # for initialization
    @torch.jit.export
    def calculate_qparams(self):
        return self.scale
    
    @torch.jit.export
    def extra_repr(self):
        pass
    
    @torch.jit.export
    def reset_min_max_vals(self):
        pass   

class LSQFakeQuantize(FakeQuantizeBase):
    s : torch.Tensor
    zero_point : torch.Tensor

    def __init__(self, observer = LSQObserver, quant_min = None, quant_max = None, **observer_kwargs):
        super().__init__()
        if quant_min is not None and quant_max is not None:
            assert quant_min <= quant_max, \
                    'quant_min must be less than or equal to quant_max'
            dtype = observer_kwargs.get("dtype", torch.quint8)
            assert torch.iinfo(dtype).min <= quant_min, 'quant_min out of bound'
            assert quant_max <= torch.iinfo(dtype).max, 'quant_max out of bound'
            observer_kwargs.update({"quant_min":quant_min, "quant_max" : quant_max})
        # not needed
        self.activation_post_process = observer(**observer_kwargs)

        self.quant_min = self.activation_post_process.quant_min
        self.quant_max = self.activation_post_process.quant_max

        self.scale = torch.nn.Parameter(torch.tensor([1.0], dtype = torch.float))
        self.register_buffer("zero_point", torch.tensor([0.0], dtype = torch.int32))
        self.dtype = self.activation_post_process.dtype
        self.qscheme = self.activation_post_process.qscheme
        self.ch_axis = self.activation_post_process.ch_axis \
                if hasattr(self.activation_post_process, 'ch_axis') else -1
        assert IS_QSCHEME_PER_CHANNEL(self.qscheme) or \
                IS_QSCHEME_PER_TENSOR(self.qscheme), \
                'Only per channel and per tensor quantization are fake quantize' + ' got qscheme : ' + str(self.qscheme)
        self.is_per_channel = IS_QSCHEME_PER_CHANNEL(self.qscheme)
        self.register_buffer('eps', torch.tensor([torch.finfo(torch.float32).eps]))

    @torch.jit.export
    def grad_scale(self, x, scale):
        y = x
        y_grad = x * scale
        return (y - y_grad).detach() + y_grad
    
    @torch.jit.export
    def round_pass(self, x):
        y = x.round()
        y_grad = x
        return ( y - y_grad).detach() + y_grad

    @torch.jit.export
    def calculate_qparams(self):
        return self.scale, self.zero_point

    def forward(self, X):
        # not implemented for observed version
        if self.observer_enabled[0] == 1:
            self.activation_post_process(X.detach())
            _scale = self.activation_post_process.calculate_qparams()
            _scale = _scale.to(self.scale.device)
            if self.scale.shape != _scale.shape:
                self.scale.resize_(_scale.shape)
            self.scale.data.copy_(_scale).detach()
        else:
            self.scale.data.clamp_(min =self.eps.item())
        if self.fake_quant_enabled[0] == 1:
            '''
            if self.is_per_channel:
                s_grad_scale = 1.0 / ((self.quant_max * X.numel()) ** 0.5)
            else:
                s_grad_scale = 1.0 / ((self.quant_max * X.numel()) ** 0.5)
            #s_scale = self.grad_scale(self.scale, s_grad_scale)
            s_scale = (self.scale - self.scale * s_grad_scale).detach() + self.scale * s_grad_scale
            X = X / s_scale
            X = torch.clamp(X, self.quant_min, self.quant_max)
            #X = self.round_pass(X)
            X = (torch.round(X) - X).detach() + X
            X = X * s_scale
            '''
            
            grad_factor = 1.0 / (self.quant_max * X.numel()) ** 0.5
            X = torch._fake_quantize_learnable_per_tensor_affine(
                    X, self.scale, torch.tensor([0.]).to(self.scale.device),
                    self.quant_min, self.quant_max, grad_factor)
        return X
    
    @torch.jit.export
    def init_mode(self):
        # Only one of the Observer and FakeQuant runs.
        self.enable_fake_quant(False)
        self.enable_observer(True)

    @torch.jit.export
    def training_mode(self):
        # Only one of the Observer and FakeQuant runs.
        self.enable_fake_quant(True)
        self.enable_observer(False)

    @torch.jit.export
    def extra_repr(self):
        return 'fake_quant_enabled = {}, observer_enabled={}, '\
                'quant_min = {}, quant_max = {}, dtype = {}, qscheme = {}, ch_axis = {}, '\
                'scale = {}, zero_point ={}'.format(
                        self.fake_quant_enabled, self.observer_enabled, self.activation_post_process.quant_min,
                        self.activation_post_process.quant_max, 
                        self.dtype, self.qscheme, self.ch_axis, self.scale, self.zero_point)

class QILObserver(UniformQuantizationObserverBase):
    scale : torch.Tensor
    p : torch.Tensor
    c : torch.Tensor

    def __init__(
            self,
            dtype = torch.qint8,
            qscheme = torch.per_tensor_affine,
            reduce_range = False,
            quant_min = None,
            quant_max = None,
            factory_kwargs = None,
            eps = torch.finfo(torch.float32).eps,
    ) -> None:
        super(QILObserver, self).__init__(
                dtype = dtype,
                qscheme = qscheme,
                reduce_range = reduce_range,
                quant_min = quant_min,
                quant_max = quant_max,
                factory_kwargs = factory_kwargs,
                eps = eps,
        )

        factory_kwargs = torch.nn.factory_kwargs(factory_kwargs)
        self.register_buffer('scale', torch.tensor([1.0], dtype = torch.float))
        self.register_buffer('p', torch.tensor([0.0], dtype = torch.float))
        self.register_buffer('c', torch.tensor([1.0], dtype = torch.float))

        if (self.qscheme == torch.per_tensor_symmetric
                and self.reduce_range
                and self.dtype == torch.quint8
        ):
            raise NotImplementedError(
                    "Cannot reduce range for symmetric \
                            quantization for quint8"
            )

    def forward(self, x_orig):
        r"""Records Initialize value of scale """
        if x_orig.numel() == 0:
            return x_orig
        x = x_orig.detach()
        x = x.to(self.scale.dtype)
        _c = x.abs().max()
        self.c.copy_(_c).detach()
        return x_orig

    # for initialization
    @torch.jit.export
    def calculate_qparams(self):
        return self.p, self.c 
    
    @torch.jit.export
    def extra_repr(self):
        pass
    
    @torch.jit.export
    def reset_min_max_vals(self):
        pass   

class QILFakeQuantize(FakeQuantizeBase):
    scale : torch.Tensor
    zero_point : torch.Tensor

    def __init__(self, observer = QILObserver, quant_min = None, quant_max = None, **observer_kwargs):
        super().__init__()
        if quant_min is not None and quant_max is not None:
            assert quant_min <= quant_max, \
                    'quant_min must be less than or equal to quant_max'
            dtype = observer_kwargs.get("dtype", torch.quint8)
            if hasattr(observer, "p"):
                dtype = getattr(getattr(observer, "p", {}), "keywords", {}).get("dtype", dtype)
            assert torch.iinfo(dtype).min <= quant_min, 'quant_min out of bound'
            assert quant_max <= torch.iinfo(dtype).max, 'quant_max out of bound'
            observer_kwargs.update({"quant_min":quant_min, "quant_max" : quant_max})
        # not needed
        self.activation_post_process = observer(**observer_kwargs)

        self.quant_min = self.activation_post_process.quant_min
        self.quant_max = self.activation_post_process.quant_max

        self.scale = torch.nn.Parameter(torch.tensor(1/self.quant_max, dtype = torch.float), requires_grad = False)
        
        # QIL Parameter
        self.c = torch.nn.Parameter(torch.tensor([1.0], dtype = torch.float))
        self.p = torch.nn.Parameter(torch.tensor([0.0], dtype = torch.float))
        self.gamma = torch.nn.Parameter(torch.tensor([1.0], dtype = torch.float))
        
        self.register_buffer("zero_point", torch.tensor([0.0], dtype = torch.int32))
        self.dtype = self.activation_post_process.dtype
        self.qscheme = self.activation_post_process.qscheme
        self.ch_axis = self.activation_post_process.ch_axis \
                if hasattr(self.activation_post_process, 'ch_axis') else -1
        assert IS_QSCHEME_PER_CHANNEL(self.qscheme) or \
                IS_QSCHEME_PER_TENSOR(self.qscheme), \
                'Only per channel and per tensor quantization are fake quantize' + ' got qscheme : ' + str(self.qscheme)
        self.is_per_channel = IS_QSCHEME_PER_CHANNEL(self.qscheme)

    @torch.jit.export
    def calculate_qparams(self):
        return self.scale, self.zero_point

    def forward(self, X):
        # not implemented for observed version
        if self.observer_enabled[0] == 1:
            self.activation_post_process(X.detach())
            _p, _c = self.activation_post_process.calculate_qparams()
            _p = _p.to(self.p.device)
            _c = _c.to(self.c.device)
            if self.p.shape != _p.shape:
                self.p.resize_(_p.shape)
            if self.c.shape != _c.shape:
                self.c.resize_(_c.shape)
            self.p.data.copy_(_p).detach()
            self.c.data.copy_(_c).detach()
            quantized_x = X
        if self.fake_quant_enabled[0] == 1:
            torch.clamp_(self.p.data,torch.tensor(0).to(self.p.device), self.c.data).detach()
            
            pi_mask = (torch.abs(X) > self.p).type(X.dtype)
            ci_mask = (torch.abs(X) <= self.c).type(X.dtype)
            i_mask = pi_mask * ci_mask

            interval_x = X * i_mask
            if self.dtype == torch.quint8:
                transformed_x = ((X - self.p) / (self.c - self.p)).clamp(0, 1)
            else:
                transformed_x = ((torch.abs(X) - self.p) / (self.c - self.p)).clamp(0, 1) * torch.sign(X)
            
            transformed_x = transformed_x / self.scale
            transformed_x = (transformed_x.round() - transformed_x).detach() + transformed_x
            transformed_x = transformed_x * self.scale
            quantized_x = transformed_x
        return quantized_x

    @torch.no_grad()   
    def quantize(self, X):
        # not implemented for observed version
        c_W = 0.5 * (self.p + self.c)
        d_W = 0.5 * (self.c - self.p)
        alpha_W = 0.5 / (d_W)
        beta_W = -0.5 * c_W / d_W + 0.5
        interval_x = X * \
                     (torch.abs(X) >= self.p).type(X.dtype) * \
                     (torch.abs(X) <= self.c).type(X.dtype)
        if self.dtype == torch.quint8:
            transformed_x = \
                    (torch.abs(X) > self.c).type(X.dtype) + \
                    alpha_W * torch.abs(interval_x) + beta_W 
        else:
            transformed_x = \
                    torch.sign(X) * (torch.abs(X) > self.c).type(X.dtype) + \
                    torch.pow(alpha_W * torch.abs(interval_x) + beta_W, self.gamma) * \
                    torch.sign(interval_x)
        
        transformed_x = transformed_x / self.scale
        transformed_x = (transformed_x.round() - transformed_x).detach() + transformed_x
        transformed_x = transformed_x * self.scale
        quantized_x = transformed_x
        return X

    @torch.jit.export
    def init_mode(self):
        # Only one of the Observer and FakeQuant runs.
        self.enable_fake_quant(False)
        self.enable_observer(True)

    @torch.jit.export
    def training_mode(self):
        # Only one of the Observer and FakeQuant runs.
        self.enable_fake_quant(True)
        self.enable_observer(False)

    @torch.jit.export
    def extra_repr(self):
        return 'fake_quant_enabled = {}, observer_enabled={}, '\
                'quant_min = {}, quant_max = {}, dtype = {}, qscheme = {}, ch_axis = {}, '\
                'scale = {}, zero_point ={}, clipping = {}, pruning = {}'.format(
                        self.fake_quant_enabled, self.observer_enabled, self.activation_post_process.quant_min,
                        self.activation_post_process.quant_max, 
                        self.dtype, self.qscheme, self.ch_axis, self.scale, self.zero_point, self.c, self.p)

class S_LSQObserver(UniformQuantizationObserverBase):
    scale : torch.Tensor
    p : torch.Tensor
    c : torch.Tensor

    def __init__(
            self,
            dtype = torch.qint8,
            qscheme = torch.per_tensor_affine,
            reduce_range = False,
            quant_min = None,
            quant_max = None,
            factory_kwargs = None,
            eps = torch.finfo(torch.float32).eps,
    ) -> None:
        super(S_LSQObserver, self).__init__(
                dtype = dtype,
                qscheme = qscheme,
                reduce_range = reduce_range,
                quant_min = quant_min,
                quant_max = quant_max,
                factory_kwargs = factory_kwargs,
                eps = eps,
        )

        factory_kwargs = torch.nn.factory_kwargs(factory_kwargs)
        self.register_buffer('scale', torch.tensor([1.0], dtype = torch.float))
        self.register_buffer('p', torch.tensor([0.0], dtype = torch.float))
        self.register_buffer('c', torch.tensor([1.0], dtype = torch.float))

        if (self.qscheme == torch.per_tensor_symmetric
                and self.reduce_range
                and self.dtype == torch.quint8
        ):
            raise NotImplementedError(
                    "Cannot reduce range for symmetric \
                            quantization for quint8"
            )

    def forward(self, x_orig):
        r"""Records Initialize value of scale """
        if x_orig.numel() == 0:
            return x_orig
        x = x_orig.detach()
        x = x.to(self.scale.dtype)
        with torch.no_grad():
            _scale = x.detach().abs().mean() * 2 / (self.quant_max ** 0.5)
            self.scale.copy_(_scale)
            _c = (self.scale * self.quant_max)
            self.c.copy_(_c)
        return x_orig
    
    # for initialization
    @torch.jit.export
    def calculate_qparams(self):
        return self.c
    
    @torch.jit.export
    def extra_repr(self):
        pass
    
    @torch.jit.export
    def reset_min_max_vals(self):
        pass   

class S_LSQFakeQuantize(FakeQuantizeBase):
    scale : torch.Tensor
    zero_point : torch.Tensor

    def __init__(self, observer = S_LSQObserver, quant_min = None, quant_max = None, block_size = 4, temperature = 0.05, hard_pruning = False,**observer_kwargs):
        super().__init__()
        if quant_min is not None and quant_max is not None:
            assert quant_min <= quant_max, \
                    'quant_min must be less than or equal to quant_max'
            dtype = observer_kwargs.get("dtype", torch.quint8)
            if hasattr(observer, "p"):
                dtype = getattr(getattr(observer, "p", {}), "keywords", {}).get("dtype", dtype)
            assert torch.iinfo(dtype).min <= quant_min, 'quant_min out of bound'
            assert quant_max <= torch.iinfo(dtype).max, 'quant_max out of bound'
            observer_kwargs.update({"quant_min":quant_min, "quant_max" : quant_max})
        # not needed
        self.activation_post_process = observer(**observer_kwargs)

        self.quant_min = self.activation_post_process.quant_min
        self.quant_max = self.activation_post_process.quant_max

        self.register_buffer("scale", torch.tensor([1.0], dtype = torch.float))
        self.register_buffer("zero_point", torch.tensor([0.0], dtype = torch.int32))
        self.soft_mask = None
        # S_LSQ Parameter
        self.c = torch.nn.Parameter(torch.tensor([1.0], dtype = torch.float))
        self.p = torch.nn.Parameter(torch.tensor([0.0], dtype = torch.float))
        
        # S_LSQ Parameter
        self.temperature = temperature
        self.block_size = block_size
        self.hard_pruning = False

        #self.soft_mask = torch.nn.Parameter(torch.tensor([1.0], dtype = torch.float), requires_grad = False)
        self.dtype = self.activation_post_process.dtype
        self.qscheme = self.activation_post_process.qscheme
        self.ch_axis = self.activation_post_process.ch_axis \
                if hasattr(self.activation_post_process, 'ch_axis') else -1
        assert IS_QSCHEME_PER_CHANNEL(self.qscheme) or \
                IS_QSCHEME_PER_TENSOR(self.qscheme), \
                'Only per channel and per tensor quantization are fake quantize' + ' got qscheme : ' + str(self.qscheme)
        self.is_per_channel = IS_QSCHEME_PER_CHANNEL(self.qscheme)
        self.register_buffer('eps', torch.tensor([torch.finfo(torch.float32).eps]))

    @torch.jit.export
    def calculate_qparams(self):
        with torch.no_grad():
            _scale = (self.c - self.p) / self.quant_max
            self.scale.copy_(_scale)
        return self.scale, self.zero_point
    
    @torch.jit.export
    def grad_scale(self,x, scale):
        y = x
        y_grad = x * scale
        return ( y - y_grad ).detach() + y_grad

    @torch.jit.export
    def soft_pruner(self, x, p):
        co, ci, kh, kw = x.shape
        x_reshape = x.reshape(co // self.block_size, self.block_size, ci, kh, kw)

        score = x_reshape.abs().mean(dim = 1, keepdim = True) - p
        if not self.hard_pruning:
            temperature = (score.abs().view(-1).sort()[0][int(score.numel() * self.temperature)] * 0.5).detach()
            _soft_mask = torch.nn.functional.sigmoid(score / temperature)
            self.soft_mask = _soft_mask
            self.soft_mask = self.soft_mask.repeat(1, self.block_size, 1, 1, 1).reshape(co, ci, kh, kw)
            return self.soft_mask

        hard_mask = (score > 0).float()
        hard_mask = hard_mask.repeat(1, self.block_size, 1, 1, 1).reshape(co, ci, kh, kw)
        return hard_mask

    def forward(self, X):
        # not implemented for observed version
        # initialize mode on observer mode
        if self.observer_enabled[0] == 1:
            self.activation_post_process(X)

            with torch.no_grad():
                _c = self.activation_post_process.calculate_qparams()
                _c = _c.to(self.c.device)
                if self.c.shape != _c.shape:
                    self.c.resize_(_c.shape)
                self.c.data.copy_(_c)

            quantized_x = X
        if self.fake_quant_enabled[0] == 1:
            #self.p.data.clamp_(torch.tensor([0], dtype = self.p.dtype).to(self.p.device), self.c.data)
            self.p.data.clamp_(min = self.eps.item(), max=self.c.data.item())
            x_numel = (X.abs() >= self.p).float().sum().detach()
            c_grad_scale = (self.quant_max / x_numel) ** 0.5 * (self.c / (self.c - self.p + self.eps)).detach() * self.quant_max
            p_grad_scale = (self.quant_max / x_numel) ** 0.5 * (self.p / (self.c - self.p + self.eps)).detach() 
            
            c_scale = self.grad_scale(self.c, c_grad_scale)
            p_scale = self.grad_scale(self.p, p_grad_scale)
            
            sign = X.sign()
            s = (c_scale - p_scale) / self.quant_max
            quantized_x = (X.abs() - p_scale) / s
            quantized_x = torch.clamp(quantized_x, 0, self.quant_max) * sign
            quantized_x = (torch.round(quantized_x) - quantized_x).detach() + quantized_x
            quantized_x = quantized_x * s

            if (len(X.shape) == 4 and X.shape[1] != 1):
                mask = self.soft_pruner(X, p_scale)
                quantized_x = quantized_x * mask
        return quantized_x
    
    @torch.jit.export
    def init_mode(self):
        # Only one of the Observer and FakeQuant runs.
        self.enable_fake_quant(False)
        self.enable_observer(True)

    @torch.jit.export
    def training_mode(self):
        # Only one of the Observer and FakeQuant runs.
        self.enable_fake_quant(True)
        self.enable_observer(False)
    
    @torch.jit.export
    def hard_pruning_mode(self):
        self.hard_pruning = True
        self.p.requires_grad = False

    @torch.jit.export
    def soft_pruning_mode(self):
        self.hard_pruning = False
        self.p.requires_grad = True

    @torch.jit.export
    def extra_repr(self):
        return 'fake_quant_enabled = {}, observer_enabled={}, '\
                'quant_min = {}, quant_max = {}, dtype = {}, qscheme = {}, ch_axis = {}, '\
                'scale = {}, zero_point ={}, clipping = {}, pruning = {}'.format(
                        self.fake_quant_enabled, self.observer_enabled, self.activation_post_process.quant_min,
                        self.activation_post_process.quant_max, 
                        self.dtype, self.qscheme, self.ch_axis, self.scale, self.zero_point, self.c, self.p)


default_affine_weight_fake_quant = LSQFakeQuantize.with_args(observer = LSQObserver, quant_min = -128, quant_max = 127,
                                                      dtype = torch.qint8, qscheme = torch.per_tensor_affine, reduce_range = False)
default_affine_activation_fake_quant = LSQFakeQuantize.with_args(observer = LSQObserver, quant_min = 0, quant_max = 255, 
                                                      dtype = torch.quint8, qscheme = torch.per_tensor_affine, reduce_range = True)
default_symmetric_weight_fake_quant = LSQFakeQuantize.with_args(observer = LSQObserver, quant_min = -127, quant_max = 127,
                                                      dtype = torch.qint8, qscheme = torch.per_tensor_symmetric, reduce_range = False)
default_symmetric_activation_fake_quant = LSQFakeQuantize.with_args(observer = LSQObserver, quant_min = 0, quant_max = 255,
                                                      dtype = torch.quint8, qscheme = torch.per_tensor_symmetric, reduce_range = True)
default_lsq_qconfig = torch.ao.quantization.QConfig(activation = default_affine_activation_fake_quant, weight = default_symmetric_weight_fake_quant)

QIL_default_affine_weight_fake_quant = QILFakeQuantize.with_args(observer = QILObserver, quant_min = -128, quant_max = 127,
                                                      dtype = torch.qint8, qscheme = torch.per_tensor_affine, reduce_range = False)
QIL_default_affine_activation_fake_quant = QILFakeQuantize.with_args(observer = QILObserver, quant_min = 0, quant_max = 255, 
                                                      dtype = torch.quint8, qscheme = torch.per_tensor_affine, reduce_range = True)
QIL_default_symmetric_weight_fake_quant = QILFakeQuantize.with_args(observer = QILObserver, quant_min = -127, quant_max = 127,
                                                      dtype = torch.qint8, qscheme = torch.per_tensor_symmetric, reduce_range = False)
QIL_default_symmetric_activation_fake_quant = QILFakeQuantize.with_args(observer = QILObserver, quant_min = 0, quant_max = 255,
                                                      dtype = torch.quint8, qscheme = torch.per_tensor_symmetric, reduce_range = True)
default_qil_qconfig = torch.ao.quantization.QConfig(activation = QIL_default_affine_activation_fake_quant, weight = QIL_default_symmetric_weight_fake_quant)

S_LSQ_default_affine_weight_fake_quant = S_LSQFakeQuantize.with_args(observer = S_LSQObserver, quant_min = -128, quant_max = 127, block_size = 4,
                                                      dtype = torch.qint8, qscheme = torch.per_tensor_affine, reduce_range = False)
S_LSQ_default_affine_activation_fake_quant = LSQFakeQuantize.with_args(observer = LSQObserver, quant_min = 0, quant_max = 255, 
                                                      dtype = torch.quint8, qscheme = torch.per_tensor_affine, reduce_range = True)
S_LSQ_default_symmetric_weight_fake_quant = S_LSQFakeQuantize.with_args(observer = S_LSQObserver, quant_min = -127, quant_max = 127, block_size = 4,
                                                      dtype = torch.qint8, qscheme = torch.per_tensor_symmetric, reduce_range = False)
S_LSQ_default_symmetric_activation_fake_quant = LSQFakeQuantize.with_args(observer = LSQObserver, quant_min = 0, quant_max = 255,
                                                      dtype = torch.quint8, qscheme = torch.per_tensor_symmetric, reduce_range = True)
default_slsq_qconfig = torch.ao.quantization.QConfig(activation = S_LSQ_default_affine_activation_fake_quant, weight = S_LSQ_default_symmetric_weight_fake_quant)

