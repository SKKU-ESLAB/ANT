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
    return list(names_str_tokens)
