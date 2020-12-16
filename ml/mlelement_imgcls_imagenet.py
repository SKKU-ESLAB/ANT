# Copyright (c) 2017-2020 SKKU ESLAB, and contributors. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import os

import tvm
from tvm import rpc
from tvm.contrib import graph_runtime as runtime

import nnstreamer_python as nns
import numpy as np

def shape_str_to_npshape(shape_str):
    shape_str_tokens = shape_str.split(":")
    return [int(token) for token in shape_str_tokens]

def shapes_str_to_npshapes(shapes_str):
    shapes_str_tokens = shapes_str.split(",")
    return [shape_str_to_npshape(token) for token in shapes_str_tokens]

def datatype_str_to_nptype(datatype_str):
    ret = None
    if datatype_str == "float32":
        ret = np.float32
    elif datatype_str == "int32":
        ret = np.int32
    elif datatype_str == "uint8":
        ret = np.uint8
    return ret

def datatypes_str_to_nptypes(datatypes_str):
    datatypes_str_tokens = datatypes_str.split(",")
    return [datatype_str_to_nptype(token) for token in datatypes_str_tokens]

def names_str_to_strarray(names_str):
    names_str_tokens = names_str.split(",")
    return [token for token in names_str_tokens]

def transform_image(image):
    # TODO: Hardcoded ImageNet dataset mean
    image = np.array(image) - np.array([103.939, 116.779, 123.68])
    image = image / np.array([57.375, 57.12, 58.395])
    image = image.transpose((2, 0, 1))
    image = image[np.newaxis, :]
    return image

class CustomFilter(object):
    def __init__(self, *args):
        # Parse arguments
        model_path = args[0]
        input_shapes = shapes_str_to_npshapes(args[1])
        input_types = datatypes_str_to_nptypes(args[2])
        output_shapes = shapes_str_to_npshapes(args[3])
        output_types = datatypes_str_to_nptypes(args[4])
        input_names = names_str_to_strarray(args[5])
        output_names = names_str_to_strarray(args[6])
        for input_type in input_types:
            if input_type is None:
                print("Invalid input_type")
                return None
        for output_type in output_types:
            if output_type is None:
                print("Invalid output_type")
                return None
        self.input_dims = []
        self.output_dims = []
        for i in range(len(input_shapes)):
            input_dim = nns.TensorShape(input_shapes[i], input_types[i])
            self.input_dims.append(input_dim)
        for i in range(len(output_shapes)):
            output_dim = nns.TensorShape(output_shapes[i], output_types[i])
            self.output_dims.append(output_dim)
        self.input_names = input_names
        self.output_names = output_names

        # Initialize TVM runtime session with given binary
        session = rpc.LocalSession()
        session.upload(os.path.join(model_path, "mod.so"))
        lib = session.load_module("mod.so")
        ctx = session.cpu() # TODO: Hardcoded CPU backend

        # Load graph and create a module
        self.graph = open(os.path.join(model_path, "mod.json")).read()
        self.module = runtime.create(self.graph, lib, ctx)

        # Load params
        self.params = bytearray(open(os.path.join(model_path, "mod.params"), "rb").read())
        self.module.load_params(self.params)
        return None

    def getInputDim(self):
        # pylint: disable=invalid-name
        return self.input_dims

    def getOutputDim(self):
        # pylint: disable=invalid-name
        return self.output_dims

    def invoke(self, input_array):
        graph = self.graph
        params = self.params
        fill_mode = "random"

        # Setting input
        inputs_dict = {}
        for i in range(len(self.input_dims)):
            input_element = input_array[i]
            input_dim = self.input_dims[i]
            input_name = self.input_names[i]

            input_tensor = np.reshape(input_element, input_dim.getDims()[::-1])[i]
            input_image = transform_image(input_tensor)
            inputs_dict[input_name] = input_image
            self.module.set_input(**inputs_dict)

        # Run inference
        self.module.run()

        # Get output tensors
        outputs = []
        for i in range(len(self.output_dims)):
            output_element = self.module.get_output(i)
            nptype = self.output_dims[i].getType()
            outputs.append(output_element.asnumpy().astype(nptype))

        return outputs
