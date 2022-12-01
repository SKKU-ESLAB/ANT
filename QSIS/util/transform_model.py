import torch
import copy
from quan import *
from model import *
@torch.no_grad()
def transform_model(model, args):
    model_clone,_ = prepare_qat_model(args,args.arch, pre_trained = False, mode = args.mode, distillation = False)
    model_clone.load_state_dict(model.state_dict())
    '''
    for n,m in model.named_modules():
        if hasattr(m, "soft_mask"):
            m.soft_mask = None

    model_clone = copy.deepcopy(model)
    '''

    model_clone.apply(hard_pruning_mode)
    '''
    if args.mode == "slsq":
        for n, m in model_clone.named_modules():
            if hasattr(m, "weight_fake_quant"):
                m.weight.data.copy_(m.weight_fake_quant(m.weight))
    '''
    for n, m in model_clone.named_modules():
        if not ("classifier" in n ) and hasattr(m, "p"):
            m.p.data.clamp_(torch.zeros_like(m.p), m.c.data)
    return model_clone

