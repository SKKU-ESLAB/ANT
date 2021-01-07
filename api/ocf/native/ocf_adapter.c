/* Copyright (c) 2017-2020 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iotjs_def.h>
#include <iotjs_uv_request.h>
#include <modules/iotjs_module_buffer.h>

#include "./ocf_resource.h"

#include "./ocf_adapter.h"

// Linked list destroyers
void oa_discovery_event_data_destroyer(void *item) {
  oa_discovery_event_data_t *event;
  event = (oa_discovery_event_data_t *)item;
  free(event->uri);
  ll_delete(event->types);
  free(event);
}
void oa_response_event_data_destroyer(void *item) {
  oa_client_response_event_data_t *event;
  event = (oa_client_response_event_data_t *)item;
  free(event->payload_string);
  free(event);
}
void oa_discovery_event_types_destroyer(void *item) { free(item); }

// OCFAdapter.initialize()
JS_FUNCTION(ocf_adapter_initialize) {
  init_ant_async_list();
  ocf_adapter_init();
  ocf_resource_init();
  return jerry_create_undefined();
}

// OCFAdapter.deinitialize()
JS_FUNCTION(ocf_adapter_deinitialize) {
  destroy_ant_async_list();
  return jerry_create_undefined();
}

// OCFAdapter.onPrepareEventLoop()
ANT_ASYNC_DECL_FUNCS(ocf_adapter_onPrepareEventLoop, NULL)
JS_FUNCTION(ocf_adapter_onPrepareEventLoop) {
  bool result;
  jerry_value_t argHandler;
  DJS_CHECK_ARGS(1, function);
  argHandler = JS_GET_ARG(0, function);

  // singleton handler
  int zero = 0;
  result =
      REGISTER_JS_HANDLER(ocf_adapter_onPrepareEventLoop, zero, argHandler);
  return jerry_create_boolean(result);
}
ANT_UV_HANDLER_FUNCTION(ocf_adapter_onPrepareEventLoop) {
  void *e;
  while ((e = GET_FIRST_EVENT_FROM_ANT_ASYNC(ocf_adapter_onPrepareEventLoop)) !=
         NULL) {
    // singleton handler
    int zero = 0;
    jerry_value_t js_handler =
        GET_JS_HANDLER_FROM_ANT_ASYNC(ocf_adapter_onPrepareEventLoop, zero);
    if (!jerry_value_is_undefined(js_handler)) {
      iotjs_invoke_callback(js_handler, jerry_create_undefined(), NULL, 0);
    } else {
      // cannot reach here
      printf("Warning: js_handler is undefined on ant async handler "
             "(ocf_adapter_onPrepareEventLoop)\n");
    }

    // Remove the first event
    // - It also calls the destroyer of the event and event data.
    REMOVE_FIRST_EVENT_FROM_ANT_ASYNC(ocf_adapter_onPrepareEventLoop);
  }
}

// OCFAdapter.onPrepareServer()
ANT_ASYNC_DECL_FUNCS(ocf_adapter_onPrepareServer, NULL)
JS_FUNCTION(ocf_adapter_onPrepareServer) {
  bool result;
  jerry_value_t argHandler;
  DJS_CHECK_ARGS(1, function);
  argHandler = JS_GET_ARG(0, function);

  // singleton handler
  int zero = 0;
  result = REGISTER_JS_HANDLER(ocf_adapter_onPrepareServer, zero, argHandler);
  return jerry_create_boolean(result);
}
ANT_UV_HANDLER_FUNCTION(ocf_adapter_onPrepareServer) {
  void *e;
  while ((e = GET_FIRST_EVENT_FROM_ANT_ASYNC(ocf_adapter_onPrepareServer)) !=
         NULL) {
    // singleton handler
    int zero = 0;
    jerry_value_t js_handler =
        GET_JS_HANDLER_FROM_ANT_ASYNC(ocf_adapter_onPrepareServer, zero);
    if (!jerry_value_is_undefined(js_handler)) {
      iotjs_invoke_callback(js_handler, jerry_create_undefined(), NULL, 0);
    } else {
      // empty handler
    }

    // Remove the first event
    // - It also calls the destroyer of the event and event data.
    REMOVE_FIRST_EVENT_FROM_ANT_ASYNC(ocf_adapter_onPrepareServer);
  }
}

// OCFAdapter.onPrepareClient()
ANT_ASYNC_DECL_FUNCS(ocf_adapter_onPrepareClient, NULL)
JS_FUNCTION(ocf_adapter_onPrepareClient) {
  bool result;
  jerry_value_t argHandler;
  DJS_CHECK_ARGS(1, function);
  argHandler = JS_GET_ARG(0, function);

  // singleton handler
  int zero = 0;
  result = REGISTER_JS_HANDLER(ocf_adapter_onPrepareClient, zero, argHandler);
  return jerry_create_boolean(result);
}
ANT_UV_HANDLER_FUNCTION(ocf_adapter_onPrepareClient) {
  void *e;
  while ((e = GET_FIRST_EVENT_FROM_ANT_ASYNC(ocf_adapter_onPrepareClient)) !=
         NULL) {
    // singleton handler
    int zero = 0;
    jerry_value_t js_handler =
        GET_JS_HANDLER_FROM_ANT_ASYNC(ocf_adapter_onPrepareClient, zero);
    if (!jerry_value_is_undefined(js_handler)) {
      iotjs_invoke_callback(js_handler, jerry_create_undefined(), NULL, 0);
    } else {
      // empty handler
    }

    // Remove the first event
    // - It also calls the destroyer of the event and event data.
    REMOVE_FIRST_EVENT_FROM_ANT_ASYNC(ocf_adapter_onPrepareClient);
  }
}

// OCFAdapter.start()
ANT_API_VOID_TO_VOID(ocf_adapter, start);

// OCFAdapter.stop()
JS_FUNCTION(ocf_adapter_stop) {
  ocf_adapter_stop_internal();
  return jerry_create_undefined();
}

// OCFAdapter.setPlatform()
JS_FUNCTION(ocf_adapter_setPlatform) {
  bool result;
  iotjs_string_t argMfgName;
  DJS_CHECK_ARGS(1, string);
  argMfgName = JS_GET_ARG(0, string);

  result = ocf_adapter_setPlatform_internal(iotjs_string_data(&argMfgName));

  iotjs_string_destroy(&argMfgName);

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

  iotjs_string_destroy(&argUri);
  iotjs_string_destroy(&argResourceType);
  iotjs_string_destroy(&argName);
  iotjs_string_destroy(&argSpecVersion);
  iotjs_string_destroy(&argDataModelVersion);

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
  bool argValue;
  DJS_CHECK_ARGS(2, string, boolean);
  argKey = JS_GET_ARG(0, string);
  argValue = JS_GET_ARG(1, boolean);
  const char *key = iotjs_string_data(&argKey);

  ocf_adapter_repSetBoolean_internal(key, argValue);

  iotjs_string_destroy(&argKey);

  return jerry_create_undefined();
}

// OCFAdapter.repSetInt()
JS_FUNCTION(ocf_adapter_repSetInt) {
  iotjs_string_t argKey;
  int argValue;
  DJS_CHECK_ARGS(2, string, number);
  argKey = JS_GET_ARG(0, string);
  argValue = (int)JS_GET_ARG(1, number);
  const char *key = iotjs_string_data(&argKey);

  ocf_adapter_repSetInt_internal(key, argValue);

  iotjs_string_destroy(&argKey);

  return jerry_create_undefined();
}

// OCFAdapter.repSetDouble()
JS_FUNCTION(ocf_adapter_repSetDouble) {
  iotjs_string_t argKey;
  double argValue;
  DJS_CHECK_ARGS(2, string, number);
  argKey = JS_GET_ARG(0, string);
  argValue = JS_GET_ARG(1, number);
  const char *key = iotjs_string_data(&argKey);

  ocf_adapter_repSetDouble_internal(key, argValue);

  iotjs_string_destroy(&argKey);

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

  iotjs_string_destroy(&argKey);
  iotjs_string_destroy(&argValue);

  return jerry_create_undefined();
}

// OCFAdapter.repSetByteArray()
JS_FUNCTION(ocf_adapter_repSetByteArray) {
  iotjs_string_t argKey;
  jerry_value_t argValue;
  DJS_CHECK_ARGS(2, string, object);
  argKey = JS_GET_ARG(0, string);
  argValue = JS_GET_ARG(1, object);
  const char *key = iotjs_string_data(&argKey);
  iotjs_bufferwrap_t *valueBuffer = iotjs_bufferwrap_from_jbuffer(argValue);

  const char *value = (const char *)valueBuffer->buffer;
  size_t value_length = iotjs_bufferwrap_length(valueBuffer);

  ocf_adapter_repSetByteArray_internal(key, (const uint8_t *)value,
                                       value_length);

  iotjs_string_destroy(&argKey);

  return jerry_create_undefined();
}

// OCFAdapter.repEndRootObject()
ANT_API_VOID_TO_VOID(ocf_adapter, repEndRootObject);

// OCFAdapter.sendResponse()
JS_FUNCTION(ocf_adapter_sendResponse) {
  jerry_value_t argRequest;
  int argStatusCode;
  DJS_CHECK_ARGS(2, object, number);
  argRequest = JS_GET_ARG(0, object);
  argStatusCode = (int)JS_GET_ARG(1, number);
  JS_DECLARE_PTR2(argRequest, void, ocf_request_nobject, ocf_request);

  ocf_adapter_sendResponse_internal(ocf_request_nobject, argStatusCode);
  return jerry_create_undefined();
}

// OCFEndPoint
const jerry_object_native_info_t ocf_endpoint_native_info = {
    .free_cb = (jerry_object_native_free_callback_t)ocf_endpoint_destroy,
};

// OCFAdapter.isDiscovering()
JS_FUNCTION(ocf_adapter_isDiscovering) {
  bool is_discovering = ocf_adapter_isDiscovering_internal();
  return jerry_create_boolean(is_discovering);
}

// OCFAdapter.stopDiscovery()
JS_FUNCTION(ocf_adapter_stopDiscovery) {
  ocf_adapter_stopDiscovery_internal();
  return jerry_create_undefined();
}

// OCFAdapter.discovery()
ANT_ASYNC_DECL_FUNCS(ocf_adapter_discovery, oa_discovery_event_data_destroyer)
JS_FUNCTION(ocf_adapter_discovery) {
  bool result;
  iotjs_string_t argResourceType;
  jerry_value_t argDiscoveryHandler;
  DJS_CHECK_ARGS(2, string, function);
  argResourceType = JS_GET_ARG(0, string);
  argDiscoveryHandler = JS_GET_ARG(1, function);
  const char *resource_type = iotjs_string_data(&argResourceType);
  if (strlen(resource_type) == 1 && resource_type[0] == ' ') {
    // If zero-length resource type is given, discover all the resources
    resource_type = NULL;
  }

  int zero = 0;
  result =
      REGISTER_JS_HANDLER(ocf_adapter_discovery, zero, argDiscoveryHandler);
  result = result && ocf_adapter_discovery_internal(resource_type);

  iotjs_string_destroy(&argResourceType);
  return jerry_create_boolean(result);
}
ANT_UV_HANDLER_FUNCTION(ocf_adapter_discovery) {
  // Get the first event
  void *e;
  while ((e = GET_FIRST_EVENT_FROM_ANT_ASYNC(ocf_adapter_discovery)) != NULL) {
    ant_async_event_t *event = (ant_async_event_t *)e;
    oa_discovery_event_data_t *event_data =
        (oa_discovery_event_data_t *)event->data;
    // Args 0: object endpoint
    // set native pointer of OCFEndPoint with oc_endpoint_t
    jerry_value_t jsEndpoint = jerry_create_object();
    jerry_set_object_native_pointer(jsEndpoint, event_data->endpoint,
                                    &ocf_endpoint_native_info);
    IOTJS_ASSERT(jerry_get_object_native_pointer(jsEndpoint, NULL,
                                                 &ocf_endpoint_native_info));

    // Args 1: string uri
    jerry_value_t jsUri =
        jerry_create_string_from_utf8((const jerry_char_t *)event_data->uri);

    // Args 2: array<string> types
    jerry_value_t jsTypes = jerry_create_object();
    for (int i = 0; i < event_data->types->len; i++) {
      char *type_item = (char *)ll_get_n(event_data->types, i);
      jerry_value_t jsTypeItem =
          jerry_create_string_from_utf8((const jerry_char_t *)type_item);
      iotjs_jval_set_property_by_index(jsTypes, (uint32_t)i, jsTypeItem);
      jerry_release_value(jsTypeItem);
    }

    // Args 3: int interface_mask
    jerry_value_t jsInterfaceMask =
        jerry_create_number((double)event_data->interface_mask);

    jerry_value_t js_handler =
        GET_JS_HANDLER_FROM_ANT_ASYNC(ocf_adapter_discovery, event->key);
    jerry_value_t js_args[] = {jsEndpoint, jsUri, jsTypes, jsInterfaceMask};
    iotjs_invoke_callback(js_handler, jerry_create_undefined(), js_args, 4);

    jerry_release_value(jsEndpoint);
    jerry_release_value(jsUri);
    jerry_release_value(jsTypes);
    jerry_release_value(jsInterfaceMask);

    // wake up OCF thread
    wakeup_ant_async_sender(event);

    // Remove the first event
    // - It also calls the destroyer of the event and event data.
    REMOVE_FIRST_EVENT_FROM_ANT_ASYNC(ocf_adapter_discovery);
  }
}

// OCFAdapter.observe()
ANT_ASYNC_DECL_FUNCS(ocf_adapter_observe, oa_response_event_data_destroyer)
OCF_REQUEST_JS_FUNCTION(ocf_adapter_observe, false)
OCF_REQUEST_UV_HANDLER_FUNCTION(ocf_adapter_observe, false)

// OCFAdapter.stopObserve()
JS_FUNCTION(ocf_adapter_stopObserve) {
  bool result;
  int argRequestId;
  jerry_value_t argOCFEndpoint;
  iotjs_string_t argUri;
  DJS_CHECK_ARGS(3, number, object, string);
  argRequestId = (int)JS_GET_ARG(0, number);
  argOCFEndpoint = JS_GET_ARG(1, object);
  argUri = JS_GET_ARG(2, string);
  JS_DECLARE_PTR2(argOCFEndpoint, void, ocf_endpoint_nobject, ocf_endpoint);
  const char *uri = iotjs_string_data(&argUri);

  result = ocf_adapter_stopObserve_internal(ocf_endpoint_nobject, uri);
  result = result && UNREGISTER_JS_HANDLER(ocf_adapter_observe, argRequestId);

  iotjs_string_destroy(&argUri);

  return jerry_create_boolean(result);
}

// OCFAdapter.get()
ANT_ASYNC_DECL_FUNCS(ocf_adapter_get, oa_response_event_data_destroyer)
OCF_REQUEST_JS_FUNCTION(ocf_adapter_get, false)
OCF_REQUEST_UV_HANDLER_FUNCTION(ocf_adapter_get, true)

// OCFAdapter.delete()
ANT_ASYNC_DECL_FUNCS(ocf_adapter_delete, oa_response_event_data_destroyer)
OCF_REQUEST_JS_FUNCTION(ocf_adapter_delete, false)
OCF_REQUEST_UV_HANDLER_FUNCTION(ocf_adapter_delete, true)

// OCFAdapter.initPost()
ANT_ASYNC_DECL_FUNCS(ocf_adapter_initPost, oa_response_event_data_destroyer)
OCF_REQUEST_JS_FUNCTION(ocf_adapter_initPost, true)
OCF_REQUEST_UV_HANDLER_FUNCTION(ocf_adapter_initPost, true)

// OCFAdapter.initPut()
ANT_ASYNC_DECL_FUNCS(ocf_adapter_initPut, oa_response_event_data_destroyer)
OCF_REQUEST_JS_FUNCTION(ocf_adapter_initPut, true)
OCF_REQUEST_UV_HANDLER_FUNCTION(ocf_adapter_initPut, true)

// OCFAdapter.post()
JS_FUNCTION(ocf_adapter_post) {
  bool result = ocf_adapter_post_internal();
  unlock_ocf_thread();
  return jerry_create_boolean(result);
}

// OCFAdapter.put()
JS_FUNCTION(ocf_adapter_put) {
  bool result = ocf_adapter_put_internal();
  unlock_ocf_thread();
  return jerry_create_boolean(result);
}

void ocf_adapter_init(void) {
  INIT_ANT_ASYNC(ocf_adapter_onPrepareEventLoop, NULL);
  INIT_ANT_ASYNC(ocf_adapter_onPrepareServer, NULL);
  INIT_ANT_ASYNC(ocf_adapter_onPrepareClient, NULL);
  INIT_ANT_ASYNC(ocf_adapter_discovery, oa_discovery_event_data_destroyer);
  INIT_ANT_ASYNC(ocf_adapter_observe, oa_response_event_data_destroyer);
  INIT_ANT_ASYNC(ocf_adapter_get, oa_response_event_data_destroyer);
  INIT_ANT_ASYNC(ocf_adapter_delete, oa_response_event_data_destroyer);
  INIT_ANT_ASYNC(ocf_adapter_initPost, oa_response_event_data_destroyer);
  INIT_ANT_ASYNC(ocf_adapter_initPut, oa_response_event_data_destroyer);
}

void InitOCFAdapterNative(jerry_value_t ocfNative) {
  // Initialize, Deinitialize
  REGISTER_ANT_API(ocfNative, ocf_adapter, initialize);
  REGISTER_ANT_API(ocfNative, ocf_adapter, deinitialize);

  // OCF Thread Initialization Handlers
  REGISTER_ANT_API(ocfNative, ocf_adapter, onPrepareEventLoop);
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
  REGISTER_ANT_API(ocfNative, ocf_adapter, repSetByteArray);
  REGISTER_ANT_API(ocfNative, ocf_adapter, repEndRootObject);

  // Send Response on Server-side
  REGISTER_ANT_API(ocfNative, ocf_adapter, sendResponse);

  // Client-side Initialization
  REGISTER_ANT_API(ocfNative, ocf_adapter, isDiscovering);
  REGISTER_ANT_API(ocfNative, ocf_adapter, stopDiscovery);
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
