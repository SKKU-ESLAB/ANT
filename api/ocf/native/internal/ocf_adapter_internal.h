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

#ifndef __OCF_ADAPTER_INTERNAL_H__
#define __OCF_ADAPTER_INTERNAL_H__

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

#include "./ant_async.h"
#include "./ll.h"

ANT_ASYNC_DECL_IN_HEADER(ocf_adapter_onPrepareEventLoop);
ANT_ASYNC_DECL_IN_HEADER(ocf_adapter_onPrepareServer);
ANT_ASYNC_DECL_IN_HEADER(ocf_adapter_onPrepareClient);

void ocf_adapter_start_internal(void);
void ocf_adapter_stop_internal(void);

bool ocf_adapter_setPlatform_internal(const char *mfg_name);
bool ocf_adapter_addDevice_internal(const char *uri, const char *resource_type,
                                    const char *name, const char *spec_version,
                                    const char *data_model_version);
bool ocf_adapter_addResource_internal(void *ocf_resource_nobject);

void ocf_adapter_repStartRootObject_internal(void);
void ocf_adapter_repSetBoolean_internal(const char *key, bool value);
void ocf_adapter_repSetInt_internal(const char *key, int value);
void ocf_adapter_repSetDouble_internal(const char *key, double value);
void ocf_adapter_repSetString_internal(const char *key, const char *value);
void ocf_adapter_repSetByteArray_internal(const char *key, const uint8_t *value,
                                          size_t value_length);
void ocf_adapter_repEndRootObject_internal(void);
void ocf_adapter_sendResponse_internal(void *ocf_request_nobject,
                                       int status_code);

ANT_ASYNC_DECL_IN_HEADER(ocf_adapter_discovery);
struct oa_discovery_event_data_s {
  char *uri;
  ll_t *types;
  int interface_mask;
  void *endpoint;
};
typedef struct oa_discovery_event_data_s oa_discovery_event_data_t;
void oa_discovery_event_data_destroyer(void *item);
void oa_discovery_event_types_destroyer(void *item);
void ocf_endpoint_destroy(void *handle);

bool ocf_adapter_isDiscovering_internal(void);
void ocf_adapter_stopDiscovery_internal(void);
bool ocf_adapter_discovery_internal(const char *resource_type);

struct oa_client_response_event_data_s {
  void *endpoint;
  int status_code;
  int request_id;
  bool is_payload_buffer;

  char *payload_buffer;
  size_t payload_buffer_length;

  char *payload_string;
  size_t payload_string_length;
};
typedef struct oa_client_response_event_data_s oa_client_response_event_data_t;
void oa_response_event_data_destroyer(void *item);

#define DECLARE_OCF_REQUEST_INTERNAL(type)                                     \
  ANT_ASYNC_DECL_IN_HEADER(type);                                              \
  bool type##_internal(int requestId, void *ocf_endpoint_nobject,              \
                       const char *uri, const char *query, int qos,            \
                       bool isPayloadBuffer);

DECLARE_OCF_REQUEST_INTERNAL(ocf_adapter_observe);
bool ocf_adapter_stopObserve_internal(void *ocf_endpoint_nobject,
                                      const char *uri);
DECLARE_OCF_REQUEST_INTERNAL(ocf_adapter_get);
DECLARE_OCF_REQUEST_INTERNAL(ocf_adapter_delete);
DECLARE_OCF_REQUEST_INTERNAL(ocf_adapter_initPost);
DECLARE_OCF_REQUEST_INTERNAL(ocf_adapter_initPut);
bool ocf_adapter_post_internal(void);
bool ocf_adapter_put_internal(void);

void initOCFAdapter(void);

void lock_ocf_thread(void);
void unlock_ocf_thread(void);

#define KEY_BUFFER_VALUE "bufferValue"
#define KEY_STRING_VALUE "stringValue"

