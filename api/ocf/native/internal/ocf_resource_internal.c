#include "ocf_resource_internal.h"
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
void ocf_resource_setHandler_internal(void *ocf_resource_nobject, int method) {
  // handler_method: to pass the request method to the JS handler later
  int *handler_method = (int *)malloc(sizeof(int));
  *handler_method = method;

  oc_resource_set_request_handler(ocf_resource_nobject, method,
                                  ocf_resource_handler, handler_method);
}
DECLARE_GLOBAL_ANT_ASYNC_HANDLER(ocf_resource_setHandler)
ANT_ASYNC_HANDLER_SETTER(ocf_resource_setHandler)
void ocf_resource_handler(oc_request_t *request,
                          oc_interface_mask_t interface_mask, void *user_data) {
  ocf_resource_setHandler_event_t *event =
      (ocf_resource_setHandler_event_t *)malloc(
          sizeof(ocf_resource_setHandler_event_t));
  oc_string_t origin_addr_ocs;
  char *origin_addr, *dest_uri;

  event->sep_response = (void *)malloc(sizeof(oc_separate_response_t));
  oc_indicate_separate_response(request, event->sep_response);

  oc_endpoint_to_string(request->origin, &origin_addr_ocs);
  origin_addr = oc_string(origin_addr_ocs);
  event->origin_addr = (char *)malloc(sizeof(char) * (strlen(origin_addr) + 1));
  strncpy(event->origin_addr, origin_addr, strlen(origin_addr));

  event->dest_device_id = (int)request->resource->device;

  dest_uri = oc_string(request->resource->uri);
  event->dest_uri = (char *)malloc(sizeof(char) * (strlen(dest_uri) + 1));
  strncpy(event->dest_uri, dest_uri, strlen(dest_uri));

  if (request->query != NULL) {
    event->query = (char *)malloc(sizeof(char) * (strlen(request->query) + 1));
    strncpy(event->query, request->query, strlen(request->query));
    event->query_len = request->query_len;
  } else {
    event->query = (char *)malloc(sizeof(char));
    event->query[0] = '\0';
    event->query_len = 0;
  }

  event->request_payload_string_len =
      oc_rep_to_json(request->request_payload, NULL, 0, true);
  event->request_payload_string =
      (char *)malloc(event->request_payload_string_len + 1);
  oc_rep_to_json(request->request_payload, event->request_payload_string,
                 event->request_payload_string_len + 1, true);

  event->interface_mask = (int)interface_mask;

  int method = *((int *)user_data); // user_data will be freed in ant-async
  event->method = method;

  CALL_ANT_ASYNC_HANDLER(ocf_resource_setHandler, (void *)event);
}
void ocf_resource_setHandler_handler_internal(void *sep_response_nobject) {
  oc_separate_response_t *sep_response =
      (oc_separate_response_t *)sep_response_nobject;
  oc_set_separate_response_buffer(sep_response);
}

void initOCFResource(void) {
  // Empty function
}