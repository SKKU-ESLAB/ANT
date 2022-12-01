from quan.observer import default_slsq_qconfig
from model.slsq_util import slsq_Conv2d, get_slsq_static_quant_module_mappings
import torch.nn as nn
import torch



if __name__ == "__main__":
    conv = slsq_Conv2d(in_channels = 5, out_channels = 5, kernel_size = 3)
    print(conv)
    conv = nn.Sequential(
        nn.Conv2d(in_channels = 512, out_channels = 512, kernel_size = 1),
        nn.BatchNorm2d(512),
        nn.ReLU(inplace = True)
    )
    torch.ao.quantization.fuse_modules_qat(conv,[['0','1','2']], inplace = True)
    print(conv)
    conv.qconfig = default_slsq_qconfig
    torch.ao.quantization.prepare_qat(conv, inplace = True)
    print(conv)
    torch.ao.quantization.convert(conv, mapping = get_slsq_static_quant_module_mappings() , inplace = True)
    print(conv)

