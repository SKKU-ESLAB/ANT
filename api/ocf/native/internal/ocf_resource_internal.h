#ifndef __OCF_RESOURCE_INTERNAL_H__
#define __OCF_RESOURCE_INTERNAL_H__

#include "ant_async_internal.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

struct ocf_resource_setHandler_event_s {
  void *request;
  pthread_mutex_t sync_mutex;
  pthread_cond_t sync_cond;

  char *origin_addr;
  int dest_device_id;
  char *dest_uri;
  char *query;
  size_t query_len;
  char *request_payload_string;
  size_t request_payload_string_len;
  int interface_mask;
  int handler_id;
};
typedef struct ocf_resource_setHandler_event_s ocf_resource_setHandler_event_t;
void ocf_resource_setHandler_event_destroyer(void *item) {
  ocf_resource_setHandler_event_t *event;
  event = (ocf_resource_setHandler_event_t *)item;
  free(event->origin_addr);
  free(event->dest_uri);
  free(event->query);
  free(event->request_payload_string);
  free(event);
}

void *ocf_resource_constructor_internal(const char *name, const char *uri,
                                        const char **types, size_t types_num,
                                        int interface_mask,
                                        int default_interface_mask,
                                        int device_id);

void ocf_resource_setDiscoverable_internal(void *ocf_resource_nobject,
                                           bool is_discoverable);
void ocf_resource_setPeriodicObservable_internal(void *ocf_resource_nobject,
                                                 int period_sec);
void ocf_resource_setHandler_internal(void *ocf_resource_nobject,
                                      int handler_id, int method);
EMIT_ANT_ASYNC_EVENT_FUNC_SETTER(ocf_resource_setHandler);

void initOCFResource(void);

#endif /* !defined(__OCF_RESOURCE_INTERNAL_H__) */