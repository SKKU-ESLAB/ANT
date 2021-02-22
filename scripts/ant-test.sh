#!/bin/bash
if [ $# -ne 2 ] && [ $# -ne 3 ]; then
  echo "usage: $1 [app name] [--run or --debug] [(optional) APP_ROOT]"
  exit -1
fi

export ANT_ROOT="$( cd "$( dirname "$0" )" && pwd -P )"
if [ $# -eq 3 ]; then
  export APP_ROOT=$3
else
  export APP_ROOT="$ANT_ROOT/apps"
fi
echo "ANT_ROOT: $ANT_ROOT"
export PYTHONPATH=$ANT_ROOT/python:$PYTHONPATH

if [ $2 == "--run" ]; then
  ${ANT_ROOT}/iotjs ${ANT_ROOT}/app-core/main.js $1 ${APP_ROOT}/$1.js 0
elif [ $2 == "--debug" ]; then
  gdb --args ${ANT_ROOT}/iotjs ${ANT_ROOT}/app-core/main.js $1 ${APP_ROOT}/$1.js 0
fi