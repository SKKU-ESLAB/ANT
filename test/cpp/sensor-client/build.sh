#!/bin/bash

gcc nil.c nil_dbus.c nil_dbus_connection.c nil_request.c cJSON.c \
  -o sensor-client -lm   \
  `pkg-config --libs --cflags dbus-1 glib-2.0 dbus-glib-1 libuv`


