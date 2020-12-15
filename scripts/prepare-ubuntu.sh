#!/bin/bash
BUILD_THREADS=4
SCRIPTS_DIR="$( cd "$( dirname "$0" )" && pwd -P )"
ANT_ROOT=$(realpath $SCRIPTS_DIR/..)
ARCH=$(uname -m)

if [[ $1 = "--no-sudo" ]];
then
  SUDO=""
else
  SUDO="sudo"
fi

if [[ ! $2 = "--skip-packages" ]];
then
  # ANT dependenies
  echo "** Install git, dbus, gtk, python2.x..."
  ${SUDO} apt-get update
  ${SUDO} apt-get install -y cmake git glib-2.0 \
    libdbus-1-dev libdbus-glib-1-2 libdbus-glib-1-dev python
  ${SUDO} apt-get install -y software-properties-common
  ${SUDO} apt-get install -y libcurl-dev libcurl4-openssl-dev
  ${SUDO} apt-get install -y libtar-dev
  if [[ $ARCH == *"arm"* ]];
  then
    ${SUDO} apt-get install -y libraspberrypi-dev
  fi
  
  # gstreamer
  echo "** Install gstreamer..."
  ${SUDO} apt-get install -y libgstreamer1.0-dev gstreamer1.0-doc gstreamer1.0-tools \
    gstreamer1.0-plugins-base libgstreamer-plugins-base1.0-dev \
    gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly \
    gstreamer1.0-libav
  
  # TVM
  echo "** Install tvm dependencies..."
  ${SUDO} apt-get install -y python3 python3-dev python3-pip python3-setuptools gcc \
    libtinfo-dev zlib1g-dev build-essential cmake libedit-dev libxml2-dev libtinfo-dev
  ${SUDO} apt-get install -y python3-opencv
  pip3 install opencv-python
  
  # nnstreamer
  # TODO: no x86 package for nnstreamer
  echo "** Install nnstreamer"
  ${SUDO} apt-add-repository -y ppa:nnstreamer
  ${SUDO} apt-get update
  ${SUDO} apt-get install -y nnstreamer-python3 nnstreamer-dev
fi
  
if [[ ! $2 = "--only-packages" ]];
then
  if [[ -f "${ANT_ROOT}/.git" ]];
  then
    # Download submodules
    echo "** Download ant dependencies..."
    cd ${ANT_ROOT}
    git submodule init
    git submodule update
  fi

  # Build TVM
  echo "** Build tvm..."
  cd ${ANT_ROOT}/dep/tvm
  git submodule init
  git submodule update
  make runtime -j${BUILD_THREADS}
  echo "export PYTHONPATH=$PWD/python:$PYTHONPATH" >> ~/.bashrc
  sudo sh -c 'echo "export PYTHONPATH=$PWD/python:$PYTHONPATH" >> /root/.bashrc'
  export PYTHONPATH=$PWD/python:$PYTHONPATH
  
  ${SUDO} chmod 777 /dev/vchiq
  
  # Build IoTivity
  echo "** Build IoTivity..."
  cd ${ANT_ROOT}/dep/iotivity/port/linux
  git submodule init
  git submodule update
  make SECURE=0

  # Build IoT.js
  echo "** Build IoT.js..."
  if [[ $ARCH = "x86_64" || $ARCH = "x86" ]];
  then
    IOTJS_BOARD_NAME=""
  elif [[ $ARCH = "aarch64" ]];
  then
    IOTJS_BOARD_NAME="--target-board=tx2"
  else
    IOTJS_BOARD_NAME="--target-board=rpi3"
  fi
  cd ${ANT_ROOT}/dep/iotjs
  ./tools/build.py ${IOTJS_BOARD_NAME} --clean
fi
