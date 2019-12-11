#!/bin/bash
# ANT dependenies
sudo apt-get update
sudo apt-get install -y cmake git libdbus-1-dev glib-2.0 libdbus-glib-1-2 libdbus-glib-1-dev
sudo apt-get install -y libgstreamer1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good \
  gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav \
  libgstreamer-plugins-base1.0-dev gstreamer1.0-doc gstreamer1.0-tools

# TVM dependencies
sudo apt-get install -y python3 python3-dev python3-pip python3-setuptools gcc \
  libtinfo-dev zlib1g-dev build-essential cmake libedit-dev libxml2-dev libtinfo-dev
#sudo apt-get install -y libprotobuf-dev
#sudo apt-get install -y python3 python3-decorator libjasper libqtgui4 libqt4-test libatlas-base-dev
#pip3 install opencv-contrib-python==4.1.0.25

# Build IoT.js
cd ../../dep/iotjs
./tools/build.py --target-board=rpi3

# Gstreamer
sudo chmod 777 /dev/vchiq

# nnstreamer
sudo apt-add-repository -y ppa:nnstreamer
sudo apt-get update
sudo apt-get install -y nnstreamer-python3 nnstreamer-dev