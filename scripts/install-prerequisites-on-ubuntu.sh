#!/bin/bash
BUILD_THREADS=4
SCRIPTS_DIR="$( cd "$( dirname "$0" )" && pwd -P )"
ANT_ROOT=$(realpath $SCRIPTS_DIR/..)

# ANT dependenies
echo "** Install git, dbus, gtk..."
sudo apt-get update
sudo apt-get install -y cmake git glib-2.0 \
  libdbus-1-dev libdbus-glib-1-2 libdbus-glib-1-dev
#sudo apt-get install -y libraspberrypi-dev

# Download submodules
echo "** Download ant dependencies..."
cd ${ANT_ROOT}
git submodule init
git submodule update

# TVM
echo "** Install tvm dependencies..."
sudo apt-get install -y python3 python3-dev python3-pip python3-setuptools gcc \
  libtinfo-dev zlib1g-dev build-essential cmake libedit-dev libxml2-dev libtinfo-dev \
  python3-opencv

echo "** Build tvm..."
cd ${ANT_ROOT}/dep/tvm
git submodule init
git submodule update
make runtime -j${BUILD_THREADS}
echo "export PYTHONPATH=$PWD/python:$PYTHONPATH" >> ~/.bashrc
export PYTHONPATH=$PWD/python:$PYTHONPATH

# gstreamer
echo "** Install gstreamer..."
sudo apt-get install -y libgstreamer1.0-dev gstreamer1.0-doc gstreamer1.0-tools \
  gstreamer1.0-plugins-base libgstreamer-plugins-base1.0-dev \
  gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly \
  gstreamer1.0-libav
sudo chmod 777 /dev/vchiq

# nnstreamer
echo "** Install nnstreamer"
sudo apt-add-repository -y ppa:nnstreamer
sudo apt-get update
sudo apt-get install -y nnstreamer-python3 nnstreamer-dev

# IoT.js
echo "** Build IoT.js..."
cd ${ANT_ROOT}/dep/iotjs
./tools/build.py --target-board=rpi3

# IoTivity
echo "** Build IoTivity..."
cd ${ANT_ROOT}/dep/iotivity/port/linux
git submodule init
git submodule update
make -j${BUILD_THREADS}