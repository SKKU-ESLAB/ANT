import tvm
from tvm.contrib import graph_runtime

import nnstreamer_python as nns

import numpy as np
import cv2

class CustomFilter(object):
    def __init__(self, *args):
        self.input_dims = [nns.TensorShape([3, 224, 224], np.uint8)]
        self.output_dims = [nns.TensorShape([1000, 1, 1, 1], np.float32)]

        # TVM
        ctx = tvm.cpu()

        batch_size = 1
        image_shape = (3, 224, 224)
        data_shape = (batch_size, ) + image_shape

        path = args[0] + '/'
        loaded_json = open(path + 'deploy_graph.json').read()
        loaded_lib = tvm.module.load(path + 'deploy_lib.tar')
        loaded_params = bytearray(open(path + 'deploy_param.params', 'rb').read())

        self.module = graph_runtime.create(loaded_json, loaded_lib, ctx)
        self.module.load_params(loaded_params)

    def getInputDim(self):
        return self.input_dims

    def getOutputDim(self):
        return self.output_dims

    def invoke(self, input_array):
        input_tensor = np.reshape(input_array[0], self.input_dims[0].getDims()[::-1])[0]
        print(input_tensor.shape)
        image = input_tensor
        print(np.mean(image))

        input_image = transform_image(input_tensor)
        self.module.run(data=input_image)

        out = self.module.get_output(0)
        print(out.shape)

        return [out.asnumpy().astype(np.float32)]


def transform_image(image):
    image = np.array(image) - np.array([103.939, 116.779, 123.68])
    image = image.transpose((2, 0, 1))
    image = image[np.newaxis, :]
    return image
