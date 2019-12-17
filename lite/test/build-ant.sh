#!/bin/bash
# Build ANT API and App Runtime for IoT.js
MODULE_PATH=../../lite/api-modules/
cd ../../dep/iotjs/
./tools/build.py --target-board=rpi3 \
    --cmake-param=-DENABLE_MODULE_ANT=ON \
    --cmake-param=-DENABLE_MODULE_ANTRUNTIME=ON \
    --cmake-param=-DENABLE_MODULE_ANTCOMPANION=ON \
    --cmake-param=-DENABLE_MODULE_ANTRESOURCE=ON \
    --cmake-param=-DENABLE_MODULE_ANTREMOTEUI=ON \
    --cmake-param=-DENABLE_MODULE_ANTSTREAM=ON \
    --cmake-param=-DENABLE_MODULE_ANTML=ON \
    --external-modules=${MODULE_PATH}
cd -

# Install ANT API and App Runtime for IoT.js
mkdir -p iotjs
cd iotjs
cp ../../../dep/iotjs/build/arm-linux/debug/bin/iotjs ./
cp -r ../../app-runtime/* ./

# Install ANT-ML
mkdir -p ml
cp -r ../../ml/*.py ./ml/
cp -r ../../ml/sample-models ./