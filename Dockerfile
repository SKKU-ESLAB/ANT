FROM ubuntu:20.04

RUN ln -sf /usr/share/zoneinfo/Asia/Seoul /etc/localtime
RUN mkdir -p /home/ant
WORKDIR /home/ant

RUN mkdir -p /home/ant/scripts/
RUN mkdir -p /home/ant/dep/

ADD . /home/ant/
RUN cd /home/ant
RUN /bin/bash ./scripts/prepare-ubuntu.sh --no-sudo
RUN cd /
