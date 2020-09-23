#include "../../common/native/ant_common.h"
#include "internal/ll.h"
#include "internal/ocf_native_internal.h"

#include "iotjs_def.h"
#include "iotjs_uv_request.h"
#include "modules/iotjs_module_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ANT_API_VOID_TO_VOID(ocf, adapter_start);

// TODO: add a function to reset g_uv_async (hardcoded now)
// Async handler order: ocf event handler -> ant async -> uv async -> js
// uv async handler
typedef struct ant_async ant_async_t;
struct ant_async {
  uv_async_t uv_async;      // libuv async
  jerry_value_t js_handler; // JS async handler
  ll_t *events;
};

// ocf.adapter.onPrepareServer
ant_async_t ocf_adapter_onPrepareServer_async;
static void ocf_adapter_onPrepareServer_ant_handler() {
  uv_async_send(&ocf_adapter_onPrepareServer_async.uv_async);
}
static void ocf_adapter_onPrepareServer_uv_handler(uv_async_t *handle) {
  jerry_value_t js_handler = ocf_adapter_onPrepareServer_async.js_handler;
  iotjs_invoke_callback(js_handler, jerry_create_undefined(), NULL, 0);
}

JS_FUNCTION(ocf_adapter_onPrepareServer) {
  jerry_value_t argHandler;
  DJS_CHECK_ARGS(1, function);
  argHandler = JS_GET_ARG(0, function);

  // IoTivity -> ant_async -(ITC)-> uv_async -> js_async
  // Store JS async handler
  if (ocf_adapter_onPrepareServer_async.js_handler != '\0') {
    fprintf(stderr, "ERROR: JS handler already registered!"
                    " (unique handler constraint)");
    return jerry_create_boolean(false);
  }
  ocf_adapter_onPrepareServer_async.js_handler = argHandler;

  // Register uv async handler to libuv
  {
    iotjs_environment_t *env = iotjs_environment_get();
    uv_loop_t *loop = iotjs_environment_loop(env);
    uv_async_init(loop, &ocf_adapter_onPrepareServer_async.uv_async,
                  ocf_adapter_onPrepareServer_uv_handler);
  }

  // Register ant async handler to internal
  ocf_adapter_onPrepareServer_internal(ocf_adapter_onPrepareServer_ant_handler);

  return jerry_create_boolean(true);
}

jerry_value_t InitOCFNative() {
  jerry_value_t ocfNative = jerry_create_object();
  REGISTER_ANT_API(ocfNative, ocf, adapter_start);
  REGISTER_ANT_API(ocfNative, ocf, adapter_onPrepareServer);

  initOCF();

  return ocfNative;
}