#!/bin/bash
# Install dependent packages
sudo apt-get update
sudo apt-get install cmake git libdbus-1-dev glib-2.0 libdbus-glib-1-2 libdbus-glib-1-dev \
  libgstreamer1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad \
  gstreamer1.0-plugins-ugly gstreamer1.0-libav libgstreamer-plugins-base1.0-dev \
  gstreamer1.0-doc gstreamer1.0-tools libtinfo-dev libprotobuf-dev \
  python3 python3-decorator libjasper libqtgui4 libqt4-test libatlas-base-dev

# Build IoT.js
cd ../../dep/iotjs
./tools/build.py --target-board=rpi3

# Gstreamer
pip3 install opencv-contrib-python==4.1.0.25
sudo chmod 777 /dev/vchiq