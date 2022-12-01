# Once for All: Train One Network and Specialize it for Efficient Deployment
# Han Cai, Chuang Gan, Tianzhe Wang, Zhekai Zhang, Song Han
# International Conference on Learning Representations (ICLR), 2020.

import torch
import torch.nn as nn
from .layers import MyModule, ZeroLayer, set_layer_from_config
from .utils import MyNetwork
from torch.ao.quantization import QuantStub, DeQuantStub

class MobileInvertedResidualBlock(MyModule):

    def __init__(self, mobile_inverted_conv, shortcut):
        super(MobileInvertedResidualBlock, self).__init__()

        self.mobile_inverted_conv = mobile_inverted_conv
        self.shortcut = shortcut
        self.skip_add = nn.quantized.FloatFunctional()

    def forward(self, x):
        if self.mobile_inverted_conv is None or isinstance(self.mobile_inverted_conv, ZeroLayer):
            assert self.mobile_inverted_conv is None or isinstance(self.mobile_inverted_conv, ZeroLayer)
            res = x
        elif self.shortcut is None or isinstance(self.shortcut, ZeroLayer):
            assert self.shortcut is None or isinstance(self.shortcut, ZeroLayer)
            res = self.mobile_inverted_conv(x)
        else:
            res = self.skip_add.add(self.mobile_inverted_conv(x), self.shortcut(x))
            #res = self.mobile_inverted_conv(x) + self.shortcut(x)
        return res
    '''
    @property
    def module_str(self):
        return '(%s, %s)' % (
            self.mobile_inverted_conv.module_str if self.mobile_inverted_conv is not None else None,
            self.shortcut.module_str if self.shortcut is not None else None
        )

    @property
    def config(self):
        return {
            'name': MobileInvertedResidualBlock.__name__,
            'mobile_inverted_conv': self.mobile_inverted_conv.config if self.mobile_inverted_conv is not None else None,
            'shortcut': self.shortcut.config if self.shortcut is not None else None,
        }
    '''
    @torch.jit.export
    @staticmethod
    def build_from_config(config):
        mobile_inverted_conv = set_layer_from_config(config['mobile_inverted_conv'])
        shortcut = set_layer_from_config(config['shortcut'])
        return MobileInvertedResidualBlock(mobile_inverted_conv, shortcut)

