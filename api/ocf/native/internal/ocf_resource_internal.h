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

#ifndef __OCF_RESOURCE_INTERNAL_H__
#define __OCF_RESOURCE_INTERNAL_H__

#include <stdbool.h>
#include <stdlib.h>

#include "./ant_async.h"

struct or_setHandler_event_data_s {
  void *request;

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
typedef struct or_setHandler_event_data_s or_setHandler_event_data_t;
void or_setHandler_event_data_destroyer(void *item) {
  or_setHandler_event_data_t *event;
  event = (or_setHandler_event_data_t *)item;
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
bool ocf_resource_destroyer_internal(void *ocf_resource_nobject);

void ocf_resource_setDiscoverable_internal(void *ocf_resource_nobject,
                                           bool is_discoverable);
void ocf_resource_setPeriodicObservable_internal(void *ocf_resource_nobject,
                                                 int period_sec);

ANT_ASYNC_DECL_IN_HEADER(ocf_resource_setHandler);
void ocf_resource_setHandler_internal(void *ocf_resource_nobject,
                                      int handler_id, int method);

void initOCFResource(void);

#endif /* !defined(__OCF_RESOURCE_INTERNAL_H__) */
