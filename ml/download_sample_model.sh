#!/bin/bash
DL_ROOT=https://raw.githubusercontent.com/SKKU-ESLAB/ant-sample-ml-models/master/rpi3_mobilenet_full/
mkdir -p sample-models/rpi3_mobilenet_full
cd sample-models/rpi3_mobilenet_full
wget ${DL_ROOT}/deploy_graph.json
wget ${DL_ROOT}/deploy_lib.tar
wget ${DL_ROOT}/deploy_lib.tar.so
wget ${DL_ROOT}/deploy_param.params
wget ${DL_ROOT}/imagenet-simple-labels.json
