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
# install-deps-raspberry-pi2_3.sh: Script to install dependent packages in
#                           raspberry pi 2 or 3 target board
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
sudo apt-get -y install g++-4.8                                               \
  wiringpi libdbus-1-dev glib-2.0 libdbus-glib-1-2                            \
  libdbus-glib-1-2-dbg libdbus-glib-1-dev zip sqlite3 libsqlite3-dev cmake    \
  libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev git  \
  python-dev python-numpy libjpeg-dev libpng-dev libtiff-dev libjasper-dev    \
  libdc1394-22-dev automake libtool libssl-dev libnl-3-dev libnl-genl-3-dev   \
  python3 udhcpd libv4l-dev libboost-dev libboost-serialization-dev gstreamer1.0

# Get the absolute path of ANT repository directory
ANT_REPO_DIR=$(dirname "$0")/../..
eval ANT_REPO_DIR=`readlink --canonicalize ${ANT_REPO_DIR}`

# Step 2. Download submodules
print_progress 2 "Download submodules..."
git submodule sync
git submodule update --init --recursive

# Step 3. Build and reinstall bluez-4.101
print_progress 3 "Build and reinstall bluez-4.101..."
# Remove existing bluez
sudo apt-get remove bluez
# Install bluez
cd ${ANT_REPO_DIR}/dep/bluez-4.101
./configure --prefix=/usr --mandir=/usr/share/man --sysconfdir=/etc \
  --localstatedir=/var --libexecdir=/lib 
make 
sudo make install

# Step 4. Set udhcpd config
print_progress 4 "Set udhcpd config..."
sudo touch /var/lib/misc/udhcpd.leases

# Step 5. Build and install libxml2-2.9.4-rc2
print_progress 5 "Build and install libxml2-2.9.4-rc2..."
cd ${ANT_REPO_DIR}/dep/libxml2-2.9.4-rc2
./autogen.sh
./configure --prefix=/usr/local/xml
make
sudo make install

# Step 6. Build and install opencv-3.0.0
print_progress 6 "Build and install opencv-3.0.0..."
cd /usr/include/linux
sudo ln -s ../libv4l1-videodev.h videodev.h
cd ${ANT_REPO_DIR}/dep/opencv-3.0.0
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D \
  BUILD_TESTS=OFF -D BUILD_PERF_TESTS=OFF ..
make -j 4
sudo make install

# Step 7. Build and install libuv-v1.7.5
print_progress 7 "Build and install libuv-v1.7.5..."
cd ${ANT_REPO_DIR}/dep/libuv-v1.7.5
sh autogen.sh
./configure
make
make check
sudo make install

# Step 8. Copy dbus config file
print_progress 8 "Copy dbus config file for ANT..."
sudo cp ${ANT_REPO_DIR}/dep/ant-dbus-config/ant.conf /etc/dbus-1/system.d/ant.conf

# Step 9. Install wpa_supplicant, wpa_cli and deletesem
print_progress 9 "Install wpa_supplicant, wpa_cli and deletesem..."
cd ${ANT_REPO_DIR}/dep/hostap/wpa_supplicant
make
cd ${ANT_REPO_DIR}/dep/deletesem
gcc -o deletesem deletesem.c -lpthread

mkdir -p /usr/bin/ant-deps
sudo cp ${ANT_REPO_DIR}/dep/hostap/wpa_supplicant/wpa_supplicant /usr/bin/ant-deps/
sudo cp ${ANT_REPO_DIR}/dep/hostap/wpa_supplicant/wpa_cli /usr/bin/ant-deps/
sudo cp ${ANT_REPO_DIR}/dep/deletesem/deletesem /usr/bin/ant-deps/
chmod +x /usr/bin/ant-deps/*

# Step 10. Build and install nodejs-4.0.0
print_progress 10 "Build and install nodejs-4.0.0..."
cd ${ANT_REPO_DIR}/dep/nodejs-4.0.0
./configure
make -j4
sudo make install

# Step 11. Install nan, node-gyp package 
cd ${ANT_REPO_DIR}
npm install nan
sudo npm install -g node-gyp

# Step 12. Install Gstreamer RPI camera source element
cd ${ANT_REPO_DIR}/dep/gst-rpicamsrc
./autogen.sh --prefix=/usr --libdir=/usr/lib/arm-linux-gnueabihf/
make
sudo make install

WARN_COLO="\033[31;47m"
INFO_COLO="\033[36m"
INIT_COLO="\033[0m"

echo -e "${WARN_COLO}Prerequisites install for Raspberry-pi2_3 completed${INIT_COLO}"
