#include "ocf_adapter_internal.h"
#include "../../../common/native/ant_common.h"

// IoTivity Lite configuration
#define OC_SERVER
#define OC_CLIENT

// IoTivity Lite
#include "oc_api.h"
#include "port/oc_clock.h"

#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

// OCF thread
static pthread_t g_ocf_thread;
static pthread_mutex_t mutex;
static pthread_cond_t cv;
static struct timespec ts;
static int g_thread_quit = 0;

void *ocf_thread_fn(void *arg);
static void signal_event_loop(void);
static void handle_signal(int signal);

// OCF Server
// static bool light_server_state = false;

// OCF Client
// #define MAX_URI_LENGTH (30)
// static char light_1[MAX_URI_LENGTH];
// static oc_endpoint_t *light_server;
// static bool light_state = false;

// static void on_get_light(oc_request_t *request, oc_interface_mask_t
// iface_mask,
//                          void *user_data) {
//   (void)user_data;
//   PRINT("GET_light:\n");
//   oc_rep_start_root_object();
//   switch (iface_mask) {
//   case OC_IF_BASELINE:
//     oc_process_baseline_interface(request->resource);
//   /* fall through */
//   case OC_IF_RW:
//     oc_rep_set_boolean(root, state, light_server_state);
//     break;
//   default:
//     break;
//   }
//   oc_rep_end_root_object();
//   oc_send_response(request, OC_STATUS_OK);
//   PRINT("Light state %d\n", light_server_state);
// }

// static void post_light(oc_client_response_t *data) {
//  PRINT("POST_light:\n");
//  if (data->code == OC_STATUS_CHANGED)
//    PRINT("POST response OK\n");
//  else
//    PRINT("POST response code %d\n", data->code);
//}

// static void on_post_light(oc_request_t *request, oc_interface_mask_t
// iface_mask,
//                           void *user_data) {
//   (void)user_data;
//   (void)iface_mask;
//   PRINT("POST_light:\n");
//   bool state = false;
//   oc_rep_t *rep = request->request_payload;
//   while (rep != NULL) {
//     PRINT("key: %s ", oc_string(rep->name));
//     switch (rep->type) {
//     case OC_REP_BOOL:
//       state = rep->value.boolean;
//       PRINT("value: %d\n", state);
//       break;
//     default:
//       oc_send_response(request, OC_STATUS_BAD_REQUEST);
//       return;
//       break;
//     }
//     rep = rep->next;
//   }
//   oc_send_response(request, OC_STATUS_CHANGED);
//   light_server_state = state;
// }

// static void observe_light(oc_client_response_t *data) {
//   PRINT("OBSERVE_light:\n");
//   oc_rep_t *rep = data->payload;
//   while (rep != NULL) {
//     PRINT("key %s, value ", oc_string(rep->name));
//     switch (rep->type) {
//     case OC_REP_BOOL:
//       PRINT("%d\n", rep->value.boolean);
//       light_state = rep->value.boolean;
//       break;
//     default:
//       break;
//     }
//     rep = rep->next;
//   }

//   if (oc_init_post(light_1, light_server, NULL, &post_light, LOW_QOS, NULL))
//   {
//     oc_rep_start_root_object();
//     oc_rep_set_boolean(root, state, !light_state);
//     oc_rep_end_root_object();
//     if (oc_do_post())
//       PRINT("Sent POST request\n");
//     else
//       PRINT("Could not send POST\n");
//   } else
//     PRINT("Could not init POST\n");
// }

// static oc_event_callback_retval_t test_post(void *data) {
//   (void)data;

//   PRINT("test_post called\n");

//   if (oc_init_post(light_1, light_server, NULL, &post_light, LOW_QOS, NULL))
//   {
//     oc_rep_start_root_object();
//     oc_rep_set_boolean(root, state, !light_state);
//     oc_rep_end_root_object();
//     if (oc_do_post())
//       PRINT("Sent POST request\n");
//     else
//       PRINT("Could not send POST\n");
//   } else
//     PRINT("Could not init POST\n");
//   return OC_EVENT_DONE;
// }

// static oc_event_callback_retval_t stop_observe(void *data) {
//   (void)data;
//   PRINT("Stopping OBSERVE\n");
//   oc_stop_observe(light_1, light_server);
//   return OC_EVENT_DONE;
// }

// static oc_discovery_flags_t
// discovery(const char *di, const char *uri, oc_string_array_t types,
//           oc_interface_mask_t iface_mask, oc_endpoint_t *endpoint,
//           oc_resource_properties_t bm, void *user_data) {
//   (void)di;
//   (void)iface_mask;
//   (void)user_data;
//   (void)bm;
//   int i;
//   size_t uri_len = strlen(uri);
//   uri_len = (uri_len >= MAX_URI_LENGTH) ? MAX_URI_LENGTH - 1 : uri_len;

