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

import numpy as np

from tvm import rpc
from tvm.contrib import graph_runtime as runtime

import nnstreamer_python as nns

import antml_util as util


def transform_image(image):
    # TODO: Hardcoded ImageNet dataset mean
    mean = np.array([0.485, 0.456, 0.406])
    std = np.array([0.229, 0.224, 0.225])

    image = (image / 255 - mean) / std
    image = image.transpose((2, 0, 1))
    image = image[np.newaxis, :]
    return image


def nms(self, detected):
    threshold_iou = 0.5
    detected = sorted(detected, key=lambda a: a['prob'])
    boxes_size = len(detected)

    _del = [False for _ in range(boxes_size)]

    for i in range(boxes_size):
        if not _del[i]:
            for j in range(i + 1, boxes_size):
                if self.iou(detected[i], detected[j]) > threshold_iou:
                    _del[j] = True

    # update result
    self.detected_objects.clear()

    for i in range(boxes_size):
        if not _del[i]:
            self.detected_objects.append(detected[i])


class CustomFilter(object):
    def __init__(self, *args):
        # Parse arguments
        print("\nML element I/O spec: \n", args, '\n')
        model_path = args[0]
        input_shapes = util.shapes_str_to_npshapes(args[1])
        input_types = util.datatypes_str_to_nptypes(args[2])
        output_shapes = util.shapes_str_to_npshapes(args[3])
        output_types = util.datatypes_str_to_nptypes(args[4])
        input_names = util.names_str_to_strarray(args[5])
        output_names = util.names_str_to_strarray(args[6])
        self.input_shapes = input_shapes
        for input_type in input_types:
            if input_type is None:
                print("Invalid input_type")
                return None
        for output_type in output_types:
            if output_type is None:
                print("Invalid output_type")
                return None
        if (len(input_shapes) > 4 or len(input_types) > 4
                or len(input_names) > 4
                or len(input_shapes) != len(input_types)
                or len(input_shapes) != len(input_names)):
            print("Invalid input count: (%d,%d,%d)".format(
                len(input_shapes), len(input_types), len(input_names)))
            return None
        if (len(output_shapes) > 4 or len(output_types) > 4
                or len(output_names) > 4
                or len(output_shapes) != len(output_types)
                or len(output_shapes) != len(output_names)):
            print("Invalid output count: (%d,%d,%d)".format(
                len(output_shapes), len(output_types), len(output_names)))
            return None
        self.input_dims = []
        self.output_dims = []
        self.input_types = input_types
        self.output_types = output_types
        for i, input_shape in enumerate(input_shapes):
            input_dim = nns.TensorShape(input_shape, input_types[i])
            self.input_dims.append(input_dim)
        for i, output_shape in enumerate(output_shapes):
            output_dim = nns.TensorShape(output_shape, output_types[i])
            self.output_dims.append(output_dim)
        self.input_names = input_names
        self.output_names = output_names

        # Initialize TVM runtime session with given binary
        session = rpc.LocalSession()
        session.upload(os.path.join(model_path, "mod.so"))
        lib = session.load_module("mod.so")
        ctx = session.cpu()  # TODO: Hardcoded CPU backend

        # Load graph and create a module
        self.graph = open(os.path.join(model_path, "mod.json")).read()
        self.module = runtime.create(self.graph, lib, ctx)
        self.ctx = ctx

        # Load params
        self.params = bytearray(
            open(os.path.join(model_path, "mod.params"), "rb").read())
        self.module.load_params(self.params)
        return None

    def getInputDim(self):
        # pylint: disable=invalid-name
        return self.input_dims

    def getOutputDim(self):
        # pylint: disable=invalid-name
        return self.output_dims

    def invoke(self, input_array):
        size = 512
        jump_output = True

        # Setting input
        inputs_dict = {}
        for i in range(len(self.input_dims)):
            input_element = input_array[i]
            input_dim = self.input_dims[i]
            input_name = self.input_names[i]

            input_tensor = np.reshape(input_element,
                                      input_dim.getDims()[::-1])[i]
            input_image = transform_image(input_tensor)
            inputs_dict[input_name] = input_image
        self.module.set_input(**inputs_dict)

        # Run inference
        self.module.run()

        # Get output tensors
        outputs = []
        num_outputs = self.module.get_num_outputs()
        if jump_output:
            _range_ = range(5, 8) if num_outputs > 5 else range(num_outputs)
        else:
            _range_ = range(num_outputs)
        for i in _range_:
            output_element = self.module.get_output(i)
            nptype = self.output_types[0]
            outputs.append(output_element.asnumpy().astype(nptype))

        app_output = postprocess_numpy(
            (1, 3, size, size),
            outputs[2],
            outputs[0],
            outputs[1],
            # anchors, regression, classification
            0.4,
            0.1)
        return app_output

# For Single-shot detector


def postprocessing_ssd(outputs):
    classes = outputs[0]  # shape: np (1, 100, 1) : js (10, 1, 1, 1)
    scores = outputs[1]  # shape: np (1, 100, 1) : js (10, 1, 1, 1)
    bboxes = outputs[2]  # shape: np (1, 100, 4) : js (4, 10, 1, 1)

    thresh = 0.4
    count = 0
    for i, _ in enumerate(bboxes[0]):
        if i == 10:
            break
        if scores[0][i][0] > thresh:
            count += 1

    sel_count = np.array([count], dtype=np.float32).reshape([1, 1, 1])
    app_output = [
        sel_count, classes[:, :10], scores[:, :10], bboxes[:, :10]
    ]
    return app_output


