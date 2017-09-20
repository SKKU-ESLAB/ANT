#!/bin/bash
g++ opencv_jpeg.cpp -o opencv_jpeg `pkg-config --libs --cflags opencv`
