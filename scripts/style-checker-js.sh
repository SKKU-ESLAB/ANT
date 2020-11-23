#!/bin/bash
ANT_BASE_DIR=$(dirname $(dirname "$0/"))
find . \
  \( -name "*.js" \) -and \
  \( -not -path "${ANT_BASE_DIR}/subprojects/*" -and \
  -not -path "${ANT_BASE_DIR}/dep/*" -and \
  -not -path "${ANT_BASE_DIR}/.eslintrc.js" -and \
  -not -path "${ANT_BASE_DIR}/node_modules/*" \) \
  | xargs npx eslint
