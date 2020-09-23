#include "ocf_native_internal.h"
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
static int quit = 0;

// OCF Server
static bool light_server_state = false;
ant_handler_v_v g_ocf_adapter_onPrepareServer_itc_handler = NULL;

// OCF Client
#define MAX_URI_LENGTH (30)
static char light_1[MAX_URI_LENGTH];
static oc_endpoint_t *light_server;
static bool light_state = false;

static void on_get_light(oc_request_t *request, oc_interface_mask_t iface_mask,
                         void *user_data) {
  (void)user_data;
  PRINT("GET_light:\n");
  oc_rep_start_root_object();
  switch (iface_mask) {
  case OC_IF_BASELINE:
    oc_process_baseline_interface(request->resource);
  /* fall through */
  case OC_IF_RW:
    oc_rep_set_boolean(root, state, light_server_state);
    break;
  default:
    break;
  }
  oc_rep_end_root_object();
  oc_send_response(request, OC_STATUS_OK);
  PRINT("Light state %d\n", light_server_state);
}

static void post_light(oc_client_response_t *data) {
  PRINT("POST_light:\n");
  if (data->code == OC_STATUS_CHANGED)
    PRINT("POST response OK\n");
  else
    PRINT("POST response code %d\n", data->code);
}

static void on_post_light(oc_request_t *request, oc_interface_mask_t iface_mask,
                          void *user_data) {
  (void)user_data;
  (void)iface_mask;
  PRINT("POST_light:\n");
  bool state = false;
  oc_rep_t *rep = request->request_payload;
  while (rep != NULL) {
    PRINT("key: %s ", oc_string(rep->name));
    switch (rep->type) {
    case OC_REP_BOOL:
      state = rep->value.boolean;
      PRINT("value: %d\n", state);
      break;
    default:
      oc_send_response(request, OC_STATUS_BAD_REQUEST);
      return;
      break;
    }
    rep = rep->next;
  }
  oc_send_response(request, OC_STATUS_CHANGED);
  light_server_state = state;
}

static void observe_light(oc_client_response_t *data) {
  PRINT("OBSERVE_light:\n");
  oc_rep_t *rep = data->payload;
  while (rep != NULL) {
    PRINT("key %s, value ", oc_string(rep->name));
    switch (rep->type) {
    case OC_REP_BOOL:
      PRINT("%d\n", rep->value.boolean);
      light_state = rep->value.boolean;
      break;
    default:
      break;
    }
    rep = rep->next;
  }

  if (oc_init_post(light_1, light_server, NULL, &post_light, LOW_QOS, NULL)) {
    oc_rep_start_root_object();
    oc_rep_set_boolean(root, state, !light_state);
    oc_rep_end_root_object();
    if (oc_do_post())
      PRINT("Sent POST request\n");
    else
      PRINT("Could not send POST\n");
  } else
    PRINT("Could not init POST\n");
}

static oc_event_callback_retval_t test_post(void *data) {
  (void)data;

  PRINT("test_post called\n");

  if (oc_init_post(light_1, light_server, NULL, &post_light, LOW_QOS, NULL)) {
    oc_rep_start_root_object();
    oc_rep_set_boolean(root, state, !light_state);
    oc_rep_end_root_object();
    if (oc_do_post())
      PRINT("Sent POST request\n");
    else
      PRINT("Could not send POST\n");
  } else
    PRINT("Could not init POST\n");
  return OC_EVENT_DONE;
}

static oc_event_callback_retval_t stop_observe(void *data) {
  (void)data;
  PRINT("Stopping OBSERVE\n");
  oc_stop_observe(light_1, light_server);
  return OC_EVENT_DONE;
}

