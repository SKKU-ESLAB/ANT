#include "../../common/native/ant_common.h"
#include "ant_async.h"
#include "internal/ll.h"
#include "internal/ocf_adapter_internal.h"
#include "ocf_resource.h"

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
  jerry_value_t jsHandler = GET_JS_HANDLER(ocf_adapter_onInitialize);
  iotjs_invoke_callback(jsHandler, jerry_create_undefined(), NULL, 0);
}
ASYNC_REGISTER(ocf_adapter_onInitialize, NULL)
JS_FUNCTION(ocf_adapter_onInitialize) {
  bool result;
  jerry_value_t argHandler;
  DJS_CHECK_ARGS(1, function);
  argHandler = JS_GET_ARG(0, function);

  printf("onInitialize setting: %d\n", (int)argHandler);
  fflush(stdout);

  result = CALL_ASYNC_REGISTER(ocf_adapter_onInitialize, argHandler);
  return jerry_create_boolean(result);
}

// OCFAdapter.onPrepareServer()
DECLARE_GLOBAL_ASYNC(ocf_adapter_onPrepareServer)
DECLARE_ANT_ASYNC_HANDLER(ocf_adapter_onPrepareServer)
UV_ASYNC_HANDLER(ocf_adapter_onPrepareServer) {
  jerry_value_t jsHandler = GET_JS_HANDLER(ocf_adapter_onPrepareServer);
  iotjs_invoke_callback(jsHandler, jerry_create_undefined(), NULL, 0);
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
  jerry_value_t jsHandler = GET_JS_HANDLER(ocf_adapter_onPrepareClient);
  iotjs_invoke_callback(jsHandler, jerry_create_undefined(), NULL, 0);
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
ANT_API_VOID_TO_VOID(ocf_adapter, start);

// OCFAdapter.stop()
ANT_API_VOID_TO_VOID(ocf_adapter, stop);

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

// OCFAdapter.addResource()
JS_FUNCTION(ocf_adapter_addResource) {
  bool result;
  jerry_value_t argOCFResource;
  DJS_CHECK_ARGS(1, object);
  argOCFResource = JS_GET_ARG(0, object);

  JS_DECLARE_PTR2(argOCFResource, void, ocf_resource_nobject, ocf_resource);
  result = ocf_adapter_addResource_internal(ocf_resource_nobject);
  return jerry_create_boolean(result);
}

// OCFAdapter.repStartRootObject()
ANT_API_VOID_TO_VOID(ocf_adapter, repStartRootObject);

// OCFAdapter.repSetBoolean()
JS_FUNCTION(ocf_adapter_repSetBoolean) {
  iotjs_string_t argKey;
  jerry_value_t argValue;
  DJS_CHECK_ARGS(2, string, boolean);
  argKey = JS_GET_ARG(0, string);
  argValue = JS_GET_ARG(1, boolean);
  const char *key = iotjs_string_data(&argKey);
  bool value = iotjs_jval_as_boolean(argValue);

  ocf_adapter_repSetBoolean_internal(key, value);
  return jerry_create_undefined();
}

// OCFAdapter.repSetInt()
JS_FUNCTION(ocf_adapter_repSetInt) {
  iotjs_string_t argKey;
  jerry_value_t argValue;
  DJS_CHECK_ARGS(2, string, number);
  argKey = JS_GET_ARG(0, string);
  argValue = JS_GET_ARG(1, number);
  const char *key = iotjs_string_data(&argKey);
  int value = (int)iotjs_jval_as_number(argValue);

  ocf_adapter_repSetInt_internal(key, value);
  return jerry_create_undefined();
}

// OCFAdapter.repSetDouble()
JS_FUNCTION(ocf_adapter_repSetDouble) {
  iotjs_string_t argKey;
  jerry_value_t argValue;
  DJS_CHECK_ARGS(2, string, number);
  argKey = JS_GET_ARG(0, string);
  argValue = JS_GET_ARG(1, number);
  const char *key = iotjs_string_data(&argKey);
  double value = iotjs_jval_as_number(argValue);

  ocf_adapter_repSetDouble_internal(key, value);
  return jerry_create_undefined();
}

// OCFAdapter.repSetString()
JS_FUNCTION(ocf_adapter_repSetString) {
  iotjs_string_t argKey;
  iotjs_string_t argValue;
  DJS_CHECK_ARGS(2, string, string);
  argKey = JS_GET_ARG(0, string);
  argValue = JS_GET_ARG(1, string);
  const char *key = iotjs_string_data(&argKey);
  const char *value = iotjs_string_data(&argValue);

  ocf_adapter_repSetString_internal(key, value);
  return jerry_create_undefined();
}

// OCFAdapter.repEndRootObject()
ANT_API_VOID_TO_VOID(ocf_adapter, repEndRootObject);

// OCFAdapter.repSetString()
JS_FUNCTION(ocf_adapter_repSendResponse) {
  jerry_value_t argRequest;
  jerry_value_t argStatusCode;
  DJS_CHECK_ARGS(2, object, number);
  argRequest = JS_GET_ARG(0, object);
  argStatusCode = JS_GET_ARG(1, number);
  JS_DECLARE_PTR2(argRequest, void, ocf_request_nobject, ocf_request);
  int status_code = (int)iotjs_jval_as_number(argStatusCode);

  ocf_adapter_sendResponse_internal(ocf_request_nobject, status_code);
  return jerry_create_undefined();
}

void InitOCFAdapterNative(jerry_value_t ocfNative) {
  // OCF Thread Initialization Handlers
  REGISTER_ANT_API(ocfNative, ocf_adapter, onInitialize);
  REGISTER_ANT_API(ocfNative, ocf_adapter, onPrepareServer);
  REGISTER_ANT_API(ocfNative, ocf_adapter, onPrepareClient);

  // OCF Thread
  REGISTER_ANT_API(ocfNative, ocf_adapter, start);
  REGISTER_ANT_API(ocfNative, ocf_adapter, stop);

  // Server-side Initialization
  REGISTER_ANT_API(ocfNative, ocf_adapter, setPlatform);
  REGISTER_ANT_API(ocfNative, ocf_adapter, addDevice);
  REGISTER_ANT_API(ocfNative, ocf_adapter, addResource);

  // Server-side Request Handler
  REGISTER_ANT_API(ocfNative, ocf_adapter, repStartRootObject);
  REGISTER_ANT_API(ocfNative, ocf_adapter, repSetBoolean);
  REGISTER_ANT_API(ocfNative, ocf_adapter, repSetInt);
  REGISTER_ANT_API(ocfNative, ocf_adapter, repSetDouble);
  REGISTER_ANT_API(ocfNative, ocf_adapter, repSetString);
  REGISTER_ANT_API(ocfNative, ocf_adapter, repEndRootObject);
  REGISTER_ANT_API(ocfNative, ocf_adapter, repSendResponse);

  // Initialize IoTivity Lite
  initOCFAdapter();
}