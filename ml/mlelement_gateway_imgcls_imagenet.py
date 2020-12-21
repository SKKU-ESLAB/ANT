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
import os

import nnstreamer_python as nns
import numpy as np
from keras.applications import mobilenet

import fragment_runner as runner
import antml_util as util

class CustomFilter(object):
    def __init__(self, *args):
        # Parse arguments
        model_path = args[0]
        input_shapes = util.shapes_str_to_npshapes(args[1])
        input_types = util.datatypes_str_to_nptypes(args[2])
        input_names = util.names_str_to_strarray(args[3])
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
        for i, input_shape in enumerate(input_shapes):
            input_dim = nns.TensorShape(input_shape, input_types[i])
            self.input_dims.append(input_dim)
        output_dim = nns.TensorShape([1, 1, 1, 1], np.int32)
        self.output_dims = [output_dim]
        self.input_names = input_names
        self.num_fragments = num_fragments
        self.target_uri = target_uri

        # Initialize fragment runner
        self.offload_from = num_fragments - 1  # Initial setting
        model_path_head = os.path.join(model_path, 'mod')
        self.interpreters = runner.load_model(model_path_head, num_fragments)

        # Connect to target
        self.is_connected = False
        self.connect_to_target()
        return None

    def getInputDim(self):
        # pylint: disable=invalid-name
        return self.input_dims

    def getOutputDim(self):
        # pylint: disable=invalid-name
        return self.output_dims

    def connect_to_target(self):
        print("Trying to connect to: {}".format(self.target_uri))
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        target_ip_addr = self.target_uri[:self.target_uri.find(":")]
        target_port = int(self.target_uri[(self.target_uri.find(":")+1):])
        try:
            self.client_socket.connect((target_ip_addr, target_port))
            self.is_connected = True
        except socket.timeout:
            return False
        return True

    def invoke(self, input_array):
        try:
            retdata = np.array([0]).reshape([1, 1, 1, 1]).astype(np.int32)
            if self.is_connected is False:
                self.connect_to_target()
                return [retdata]

            # inference locally
            head_from = 0
            head_to = self.offload_from - 1
            input_tensor = input_array[0]
            input_tensor = np.reshape(
                input_tensor, self.input_dims[0].getDims()[::-1])[0]
            head_in_tensor = mobilenet.preprocess_input(input_tensor)
            head_out_tensor = runner.run_fragments(self.interpreters,
                                                   head_in_tensor, head_from, head_to)

            # send the output tensor to the target URI
            tail_from = self.offload_from

            payload_buffer = head_out_tensor.tobytes()

            payload_length = len(payload_buffer)
            payload_length_buffer = payload_length.to_bytes(4, 'big')

            tail_from_buffer = tail_from.to_bytes(4, 'big')

            self.client_socket.send(payload_length_buffer)
            self.client_socket.send(tail_from_buffer)
            self.client_socket.send(payload_buffer)

            result_buffer = self.client_socket.recv(4)
            result = int.from_bytes(result_buffer, byteorder='big')
            if 0 <= result < self.num_fragments:
                self.offload_from = result

        except socket_error as serr:
            if serr.errno == errno.ECONNREFUSED:
                self.is_connected = False
            if serr.errno != errno.ECONNREFUSED:
                raise serr

        return [retdata]
