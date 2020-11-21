# From resin/rpi-raspbian:wheezy
# COPY qemu-arm-static /usr/bin/qemu-arm-static

FROM docker:20.04

RUN timedatectl set-timezone 'Asia/Seoul'
RUN mkdir -p /home/ant
WORKDIR /home/ant
ADD . /home/ant
RUN cd /home/ant
RUN ./scripts/prepare-ubuntu.sh --no-sudo --except-iotjs