def bbox_transform(anchors, regression):
    """
    decode_box_outputs adapted from
    https://github.com/google/automl/blob/master/efficientdet/anchors.py

    Args:
        anchors: [batchsize, boxes, (boxes_y1, boxes_x1, boxes_y2, boxes_x2)]
        regression: [batchsize, boxes, (dy, dx, dh, dw)]

    Returns:

    """
    y_centers_a = (anchors[..., 0] + anchors[..., 2]) / 2
    x_centers_a = (anchors[..., 1] + anchors[..., 3]) / 2
    height_a = anchors[..., 2] - anchors[..., 0]
    width_a = anchors[..., 3] - anchors[..., 1]

    width = np.exp(regression[..., 3]) * width_a
    height = np.exp(regression[..., 2]) * height_a

    y_centers = regression[..., 0] * height_a + y_centers_a
    x_centers = regression[..., 1] * width_a + x_centers_a

    ymin = y_centers - height / 2.
    xmin = x_centers - width / 2.
    ymax = y_centers + height / 2.
    xmax = x_centers + width / 2.

    return np.stack([xmin, ymin, xmax, ymax], 2)


def clip_boxes(boxes, img_shape):
    _, _, height, width = img_shape

    boxes[:, :, 0] = np.clip(boxes[:, :, 0], a_min=0, a_max=np.inf)
    boxes[:, :, 1] = np.clip(boxes[:, :, 1], a_min=0, a_max=np.inf)

    boxes[:, :, 2] = np.clip(boxes[:, :, 2], a_min=-np.inf, a_max=width - 1)
    boxes[:, :, 3] = np.clip(boxes[:, :, 3], a_min=-np.inf, a_max=height - 1)

    return boxes


# nms numpy version
def nms_cpu(boxes, scores, overlap_threshold=0.5, min_mode=False):
    boxes_x1 = boxes[:, 0]
    boxes_y1 = boxes[:, 1]
    boxes_x2 = boxes[:, 2]
    boxes_y2 = boxes[:, 3]

    areas = (boxes_x2 - boxes_x1 + 1) * (boxes_y2 - boxes_y1 + 1)
    order = scores.argsort()[::-1]

    keep = []
    while order.size > 0:
        keep.append(order[0])
        xx1 = np.maximum(boxes_x1[order[0]], boxes_x1[order[1:]])
        yy1 = np.maximum(boxes_y1[order[0]], boxes_y1[order[1:]])
        xx2 = np.minimum(boxes_x2[order[0]], boxes_x2[order[1:]])
        yy2 = np.minimum(boxes_y2[order[0]], boxes_y2[order[1:]])

        width = np.maximum(0.0, xx2 - xx1 + 1)
        height = np.maximum(0.0, yy2 - yy1 + 1)
        inter = width * height

        if min_mode:
            ovr = inter / np.minimum(areas[order[0]], areas[order[1:]])
        else:
            ovr = inter / (areas[order[0]] + areas[order[1:]] - inter)

        inds = np.where(ovr <= overlap_threshold)[0]
        order = order[inds + 1]
    return keep


def postprocess_numpy(input_shape, anchors, regression, classification,
                      threshold, iou_threshold):
    transformed_anchors = bbox_transform(anchors, regression)
    transformed_anchors = clip_boxes(transformed_anchors, input_shape)

    scores = np.max(classification, axis=2, keepdims=True)
    scores_over_thresh = (scores > threshold)[:, :, 0]

    out = []
    for i in range(input_shape[0]):
        if scores_over_thresh.sum() == 0:
            out.append({
                'rois': np.array(()),
                'class_ids': np.array(()),
                'scores': np.array(()),
            })

        classification_per = np.transpose(
            classification[i, scores_over_thresh[i, :], ...], (1, 0))
        transformed_anchors_per = transformed_anchors[i,
                                                      scores_over_thresh[i, :],
                                                      ...]
        scores_per = scores[i, scores_over_thresh[i, :], ...]
        anchors_nms_idx = nms_cpu(transformed_anchors_per,
                                  scores_per[:, 0],
                                  overlap_threshold=iou_threshold)
        anchors_nms_idx = np.array(anchors_nms_idx)

        if anchors_nms_idx.shape[0] != 0:
            scores_ = classification_per[:, anchors_nms_idx].max(0)
            classes_ = classification_per[:, anchors_nms_idx].argmax(0)
            boxes_ = transformed_anchors_per[anchors_nms_idx, :]

            out.append({
                'rois': boxes_,
                'class_ids': classes_,
                'scores': scores_,
            })
        else:
            out.append({
                'rois': np.array(()),
                'class_ids': np.array(()),
                'scores': np.array(()),
            })
    app_output = []
    num = out[0]['scores'].shape[0]
    if num == 0:
        return [
            np.array([[[0]]], dtype=np.float32),
            np.zeros((1, 10, 1), dtype=np.float32),
            np.zeros((1, 10, 1), dtype=np.float32),
            np.zeros((1, 10, 4), dtype=np.float32)
        ]

    # Output formatting
    num_objects = np.array(num, dtype=np.float32).reshape(1, 1, 1)

    num_classes = np.zeros(10, dtype=np.float32)
    num_classes[:num] = out[0]['class_ids']
    num_classes = num_classes.reshape(1, 10, 1)

    num_scores = np.zeros(10, dtype=np.float32)
    num_scores[:num] = out[0]['scores']
    num_scores = num_scores.reshape(1, 10, 1)

    num_bboxes = np.zeros((10, 4), dtype=np.float32)
    num_bboxes[:num] = out[0]['rois']
    num_bboxes = num_bboxes.reshape(1, 10, 4)

    app_output = [num_objects, num_classes, num_scores, num_bboxes]

    return app_output
