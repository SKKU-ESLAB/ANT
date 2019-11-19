#!/bin/bash
HOST_SRC=192.168.0.4
echo "Host source: >>$HOST_SRC<<"
rsync -avzh ${HOST_SRC}:repository/iotjs/ant-iotjs .
cd iotjs
./tools/build.py --target-board=rpi3 \
  --external-modules=../ant-iotjs/ant \
  --cmake-param=-DENABLE_MODULE_ANT=ON
cd ..
