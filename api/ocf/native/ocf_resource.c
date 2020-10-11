#include "ocf_resource.h"
#include "../../common/native/ant_common.h"
#include "internal/ant_async.h"
#include "internal/ll.h"
#include "internal/ocf_resource_internal.h"

#include "iotjs_def.h"
#include "iotjs_uv_request.h"
#include "modules/iotjs_module_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ocf_resource_init(void) { INIT_ANT_ASYNC(ocf_resource_setHandler); }

// OCFResource
void ocf_resource_destroy(void *handle);
const jerry_object_native_info_t ocf_resource_native_info = {
    .free_cb = (jerry_object_native_free_callback_t)ocf_resource_destroy,
};
void ocf_resource_destroy(void *handle) {
  // Do not anything
  // (The lifecycle of OCFResource is controlled by IoTivity Lite.)
}

// OCFRequest
void ocf_request_destroy(void *handle);
const jerry_object_native_info_t ocf_request_native_info = {
    .free_cb = (jerry_object_native_free_callback_t)ocf_request_destroy,
};
void ocf_request_destroy(void *handle) {
  // Do not anything
  // (The lifecycle of OCFRequest is controlled by IoTivity Lite.)
}

// OCFResource.addResource()
JS_FUNCTION(ocf_resource_constructor) {
  jerry_value_t argSelf;
  DJS_CHECK_ARGS(1, object);

  // Arg 0: object self
  argSelf = JS_GET_ARG(0, object);

  // Get properties from self
  jerry_value_t jsName = iotjs_jval_get_property(argSelf, "name");
  jerry_value_t jsUri = iotjs_jval_get_property(argSelf, "uri");
  jerry_value_t jsTypes =
      iotjs_jval_as_array(iotjs_jval_get_property(argSelf, "types"));
  jerry_value_t jsInterfaceMask =
      iotjs_jval_get_property(argSelf, "interface_mask");
  jerry_value_t jsDefaultInterfaceMask =
      iotjs_jval_get_property(argSelf, "default_interface_mask");
  jerry_value_t jsDevice = iotjs_jval_get_property(argSelf, "device");
  jerry_value_t jsDeviceId = iotjs_jval_get_property(jsDevice, "id");

  // string name
  iotjs_string_t jsstrName = iotjs_jval_as_string(jsName);
  const char *name = iotjs_string_data(&jsstrName);

  // uri
  iotjs_string_t jsstrUri = iotjs_jval_as_string(jsUri);
  const char *uri = iotjs_string_data(&jsstrUri);

  // types
  jerry_value_t jsTypesLength = iotjs_jval_get_property(jsTypes, "length");
  size_t types_length = iotjs_jval_as_number(jsTypesLength);
  char **types = (char **)malloc(sizeof(const char *) * types_length);
  for (uint32_t i = 0; i < types_length; i++) {
    jerry_value_t jsType = iotjs_jval_get_property_by_index(jsTypes, i);
    iotjs_string_t jsstrType = iotjs_jval_as_string(jsType);

    const char *type = iotjs_string_data(&jsstrType);
    types[i] = (char *)malloc(strlen(type) + 1);
    strncpy(types[i], type, strlen(type) + 1);

    iotjs_string_destroy(&jsstrType);
    jerry_release_value(jsType);
  }

  // interface_mask, default_interface_mask, device_id
  int interface_mask = (int)iotjs_jval_as_number(jsInterfaceMask);
  int default_interface_mask =
      (int)iotjs_jval_as_number(jsDefaultInterfaceMask);
  int device_id = (int)iotjs_jval_as_number(jsDeviceId);

  void *ocf_resource_nobject = ocf_resource_constructor_internal(
      name, uri, (const char **)types, types_length, interface_mask,
      default_interface_mask, device_id);
  for (size_t i = 0; i < types_length; i++) {
    free(types[i]);
  }
  free(types);

  // set native pointer of OCFResource with oc_resource
  jerry_set_object_native_pointer(argSelf, ocf_resource_nobject,
                                  &ocf_resource_native_info);
  IOTJS_ASSERT(jerry_get_object_native_pointer(argSelf, NULL,
                                               &ocf_resource_native_info));

  // Release properties
  iotjs_string_destroy(&jsstrName);
  iotjs_string_destroy(&jsstrUri);
  jerry_release_value(jsName);
  jerry_release_value(jsUri);
  jerry_release_value(jsTypes);
  jerry_release_value(jsInterfaceMask);
  jerry_release_value(jsDefaultInterfaceMask);
  jerry_release_value(jsDevice);
  jerry_release_value(jsDeviceId);
  jerry_release_value(jsTypesLength);

  return jerry_create_undefined();
}

// OCFResource.setDiscoverable()
JS_FUNCTION(ocf_resource_setDiscoverable) {
  jerry_value_t argSelf;
  bool argIsDiscoverable;
  DJS_CHECK_ARGS(2, object, boolean);
  argSelf = JS_GET_ARG(0, object);
  argIsDiscoverable = JS_GET_ARG(1, boolean);
  JS_DECLARE_PTR2(argSelf, void, self, ocf_resource);

  ocf_resource_setDiscoverable_internal(self, argIsDiscoverable);
  return jerry_create_undefined();
}

