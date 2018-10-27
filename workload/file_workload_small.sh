#!/bin/bash
for i in {1..25}
do
  echo "$i/25"
  scp -r rpi:workload/small ~/Downloads/
  sleep 1
done
