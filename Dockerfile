# From resin/rpi-raspbian:wheezy
# COPY qemu-arm-static /usr/bin/qemu-arm-static

FROM sedden/rpi-raspbian-qemu:wheezy

RUN apt-get update && apt-get install python vim git
RUN mkdir -p /home/pi/ANT
WORKDIR /home/pi/ANT
ADD . /home/pi/ANT
RUN cd /home/pi/ANT
RUN pwd
