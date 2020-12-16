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


def datatype_str_to_nptype(datatype_str):
    ret = None
    if datatype_str == "float32":
        ret = np.float32
    elif datatype_str == "int32":
        ret = np.int32
    elif datatype_str == "uint8":
        ret = np.uint8
    return ret

def transform_image(image):
    # TODO: Hardcoded ImageNet dataset mean
    image = np.array(image) - np.array([103.939, 116.779, 123.68])
    image = image / np.array([57.375, 57.12, 58.395])
    image = image.transpose((2, 0, 1))
    image = image[np.newaxis, :]
    return image

#def get_input_info(graph_str, params):
#    shape_dict = {}
#    dtype_dict = {}
#    # Use a special function to load the binary params back into a dict
#    load_arr = tvm.get_global_func("tvm.relay._load_param_dict")(params)
#    param_names = [v.name for v in load_arr]
#    graph = json.loads(graph_str)
#    for node_id in graph["arg_nodes"]:
#        node = graph["nodes"][node_id]
#        # If a node is not in the params, infer it to be an input node
#        name = node["name"]
#        if name not in param_names:
#            shape_dict[name] = graph["attrs"]["shape"][1][node_id]
#            dtype_dict[name] = graph["attrs"]["dltype"][1][node_id]
#    return shape_dict, dtype_dict

def make_inputs_dict(inputs_file, shape_dict, dtype_dict, fill_mode):
    try:
        inputs = np.load(inputs_file) if inputs_file else {}
    except IOError as ex:
        raise print("Error loading inputs file: %s" % ex)

    # First check all the keys in inputs exist in the graph
    for input_name in inputs:
        if input_name not in shape_dict.keys():
            raise print(
                "the input tensor '{}' is not in the graph. Expected inputs: '{}'".format(
                    input_name, shape_dict.keys()
                )
            )

    # Now construct the input dict, generating tensors where no
    # data already exists in 'inputs'
    inputs_dict = {}
    for input_name in shape_dict:
        print("input_name: %s" % input_name) ###
        
        if input_name in inputs.keys():
            inputs_dict[input_name] = inputs[input_name]
        else:
            shape = shape_dict[input_name]
            dtype = dtype_dict[input_name]
            data = generate_tensor_data(shape, dtype, fill_mode)
            inputs_dict[input_name] = data

    return inputs_dict

class CustomFilter(object):
    def __init__(self, *args):
        # Parse arguments
        model_path = args[0]
        input_shape = shape_str_to_npshape(args[1])
        input_type = datatype_str_to_nptype(args[2])
        output_shape = shape_str_to_npshape(args[3])
        output_type = datatype_str_to_nptype(args[4])
        if input_type is None:
            print("Invalid input_shape")
            return None
        if output_type is None:
            print("Invalid output_shape")
            return None
        self.input_dims = [nns.TensorShape(input_shape, input_type)]
        self.output_dims = [nns.TensorShape(output_shape, output_type)]

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
        input_tensor = np.reshape(input_array[0], self.input_dims[0].getDims()[::-1])[0]
        input_image = transform_image(input_tensor)
#        shape_dict, dtype_dict = get_input_info(graph, params)
#        inputs_dict = make_inputs_dict(inputs_file, shape_dict, dtype_dict, fill_mode)
        inputs_dict = {}
        inputs_dict["input0"] = input_image
        self.module.set_input(**inputs_dict)

        # Run inference
        self.module.run()

        # Get output tensor
        out = self.module.get_output(0)
        return [out.asnumpy().astype(np.float32)]
