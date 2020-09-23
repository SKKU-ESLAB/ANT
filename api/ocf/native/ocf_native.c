#include "../../common/native/ant_common.h"
#include "ant_async.h"
#include "internal/ocf_native_internal.h"

#include "iotjs_def.h"
#include "iotjs_uv_request.h"
#include "modules/iotjs_module_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// OCFAdapter.onPrepareServer()
DECLARE_GLOBAL_ASYNC(ocf_adapter_onPrepareServer)
DECLARE_ANT_ASYNC_HANDLER(ocf_adapter_onPrepareServer)
UV_ASYNC_HANDLER(ocf_adapter_onPrepareServer) {
  // void *event = GET_MR_EVENT(ocf_adapter_onPrepareServer);
  jerry_value_t jsHandler = GET_JS_HANDLER(ocf_adapter_onPrepareServer);
  iotjs_invoke_callback(jsHandler, jerry_create_undefined(), NULL, 0);
  // DESTROY_EVENTS(ocf_adapter_onPrepareServer);
}
ASYNC_REGISTER(ocf_adapter_onPrepareServer, NULL)
JS_FUNCTION(ocf_adapter_onPrepareServer) {
  bool result;
  jerry_value_t argHandler;
  DJS_CHECK_ARGS(1, function);
  argHandler = JS_GET_ARG(0, function);

  result = CALL_ASYNC_REGISTER(ocf_adapter_onPrepareServer, argHandler);
  return jerry_create_boolean(result);
}

// OCFAdapter.onPrepareClient()
DECLARE_GLOBAL_ASYNC(ocf_adapter_onPrepareClient)
DECLARE_ANT_ASYNC_HANDLER(ocf_adapter_onPrepareClient)
UV_ASYNC_HANDLER(ocf_adapter_onPrepareClient) {
  // void *event = GET_MR_EVENT(ocf_adapter_onPrepareClient);
  jerry_value_t jsHandler = GET_JS_HANDLER(ocf_adapter_onPrepareClient);
  iotjs_invoke_callback(jsHandler, jerry_create_undefined(), NULL, 0);
  // DESTROY_EVENTS(ocf_adapter_onPrepareClient);
}
ASYNC_REGISTER(ocf_adapter_onPrepareClient, NULL)
JS_FUNCTION(ocf_adapter_onPrepareClient) {
  bool result;
  jerry_value_t argHandler;
  DJS_CHECK_ARGS(1, function);
  argHandler = JS_GET_ARG(0, function);

  result = CALL_ASYNC_REGISTER(ocf_adapter_onPrepareClient, argHandler);
  return jerry_create_boolean(result);
}

// OCFAdapter.start()
ANT_API_VOID_TO_VOID(ocf, adapter_start);

// OCFAdapter.stop()
ANT_API_VOID_TO_VOID(ocf, adapter_stop);

jerry_value_t InitOCFNative() {
  jerry_value_t ocfNative = jerry_create_object();
  REGISTER_ANT_API(ocfNative, ocf, adapter_onPrepareServer);
  REGISTER_ANT_API(ocfNative, ocf, adapter_onPrepareClient);
  REGISTER_ANT_API(ocfNative, ocf, adapter_start);
  REGISTER_ANT_API(ocfNative, ocf, adapter_stop);

  initOCF();

  return ocfNative;
}