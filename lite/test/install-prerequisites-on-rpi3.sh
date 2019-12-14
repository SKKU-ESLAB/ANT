#!/bin/bash
# ANT dependenies
sudo apt-get update
sudo apt-get install -y cmake git
sudo apt-get install -y libdbus-1-dev glib-2.0 libdbus-glib-1-2 libdbus-glib-1-dev
sudo apt-get install -y libraspberrypi-dev

# TVM dependencies
sudo apt-get install -y python3 python3-dev python3-pip python3-setuptools gcc \
  libtinfo-dev zlib1g-dev build-essential cmake libedit-dev libxml2-dev libtinfo-dev \
  python3-opencv
cd ../../dep/tvm
make runtime -j2
echo "export PYTHONPATH=$PWD/python:$PYTHONPATH" >> ~/.bashrc

# Build IoT.js
cd ../../dep/iotjs
./tools/build.py --target-board=rpi3

# Install Gstreamer and its elements
sudo apt-get install -y libgstreamer1.0-dev gstreamer1.0-doc gstreamer1.0-tools \
  gstreamer1.0-plugins-base libgstreamer-plugins-base1.0-dev \
  gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly \
  gstreamer1.0-libav
sudo chmod 777 /dev/vchiq

# Install nnstreamer
sudo apt-add-repository -y ppa:nnstreamer
sudo apt-get update
sudo apt-get install -y nnstreamer-python3 nnstreamer-dev