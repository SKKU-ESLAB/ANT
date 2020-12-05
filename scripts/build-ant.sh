#!/bin/bash
SCRIPTS_DIR="$( cd "$( dirname "$0" )" && pwd -P )"
ANT_ROOT=$(realpath $SCRIPTS_DIR/..)
MODULE_PATH=${ANT_ROOT}/api/
OUT_PATH=${ANT_ROOT}/out/
ARCH=$(uname -m)

# Build ANT API and App Runtime for IoT.js
cd ${ANT_ROOT}/dep/iotjs/
if [[ $ARCH = "x86_64" || $ARCH = "x86" ]];
then
  IOTJS_BOARD_NAME=""
elif [[ $ARCH = "aarch64" ]];
then
  IOTJS_BOARD_NAME="tx2"
else
  IOTJS_BOARD_NAME="--target-board=rpi3"
fi
./tools/build.py ${IOTJS_BOARD_NAME} \
    --cmake-param=-DENABLE_MODULE_ANT=ON \
    --cmake-param=-DENABLE_MODULE_ANTRUNTIME=ON \
    --cmake-param=-DENABLE_MODULE_ANTCOMPANION=ON \
    --cmake-param=-DENABLE_MODULE_ANTRESOURCE=ON \
    --cmake-param=-DENABLE_MODULE_ANTREMOTEUI=ON \
    --cmake-param=-DENABLE_MODULE_ANTSTREAM=ON \
    --cmake-param=-DENABLE_MODULE_ANTML=ON \
    --cmake-param=-DENABLE_MODULE_OCF=ON \
    --external-modules=${MODULE_PATH} \
    --no-init-submodule
BUILD_RESULT=$?
if [[ ${BUILD_RESULT} -eq 1 ]];
then
  BUILD_RESULT=0
fi
cd -

# Install ANT Running Script
mkdir -p ${OUT_PATH}
cp ${ANT_ROOT}/scripts/ant.sh ${OUT_PATH}/

# Install ANT API and App Runtime for IoT.js
cp ${ANT_ROOT}/dep/iotjs/build/${ARCH}-linux/debug/bin/iotjs ${OUT_PATH}/
cp -r ${ANT_ROOT}/app-runtime/* ${OUT_PATH}/

# Install ANT-ML
mkdir -p ${OUT_PATH}/ml
cp -r ${ANT_ROOT}/ml/*.py ${OUT_PATH}/ml/
cp -r ${ANT_ROOT}/ml/sample-models ${OUT_PATH}/ml/

exit ${BUILD_RESULT}
