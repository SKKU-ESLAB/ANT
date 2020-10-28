#include "../../common/native/ant_common.h"
#include "internal/ant_runtime_native_internal.h"

#include "iotjs_def.h"
#include "iotjs_uv_request.h"
#include "modules/iotjs_module_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ANT_API_VOID_TO_NUMBER(ant_runtime, getPssInKB);

jerry_value_t InitANTRuntimeNative() {
  jerry_value_t antRuntimeNative = jerry_create_object();
  REGISTER_ANT_API(antRuntimeNative, ant_runtime, getPssInKB);

  initANTRuntime();

  return antRuntimeNative;
}