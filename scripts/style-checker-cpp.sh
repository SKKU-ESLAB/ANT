#!/bin/bash
ANT_BASE_DIR=$(dirname $(dirname "$0/"))
find . \( -not -path "${ANT_BASE_DIR}/subprojects/*" -and -not -path "${ANT_BASE_DIR}/dep/*" -and -not -path "${ANT_BASE_DIR}/rtos/*" \) \
  -and \( -name "*.h" -or -name "*.cpp" -or -name "*.cc" -or -name "*.c" \) | xargs cpplint --filter=-readability/casting,-build/header_guard,-build/include_subdir,-build/include_what_you_use,-readability/braces,-build/include,-runtime/threadsafe_fn,-runtime/int
