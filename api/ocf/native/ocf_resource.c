#include "ocf_resource.h"
#include "../../common/native/ant_common.h"
#include "ant_async.h"
#include "internal/ll.h"
#include "internal/ocf_resource_internal.h"

#include "iotjs_def.h"
#include "iotjs_uv_request.h"
#include "modules/iotjs_module_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  iotjs_string_t argName, argUri;
  jerry_value_t argTypes, argInterfaceMask, argDefaultInterfaceMask,
      argDeviceId;
  DJS_CHECK_ARGS(1, object);
  argSelf = JS_GET_ARG(0, object);
  argName = iotjs_jval_as_string(iotjs_jval_get_property(argSelf, "name"));
  argUri = iotjs_jval_as_string(iotjs_jval_get_property(argSelf, "uri"));
  argTypes = iotjs_jval_as_array(iotjs_jval_get_property(argSelf, "types"));
  argInterfaceMask = iotjs_jval_get_property(argSelf, "interface_mask");
  argDefaultInterfaceMask =
      iotjs_jval_get_property(argSelf, "default_interface_mask");
  argDeviceId =
      iotjs_jval_get_property(iotjs_jval_get_property(argSelf, "device"), "id");

  const char *name = iotjs_string_data(&argName);
  const char *uri = iotjs_string_data(&argUri);
  size_t types_len =
      iotjs_jval_as_number(iotjs_jval_get_property(argTypes, "length"));
  const char **types = (const char **)malloc(sizeof(const char *) * types_len);
  for (uint32_t i = 0; i < types_len; i++) {
    iotjs_string_t type_jsstr =
        iotjs_jval_as_string(iotjs_jval_get_property_by_index(argTypes, i));
    types[i] = iotjs_string_data(&type_jsstr);
  }
  int interface_mask = (int)iotjs_jval_as_number(argInterfaceMask);
  int default_interface_mask =
      (int)iotjs_jval_as_number(argDefaultInterfaceMask);
  int device_id = (int)iotjs_jval_as_number(argDeviceId);

  void *ocf_resource_nobject = ocf_resource_constructor_internal(
      name, uri, types, types_len, interface_mask, default_interface_mask,
      device_id);
  free(types);

  // set native pointer of OCFResource with oc_resource
  jerry_set_object_native_pointer(argSelf, ocf_resource_nobject,
                                  &ocf_resource_native_info);
  IOTJS_ASSERT(jerry_get_object_native_pointer(argSelf, NULL,
                                               &ocf_resource_native_info));

  return jerry_create_undefined();
}

// OCFResource.setDiscoverable()
JS_FUNCTION(ocf_resource_setDiscoverable) {
  jerry_value_t argSelf, argIsDiscoverable;
  DJS_CHECK_ARGS(2, object, boolean);
  argSelf = JS_GET_ARG(0, object);
  argIsDiscoverable = JS_GET_ARG(0, boolean);
  bool is_discoverable = iotjs_jval_as_boolean(argIsDiscoverable);
  JS_DECLARE_PTR2(argSelf, void, self, ocf_resource);

  ocf_resource_setDiscoverable_internal(self, is_discoverable);
  return jerry_create_undefined();
}

// OCFResource.setPeriodicObservable()
JS_FUNCTION(ocf_resource_setPeriodicObservable) {
  jerry_value_t argSelf, argPeriodSec;
  DJS_CHECK_ARGS(2, object, number);
  argSelf = JS_GET_ARG(0, object);
  argPeriodSec = JS_GET_ARG(0, number);
  int period_sec = (int)iotjs_jval_as_number(argPeriodSec);
  JS_DECLARE_PTR2(argSelf, void, self, ocf_resource);

  ocf_resource_setPeriodicObservable_internal(self, period_sec);
  return jerry_create_undefined();
}

// OCFResource.setHandler()
DECLARE_GLOBAL_ASYNC(ocf_resource_setHandler)
DECLARE_ANT_ASYNC_HANDLER(ocf_resource_setHandler)
UV_ASYNC_HANDLER(ocf_resource_setHandler) {
  ocf_resource_setHandler_event_t *event =
      (ocf_resource_setHandler_event_t *)GET_MR_EVENT(ocf_resource_setHandler);
  jerry_value_t js_handler = GET_JS_HANDLER(ocf_resource_setHandler);

  jerry_value_t js_ocf_request, js_method;

  // separate response
  ocf_resource_setHandler_handler_internal(event->sep_response);

  // Args 1: OCFRequest request
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

  // set native pointer of OCFRequest with oc_separate_response
  jerry_set_object_native_pointer(js_ocf_request, event->sep_response,
                                  &ocf_request_native_info);
  IOTJS_ASSERT(jerry_get_object_native_pointer(js_ocf_request, NULL,
                                               &ocf_request_native_info));

  // Args 2: int method
  js_method = jerry_create_number((double)event->method);
  jerry_value_t js_args[] = {js_ocf_request, js_method};

  iotjs_invoke_callback(js_handler, jerry_create_undefined(), js_args, 2);
  DESTROY_EVENTS(ocf_resource_setHandler);
}
ASYNC_REGISTER(ocf_resource_setHandler, ocf_resource_setHandler_event_destroyer)
JS_FUNCTION(ocf_resource_setHandler) {
  bool result;
  jerry_value_t argSelf, argMethod, argHandler;
  DJS_CHECK_ARGS(3, object, number, function);
  argSelf = JS_GET_ARG(0, object);
  argMethod = JS_GET_ARG(1, number);
  argHandler = JS_GET_ARG(2, function);
  JS_DECLARE_PTR2(argSelf, void, self, ocf_resource);
  int method = (int)iotjs_jval_as_number(argMethod);

  // register ant async handler
  result = CALL_ASYNC_REGISTER(ocf_resource_setHandler, argHandler);

  // register a handler to IoTivity Lite
  ocf_resource_setHandler_internal(self, method);
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