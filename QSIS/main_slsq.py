import logging
from pathlib import Path

import torch
import trainer
import quan
import util
from model import *
import yaml
import wandb
import random
import numpy as np
import os 
def set_seed(seed: int):
    random.seed(seed)
    np.random.seed(seed)
    torch.manual_seed(seed)
    if torch.cuda.device_count() > 0:
        torch.cuda.manual_seed_all(seed)
    torch.backends.cudnn.deterministic = True
    os.environ['PYTHONHASHSEED'] = str(seed)

def main():
    script_dir = Path.cwd()
    set_seed(42)
    data_args = trainer.dataloader_arguments(dataset = "imagenet", num_classes = 1000, path = "/data/imagenet", batch_size = 128)
    optimizer_args = trainer.optimizer_arguments(weight_decay = 4e-5, learning_rate = 1e-4)
    args = trainer.training_arguments(name = "88",device_gpu = [0], optimizer = optimizer_args, dataloader = data_args, arch = 'once_for_all', mode = "slsq", epochs = 70)
    output_dir = script_dir / args.output_dir
    output_dir.mkdir(exist_ok = True)
    args.lamb = 0.1
    args.apex = False
    log_dir = util.init_logger(args.name, output_dir, script_dir / 'logging.conf')
    args.log_dir = log_dir
    logger = logging.getLogger()
    
    experimental_name = str(args.dataloader.dataset) + str(args.arch) + str(args.name) + str(args.lamb)
    with open(log_dir / 'args.yaml',"w") as yaml_file:
        yaml.dump(args, yaml_file)

    wandb.init(reinit=True, name = experimental_name, project="SLSQ_Quant_Pytorch")
    pymonitor = util.ProgressMonitor(logger)
    monitors = [pymonitor]
    if args.device_type == "cpu" or not torch.cuda.is_available() or args.device_gpu == []:
        args.device_gpu = []
    else:
        available_gpu = torch.cuda.device_count()
        for dev_id in args.device_gpu:
            if dev_id >= available_gpu:
                logger.error('GPU device ID {0} requested, but only {1} devices available'.format(dev_id, available_gpu))
                exit(1)
        torch.cuda.set_device(args.device_gpu[0])

    train_loader, val_loader, test_loader = util.load_data(args.dataloader)

    logger.info('Dataset `%s` size:' % args.dataloader.dataset +
                '\n          Training Set = %d (%d)' % (len(train_loader.sampler), len(train_loader)) +
                '\n        Validation Set = %d (%d)' % (len(val_loader.sampler), len(val_loader)) +
                '\n              Test Set = %d (%d)' % (len(test_loader.sampler), len(test_loader)))

    #qat_model,teacher_model = prepare_qat_model(args = args,model_name = args.arch, pre_trained = True, mode = args.mode, distillation = True)
    qat_model, teacher_model= prepare_qat_model(args = args, model_name = args.arch, pre_trained = True, mode = args.mode, distillation = False)
    criterion = torch.nn.CrossEntropyLoss().to(args.device_type)
    
    optimizer = torch.optim.AdamW(qat_model.parameters(), lr = args.optimizer.learning_rate, 
                                weight_decay = args.optimizer.weight_decay)
    #lr_scheduler = util.lr_scheduler(optimizer, batch_size = train_loader.batch_size, 
    #                                num_samples = len(train_loader.sampler),
    #                                **(args.scheduler.__dict__))
    lr_scheduler = util.lr_scheduler(optimizer, batch_size = train_loader.batch_size,
            num_samples = len(train_loader.sampler),
            mode = "cos_warm_restarts", cycle = 10, update_per_batch = True)

    logger.info(('Optimizer: %s' % optimizer).replace('\n', '\n' + ' ' * 11))
    logger.info('LR scheduler: %s\n' % lr_scheduler)
    print("*************soft_pruning_mode*******************")
    trainer.train_qat_slsq(train_loader, val_loader, test_loader,qat_model, teacher_model,criterion, 
                            optimizer, lr_scheduler, args.epochs, monitors, args, init_qparams = True, 
                            hard_pruning = False)
    print("*************hard_pruning_mode*******************")
    optimizer = torch.optim.AdamW(qat_model.parameters(), lr = args.optimizer.learning_rate, 
                                weight_decay = args.optimizer.weight_decay)

    lr_scheduler = util.lr_scheduler(optimizer, batch_size = train_loader.batch_size,
            num_samples = len(train_loader.sampler),
            mode = "cos_warm_restarts", cycle = 10, update_per_batch = True)
    trainer.train_qat_slsq(train_loader, val_loader, test_loader, qat_model, teacher_model, criterion, 
            optimizer, lr_scheduler, args.epochs, monitors, args, init_qparams = False, hard_pruning = True)

if __name__ == "__main__":
    main()
