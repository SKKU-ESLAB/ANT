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

// TODO: string length hard-coding
#define MAX_STRING_LEN 100

// OCFAdapter native object declaration
struct ocf_adapter_s {
  char mfg_name[MAX_STRING_LEN];
  ll_t *devices;

  jerry_value_t jobject;
};
typedef struct ocf_adapter_s ocf_adapter_t;
ANT_DEFINE_NATIVE_HANDLE_INFO(ocf_adapter);

// OCFDevice native object declaration
struct ocf_device_s {
  char uri[MAX_STRING_LEN];
  char resource_type[MAX_STRING_LEN];
  char name[MAX_STRING_LEN];
  char spec_version[MAX_STRING_LEN];
  char data_model_version[MAX_STRING_LEN];
};
typedef struct ocf_device_s ocf_device_t;
void devices_item_destroyer(void *item) {
  ocf_device_t *device;
  device = (ocf_device_t *)item;
  free(device);
}

// OCFAdapter native object operations
ocf_adapter_t *ocf_adapter_create(const jerry_value_t jobject) {
  ocf_adapter_t *nobject = (ocf_adapter_t *)malloc(sizeof(ocf_adapter_t));
  nobject->mfg_name[0] = '\0';
  nobject->devices = ll_new(devices_item_destroyer);
  nobject->jobject = jobject;

  jerry_set_object_native_pointer(argSelf, nobject,
                                  &GET_NATIVE_INFO(ocf_adapter));
  IOTJS_ASSERT(jerry_get_object_native_pointer(jobject, NULL,
                                               &GET_NATIVE_INFO(ocf_adapter)));
  return oa;
}
static void ocf_adapter_destroy(ocf_adapter_t *nobject) {
  ll_delete(nobject->devices);
  IOTJS_RELEASE(nobject);
}

// OCFAdapter.constructor()
JS_FUNCTION(ocf_adapter_constructor) {
  bool result;
  jerry_value_t argSelf, argMfgName;
  DJS_CHECK_ARGS(2, object, string);
  argSelf = JS_GET_ARG(0, object);
  argMfgName = JS_GET_ARG(1, string);

  ocf_adapter_t *oa = ocf_adapter_create(argSelf);
  strncpy(oa->mfg_name, iotjs_string_data(argMfgName), MAX_STRING_LEN);

  result = CALL_ASYNC_REGISTER(ocf_adapter_onPrepareClient, argSelf);
  return jerry_create_undefined();
}

// OCFAdapter.setPlatform()
JS_FUNCTION(ocf_adapter_setPlatform) {
  bool result;
  jerry_value_t argSelf, argMfgName;
  DJS_CHECK_ARGS(2, object, string);
  argSelf = JS_GET_ARG(0, object);
  argMfgName = JS_GET_ARG(1, string);

  ocf_adapter_t *oa = GET_NOBJECT_P(argSelf, ocf_adapter);
  strncpy(oa->mfg_name, iotjs_string_data(argMfgName), MAX_STRING_LEN);

  return jerry_create_undefined();
}

// OCFAdapter.addDevice()
JS_FUNCTION(ocf_adapter_addDevice) {
  bool result;
  jerry_value_t argSelf, argUri, argResourceType, argName, argSpecVersion,
      argDataModelVersion;
  DJS_CHECK_ARGS(6, object, string, string, string, string, string);
  argSelf = JS_GET_ARG(0, object);
  argUri = JS_GET_ARG(1, string);
  argResourceType = JS_GET_ARG(2, string);
  argName = JS_GET_ARG(3, string);
  argSpecVersion = JS_GET_ARG(4, string);
  argDataModelVersion = JS_GET_ARG(5, string);

  ocf_adapter_t *oa = GET_NOBJECT_P(argSelf, ocf_adapter);
  ocf_device_t *device = (ocf_device_t *)malloc(sizeof(ocf_device_t));

  strncpy(device->uri, iotjs_string_data(argUri), MAX_STRING_LEN);
  strncpy(device->resource_type, iotjs_string_data(argResourceType),
          MAX_STRING_LEN);
  strncpy(device->name, iotjs_string_data(argName), MAX_STRING_LEN);
  strncpy(device->spec_version, iotjs_string_data(argSpecVersion),
          MAX_STRING_LEN);
  strncpy(device->data_model_version, iotjs_string_data(argDataModelVersion),
          MAX_STRING_LEN);

  return jerry_create_undefined();
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

void InitOCFAdapterNative(jerry_value_t ocfNative) {
  REGISTER_ANT_API(ocfNative, ocf, adapter_constructor);
  REGISTER_ANT_API(ocfNative, ocf, adapter_setPlatform);
  REGISTER_ANT_API(ocfNative, ocf, adapter_addDevice);
  REGISTER_ANT_API(ocfNative, ocf, adapter_onPrepareServer);
  REGISTER_ANT_API(ocfNative, ocf, adapter_onPrepareClient);
  REGISTER_ANT_API(ocfNative, ocf, adapter_start);
  REGISTER_ANT_API(ocfNative, ocf, adapter_stop);
}