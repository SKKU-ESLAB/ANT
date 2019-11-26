#!/bin/bash
MODULE_PATH=../../lite/api-modules/
cd ../../dep/iotjs/
./tools/build.py --target-board=rpi3 --cmake-param=-DENABLE_MODULE_ANT=ON \
    --external-modules=${MODULE_PATH}/ant
cd -

mkdir -p iotjs
cd iotjs
cp ../../../dep/iotjs/build/arm-linux/debug/bin/iotjs ./
cp -r ../../app-runtime/* ./