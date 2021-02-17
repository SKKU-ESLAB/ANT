#!/bin/bash
export ANT_ROOT="$( cd "$( dirname "$0" )" && pwd -P )"
echo "ANT_ROOT: $ANT_ROOT"
if [ $# -ne 2 ]; then
  echo "usage: $1 [app name] [--run or --debug]"
  exit -1
fi

if [ $2 == "--run" ]; then
  ${ANT_ROOT}/iotjs ${ANT_ROOT}/app-core/main.js ${1} ${ANT_ROOT}/apps/${1}.js 0
elif [ $2 == "--debug" ]; then
  gdb --args ${ANT_ROOT}/iotjs ${ANT_ROOT}/app-core/main.js ${1} ${ANT_ROOT}/apps/${1}.js 0
fi