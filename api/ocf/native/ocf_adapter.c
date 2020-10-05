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

// OCFEndPoint
const jerry_object_native_info_t ocf_endpoint_native_info = {
    .free_cb = (jerry_object_native_free_callback_t)ocf_endpoint_destroy,
};

// OCFAdapter.discovery()
DECLARE_GLOBAL_ASYNC(ocf_adapter_discovery)
DECLARE_ANT_ASYNC_HANDLER(ocf_adapter_discovery)
UV_ASYNC_HANDLER(ocf_adapter_discovery) {
  ocf_adapter_discovery_event_t *event =
      (ocf_adapter_discovery_event_t *)GET_MR_EVENT(ocf_adapter_discovery);
  jerry_value_t js_handler = GET_JS_HANDLER(ocf_adapter_discovery);

  // Args 0: object endpoint
  // set native pointer of OCFEndPoint with oc_endpoint_t
  jerry_value_t js_endpoint = jerry_create_object();
  jerry_set_object_native_pointer(js_endpoint, event->endpoint,
                                  &ocf_endpoint_native_info);
  IOTJS_ASSERT(jerry_get_object_native_pointer(js_endpoint, NULL,
                                               &ocf_endpoint_native_info));

  // Args 1: string uri
  jerry_value_t js_uri =
      jerry_create_string_from_utf8((const jerry_char_t *)event->uri);

  // Args 2: array<string> types
  jerry_value_t js_types = jerry_create_object();
  for (int i = 0; i < event->types->len; i++) {
    char *type_item = (char *)ll_get_n(event->types, i);
    jerry_value_t js_type_item =
        jerry_create_string_from_utf8((const jerry_char_t *)type_item);
    iotjs_jval_set_property_by_index(js_types, (uint32_t)i, js_type_item);
  }

  // Args 3: int interface_mask
  jerry_value_t js_interface_mask =
      jerry_create_number((double)event->interface_mask);

  jerry_value_t js_args[] = {js_endpoint, js_uri, js_types, js_interface_mask};

  iotjs_invoke_callback(js_handler, jerry_create_undefined(), js_args, 4);
  DESTROY_EVENTS(ocf_adapter_discovery);
}
ASYNC_REGISTER(ocf_adapter_discovery, ocf_adapter_discovery_event_destroyer)
JS_FUNCTION(ocf_adapter_discovery) {
  bool result;
  iotjs_string_t argResourceType;
  jerry_value_t argDiscoveryHandler;
  DJS_CHECK_ARGS(2, string, function);
  argResourceType = JS_GET_ARG(0, string);
  argDiscoveryHandler = JS_GET_ARG(1, function);
  const char *resource_type = iotjs_string_data(&argResourceType);

  result = CALL_ASYNC_REGISTER(ocf_adapter_discovery, argDiscoveryHandler);
  ocf_adapter_discovery_internal(resource_type);
  return jerry_create_boolean(result);
}

// OCFAdapter.observe()
JS_FUNCTION(ocf_adapter_observe) {
  jerry_value_t argEndpoint;
  iotjs_string_t argUri;
  jerry_value_t argResponseHandler;
  DJS_CHECK_ARGS(3, object, string, function);
  argEndpoint = JS_GET_ARG(0, object);
  argUri = JS_GET_ARG(1, string);
  argResponseHandler = JS_GET_ARG(2, function);
  // TODO: parsing endpoint
  const char *uri = iotjs_string_data(&argUri);
  IOTJS_UNUSED(uri);
  IOTJS_UNUSED(argEndpoint);
  IOTJS_UNUSED(argResponseHandler);

  // TODO: ocf_adapter_observe_internal(..., uri, argResponseHandler);
  return jerry_create_undefined();
}

// OCFAdapter.stopObserve()
JS_FUNCTION(ocf_adapter_stopObserve) {
  jerry_value_t argEndpoint;
  iotjs_string_t argUri;
  DJS_CHECK_ARGS(2, object, string);
  argEndpoint = JS_GET_ARG(0, object);
  argUri = JS_GET_ARG(1, string);
  // TODO: parsing endpoint
  const char *uri = iotjs_string_data(&argUri);
  IOTJS_UNUSED(argEndpoint);
  IOTJS_UNUSED(uri);

  // TODO: ocf_adapter_stopObserve_internal(..., uri);
  return jerry_create_undefined();
}

// OCFAdapter.get()
JS_FUNCTION(ocf_adapter_get) {
  jerry_value_t argEndpoint;
  iotjs_string_t argUri;
  iotjs_string_t argQuery;
  jerry_value_t argQos;
  jerry_value_t argResponseHandler;
  DJS_CHECK_ARGS(5, object, string, string, number, function);
  argEndpoint = JS_GET_ARG(0, object);
  argUri = JS_GET_ARG(1, string);
  argQuery = JS_GET_ARG(2, string);
  argQos = JS_GET_ARG(3, number);
  argResponseHandler = JS_GET_ARG(4, function);
  // TODO: parsing endpoint
  const char *uri = iotjs_string_data(&argUri);
  const char *query = iotjs_string_data(&argQuery);
  int qos = (int)iotjs_jval_as_number(argQos);
  IOTJS_UNUSED(argEndpoint);
  IOTJS_UNUSED(argResponseHandler);
  IOTJS_UNUSED(uri);
  IOTJS_UNUSED(query);
  IOTJS_UNUSED(qos);

  // TODO: ocf_adapter_get_internal(..., uri, query, qos, argResponseHandler);
  return jerry_create_undefined();
}

