#!/bin/bash
jsdoc2md --configure .jsdoc/jsdoc.json \
         --files api/antruntime/**/*.js > docs/api/Runtime-API.md

jsdoc2md --configure .jsdoc/jsdoc.json \
         --files api/antcamera/**/*.js > docs/api/Camera-API.md

jsdoc2md --configure .jsdoc/jsdoc.json \
         --files api/antcompanion/**/*.js > docs/api/Companion-API.md

jsdoc2md --configure .jsdoc/jsdoc.json \
         --files api/antgateway/**/*.js > docs/api/Gateway-API.md

jsdoc2md --configure .jsdoc/jsdoc.json \
         --files api/antml/**/*.js > docs/api/ML-API.md

jsdoc2md --configure .jsdoc/jsdoc.json \
         --files api/antremoteui/**/*.js > docs/api/Remote-UI-API.md

jsdoc2md --configure .jsdoc/jsdoc.json \
         --files api/antresource/**/*.js > docs/api/Resource-API.md

jsdoc2md --configure .jsdoc/jsdoc.json \
         --files api/antstream/**/*.js > docs/api/Stream-API.md

jsdoc2md --configure .jsdoc/jsdoc.json \
         --files api/ocf/**/*.js > docs/api/OCF-API.md
