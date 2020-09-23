#include "../../common/native/ant_common.h"
#include "ant_async.h"
#include "internal/ocf_native_internal.h"

#include "iotjs_def.h"
#include "iotjs_uv_request.h"
#include "modules/iotjs_module_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ANT_API_VOID_TO_VOID(ocf, adapter_start);

DECLARE_GLOBAL_ASYNC(ocf_adapter_onPrepareServer)
DECLARE_ASYNC_HANDLERS(ocf_adapter_onPrepareServer)
ASYNC_REGISTER(ocf_adapter_onPrepareServer)
JS_FUNCTION(ocf_adapter_onPrepareServer) {
  bool result;
  jerry_value_t argHandler;
  DJS_CHECK_ARGS(1, function);
  argHandler = JS_GET_ARG(0, function);

  result = CALL_ASYNC_REGISTER(ocf_adapter_onPrepareServer, argHandler);
  return jerry_create_boolean(result);
}

jerry_value_t InitOCFNative() {
  jerry_value_t ocfNative = jerry_create_object();
  REGISTER_ANT_API(ocfNative, ocf, adapter_start);
  REGISTER_ANT_API(ocfNative, ocf, adapter_onPrepareServer);

  initOCF();

  return ocfNative;
}