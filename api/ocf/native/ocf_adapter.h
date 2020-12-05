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

#ifndef __OCF_ADAPTER_H__
#define __OCF_ADAPTER_H__

#include "iotjs_def.h"
#include "iotjs_uv_request.h"
#include "ocf_common.h"

#include "../../common/native/ant_common.h"
#include "./internal/ant_async.h"
#include "./internal/ll.h"
#include "./internal/ocf_adapter_internal.h"
#include "./ocf_resource.h"

void ocf_adapter_init(void);

void InitOCFAdapterNative(jerry_value_t ocfNative);

#define OCF_REQUEST_JS_FUNCTION(type, is_lock_required)                        \
  JS_FUNCTION(type) {                                                          \
    bool result;                                                               \
    jerry_value_t argRequest;                                                  \
    jerry_value_t argResponseHandler;                                          \
    argRequest = JS_GET_ARG(0, object);                                        \
    argResponseHandler = JS_GET_ARG(1, function);                              \
                                                                               \
    jerry_value_t jsRequestId = iotjs_jval_get_property(argRequest, "id");     \
    jerry_value_t jsOCFEndpoint =                                              \
        iotjs_jval_get_property(argRequest, "endpoint");                       \
    jerry_value_t jsUri = iotjs_jval_get_property(argRequest, "uri");          \
    jerry_value_t jsQuery = iotjs_jval_get_property(argRequest, "query");      \
    jerry_value_t jsQos = iotjs_jval_get_property(argRequest, "qos");          \
                                                                               \
    int requestId = (int)iotjs_jval_as_number(jsRequestId);                    \
                                                                               \
    jsOCFEndpoint = iotjs_jval_as_object(jsOCFEndpoint);                       \
    JS_DECLARE_PTR2(jsOCFEndpoint, void, ocf_endpoint_nobject, ocf_endpoint);  \
                                                                               \
    iotjs_string_t jsstrUri = iotjs_jval_as_string(jsUri);                     \
    const char *uri = iotjs_string_data(&jsstrUri);                            \
                                                                               \
    iotjs_string_t jsstrQuery = iotjs_jval_as_string(jsQuery);                 \
    const char *query = iotjs_string_data(&jsstrQuery);                        \
                                                                               \
    int qos = (int)iotjs_jval_as_number(jsQos);                                \
                                                                               \
    if (is_lock_required) {                                                    \
      lock_ocf_thread();                                                       \
    }                                                                          \
    result = REGISTER_JS_HANDLER(type, requestId, argResponseHandler);         \
    result = result && type##_internal(requestId, ocf_endpoint_nobject, uri,   \
                                       query, qos);                            \
                                                                               \
    jerry_release_value(jsRequestId);                                          \
    jerry_release_value(jsOCFEndpoint);                                        \
    jerry_release_value(jsUri);                                                \
    jerry_release_value(jsQuery);                                              \
    jerry_release_value(jsQos);                                                \
    iotjs_string_destroy(&jsstrUri);                                           \
    iotjs_string_destroy(&jsstrQuery);                                         \
                                                                               \
    return jerry_create_boolean(result);                                       \
  }

#define OCF_REQUEST_UV_HANDLER_FUNCTION(type, one_way)                         \
  ANT_UV_HANDLER_FUNCTION(type) {                                              \
    void *e;                                                                   \
    while ((e = GET_FIRST_EVENT_FROM_ANT_ASYNC(type)) != NULL) {               \
      ant_async_event_t *event = (ant_async_event_t *)e;                       \
      oa_client_response_event_data_t *event_data =                            \
          (oa_client_response_event_data_t *)event->data;                      \
                                                                               \
      iotjs_string_t jsstrPayload = iotjs_string_create();                     \
      iotjs_string_append(&jsstrPayload, event_data->payload_string,           \
                          strlen(event_data->payload_string));                 \
                                                                               \
      jerry_value_t jsEndpoint = jerry_create_object();                        \
      jerry_set_object_native_pointer(jsEndpoint, event_data->endpoint,        \
                                      &ocf_endpoint_native_info);              \
      IOTJS_ASSERT(jerry_get_object_native_pointer(                            \
          jsEndpoint, NULL, &ocf_endpoint_native_info));                       \
                                                                               \
      jerry_value_t jsRequestId = jerry_create_number(event_data->request_id); \
      jerry_value_t jsResponse = jerry_create_object();                        \
      iotjs_jval_set_property_string(jsResponse, "payload", &jsstrPayload);    \
      iotjs_jval_set_property_number(jsResponse, "status_code",                \
                                     (double)event_data->status_code);         \
      iotjs_jval_set_property_jval(jsResponse, "endpoint", jsEndpoint);        \
                                                                               \
      jerry_value_t js_handler =                                               \
          GET_JS_HANDLER_FROM_ANT_ASYNC(type, event_data->request_id);         \
      jerry_value_t js_args[] = {jsRequestId, jsResponse};                     \
      iotjs_invoke_callback(js_handler, jerry_create_undefined(), js_args, 2); \
                                                                               \
      iotjs_string_destroy(&jsstrPayload);                                     \
      jerry_release_value(jsRequestId);                                        \
      jerry_release_value(jsEndpoint);                                         \
      jerry_release_value(jsResponse);                                         \
                                                                               \
      REMOVE_FIRST_EVENT_FROM_ANT_ASYNC(type);                                 \
                                                                               \
      if (one_way)                                                             \
        UNREGISTER_JS_HANDLER(type, event_data->request_id);                   \
    }                                                                          \
  }

#endif /* !defined(__OCF_ADAPTER_H__) */
