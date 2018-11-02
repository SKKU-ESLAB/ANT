#!/bin/bash
for i in {1..5}
do
  echo "$i/5"
  scp -r rpi:workload/large ~/Downloads/
done