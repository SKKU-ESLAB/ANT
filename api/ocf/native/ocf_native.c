#include "../../common/native/ant_common.h"
#include "internal/ocf_native_internal.h"

#include "iotjs_def.h"
#include "iotjs_uv_request.h"
#include "modules/iotjs_module_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ANT_API_VOID_TO_NUMBER(ocf, initialize);

jerry_value_t InitOCFNative() {
  jerry_value_t ocfNative = jerry_create_object();
  REGISTER_ANT_API(ocfNative, ocf, initialize);

  initOCF();

  return ocfNative;
}