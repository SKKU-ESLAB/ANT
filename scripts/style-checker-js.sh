#!/bin/bash
ANT_BASE_DIR=$(dirname $(dirname "$0/"))
find . \
  \( -name "*.js" \) -and \
  \( -not -path "${ANT_BASE_DIR}/subprojects/*" -and \
  -not -path "${ANT_BASE_DIR}/dep/*" -and \
  -not -path "${ANT_BASE_DIR}/.eslintrc.js" -and \
  -not -path "${ANT_BASE_DIR}/node_modules/*" -and \
  -not -path "${ANT_BASE_DIR}/apps/*" -and \
  -not -path "${ANT_BASE_DIR}/test/appbench/*" -and \
  -not -path "*.eslintrc*.js" \) \
  | xargs npx eslint
find ./apps \
  \( -name "*.js" \) \
  | xargs npx eslint -c ./.eslintrc-apps.js