#!/bin/bash
mkdir -p nodejs
cd nodejs

MODULE_PATH=../../api-modules/
npm install ${MODULE_PATH}/ant

cp -r ../../app-runtime/* ./