// OCFResource.setPeriodicObservable()
JS_FUNCTION(ocf_resource_setPeriodicObservable) {
  jerry_value_t argSelf;
  int argPeriodSec;
  DJS_CHECK_ARGS(2, object, number);
  argSelf = JS_GET_ARG(0, object);
  argPeriodSec = (int)JS_GET_ARG(1, number);
  JS_DECLARE_PTR2(argSelf, void, self, ocf_resource);

  ocf_resource_setPeriodicObservable_internal(self, argPeriodSec);
  return jerry_create_undefined();
}

// OCFResource.setHandler()
GLOBAL_ANT_ASYNC(ocf_resource_setHandler)
REGISTER_ANT_ASYNC_HANDLER_FUNC(ocf_resource_setHandler,
                                ocf_resource_setHandler_event_destroyer)
EMIT_ANT_ASYNC_EVENT_FUNC(ocf_resource_setHandler)
ANT_UV_HANDLER_FUNCTION(ocf_resource_setHandler) {
  ocf_resource_setHandler_event_t *event =
      (ocf_resource_setHandler_event_t *)GET_MR_EVENT(ocf_resource_setHandler);
  jerry_value_t js_handler = GET_JS_HANDLER(ocf_resource_setHandler);

  jerry_value_t js_ocf_request, js_method;

  // Args 0: OCFRequest request
  js_ocf_request = jerry_create_object();
  iotjs_string_t origin_addr_jsstr = iotjs_string_create();
  iotjs_string_t dest_uri_jsstr = iotjs_string_create();
  iotjs_string_t query_jsstr = iotjs_string_create();
  iotjs_string_t request_payload_string_jsstr = iotjs_string_create();
  iotjs_string_append(&origin_addr_jsstr, event->origin_addr,
                      strlen(event->origin_addr));
  iotjs_string_append(&dest_uri_jsstr, event->dest_uri,
                      strlen(event->dest_uri));
  iotjs_string_append(&query_jsstr, event->query, strlen(event->query));
  iotjs_string_append(&request_payload_string_jsstr,
                      event->request_payload_string,
                      strlen(event->request_payload_string));
  iotjs_jval_set_property_string(js_ocf_request, "origin_addr",
                                 &origin_addr_jsstr);
  iotjs_jval_set_property_number(js_ocf_request, "dest_device_id",
                                 (int)event->dest_device_id);
  iotjs_jval_set_property_string(js_ocf_request, "dest_uri", &dest_uri_jsstr);
  iotjs_jval_set_property_string(js_ocf_request, "query", &query_jsstr);
  iotjs_jval_set_property_string(js_ocf_request, "request_payload_string",
                                 &request_payload_string_jsstr);

  // set native pointer of OCFRequest with oc_request_t
  jerry_set_object_native_pointer(js_ocf_request, event->request,
                                  &ocf_request_native_info);
  IOTJS_ASSERT(jerry_get_object_native_pointer(js_ocf_request, NULL,
                                               &ocf_request_native_info));

  // Args 1: int method
  js_method = jerry_create_number((double)event->method);
  jerry_value_t js_args[] = {js_ocf_request, js_method};

  // call JS handler
  iotjs_invoke_callback(js_handler, jerry_create_undefined(), js_args, 2);

  // Release arguments
  iotjs_string_destroy(&origin_addr_jsstr);
  iotjs_string_destroy(&dest_uri_jsstr);
  iotjs_string_destroy(&query_jsstr);
  iotjs_string_destroy(&request_payload_string_jsstr);
  jerry_release_value(js_ocf_request);
  jerry_release_value(js_method);

  // wake up OCF thread
  pthread_mutex_lock(&event->sync_mutex);
  pthread_cond_signal(&event->sync_cond);
  pthread_mutex_unlock(&event->sync_mutex);

  DESTROY_EVENTS(ocf_resource_setHandler);
}
JS_FUNCTION(ocf_resource_setHandler) {
  bool result;
  jerry_value_t argSelf;
  int argHandlerId;
  int argMethod;
  jerry_value_t argHandler;
  DJS_CHECK_ARGS(4, object, number, number, function);
  argSelf = JS_GET_ARG(0, object);
  argHandlerId = JS_GET_ARG(1, number);
  argMethod = JS_GET_ARG(2, number);
  argHandler = JS_GET_ARG(3, function);
  JS_DECLARE_PTR2(argSelf, void, self, ocf_resource);

  // register ant async handler
  result =
      REGISTER_ANT_HANDLER(ocf_resource_setHandler, argHandlerId, argHandler);

  // register a handler to IoTivity Lite
  ocf_resource_setHandler_internal(self, argHandlerId, argMethod);
  return jerry_create_boolean(result);
}

void InitOCFResourceNative(jerry_value_t ocfNative) {
  REGISTER_ANT_API(ocfNative, ocf_resource, constructor);
  REGISTER_ANT_API(ocfNative, ocf_resource, setDiscoverable);
  REGISTER_ANT_API(ocfNative, ocf_resource, setPeriodicObservable);
  REGISTER_ANT_API(ocfNative, ocf_resource, setHandler);

  // Initialize IoTivity Lite
  // initOCFResource();
}