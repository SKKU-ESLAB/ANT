#!/bin/bash
gst-launch-1.0 -v tcpclientsrc host=127.0.0.1 port=5000 ! gdpdepay ! rtph264depay ! omxh264dec ! autovideosink sync=false
