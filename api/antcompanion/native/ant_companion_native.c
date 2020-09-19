#include "internal/ant_companion_native_internal.h"
#include "../../common/native/ant_common.h"

#include "iotjs_def.h"
#include "iotjs_uv_request.h"
#include "modules/iotjs_module_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ANT_API_STRING_TO_STRING(ant_companion, getMyIPAddress);

jerry_value_t InitANTCompanionNative() {
  jerry_value_t antCompanionNative = jerry_create_object();
  REGISTER_ANT_API(antCompanionNative, ant_companion, getMyIPAddress);

  initANTCompanion();

  return antCompanionNative;
}