#include "ocf_adapter_internal.h"

#include "../../../common/native/ant_common.h"
#include "ant_async.h"

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

static void *ocf_thread_fn(void *arg);
static void signal_event_loop(void);
static void handle_signal(int signal);

// OCFAdapter.onPrepareEventLoop()
static int oa_prepare_event_loop(void) {
  int zero = 0;
  bool res =
      EMIT_ANT_ASYNC_EVENT(ocf_adapter_onPrepareEventLoop, zero, (void *)NULL);
  return (res) ? 0 : -1;
}

// OCFAdapter.onPrepareServer()
static void oa_prepare_server(void) {
  int zero = 0;
  EMIT_ANT_ASYNC_EVENT(ocf_adapter_onPrepareServer, zero, (void *)NULL);
  return;
}

// OCFAdapter.onPrepareClient()
static void oa_prepare_client(void) {
  int zero = 0;
  EMIT_ANT_ASYNC_EVENT(ocf_adapter_onPrepareClient, zero, (void *)NULL);
  return;
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
  oc_request_t *request = (oc_request_t *)ocf_request_nobject;
  oc_send_response(request, status_code);
}

void ocf_endpoint_destroy(void *handle) {
  oc_endpoint_t *endpoint = (oc_endpoint_t *)handle;
  oc_free_endpoint(endpoint);
}

bool g_is_discovering = false;
bool ocf_adapter_isDiscovering_internal(void) { return g_is_discovering; }
void ocf_adapter_stopDiscovery_internal(void) { g_is_discovering = false; }
static oc_discovery_flags_t
oa_on_discovery(const char *di, const char *uri, oc_string_array_t types,
                oc_interface_mask_t iface_mask, oc_endpoint_t *endpoint,
                oc_resource_properties_t bm, void *user_data) {
  // Discovery event
  oa_discovery_event_data_t *event_data;
  event_data =
      (oa_discovery_event_data_t *)malloc(sizeof(oa_discovery_event_data_t));

  // event_data->uri
  event_data->uri = malloc(sizeof(char) * (strlen(uri) + 1));
  strncpy(event_data->uri, uri, strlen(uri) + 1);

  // event_data->types
  event_data->types = ll_new(oa_discovery_event_types_destroyer);
  int i;
  for (i = 0; i < (int)oc_string_array_get_allocated_size(types); i++) {
    char *type = oc_string_array_get_item(types, i);
    size_t type_len = strlen(type) + 1;
    char *new_type = (char *)malloc(sizeof(char) * (type_len + 1));
    strncpy(new_type, type, type_len + 1);
    ll_insert_last(event_data->types, new_type);
  }

  // event_data->interface_mask
  event_data->interface_mask = (int)iface_mask;

  // event_data->endpoint
  oc_endpoint_copy((oc_endpoint_t *)event_data->endpoint, endpoint);

  pthread_mutex_init(&event_data->sync_mutex, NULL);
  pthread_cond_init(&event_data->sync_cond, NULL);
  int zero = 0;
  EMIT_ANT_ASYNC_EVENT(ocf_adapter_discovery, zero, (void *)event_data);

  // wait for JS thread's handler
  pthread_cond_wait(&event_data->sync_cond, &event_data->sync_mutex);
  pthread_mutex_unlock(&event_data->sync_mutex);

  return (g_is_discovering) ? OC_CONTINUE_DISCOVERY : OC_STOP_DISCOVERY;
}
bool ocf_adapter_discovery_internal(const char *resource_type) {
  if (g_is_discovering)
    return false;
  g_is_discovering = true;
  oc_do_ip_discovery(resource_type, &oa_on_discovery, NULL);
  return true;
}

OCF_REQUEST_INTERNAL_HANDLER(ocf_adapter_observe)
OCF_REQUEST_INTERNAL(ocf_adapter_observe, oc_do_observe)

bool ocf_adapter_stopObserve_internal(void *ocf_endpoint_nobject,
                                      const char *uri) {
  oc_endpoint_t *endpoint = (oc_endpoint_t *)ocf_endpoint_nobject;
  return oc_stop_observe(uri, endpoint);
}

OCF_REQUEST_INTERNAL_HANDLER(ocf_adapter_get)
OCF_REQUEST_INTERNAL(ocf_adapter_get, oc_do_get)

OCF_REQUEST_INTERNAL_HANDLER(ocf_adapter_delete)
OCF_REQUEST_INTERNAL(ocf_adapter_delete, oc_do_delete)

OCF_REQUEST_INTERNAL_HANDLER(ocf_adapter_initPost)
OCF_REQUEST_INTERNAL(ocf_adapter_initPost, oc_init_post)

OCF_REQUEST_INTERNAL_HANDLER(ocf_adapter_initPut)
OCF_REQUEST_INTERNAL(ocf_adapter_initPut, oc_init_put)

bool ocf_adapter_post_internal(void) { return oc_do_post(); }
bool ocf_adapter_put_internal(void) { return oc_do_put(); }

void initOCFAdapter(void) {
  // Empty function
}

void *ocf_thread_fn(void *arg) {
  // On OCF Thread
  struct sigaction sa;
  sigfillset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handle_signal;
  sigaction(SIGINT, &sa, NULL);

  printf("OCF thread launched.\n");

  static const oc_handler_t handler = {.signal_event_loop = signal_event_loop,
                                       .init = oa_prepare_event_loop,
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