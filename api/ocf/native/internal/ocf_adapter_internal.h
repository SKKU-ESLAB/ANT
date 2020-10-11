#ifndef __OCF_ADAPTER_INTERNAL_H__
#define __OCF_ADAPTER_INTERNAL_H__

#include "ant_async_internal.h"
#include "ll.h"
#include <stdbool.h>
#include <stdlib.h>

void ocf_adapter_init_internal(void);
void ocf_adapter_deinit_internal(void);

EMIT_ANT_ASYNC_EVENT_FUNC_SETTER(ocf_adapter_onInitialize);
EMIT_ANT_ASYNC_EVENT_FUNC_SETTER(ocf_adapter_onPrepareServer);
EMIT_ANT_ASYNC_EVENT_FUNC_SETTER(ocf_adapter_onPrepareClient);

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
void ocf_adapter_repEndRootObject_internal(void);
void ocf_adapter_sendResponse_internal(void *ocf_request_nobject,
                                       int status_code);

struct ocf_adapter_discovery_event_s {
  char *uri;
  ll_t *types;
  int interface_mask;
  void *endpoint;
};
typedef struct ocf_adapter_discovery_event_s ocf_adapter_discovery_event_t;
void ocf_adapter_discovery_event_destroyer(void *item) {
  ocf_adapter_discovery_event_t *event;
  event = (ocf_adapter_discovery_event_t *)item;
  free(event->uri);
  ll_delete(event->types);
  free(event);
}
void ocf_adapter_discovery_event_types_destroyer(void *item) { free(item); }
void ocf_endpoint_destroy(void *handle);

EMIT_ANT_ASYNC_EVENT_FUNC_SETTER(ocf_adapter_discovery);
void ocf_adapter_discovery_internal(const char *resource_type);

struct ocf_client_response_event_s {
  void *endpoint;
  char *payload_string;
  size_t payload_string_length;
  int status_code;
};
typedef struct ocf_client_response_event_s ocf_client_response_event_t;
void ocf_adapter_response_event_destroyer(void *item) {
  ocf_client_response_event_t *event;
  event = (ocf_client_response_event_t *)item;
  free(event->payload_string);
  free(event);
}

EMIT_ANT_ASYNC_EVENT_FUNC_SETTER(ocf_adapter_observe);
bool ocf_adapter_observe_internal(void *ocf_endpoint_nobject, const char *uri,
                                  const char *query, int qos);
bool ocf_adapter_stopObserve_internal(void *ocf_endpoint_nobject,
                                      const char *uri);

void initOCFAdapter(void);

#endif /* !defined(__OCF_ADAPTER_INTERNAL_H__) */