#!/bin/bash
sudo raspivid -t 0 -h 720 -w 1280 -fps 30 -hf -b 2000000 -n -o - | gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=1 pt=96 ! Gdppay ! Tcpserversink host=192.168.0.33 port=5000
