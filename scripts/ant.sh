#!/bin/bash
export ANT_ROOT="$( cd "$( dirname "$0" )" && pwd -P )"
echo "ANT_ROOT: $ANT_ROOT"
${ANT_ROOT}/iotjs ${ANT_ROOT}/main.js
