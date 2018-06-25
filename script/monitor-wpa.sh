#!/bin/bash
if [ "$EUID" -ne 0 ]
then echo "Please run as root"
  exit
fi

i=0
while true; do
  echo ""
  echo "Try $i"
  i=`expr $i + 1`
  wpa_cli status
  sleep 2
done
