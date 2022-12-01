import torch
import json

class dataloader_arguments:
    def __init__(self, dataset = "imagenet", num_classes = 1000, path = "/data/imagenet", batch_size = 256,  
                workers = 16, pin_memory = True, val_split = 0., deterministic = True, serialized = False):
        super(dataloader_arguments, self).__init__()
        self.dataset = dataset
        self.num_classes = num_classes
        self.path = path
        self.batch_size = batch_size
        self.workers = workers
        self.val_split = val_split
        self.pin_memory = pin_memory
        self.deterministic = deterministic
        self.serialized = serialized
    def __repr__(self):
        return str(self.__dict__)
class scheduler_arguments:
    def __init__(self, update_per_batch = True, mode = "multi_step", milestones = [30, 60, 90], gamma = 0.1):
        super(scheduler_arguments, self).__init__()
        self.update_per_batch = update_per_batch
        self.mode = mode
        self.milestones = milestones
        self.gamma = gamma

    def __repr__(self):
        return str(self.__dict__)
class optimizer_arguments:
    def __init__(self, learning_rate = 0.01, momentum = 0.9, weight_decay = 0.0001):
        super(optimizer_arguments, self).__init__()
        self.learning_rate = learning_rate
        self.momentum = momentum
        self.weight_decay = weight_decay

    def __repr__(self):
        return str(self.__dict__)
class training_arguments:
    def __init__(self, name, arch = 'resnet18',device_type = "cuda", n_gpu = 1, log_num_best_scores = 3, resume = False,
                 resume_path = "", pre_trained = True, epochs = 120, log_print_freq = 20, output_dir = './out',
                device_gpu = [0], optimizer = optimizer_arguments(), scheduler = scheduler_arguments(),
                dataloader = dataloader_arguments(), log_dir = './', mode = 'lsq'):
        super(training_arguments,self).__init__()
        self.arch = arch
        self.device_gpu = device_gpu
        self.name = name
        self.n_gpu = n_gpu
        self.device_type = device_type
        self.log_print_freq = log_print_freq
        self.resume_path = resume_path
        self.pre_trained = pre_trained
        self.epochs = epochs
        self.log_print_freq = log_print_freq
        self.output_dir = output_dir
        self.scheduler = scheduler
        self.optimizer = optimizer
        self.dataloader = dataloader
        self.resume = resume
        self.log_num_best_scores = log_num_best_scores
        self.log_dir = log_dir
        self.mode = mode
    def __repr__(self):
        return str(self.__dict__)
