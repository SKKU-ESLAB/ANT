#!/bin/bash
if [ not $1 = '--skip-deps' ]
then
  pip3 install -r requirements.txt
  npm install
fi

ANT_BASE_DIR=$(dirname $(dirname "$0/"))
${ANT_BASE_DIR}/scripts/style-check-py.sh
${ANT_BASE_DIR}/scripts/style-check-cpp.sh
${ANT_BASE_DIR}/scripts/style-check-js.sh
