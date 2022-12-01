import json
import os
import os.path as osp
import argparse
import math
import copy
from tqdm import tqdm

import torch.nn as nn
import torch.backends.cudnn as cudnn
import torch.utils.data
from torchvision import transforms, datasets

from utils import AverageMeter, accuracy, profile
from model_zoo import ProxylessNASNets, MobileNetV3

parser = argparse.ArgumentParser()
parser.add_argument(
	'-p',
	'--path',
	help='The path of imagenet',
	type=str,
	default='/dataset/imagenet')
parser.add_argument(
	'-g',
	'--gpu',
	help='The gpu(s) to use',
	type=str,
	default='all')
parser.add_argument(
	'-b',
	'--batch-size',
	help='The batch on every device for validation',
	type=int,
	default=100)
parser.add_argument(
	'-j',
	'--workers',
	help='Number of workers',
	type=int,
	default=20)

args = parser.parse_args()
if args.gpu == 'all':
	device_list = range(torch.cuda.device_count())
	args.gpu = ','.join(str(_) for _ in device_list)
else:
	device_list = [int(_) for _ in args.gpu.split(',')]
os.environ['CUDA_VISIBLE_DEVICES'] = args.gpu
args.batch_size = args.batch_size * max(len(device_list), 1)

# build network
net_config = json.load(open('net.config', 'r'))

if net_config['name'] == ProxylessNASNets.__name__:
	net = ProxylessNASNets.build_from_config(net_config)
elif net_config['name'] == MobileNetV3.__name__:
	net = MobileNetV3.build_from_config(net_config)
else:
	raise ValueError('Not supported network type: %s' % net_config['name'])

if os.path.exists('run.config'):
	image_size = json.load(open('run.config'))['image_size']
else:
	image_size = 224

init = torch.load('init', map_location='cpu')['state_dict']
net.load_state_dict(init)

macs, _ = profile(copy.deepcopy(net), input_size=(1, 3, image_size, image_size))
macs /= 1e6
num_params = sum(p.numel() for p in net.parameters() if p.requires_grad) / 1e6
print('MAC: %.1fM, ' % macs, '#Params: %.1fM' % num_params)

data_loader = torch.utils.data.DataLoader(
	datasets.ImageFolder(
		osp.join(
			args.path,
			'val'),
		transforms.Compose(
			[
				transforms.Resize(int(math.ceil(image_size / 0.875))),
				transforms.CenterCrop(image_size),
				transforms.ToTensor(),
				transforms.Normalize(
					mean=[
						0.485,
						0.456,
						0.406],
					std=[
						0.229,
						0.224,
						0.225]),
			])),
	batch_size=args.batch_size,
	shuffle=True,
	num_workers=args.workers,
	pin_memory=True,
	drop_last=False,
)

net = torch.nn.DataParallel(net).cuda()
cudnn.benchmark = True
criterion = nn.CrossEntropyLoss().cuda()

net.eval()
losses = AverageMeter()
top1 = AverageMeter()
top5 = AverageMeter()

with torch.no_grad():
	with tqdm(total=len(data_loader), desc='Validate') as t:
		for i, (images, labels) in enumerate(data_loader):
			images, labels = images.cuda(), labels.cuda()
			# compute output
			output = net(images)
			loss = criterion(output, labels)
			# measure accuracy and record loss
			acc1, acc5 = accuracy(output, labels, topk=(1, 5))

			losses.update(loss.item(), images.size(0))
			top1.update(acc1[0].item(), images.size(0))
			top5.update(acc5[0].item(), images.size(0))
			t.set_postfix({
				'loss': losses.avg,
				'top1': top1.avg,
				'top5': top5.avg,
				'img_size': images.size(2),
			})
			t.update(1)

print('Results: loss=%.5f,\t top1=%.1f,\t top5=%.1f' % (losses.avg, top1.avg, top5.avg))
