#include "ocf_resource_internal.h"

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

// OCFResource()
void *ocf_resource_constructor_internal(const char *name, const char *uri,
                                        const char **types, size_t types_len,
                                        int interface_mask,
                                        int default_interface_mask,
                                        int device_id) {
  oc_resource_t *ocf_resource_nobject =
      oc_new_resource(name, uri, (uint8_t)types_len, (size_t)device_id);
  for (size_t i = 0; i < types_len; i++) {
    oc_resource_bind_resource_type(ocf_resource_nobject, types[i]);
  }
  oc_resource_bind_resource_interface(ocf_resource_nobject, interface_mask);
  oc_resource_set_default_interface(ocf_resource_nobject,
                                    default_interface_mask);
  return (void *)ocf_resource_nobject;
}

bool ocf_resource_destroyer_internal(void *ocf_resource_nobject) {
  oc_resource_t *resource = (oc_resource_t *)ocf_resource_nobject;
  return oc_delete_resource(resource);
}

// OCFResource.setDiscoverable()
void ocf_resource_setDiscoverable_internal(void *ocf_resource_nobject,
                                           bool is_discoverable) {
  oc_resource_set_discoverable((oc_resource_t *)ocf_resource_nobject,
                               is_discoverable);
}

// OCFResource.setPeriodicObservable()
void ocf_resource_setPeriodicObservable_internal(void *ocf_resource_nobject,
                                                 int period_sec) {
  oc_resource_set_periodic_observable((oc_resource_t *)ocf_resource_nobject,
                                      period_sec);
}

// OCFResource.setHandler()
static void ocf_resource_handler(oc_request_t *request,
                                 oc_interface_mask_t interface_mask,
                                 void *user_data);
void ocf_resource_setHandler_internal(void *ocf_resource_nobject,
                                      int handler_id, int method) {
  // user_data: to pass the handler id to the JS handler later
  void *user_data = (void *)handler_id;

  oc_resource_set_request_handler(ocf_resource_nobject, method,
                                  ocf_resource_handler, user_data);
}
void ocf_resource_handler(oc_request_t *request,
                          oc_interface_mask_t interface_mask, void *user_data) {
  or_setHandler_event_data_t *event_data =
      (or_setHandler_event_data_t *)malloc(sizeof(or_setHandler_event_data_t));
  oc_string_t origin_addr_ocs;
  char *origin_addr, *dest_uri;

  // Setting the attributes of event data
  oc_endpoint_to_string(request->origin, &origin_addr_ocs);
  origin_addr = oc_string(origin_addr_ocs);
  if(origin_addr == NULL) {
    free(event_data);
    return;
  }

  event_data->origin_addr =
      (char *)malloc(sizeof(char) * (strlen(origin_addr) + 1));
  strncpy(event_data->origin_addr, origin_addr, strlen(origin_addr) + 1);

  // printf("origin_addr: %x/%s\n", origin_addr, origin_addr);

  event_data->dest_device_id = (int)request->resource->device;

  dest_uri = oc_string(request->resource->uri);
  event_data->dest_uri = (char *)malloc(sizeof(char) * (strlen(dest_uri) + 1));
  strncpy(event_data->dest_uri, dest_uri, strlen(dest_uri) + 1);

  if (request->query != NULL) {
    event_data->query =
        (char *)malloc(sizeof(char) * (strlen(request->query) + 1));
    strncpy(event_data->query, request->query, strlen(request->query) + 1);
    event_data->query_len = request->query_len;
  } else {
    event_data->query = (char *)malloc(sizeof(char));
    event_data->query[0] = '\0';
    event_data->query_len = 0;
  }

  event_data->request_payload_string_len =
      oc_rep_to_json(request->request_payload, NULL, 0, true);
  event_data->request_payload_string =
      (char *)malloc(event_data->request_payload_string_len + 1);
  oc_rep_to_json(request->request_payload, event_data->request_payload_string,
                 event_data->request_payload_string_len + 1, true);

  event_data->interface_mask = (int)interface_mask;

  int handler_id = (int)user_data;
  event_data->handler_id = handler_id;

  event_data->request = (void *)request;

  EMIT_ANT_ASYNC_EVENT(ocf_resource_setHandler, handler_id, (void *)event_data,
                       true);
}

void initOCFResource(void) {
  // Empty function
}