static oc_discovery_flags_t
discovery(const char *di, const char *uri, oc_string_array_t types,
          oc_interface_mask_t iface_mask, oc_endpoint_t *endpoint,
          oc_resource_properties_t bm, void *user_data) {
  (void)di;
  (void)iface_mask;
  (void)user_data;
  (void)bm;
  int i;
  size_t uri_len = strlen(uri);
  uri_len = (uri_len >= MAX_URI_LENGTH) ? MAX_URI_LENGTH - 1 : uri_len;

  for (i = 0; i < (int)oc_string_array_get_allocated_size(types); i++) {
    char *t = oc_string_array_get_item(types, i);
    if (strlen(t) == 11 && strncmp(t, "oic.r.light", 11) == 0) {
      strncpy(light_1, uri, uri_len);
      light_1[uri_len] = '\0';
      oc_endpoint_list_copy(&light_server, endpoint);

      PRINT("Resource %s hosted at endpoints:\n", light_1);
      oc_endpoint_t *ep = endpoint;
      while (ep != NULL) {
        PRINTipaddr(*ep);
        PRINT("\n");
        ep = ep->next;
      }

      oc_do_observe(light_1, light_server, NULL, &observe_light, LOW_QOS, NULL);
      oc_set_delayed_callback(NULL, &test_post, 5);
      oc_set_delayed_callback(NULL, &stop_observe, 20);
      return OC_STOP_DISCOVERY;
    }
  }
  return OC_CONTINUE_DISCOVERY;
}

static int app_init(void) {
  int ret = oc_init_platform("Intel", NULL, NULL);
  ret |= oc_add_device("/oic/d", "oic.d.light", "Kishen's light", "ocf.1.0.0",
                       "ocf.res.1.0.0", NULL, NULL);
  return ret;
}

static void signal_event_loop(void) {
  pthread_mutex_lock(&mutex);
  pthread_cond_signal(&cv);
  pthread_mutex_unlock(&mutex);
}

static void handle_signal(int signal) {
  (void)signal;
  signal_event_loop();
  quit = 1;
}

// Ready for server
static void register_resources(void) {
  if (g_ocf_adapter_onPrepareServer_itc_handler != NULL) {
    g_ocf_adapter_onPrepareServer_itc_handler();
  }

  // OCF.createResource()
  oc_resource_t *res = oc_new_resource("lightbulb", "/light/1", 1, 0);
  oc_resource_bind_resource_type(res, "oic.r.light");
  oc_resource_bind_resource_interface(res, OC_IF_RW);
  oc_resource_set_default_interface(res, OC_IF_RW);

  // OCFResource.setDiscoverable()
  oc_resource_set_discoverable(res, true);

  // OCFResource.setPeriodicObservable()
  oc_resource_set_periodic_observable(res, 1);

  // OCFResource.setHandler()
  oc_resource_set_request_handler(res, OC_GET, on_get_light, NULL);
  oc_resource_set_request_handler(res, OC_POST, on_post_light, NULL);
  oc_add_resource(res);
}

// Ready for client
static void issue_requests(void) {
  oc_do_ip_discovery("oic.r.light", &discovery, NULL);
}

void *ocf_thread_fn(void *arg) {
  // On OCF Thread
  // TODO: implement it
  struct sigaction sa;
  sigfillset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handle_signal;
  sigaction(SIGINT, &sa, NULL);

  static const oc_handler_t handler = {.init = app_init,
                                       .signal_event_loop = signal_event_loop,
                                       .register_resources = register_resources,
                                       .requests_entry = issue_requests};

  oc_clock_time_t next_event;

#ifdef OC_STORAGE
  oc_storage_config("./server_creds");
#endif /* OC_STORAGE */

  int init = oc_main_init(&handler);
  if (init < 0)
    return NULL;

  while (quit != 1) {
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

  return NULL;
}

void ocf_adapter_start_internal(void) {
  pthread_create(&g_ocf_thread, NULL, &ocf_thread_fn, NULL);
}

void ocf_adapter_onPrepareServer_internal(ant_handler_v_v handler) {
  g_ocf_adapter_onPrepareServer_itc_handler = handler;
}

void initOCF(void) {
  // Empty function
}