"""
Creates a MobileNetV2 Model as defined in:
Mark Sandler, Andrew Howard, Menglong Zhu, Andrey Zhmoginov, Liang-Chieh Chen. (2018). 
MobileNetV2: Inverted Residuals and Linear Bottlenecks
arXiv preprint arXiv:1801.04381.
import from https://github.com/tonylins/pytorch-mobilenet-v2
"""
from torch.ao.quantization import QuantStub, DeQuantStub
import torch
import torch.nn as nn
from torch.hub import load_state_dict_from_url
import math
__all__ = ['mobilenetv2','mobilenetv2_01', 'mobilenetv2_25', 'mobilenetv2_35', 'mobilenetv2_50',
           'mobilenetv2_75', 'mobilenetv2_100']

model_urls = {
    'mobilenetv2': 'https://github.com/d-li14/mobilenetv2.pytorch/raw/master/pretrained/mobilenetv2-c5e733a8.pth',
    'mobilenetv2_0.1': 'https://github.com/d-li14/mobilenetv2.pytorch/raw/master/pretrained/mobilenetv2_0.1-7d1d638a.pth',
    'mobilenetv2_0.25': 'https://github.com/d-li14/mobilenetv2.pytorch/raw/master/pretrained/mobilenetv2_0.25-b61d2159.pth',
    'mobilenetv2_0.35': 'https://github.com/d-li14/mobilenetv2.pytorch/blob/master/pretrained/mobilenetv2_0.35-b2e15951.pth',
    'mobilenetv2_0.5': 'https://github.com/d-li14/mobilenetv2.pytorch/raw/master/pretrained/mobilenetv2_0.5-eaa6f9ad.pth',
    'mobilenetv2_0.75': 'https://github.com/d-li14/mobilenetv2.pytorch/raw/master/pretrained/mobilenetv2_0.75-dace9791.pth',
    'mobilenetv2_1.0': 'https://github.com/d-li14/mobilenetv2.pytorch/raw/master/pretrained/mobilenetv2_1.0-0c6065bc.pth',
    'mobilenetv2_128': 'https://github.com/d-li14/mobilenetv2.pytorch/raw/master/pretrained/mobilenetv2_128x128-fd66a69d.pth',
    'mobilenetv2_160': 'https://github.com/d-li14/mobilenetv2.pytorch/raw/master/pretrained/mobilenetv2_160x160-64dc7fa1.pth',
    'mobilenetv2_192': 'https://github.com/d-li14/mobilenetv2.pytorch/raw/master/pretrained/mobilenetv2_192x192-e423d99e.pth',
    'mobilenetv2_96' : 'https://github.com/d-li14/mobilenetv2.pytorch/raw/master/pretrained/mobilenetv2_96x96-ff0e83d8.pth'
}



