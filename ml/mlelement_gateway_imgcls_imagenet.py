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

import errno
import socket
from socket import error as socket_error
import time
import os

import nnstreamer_python as nns
import numpy as np
from keras.preprocessing import image
from keras.applications import imagenet_utils, mobilenet

import fragment_runner as runner

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
        input_names = names_str_to_strarray(args[3])
        num_fragments = int(args[4])
        target_uri = args[5]

        for input_type in input_types:
            if input_type is None:
                print("Invalid input_type")
                return None
        if (len(input_shapes) > 4 or len(input_types) > 4 or len(input_names) > 4
                or len(input_shapes) != len(input_types)
                or len(input_shapes) != len(input_names)):
            print("Invalid input count: (%d,%d,%d)".format(
                len(input_shapes), len(input_types), len(input_names)))
            return None
        self.input_dims = []
        for i in range(len(input_shapes)):
            input_dim = nns.TensorShape(input_shapes[i], input_types[i])
            self.input_dims.append(input_dim)
        self.input_names = input_names
        self.num_fragments = num_fragments
        self.target_uri = target_uri

        # Initialize fragment runner
        self.offload_from = num_fragments - 1 # Initial setting
        model_path_head = os.path.join(model_path, 'mod')
        self.interpreters = runner.load_model(model_path_head, num_fragments)

        # Connect to target
        self.connectToTarget()

        return None

    def getInputDim(self):
        # pylint: disable=invalid-name
        return self.input_dims

#    def getOutputDim(self):
#        # pylint: disable=invalid-name
#        return self.output_dims

    def connectToTarget(self):
        print("Trying to connect to: {}".format(self.target_uri))
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        target_ip_addr = self.target_uri[:self.target_uri.find(":")]
        target_port = int(self.target_uri[(self.target_uri.find(":")+1):])
        try:
            self.client_socket.timeout(1.0)
            self.client_socket.connect((target_ip_addr, target_port))
        except socket.timeout:
            return False
        return True

    def invoke(self, input_array):
        try:
            if self.client_socket is None:
                isConnected = connetToTarget()
                if isConnected is False:
                    data = [ 0 ]
                    return np.array(data)

                # inference locally
                head_from = 0
                head_to = self.offload_from - 1
                head_in_tensor = process_image_for_mobilenet_imagenet("test.jpg")
                head_out_tensor = runner.run_fragments(self.interpreters, head_in_tensor, head_from, head_to)
                output_tensor = runner.run_fragments(self.interpreters,
                        input_array[0], 0, self.offload_from - 1)

                # send the output tensor to the target URI
                tail_from = self.offload_from
                tail_to = num_fragments - 1

                payload_buffer = head_out_tensor.tobytes()

                payload_length = len(payload_buffer)
                payload_length_buffer = payload_length.to_bytes(4, 'big')

                tail_from_buffer = tail_from.to_bytes(4, 'big')

                client_socket.send(payload_length_buffer)
                client_socket.send(tail_from_buffer)
                client_socket.send(payload_buffer)

                result_buffer = client_socket.recv(4)
                result = int.from_bytes(result_buffer, byteorder='big')
                if result >= 0 and result < num_fragments:
                    self.offload_from = result

        except socket_error as serr:
            if serr.errno != errno.ECONNREFUSED:
                raise serr
            else:
                self.client_socket = None

        return output_tensor
