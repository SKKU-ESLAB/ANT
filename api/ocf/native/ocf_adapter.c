#include "../../common/native/ant_common.h"
#include "ant_async.h"
#include "internal/ll.h"
#include "internal/ocf_adapter_internal.h"

#include "iotjs_def.h"
#include "iotjs_uv_request.h"
#include "modules/iotjs_module_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// OCFAdapter.onInitialize()
DECLARE_GLOBAL_ASYNC(ocf_adapter_onInitialize)
DECLARE_ANT_ASYNC_HANDLER(ocf_adapter_onInitialize)
UV_ASYNC_HANDLER(ocf_adapter_onInitialize) {
  // void *event = GET_MR_EVENT(ocf_adapter_onInitialize);
  jerry_value_t jsHandler = GET_JS_HANDLER(ocf_adapter_onInitialize);
  iotjs_invoke_callback(jsHandler, jerry_create_undefined(), NULL, 0);
  // DESTROY_EVENTS(ocf_adapter_onInitialize);
}
ASYNC_REGISTER(ocf_adapter_onInitialize, NULL)
JS_FUNCTION(ocf_adapter_onInitialize) {
  bool result;
  jerry_value_t argHandler;
  DJS_CHECK_ARGS(1, function);
  argHandler = JS_GET_ARG(0, function);

  result = CALL_ASYNC_REGISTER(ocf_adapter_onInitialize, argHandler);
  return jerry_create_boolean(result);
}

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

// OCFAdapter.setPlatform()
JS_FUNCTION(ocf_adapter_setPlatform) {
  bool result;
  iotjs_string_t argMfgName;
  DJS_CHECK_ARGS(1, string);
  argMfgName = JS_GET_ARG(0, string);

  result = ocf_adapter_setPlatform_internal(iotjs_string_data(&argMfgName));
  return jerry_create_boolean(result);
}

// OCFAdapter.addDevice()
JS_FUNCTION(ocf_adapter_addDevice) {
  bool result;
  iotjs_string_t argUri, argResourceType, argName, argSpecVersion,
      argDataModelVersion;
  DJS_CHECK_ARGS(5, string, string, string, string, string);
  argUri = JS_GET_ARG(0, string);
  argResourceType = JS_GET_ARG(1, string);
  argName = JS_GET_ARG(2, string);
  argSpecVersion = JS_GET_ARG(3, string);
  argDataModelVersion = JS_GET_ARG(4, string);

  result = ocf_adapter_addDevice_internal(
      iotjs_string_data(&argUri), iotjs_string_data(&argResourceType),
      iotjs_string_data(&argName), iotjs_string_data(&argSpecVersion),
      iotjs_string_data(&argDataModelVersion));
  return jerry_create_boolean(result);
}

void InitOCFAdapterNative(jerry_value_t ocfNative) {
  REGISTER_ANT_API(ocfNative, ocf, adapter_onInitialize);
  REGISTER_ANT_API(ocfNative, ocf, adapter_onPrepareServer);
  REGISTER_ANT_API(ocfNative, ocf, adapter_onPrepareClient);

  REGISTER_ANT_API(ocfNative, ocf, adapter_start);
  REGISTER_ANT_API(ocfNative, ocf, adapter_stop);

  REGISTER_ANT_API(ocfNative, ocf, adapter_setPlatform);
  REGISTER_ANT_API(ocfNative, ocf, adapter_addDevice);

  // Initialize IoTivity Lite
  initOCFAdapter();
}