//   for (i = 0; i < (int)oc_string_array_get_allocated_size(types); i++) {
//     char *t = oc_string_array_get_item(types, i);
//     if (strlen(t) == 11 && strncmp(t, "oic.r.light", 11) == 0) {
//       strncpy(light_1, uri, uri_len);
//       light_1[uri_len] = '\0';
//       oc_endpoint_list_copy(&light_server, endpoint);

//       PRINT("Resource %s hosted at endpoints:\n", light_1);
//       oc_endpoint_t *ep = endpoint;
//       while (ep != NULL) {
//         PRINTipaddr(*ep);
//         PRINT("\n");
//         ep = ep->next;
//       }

//       oc_do_observe(light_1, light_server, NULL, &observe_light, LOW_QOS,
//       NULL); oc_set_delayed_callback(NULL, &test_post, 5);
//       oc_set_delayed_callback(NULL, &stop_observe, 20);
//       return OC_STOP_DISCOVERY;
//     }
//   }
//   return OC_CONTINUE_DISCOVERY;
// }

// OCFAdapter.onInitialize()
DECLARE_GLOBAL_ANT_ASYNC_HANDLER(ocf_adapter_onInitialize)
ANT_ASYNC_HANDLER_SETTER(ocf_adapter_onInitialize)
static int oa_initialize(void) {
  CALL_ANT_ASYNC_HANDLER(ocf_adapter_onInitialize, NULL);
  return true;
}

// OCFAdapter.onPrepareServer()
DECLARE_GLOBAL_ANT_ASYNC_HANDLER(ocf_adapter_onPrepareServer)
ANT_ASYNC_HANDLER_SETTER(ocf_adapter_onPrepareServer)
static void oa_prepare_server(void) {
  CALL_ANT_ASYNC_HANDLER(ocf_adapter_onPrepareServer, NULL);
}

// OCFAdapter.onPrepareClient()
DECLARE_GLOBAL_ANT_ASYNC_HANDLER(ocf_adapter_onPrepareClient)
ANT_ASYNC_HANDLER_SETTER(ocf_adapter_onPrepareClient)
static void oa_prepare_client(void) {
  CALL_ANT_ASYNC_HANDLER(ocf_adapter_onPrepareClient, NULL);
}

// OCFAdapter.start()
void ocf_adapter_start_internal(void) {
  pthread_create(&g_ocf_thread, NULL, &ocf_thread_fn, NULL);
}

// OCFAdapter.stop()
void ocf_adapter_stop_internal(void) {
  g_thread_quit = 1;
  signal_event_loop();
}

// OCFAdapter.setPlatform()
bool ocf_adapter_setPlatform_internal(const char *mfg_name) {
  return oc_init_platform(mfg_name, NULL, NULL);
}

// OCFAdapter.addDevice()
bool ocf_adapter_addDevice_internal(const char *uri, const char *resource_type,
                                    const char *name, const char *spec_version,
                                    const char *data_model_version) {
  return oc_add_device(uri, resource_type, name, spec_version,
                       data_model_version, NULL, NULL);
}

// OCFAdapter.addResource()
bool ocf_adapter_addResource_internal(void *ocf_resource_nobject) {
  return oc_add_resource((oc_resource_t *)ocf_resource_nobject);
}

void ocf_adapter_repStartRootObject_internal(void) {
  oc_rep_start_root_object();
}
void ocf_adapter_repSetBoolean_internal(const char *key, bool value) {
  g_err |= cbor_encode_text_string(&root_map, key, strlen(key));
  g_err |= cbor_encode_boolean(&root_map, value);
}
void ocf_adapter_repSetInt_internal(const char *key, int value) {
  g_err |= cbor_encode_text_string(&root_map, key, strlen(key));
  g_err |= cbor_encode_int(&root_map, value);
}
void ocf_adapter_repSetDouble_internal(const char *key, double value) {
  g_err |= cbor_encode_text_string(&root_map, key, strlen(key));
  g_err |= cbor_encode_double(&root_map, value);
}
void ocf_adapter_repSetString_internal(const char *key, const char *value) {
  g_err |= cbor_encode_text_string(&root_map, key, strlen(key));
  if ((const char *)value != NULL) {
    g_err |= cbor_encode_text_string(&root_map, value, strlen(value));
  } else {
    g_err |= cbor_encode_text_string(&root_map, "", 0);
  }
}
void ocf_adapter_repEndRootObject_internal(void) { oc_rep_end_root_object(); }
void ocf_adapter_sendResponse_internal(void *ocf_request_nobject,
                                       int status_code) {
  oc_separate_response_t *sep_response =
      (oc_separate_response_t *)ocf_request_nobject;
  oc_send_separate_response(sep_response, status_code);
}

