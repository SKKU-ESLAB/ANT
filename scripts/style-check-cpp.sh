#!/bin/bash
ANT_BASE_DIR=$(dirname $(dirname "$0/"))
find . \( -not -path "${ANT_BASE_DIR}/subprojects/*" -and -not -path "${ANT_BASE_DIR}/dep/*" \) \
  -and \( -name "*.h" -or -name "*.cpp" -or -name "*.cc" -or -name "*.c" \) | xargs cpplint
