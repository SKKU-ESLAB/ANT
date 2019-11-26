#!/bin/bash
MODULE_PATH=../../lite/api-modules/
cd ../../dep/iotjs/
./tools/build.py --target-board=rpi3 --cmake-param=-DENABLE_MODULE_ANT=ON \
    --external-modules=${MODULE_PATH}/ant
cd -