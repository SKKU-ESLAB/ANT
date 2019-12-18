import tvm
from tvm.contrib import graph_runtime
import time

import nnstreamer_python as nns

import numpy as np
import cv2


def shape_str_to_npshape(shape_str):
    shape_str_tokens = shape_str.split(":")
    return [int(token) for token in shape_str_tokens]


def datatype_str_to_nptype(datatype_str):
    if datatype_str == "float32":
        return np.float32
    elif datatype_str == "int32":
        return np.int32
    elif datatype_str == "uint8":
        return np.uint8
    else:
        return None


class CustomFilter(object):
    def __init__(self, *args):
        # TVM
        ctx = tvm.cpu()  # Hardcoded backend

        model_path = args[0] + '/'
        input_shape = shape_str_to_npshape(args[1])
        input_type = datatype_str_to_nptype(args[2])
        output_shape = shape_str_to_npshape(args[3])
        output_type = datatype_str_to_nptype(args[4])

        if input_type == None:
            print("Invalid input_shape")
            return None
        elif output_type == None:
            print("Invalid output_shape")
            return None
        self.input_dims = [nns.TensorShape(input_shape, input_type)]
        self.output_dims = [nns.TensorShape(output_shape, output_type)]

        loaded_json = open(model_path + 'deploy_graph.json').read()
        loaded_lib = tvm.module.load(model_path + 'deploy_lib.tar')
        loaded_params = bytearray(
            open(model_path + 'deploy_param.params', 'rb').read())

        self.module = graph_runtime.create(loaded_json, loaded_lib, ctx)
        self.module.load_params(loaded_params)

    def getInputDim(self):
        return self.input_dims

    def getOutputDim(self):
        return self.output_dims

    def invoke(self, input_array):
        preprocess_start_ts = time.time()

        input_tensor = np.reshape(
            input_array[0], self.input_dims[0].getDims()[::-1])[0]
        image = input_tensor

        input_image = transform_image(input_tensor)

        process_start_ts = time.time()

        self.module.run(data=input_image)

        out = self.module.get_output(0)

        process_end_ts = time.time()

        # print("Preprocessing(reshape+transform): %f ms / Inference(TVM): %f ms"
        #  % ((process_start_ts - preprocess_start_ts) / 1000,
        #     (process_end_ts - process_start_ts) / 1000))

        return [out.asnumpy().astype(np.float32)]


def transform_image(image):
    # TODO: Hardcoded image mean
    image = np.array(image) - np.array([103.939, 116.779, 123.68])
    image = image / np.array([57.375, 57.12, 58.395])
    image = image.transpose((2, 0, 1))
    image = image[np.newaxis, :]
    return image
