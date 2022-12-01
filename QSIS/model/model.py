import logging

from .resnet import *
from .resnet_cifar import *
from .mobilenet import *
from .mobilenet_cifar10 import *
from quan import *
import copy
import json
from .once_for_all import *
def create_model(args, model_name = "resnet", pre_trained = True):
    logger = logging.getLogger()
    model = None
    if model_name == 'resnet18' or model_name == 'resnet':
        model = resnet18(pretrained=pre_trained)
    elif model_name == 'resnet34':
        model = resnet34(pretrained=pre_trained)
    elif model_name == 'resnet50':
        model = resnet50(pretrained=pre_trained)
    elif model_name == 'resnet101':
        model = resnet101(pretrained=pre_trained)
    elif model_name == 'resnet152':
        model = resnet152(pretrained=pre_trained)

    elif model_name == 'resnet20':
        model = resnet20(pretrained=pre_trained)
    elif model_name == 'resnet32':
        model = resnet32(pretrained=pre_trained)
    elif model_name == 'resnet44':
        model = resnet44(pretrained=pre_trained)
    elif model_name == 'resnet56':
        model = resnet56(pretrained=pre_trained)
    elif model_name == 'resnet110':
        model = resnet152(pretrained=pre_trained)
    elif model_name == 'resnet1202':
        model = resnet1202(pretrained=pre_trained)
    
    if (model_name == 'MobileNetv2' or 'mobilenetv2') and args.dataloader.dataset == "cifar10":
        model = mobilenetv2_cifar10(pre_trained = pre_trained)
    elif model_name == 'MobileNetv2' or model_name == 'mobilenetv2':
        model = mobilenetv2(pretrained = pre_trained)
    elif model_name == 'mobilenetv2_0.1':
        model = mobilenetv2_01(pretrained=pre_trained)
    elif model_name == 'mobilenetv2_0.25':
        model = mobilenetv2_25(pretrained=pre_trained)
    elif model_name == 'mobilenetv2_0.35':
        model = mobilenetv2_35(pretrained=pre_trained)
    elif model_name == 'mobilenetv2_0.5':
        model = mobilenetv2_50(pretrained=pre_trained)
    elif model_name == 'mobilenetv2_0.75':
        model = mobilenetv2_75(pretrained=pre_trained)
    elif model_name == 'mobilenetv2_1.0':
        model = mobilenetv2_100(pretrained=pre_trained)
    
    if model_name == "once_for_all":
        net_config = json.load(open('net.config', 'r'))
        model = MobileNetV3.build_from_config(net_config)

        init = torch.load('init', map_location = 'cpu')['state_dict']
        model.load_state_dict(init)

    if model is None:
        logger.error('Model architecture `%s` is not supported' % (model_name))
        exit(-1)

    msg = 'Created `%s` model' % (model_name)
    msg += '\n          Use pre-trained model = %s' % pre_trained
    logger.info(msg)

    return model

def prepare_qat_model(args, model_name = 'resnet', pre_trained = True, mode = "lsq", distillation = False):
    model = create_model(args, model_name, pre_trained)
    if distillation:
        teacher_model = copy.deepcopy(model)
    if mode == "lsq":
        qconfig = default_lsq_qconfig
        model.qconfig = qconfig
        model.fuse_model()
        model.train()
        for n, m in model.named_children():
            if not ("first" in n or "classifier" in n):
                m.qconfig = qconfig
                torch.ao.quantization.prepare_qat(m, inplace = True)
                for n1, m1 in m.named_children():
                    m1.qconfig = qconfig
                    torch.ao.quantization.prepare_qat(m1, inplace = True)
    elif mode == "qil":
            qconfig = default_qil_qconfig
            model.qconfig = qconfig
            model.fuse_model()
            model.train()
            for n, m in model.named_children():
                if "layer" in n or "quant" in n or "fc" in n or "conv" in n or "classifier" in n:
                    m.qconfig = qconfig
                    torch.ao.quantization.prepare_qat(m, inplace = True)
    elif mode == "slsq":
        qconfig = default_slsq_qconfig
        model.qconfig = qconfig
        model.fuse_model()
        model.train()
        for n, m in model.named_children():
            if not ("first" in n or "classifier" in n):
                m.qconfig = qconfig
                torch.ao.quantization.prepare_qat(m, inplace = True)
                for n1, m1 in m.named_children():
                    m1.qconfig = qconfig
                    torch.ao.quantization.prepare_qat(m1, inplace = True)
    if distillation:
        return model, teacher_model
    print(model)
    return model, None