class MobileNetV3(MyNetwork):

    def __init__(self, first_conv, blocks, final_expand_layer, feature_mix_layer, classifier):
        super(MobileNetV3, self).__init__()

        self.first_conv = first_conv
        self.blocks = nn.ModuleList(blocks)
        self.final_expand_layer = final_expand_layer
        self.feature_mix_layer = feature_mix_layer
        self.classifier = classifier
        self.quant = QuantStub()
        self.dequant = DeQuantStub()
        self.avg_pool = torch.nn.AdaptiveAvgPool2d(1)

    def forward(self, x):
        x = self.first_conv(x)
        x = self.quant(x)
        for block in self.blocks:
            x = block(x)
        x = self.final_expand_layer(x)
        #x = x.mean(3, keepdim=True).mean(2, keepdim=True)  # global average pooling
        x = self.avg_pool(x)
        x = self.feature_mix_layer(x)
        x = x.flatten(1)
        x = self.dequant(x)
        x = self.classifier(x)
        return x

    def fuse_model(self):
        for module_name, module in self.named_children():
            if "first" in module_name or "final" in module_name:
                torch.ao.quantization.fuse_modules_qat(module, [["conv", "bn"]], inplace = True)
            else:
                for basic_block_name, basic_block in module.named_children():
                    for bbn, bb in basic_block.named_children():
                        if bbn == "mobile_inverted_conv":
                            for bbbn, bbb in bb.named_children():
                                if hasattr(bbb, "act") and isinstance(bbb.act, nn.ReLU):
                                    torch.ao.quantization.fuse_modules_qat(bbb, [["conv", "bn", "act"]], inplace = True)
                                else:
                                    torch.ao.quantization.fuse_modules_qat(bbb, [["conv", "bn"]], inplace = True)
                                if bbbn == "depth_conv":
                                    for bbbbn, bbbb in bbb.named_children():
                                        if hasattr(bbbb, "fc"):
                                            torch.ao.quantization.fuse_modules_qat(bbbb.fc, [["reduce", "relu"]], inplace = True)
        print(self)
    '''
    @property
    def module_str(self):
        _str = self.first_conv.module_str + '\n'
        for block in self.blocks:
            _str += block.module_str + '\n'
        _str += self.final_expand_layer.module_str + '\n'
        _str += self.feature_mix_layer.module_str + '\n'
        _str += self.classifier.module_str
        return _str

    @property
    def config(self):
        return {
            'name': MobileNetV3.__name__,
            'bn': self.get_bn_param(),
            'first_conv': self.first_conv.config,
            'blocks': [
                block.config for block in self.blocks
            ],
            'final_expand_layer': self.final_expand_layer.config,
            'feature_mix_layer': self.feature_mix_layer.config,
            'classifier': self.classifier.config,
        }
    '''
    @torch.jit.export
    @staticmethod
    def build_from_config(config):
        first_conv = set_layer_from_config(config['first_conv'])
        final_expand_layer = set_layer_from_config(config['final_expand_layer'])
        feature_mix_layer = set_layer_from_config(config['feature_mix_layer'])
        classifier = set_layer_from_config(config['classifier'])

        blocks = []
        for block_config in config['blocks']:
            blocks.append(MobileInvertedResidualBlock.build_from_config(block_config))

        net = MobileNetV3(first_conv, blocks, final_expand_layer, feature_mix_layer, classifier)
        if 'bn' in config:
            net.set_bn_param(**config['bn'])
        else:
            net.set_bn_param(momentum=0.1, eps=1e-3)

        return net

class ProxylessNASNets(MyNetwork):

    def __init__(self, first_conv, blocks, feature_mix_layer, classifier):
        super(ProxylessNASNets, self).__init__()

        self.first_conv = first_conv
        self.blocks = nn.ModuleList(blocks)
        self.feature_mix_layer = feature_mix_layer
        self.classifier = classifier

    def forward(self, x):
        x = self.first_conv(x)
        for block in self.blocks:
            x = block(x)
        if self.feature_mix_layer is not None:
            x = self.feature_mix_layer(x)
        x = x.mean(3).mean(2)
        x = self.classifier(x)
        return x
    '''
    @property
    def module_str(self):
        _str = self.first_conv.module_str + '\n'
        for block in self.blocks:
            _str += block.module_str + '\n'
        _str += self.feature_mix_layer.module_str + '\n'
        _str += self.classifier.module_str
        return _str

    @property
    def config(self):
        return {
            'name': ProxylessNASNets.__name__,
            'bn': self.get_bn_param(),
            'first_conv': self.first_conv.config,
            'blocks': [
                block.config for block in self.blocks
            ],
            'feature_mix_layer': None if self.feature_mix_layer is None else self.feature_mix_layer.config,
            'classifier': self.classifier.config,
        }
    '''
    @torch.jit.export
    @staticmethod
    def build_from_config(config):
        first_conv = set_layer_from_config(config['first_conv'])
        feature_mix_layer = set_layer_from_config(config['feature_mix_layer'])
        classifier = set_layer_from_config(config['classifier'])

        blocks = []
        for block_config in config['blocks']:
            blocks.append(MobileInvertedResidualBlock.build_from_config(block_config))

        net = ProxylessNASNets(first_conv, blocks, feature_mix_layer, classifier)
        if 'bn' in config:
            net.set_bn_param(**config['bn'])
        else:
            net.set_bn_param(momentum=0.1, eps=1e-3)

        return net

