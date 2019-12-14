#!/bin/bash
mkdir -p nodejs
cd nodejs

MODULE_PATH=../../api-modules/
npm install ${MODULE_PATH}/ant
# request: for HTTP request
npm install request

cp -r ../../app-runtime/* ./