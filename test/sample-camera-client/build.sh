#!/bin/bash
g++  ANTdbusInterface.cpp -o test `pkg-config --libs --cflags dbus-1 glib-2.0`

