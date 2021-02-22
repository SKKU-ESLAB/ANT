#!/bin/bash
export ANT_ROOT="$( cd "$( dirname "$0" )" && pwd -P )"
echo "ANT_ROOT: $ANT_ROOT"
export PYTHONPATH=$ANT_ROOT/python:$PYTHONPATH
n run 10 ./app-launcher/index.js
