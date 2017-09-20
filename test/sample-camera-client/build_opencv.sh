#!/bin/bash
g++ opencv_test.cpp -o opencv `pkg-config --libs --cflags opencv`