void ocf_endpoint_destroy(void *handle) {
  oc_endpoint_t *endpoint = (oc_endpoint_t *)handle;
  oc_free_endpoint(endpoint);
}
DECLARE_GLOBAL_ANT_ASYNC_HANDLER(ocf_adapter_discovery)
ANT_ASYNC_HANDLER_SETTER(ocf_adapter_discovery)
static oc_discovery_flags_t
oa_on_discovery(const char *di, const char *uri, oc_string_array_t types,
                oc_interface_mask_t iface_mask, oc_endpoint_t *endpoint,
                oc_resource_properties_t bm, void *user_data) {
  // Discovery event
  ocf_adapter_discovery_event_t *event;
  event = (ocf_adapter_discovery_event_t *)malloc(
      sizeof(ocf_adapter_discovery_event_t));

  // event->uri
  event->uri = malloc(sizeof(char) * (strlen(uri) + 1));
  strncpy(event->uri, uri, strlen(uri) + 1);

  // event->types
  event->types = ll_new(ocf_adapter_discovery_event_types_destroyer);
  int i;
  for (i = 0; i < (int)oc_string_array_get_allocated_size(types); i++) {
    char *type = oc_string_array_get_item(types, i);
    size_t type_len = strlen(type) + 1;
    char *new_type = (char *)malloc(sizeof(char) * (type_len + 1));
    strncpy(new_type, type, type_len + 1);
    ll_insert_last(event->types, new_type);
  }

  // event->interface_mask
  event->interface_mask = (int)iface_mask;

  // event->endpoint
  oc_endpoint_copy((oc_endpoint_t *)event->endpoint, endpoint);

  CALL_ANT_ASYNC_HANDLER(ocf_adapter_discovery, event);
  return OC_CONTINUE_DISCOVERY;
}
void ocf_adapter_discovery_internal(const char *resource_type) {
  oc_do_ip_discovery(resource_type, &oa_on_discovery, NULL);
}

DECLARE_GLOBAL_ANT_ASYNC_HANDLER(ocf_adapter_observe)
ANT_ASYNC_HANDLER_SETTER(ocf_adapter_observe)
static void oa_on_observe(oc_client_response_t *data) {
  // Client response event
  ocf_client_response_event_t *event;
  event = (ocf_client_response_event_t *)malloc(
      sizeof(ocf_client_response_event_t));

  // event->endpoint
  oc_endpoint_copy((oc_endpoint_t *)event->endpoint, data->endpoint);

  // event->payload, event->payloadLength
  event->payload_string_length = oc_rep_to_json(data->payload, NULL, 0, true);
  event->payload_string = (char *)malloc(event->payload_string_length + 1);
  oc_rep_to_json(data->payload, event->payload_string,
                 event->payload_string_length + 1, true);

  // event->statusCode
  event->status_code = data->code;

  CALL_ANT_ASYNC_HANDLER(ocf_adapter_observe, event);
  return;
}
bool ocf_adapter_observe_internal(void *ocf_endpoint_nobject, const char *uri,
                                  const char *query, int qos) {
  oc_endpoint_t *endpoint = (oc_endpoint_t *)ocf_endpoint_nobject;
  oc_qos_t oc_qos = (qos == HIGH_QOS) ? HIGH_QOS : LOW_QOS;
  return oc_do_observe(uri, endpoint, query, &oa_on_observe, oc_qos, NULL);
}

bool ocf_adapter_stopObserve_internal(void *ocf_endpoint_nobject,
                                      const char *uri) {
  oc_endpoint_t *endpoint = (oc_endpoint_t *)ocf_endpoint_nobject;
  return oc_stop_observe(uri, endpoint);
}

void initOCFAdapter(void) {
  // Empty function
}

void *ocf_thread_fn(void *arg) {
  // On OCF Thread
  // TODO: implement it
  struct sigaction sa;
  sigfillset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handle_signal;
  sigaction(SIGINT, &sa, NULL);

  printf("OCF thread launched.\n");

  static const oc_handler_t handler = {.signal_event_loop = signal_event_loop,
                                       .init = oa_initialize,
                                       .register_resources = oa_prepare_server,
                                       .requests_entry = oa_prepare_client};

  oc_clock_time_t next_event;

#ifdef OC_STORAGE
  oc_storage_config("./server_creds");
#endif /* OC_STORAGE */

  int init = oc_main_init(&handler);
  if (init < 0)
    return NULL;

  while (g_thread_quit != 1) {
    next_event = oc_main_poll();
    pthread_mutex_lock(&mutex);
    if (next_event == 0) {
      pthread_cond_wait(&cv, &mutex);
    } else {
      ts.tv_sec = (next_event / OC_CLOCK_SECOND);
      ts.tv_nsec = (next_event % OC_CLOCK_SECOND) * 1.e09 / OC_CLOCK_SECOND;
      pthread_cond_timedwait(&cv, &mutex, &ts);
    }
    pthread_mutex_unlock(&mutex);
  }

  oc_main_shutdown();
  printf("OCF thread terminates...\n");

  return NULL;
}

void signal_event_loop(void) {
  pthread_mutex_lock(&mutex);
  pthread_cond_signal(&cv);
  pthread_mutex_unlock(&mutex);
}

void handle_signal(int signal) {
  (void)signal;
  signal_event_loop();
  g_thread_quit = 1;
}