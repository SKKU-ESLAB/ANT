#!/bin/bash
#
##########################################################################
#
# Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
#
# Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
##########################################################################
# install-deps-odroid-xu4.sh: Script to install dependent packages in
#                           odroid xu4 target board
##########################################################################

# print_progress(): Print the progress
#  - $1(STEP_NUM): (int) Progress step's number
#  - $2(STEP_DESC): (string) Progress step's description
print_progress() {
  STEP_NUM=$1
  STEP_DESC=$2

  WARN_COLO="\033[31;47m"
  INFO_COLO="\033[36m"
  INIT_COLO="\033[0m"

  echo -e "${INFO_COLO}Step ${STEP_NUM}. ${STEP_DESC} ${INIT_COLO}"
}

# Step 1. Install packages by apt-get
print_progress 1 "Install dependent packages..."
sudo apt-get update
sudo apt-get -y install libdbus-1-dev libglib2.0-dev bison byacc          \
  libdbus-glib-1-dev zip libsqlite3-dev cmake git                         \
  automake libtool libssl-dev libnl-3-dev nodejs npm                      \
  libnl-genl-3-dev udhcpd libopencv-dev libxml2-dev                       \
  libgstreamer1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good \
  gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav   \
  gstreamer1.0-tools libgstreamer-plugins-base1.0-dev gstreamer1.0-opencv scons
# glib-2.0

## Caffe Library Dependency
#sudo apt-get -y install libprotobuf-dev libleveldb-dev libsnappy-dev          \
#  libhdf5-serial-dev protobuf-compiler libatlas-base-dev libopenblas-dev      \
#  libgflags-dev libgoogle-glog-dev liblmdb-dev python-numpy python-scipy      \
#  python-yaml python-six python-pip
#sudo apt-get -y install --no-install-recommends libboost-all-dev

# Get the absolute path of ANT repository directory
ANT_REPO_DIR=$(dirname "$0")/../..
eval ANT_REPO_DIR=`readlink --canonicalize ${ANT_REPO_DIR}`

# Step 2. Download submodules
print_progress 2 "Download submodules..."
git submodule sync
git submodule update --init --recursive

# Step 3. Build and reinstall bluez-4.101
print_progress 3 "Build and reinstall bluez-4.101..."
# Install bluez
cd ${ANT_REPO_DIR}/dep/bluez-4.101
./configure --prefix=/usr --mandir=/usr/share/man --sysconfdir=/etc \
  --localstatedir=/var --libexecdir=/lib
make -j4
sudo make install
sudo systemctl unmask bluetooth
sudo service bluetooth start

# Step 4. Set udhcpd config
print_progress 4 "Set udhcpd config..."
sudo touch /var/lib/misc/udhcpd.leases

# Step 5. Link include directory for libxml2
print_progress 5 "Link include directory for libxml2..."
sudo ln -s /usr/include/libxml2/libxml /usr/include/libxml

# Step 6. Build and install libuv-v1.7.5
print_progress 6 "Build and install libuv-v1.7.5..."
cd ${ANT_REPO_DIR}/dep/libuv-v1.7.5
sh autogen.sh
./configure
make -j4
sudo make install

# Step 7. Copy dbus config file
print_progress 7 "Copy dbus config file for ANT..."
sudo cp ${ANT_REPO_DIR}/dep/ant-dbus-config/ant.conf /etc/dbus-1/system.d/ant.conf

# Step 8. Install wpa_supplicant, wpa_cli and deletesem
print_progress 8 "Install wpa_supplicant, wpa_cli and deletesem..."
cd ${ANT_REPO_DIR}/dep/hostap/wpa_supplicant
make -j4

cd ${ANT_REPO_DIR}/dep/deletesem
gcc -o deletesem deletesem.c -lpthread

sudo mkdir -p /usr/bin/ant-deps
sudo cp ${ANT_REPO_DIR}/dep/hostap/wpa_supplicant/wpa_supplicant /usr/bin/ant-deps/
sudo cp ${ANT_REPO_DIR}/dep/hostap/wpa_supplicant/wpa_cli /usr/bin/ant-deps/
sudo cp ${ANT_REPO_DIR}/dep/deletesem/deletesem /usr/bin/ant-deps/
sudo chmod +x /usr/bin/ant-deps/*

# Step 9. Install nodejs-4.x package
#print_progress 9 "Install nodejs-4.x package..."
#curl -sL https://deb.nodesource.com/setup_4.x | sudo -E bash -
#sudo apt-get -y install nodejs

# Step 10. Install nan, node-gyp package
#print_progress 10 "Install nan, node-gyp package..."
#cd ${ANT_REPO_DIR}
#npm install nan
#sudo npm install -g node-gyp

# Step 11. Install Gstreamer RPI camera source element
#cd ${ANT_REPO_DIR}/dep/gst-rpicamsrc
#./autogen.sh --prefix=/usr --libdir=/usr/lib/arm-linux-gnueabihf/
#make -j4
#sudo make install

# Step 12. Install Caffe Framework
#cd ${ANT_REPO_DIR}/dep/caffe
#cp Makefile.config.rpi Makefile.config
#make -j4 all distribute

#sudo cp -a distribute/lib/libcaffe.so* /usr/lib
#sudo cp -r distribute/include/caffe/ /usr/include

# Install ComputeLibrary
cd ${ANT_REPO_DIR}/dep/ComputeLibrary
sudo cp build/libarm_compute*.so /usr/lib
sudo cp -r arm_compute /usr/include/

# Step 13. Install FANN Library
cd ${ANT_REPO_DIR}/dep/fann
mkdir build
cd build
cmake ..
make -j4
sudo make install

WARN_COLO="\033[31;47m"
INFO_COLO="\033[36m"
INIT_COLO="\033[0m"

echo -e "${WARN_COLO}Prerequisites install for Odroid-xu4 completed${INIT_COLO}"
