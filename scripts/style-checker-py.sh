#!/bin/bash
ANT_BASE_DIR=$(dirname $(dirname "$0/"))
find . -name "*.py" \
  -not -path "${ANT_BASE_DIR}/subprojects/*" \
  -not -path "${ANT_BASE_DIR}/dep/*" \
  -not -path "${ANT_BASE_DIR}/menuconfig.py" \
  | xargs pylint --disable=fixme,import-error,useless-object-inheritance,C0114,C0115,C0116,R0902,R0914,E1305,R0913,E1121,R0801
