#include "../../common/native/ant_common.h"
#include "ocf_adapter.h"

#include "iotjs_def.h"
#include "iotjs_uv_request.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

jerry_value_t InitOCFNative() {
  // Initialize sub-objects of OCF API
  jerry_value_t ocfNative = jerry_create_object();
  InitOCFAdapterNative(ocfNative);

  // Initialize IoTivity Lite
  initOCF();
  return ocfNative;
}