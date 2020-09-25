#ifndef __OCF_RESOURCE_INTERNAL_H__
#define __OCF_RESOURCE_INTERNAL_H__

#include "ant_async_internal.h"
#include <stdbool.h>
#include <stdlib.h>

struct ocf_resource_setHandler_event_s {
  void *sep_response;
  char *origin_addr;
  int dest_device_id;
  char *dest_uri;
  char *query;
  int query_len;
  char *request_payload_string;
  int request_payload_string_len;
  int interface_mask;
};
typedef struct ocf_resource_setHandler_event_s ocf_resource_setHandler_event_t;
void ocf_adapter_setHandler_event_destroyer(void *item) {
  ocf_resource_setHandler_event_t *event;
  event = (ocf_resource_setHandler_event_t *)item;
  free(event->sep_response);
  free(event->origin_addr);
  free(event->dest_uri);
  free(event->query);
  free(event->request_payload_string);
  free(event);
}

void *ocf_resource_constructor_internal(const char *name, const char *uri,
                                        const char **types, int types_num,
                                        int interface_mask,
                                        int default_interface_mask,
                                        int device_id);

void ocf_resource_setDiscoverable_internal(void *ocf_resource_nobject,
                                           bool is_discoverable);
void ocf_resource_setPeriodicObservable_internal(void *ocf_resource_nobject,
                                                 int period_sec);
void ocf_resource_setHandler_internal(void *ocf_resource_nobject, int method);
DECLARE_ANT_ASYNC_HANDLER_SETTER(ocf_resource_setHandler);
void ocf_resouce_setHandler_handler_internal(void *sep_response_nobject);

void initOCFResource(void);

#endif /* !defined(__OCF_RESOURCE_INTERNAL_H__) */