#!/bin/bash
for i in {1..10}
do
  echo "$i/10"
  scp -r rpi:workload/large ~/Downloads/
done