def _make_divisible(v, divisor, min_value=None):
    """
    This function is taken from the original tf repo.
    It ensures that all layers have a channel number that is divisible by 8
    It can be seen here:
    https://github.com/tensorflow/models/blob/master/research/slim/nets/mobilenet/mobilenet.py
    :param v:
    :param divisor:
    :param min_value:
    :return:
    """
    if min_value is None:
        min_value = divisor
    new_v = max(min_value, int(v + divisor / 2) // divisor * divisor)
    # Make sure that round down does not go down by more than 10%.
    if new_v < 0.9 * v:
        new_v += divisor
    return new_v


def conv_3x3_bn(inp, oup, stride):
    return nn.Sequential(
        nn.Conv2d(inp, oup, 3, stride, 1, bias=False),
        nn.BatchNorm2d(oup),
        nn.ReLU(inplace=True)
    )


def conv_1x1_bn(inp, oup):
    return nn.Sequential(
        nn.Conv2d(inp, oup, 1, 1, 0, bias=False),
        nn.BatchNorm2d(oup),
        nn.ReLU(inplace=True)
    )


class InvertedResidual(nn.Module):
    def __init__(self, inp, oup, stride, expand_ratio):
        super(InvertedResidual, self).__init__()
        assert stride in [1, 2]

        hidden_dim = round(inp * expand_ratio)
        self.identity = stride == 1 and inp == oup

        self.skip_add = nn.quantized.FloatFunctional()
        if expand_ratio == 1:
            self.conv = nn.Sequential(
                 # dw
                nn.Conv2d(hidden_dim, hidden_dim, 3, stride, 1, groups=hidden_dim, bias=False),
                nn.BatchNorm2d(hidden_dim),
                nn.ReLU(inplace=True),
                # pw-linear
                nn.Conv2d(hidden_dim, oup, 1, 1, 0, bias=False),
                nn.BatchNorm2d(oup),
            )
        else:
            self.conv = nn.Sequential(
                # pw
                nn.Conv2d(inp, hidden_dim, 1, 1, 0, bias=False),
                nn.BatchNorm2d(hidden_dim),
                nn.ReLU(inplace=True),
                # dw
                nn.Conv2d(hidden_dim, hidden_dim, 3, stride, 1, groups=hidden_dim, bias=False),
                nn.BatchNorm2d(hidden_dim),
                nn.ReLU(inplace=True),
                # pw-linear
                nn.Conv2d(hidden_dim, oup, 1, 1, 0, bias=False),
                nn.BatchNorm2d(oup),
            )

    def forward(self, x):
        if self.identity:
            return self.skip_add.add(x, self.conv(x))
        else:
            return self.conv(x)


class MobileNetV2(nn.Module):
    def __init__(self, num_classes=1000, width_mult=1.):
        super(MobileNetV2, self).__init__()
        # setting of inverted residual blocks
        self.cfgs = [
            # t, c, n, s
            [1,  16, 1, 1],
            [6,  24, 2, 2],
            [6,  32, 3, 2],
            [6,  64, 4, 2],
            [6,  96, 3, 1],
            [6, 160, 3, 2],
            [6, 320, 1, 1],
        ]

        # building first layer
        input_channel = _make_divisible(32 * width_mult, 4 if width_mult == 0.1 else 8)
        #layers = [conv_3x3_bn(3, input_channel, 2)]
        self.first_layer = conv_3x3_bn(3, input_channel, 2)
        layers = []
        # building inverted residual blocks
        block = InvertedResidual
        for t, c, n, s in self.cfgs:
            output_channel = _make_divisible(c * width_mult, 4 if width_mult == 0.1 else 8)
            for i in range(n):
                layers.append(block(input_channel, output_channel, s if i == 0 else 1, t))
                input_channel = output_channel
        self.features = nn.Sequential(*layers)
        # building last several layers
        output_channel = _make_divisible(1280 * width_mult, 4 if width_mult == 0.1 else 8) if width_mult > 1.0 else 1280
        self.conv = conv_1x1_bn(input_channel, output_channel)
        self.avgpool = nn.AdaptiveAvgPool2d((1, 1))
        self.classifier = nn.Linear(output_channel, num_classes)
        
        self._initialize_weights()
        self.quant = QuantStub()
        self.dequant = DeQuantStub()
    def forward(self, x):
        x = self.first_layer(x)
        x = self.quant(x)
        x = self.features(x)
        x = self.conv(x)
        x = self.avgpool(x)
        x = torch.flatten(x, 1)
        x = self.dequant(x)
        x = self.classifier(x)
        return x

    def _initialize_weights(self):
        for m in self.modules():
            if isinstance(m, nn.Conv2d):
                n = m.kernel_size[0] * m.kernel_size[1] * m.out_channels
                m.weight.data.normal_(0, math.sqrt(2. / n))
                if m.bias is not None:
                    m.bias.data.zero_()
            elif isinstance(m, nn.BatchNorm2d):
                m.weight.data.fill_(1)
                m.bias.data.zero_()
            elif isinstance(m, nn.Linear):
                m.weight.data.normal_(0, 0.01)
                m.bias.data.zero_()

    def fuse_model(self):
        
            for module_name, module in self.named_children():
                if "first" in module_name:
                    torch.ao.quantization.fuse_modules_qat(module, [['0','1','2']], inplace = True)
                elif "conv" in module_name:
                    torch.ao.quantization.fuse_modules_qat(module, [['0','1','2']], inplace = True)
                else:
                    for basic_block_name, basic_block in module.named_children():
                        if  basic_block_name == "0":
                            for sub_block_name, sub_block in basic_block.named_children():
                                if "conv" in sub_block_name:
                                    torch.ao.quantization.fuse_modules_qat(sub_block, [['0','1','2'], ['3','4']], inplace = True)
                        else:
                            for sub_block_name, sub_block in basic_block.named_children():
                                if "conv" in sub_block_name:
                                    torch.ao.quantization.fuse_modules_qat(sub_block, [['0','1','2'], ['3','4', '5'], ['6', '7']], inplace = True)

def _mobilenetv2(arch, width_mult, pretrained, progress,**kwargs):
    model = MobileNetV2(width_mult = width_mult, **kwargs)
    if pretrained:
        print("********************pre-trained*****************")
        state_dict = load_state_dict_from_url(model_urls[arch], progress = progress)
        new_state_dict = {}
        for k,v in state_dict.items():
            if "features.0" in k:
                key_list = k.split('.')
                key_list[0] = "first_layer"
                del key_list[1]
                new_key = ".".join(key_list)
                new_state_dict[new_key] = v
            elif "features" in k:
                key_list = k.split('.')
                key_list[1] = str(int(key_list[1]) - 1)
                new_key = ".".join(key_list)
                new_state_dict[new_key] = v
            else:
                new_state_dict[k] = v
        model.load_state_dict(new_state_dict)
    return model

def mobilenetv2(pretrained = True, progress = True, **kwargs):
    return _mobilenetv2('mobilenetv2',1., pretrained, progress, **kwargs)

def mobilenetv2_01(pretrained = True, progress = True, **kwargs):
    return _mobilenetv2('mobilenetv2_0.1',0.1, pretrained, progress, **kwargs)

def mobilenetv2_25(pretrained = True, progress = True, **kwargs):
    return _mobilenetv2('mobilenetv2_0.25',0.25,pretrained, progress, **kwargs)

def mobilenetv2_35(pretrained = True, progress = True, **kwargs):
    return _mobilenetv2('mobilenetv2_0.35',0.35,pretrained, progress, **kwargs)

def mobilenetv2_50(pretrained = True, progress = True, **kwargs):
    return _mobilenetv2('mobilenetv2_0.5', 0.5,pretrained, progress, **kwargs)

def mobilenetv2_75(pretrained = True, progress = True, **kwargs):
    return _mobilenetv2('mobilenetv2_0.75',0.75,pretrained, progress, **kwargs)

def mobilenetv2_100(pretrained = True, progress = True, **kwargs):
    return _mobilenetv2('mobilenetv2_1.0', 1.0,pretrained, progress, **kwargs)