/* (Native) JS_FUNCTION(type)
 * -> {optype}_internal() -> Send OCF request */
#define OCF_REQUEST_INTERNAL(type, ocf_request_function)                       \
  bool type##_internal(int requestId, void *ocf_endpoint_nobject,              \
                       const char *uri, const char *query, int qos,            \
                       bool isPayloadBuffer) {                                 \
    oc_endpoint_t *endpoint = (oc_endpoint_t *)ocf_endpoint_nobject;           \
    oc_qos_t oc_qos = (qos == HIGH_QOS) ? HIGH_QOS : LOW_QOS;                  \
    /* Because multiple responses can occur, user_data must NOT be dynamically \
     * allocated */                                                            \
    int user_data = (isPayloadBuffer) ? requestId : -requestId;                \
    bool res = ocf_request_function(uri, endpoint, query, &type##_handler,     \
                                    oc_qos, (void *)user_data);                \
    return res;                                                                \
  }

/* Receive OCF response
 * -> (Native) {optype}_handler()
 * -> ...
 * -> (JS) {anonymous_handler}() */
#define OCF_REQUEST_INTERNAL_HANDLER(type)                                     \
  static void type##_handler(oc_client_response_t *data) {                     \
    oa_client_response_event_data_t *event_data;                               \
    event_data = (oa_client_response_event_data_t *)malloc(                    \
        sizeof(oa_client_response_event_data_t));                              \
                                                                               \
    oc_endpoint_t *endpoint = (oc_endpoint_t *)malloc(sizeof(oc_endpoint_t));  \
    memcpy(endpoint, data->endpoint, sizeof(oc_endpoint_t));                   \
    event_data->endpoint = (void *)endpoint;                                   \
    event_data->status_code = data->code;                                      \
                                                                               \
    int user_data = (int)data->user_data;                                      \
    int request_id = (user_data > 0) ? user_data : -user_data;                 \
    bool is_payload_buffer = (user_data > 0);                                  \
    event_data->request_id = request_id;                                       \
    event_data->is_payload_buffer = is_payload_buffer;                         \
                                                                               \
    bool result_getstr = false;                                                \
    assert(data != NULL);                                                      \
    assert(data->payload != NULL);                                             \
    if (event_data->is_payload_buffer) {                                       \
      char *payload_buffer;                                                    \
      size_t payload_buffer_length;                                            \
      result_getstr =                                                          \
          oc_rep_get_byte_string(data->payload, KEY_BUFFER_VALUE,              \
                                 &payload_buffer, &payload_buffer_length);     \
      assert(result_getstr);                                                   \
      event_data->payload_buffer = (char *)malloc(payload_buffer_length);      \
      memcpy(event_data->payload_buffer, payload_buffer,                       \
             payload_buffer_length);                                           \
      event_data->payload_buffer_length = payload_buffer_length;               \
                                                                               \
      char *payload_string;                                                    \
      size_t payload_string_length;                                            \
      result_getstr =                                                          \
          oc_rep_get_string(data->payload, KEY_STRING_VALUE, &payload_string,  \
                            &payload_string_length);                           \
      assert(result_getstr);                                                   \
      event_data->payload_string = (char *)malloc(payload_string_length + 1);  \
      memcpy(event_data->payload_string, payload_string,                       \
             payload_string_length);                                           \
    } else {                                                                   \
      event_data->payload_string_length =                                      \
          oc_rep_to_json(data->payload, NULL, 0, true);                        \
      event_data->payload_string =                                             \
          (char *)malloc(event_data->payload_string_length + 1);               \
      oc_rep_to_json(data->payload, event_data->payload_string,                \
                     event_data->payload_string_length + 1, true);             \
    }                                                                          \
                                                                               \
    EMIT_ANT_ASYNC_EVENT(type, event_data->request_id, (void *)event_data,     \
                         false);                                               \
  }

#endif /* !defined(__OCF_ADAPTER_INTERNAL_H__) */