// OCFAdapter.delete()
JS_FUNCTION(ocf_adapter_delete) {
  jerry_value_t argEndpoint;
  iotjs_string_t argUri;
  iotjs_string_t argQuery;
  jerry_value_t argQos;
  jerry_value_t argResponseHandler;
  DJS_CHECK_ARGS(5, object, string, string, number, function);
  argEndpoint = JS_GET_ARG(0, object);
  argUri = JS_GET_ARG(1, string);
  argQuery = JS_GET_ARG(2, string);
  argQos = JS_GET_ARG(3, number);
  argResponseHandler = JS_GET_ARG(4, function);
  // TODO: parsing endpoint
  const char *uri = iotjs_string_data(&argUri);
  const char *query = iotjs_string_data(&argQuery);
  int qos = (int)iotjs_jval_as_number(argQos);
  IOTJS_UNUSED(argEndpoint);
  IOTJS_UNUSED(argResponseHandler);
  IOTJS_UNUSED(uri);
  IOTJS_UNUSED(query);
  IOTJS_UNUSED(qos);

  // TODO: ocf_adapter_delete_internal(..., uri, query, qos,
  // argResponseHandler);
  return jerry_create_undefined();
}

// OCFAdapter.initPost()
JS_FUNCTION(ocf_adapter_initPost) {
  jerry_value_t argEndpoint;
  iotjs_string_t argUri;
  iotjs_string_t argQuery;
  jerry_value_t argQos;
  jerry_value_t argResponseHandler;
  DJS_CHECK_ARGS(5, object, string, string, number, function);
  argEndpoint = JS_GET_ARG(0, object);
  argUri = JS_GET_ARG(1, string);
  argQuery = JS_GET_ARG(2, string);
  argQos = JS_GET_ARG(3, number);
  argResponseHandler = JS_GET_ARG(4, function);
  // TODO: parsing endpoint
  const char *uri = iotjs_string_data(&argUri);
  const char *query = iotjs_string_data(&argQuery);
  int qos = (int)iotjs_jval_as_number(argQos);
  IOTJS_UNUSED(argEndpoint);
  IOTJS_UNUSED(argResponseHandler);
  IOTJS_UNUSED(uri);
  IOTJS_UNUSED(query);
  IOTJS_UNUSED(qos);

  // TODO: ocf_adapter_initPost_internal(..., uri, query, qos,
  // argResponseHandler);
  return jerry_create_undefined();
}

// OCFAdapter.initPut()
JS_FUNCTION(ocf_adapter_initPut) {
  jerry_value_t argEndpoint;
  iotjs_string_t argUri;
  iotjs_string_t argQuery;
  jerry_value_t argQos;
  jerry_value_t argResponseHandler;
  DJS_CHECK_ARGS(5, object, string, string, number, function);
  argEndpoint = JS_GET_ARG(0, object);
  argUri = JS_GET_ARG(1, string);
  argQuery = JS_GET_ARG(2, string);
  argQos = JS_GET_ARG(3, number);
  argResponseHandler = JS_GET_ARG(4, function);
  // TODO: parsing endpoint
  const char *uri = iotjs_string_data(&argUri);
  const char *query = iotjs_string_data(&argQuery);
  int qos = (int)iotjs_jval_as_number(argQos);
  IOTJS_UNUSED(argEndpoint);
  IOTJS_UNUSED(argResponseHandler);
  IOTJS_UNUSED(uri);
  IOTJS_UNUSED(query);
  IOTJS_UNUSED(qos);

  // TODO: ocf_adapter_initPut_internal(..., uri, query, qos,
  // argResponseHandler);
  return jerry_create_undefined();
}

// OCFAdapter.post()
JS_FUNCTION(ocf_adapter_post) {
  // TODO: ocf_adapter_post_internal();
  return jerry_create_undefined();
}

// OCFAdapter.put()
JS_FUNCTION(ocf_adapter_put) {
  // TODO: ocf_adapter_put_internal();
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

  // OC Representation
  REGISTER_ANT_API(ocfNative, ocf_adapter, repStartRootObject);
  REGISTER_ANT_API(ocfNative, ocf_adapter, repSetBoolean);
  REGISTER_ANT_API(ocfNative, ocf_adapter, repSetInt);
  REGISTER_ANT_API(ocfNative, ocf_adapter, repSetDouble);
  REGISTER_ANT_API(ocfNative, ocf_adapter, repSetString);
  REGISTER_ANT_API(ocfNative, ocf_adapter, repEndRootObject);

  // Send Response on Server-side
  REGISTER_ANT_API(ocfNative, ocf_adapter, repSendResponse);

  // Client-side Initialization
  REGISTER_ANT_API(ocfNative, ocf_adapter, discovery);
  REGISTER_ANT_API(ocfNative, ocf_adapter, observe);
  REGISTER_ANT_API(ocfNative, ocf_adapter, stopObserve);
  REGISTER_ANT_API(ocfNative, ocf_adapter, get);
  REGISTER_ANT_API(ocfNative, ocf_adapter, delete);
  REGISTER_ANT_API(ocfNative, ocf_adapter, initPost);
  REGISTER_ANT_API(ocfNative, ocf_adapter, initPut);
  REGISTER_ANT_API(ocfNative, ocf_adapter, post);
  REGISTER_ANT_API(ocfNative, ocf_adapter, put);

  // Initialize IoTivity Lite
  initOCFAdapter();
}