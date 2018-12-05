# From resin/rpi-raspbian:wheezy
# COPY qemu-arm-static /usr/bin/qemu-arm-static

FROM sedden/rpi-raspbian-qemu:wheezy

RUN apt-get update
RUN mkdir -p /home/pi/ANT
WORKDIR /home/pi/ANT
ADD . /home/pi/ANT
