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

#ifndef __ANT_ASYNC_H__
#define __ANT_ASYNC_H__

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

#include <iotjs_def.h>
#include <iotjs_uv_request.h>
#include <modules/iotjs_module_buffer.h>

#include "./hashmap.h"
#include "./ll.h"

// ANT async handler procedure:
// 1. [External thread]
//    an event occurs -> type_ant_handler() -> uv_async_send()
// 2. [JS thread]
//    uv async event occurs -> type_uv_handler() -> g_type_async.js_handler()

struct ant_async_s {
  // libuv async
  uv_async_t uv_async;

  // js_handler map
  map_t handler_map;
  ll_t *event_queue;
  char *name;
};
typedef struct ant_async_s ant_async_t;

struct ant_async_event_s {
  int key;
  void *data;

  pthread_mutex_t sync_mutex;
  pthread_cond_t sync_cond;
};
typedef struct ant_async_event_s ant_async_event_t;

// ant_async list
extern ll_t *g_ant_async_list;
void init_ant_async_list();
void destroy_ant_async_list();
void insert_ant_async_to_list(ant_async_t *async);

// ant_async
ant_async_t *create_ant_async(uv_async_cb uv_handler_fn,
                              gen_fun_t event_queue_data_destroyer,
                              const char *name);
void destroy_ant_async(ant_async_t *ant_async);
bool add_js_handler_to_ant_async(ant_async_t *ant_async, int key,
                                 jerry_value_t js_handler);
bool remove_js_handler_from_ant_async(ant_async_t *ant_async, int key);
jerry_value_t get_js_handler_from_ant_async(ant_async_t *ant_async, int key);
bool emit_ant_async_event(ant_async_t *ant_async, int key, void *event_data,
                          bool sync_mode);
void wakeup_ant_async_sender(ant_async_event_t *ant_async_event);

// event_queue in ant_async
void enqueue_event_to_ant_async(ant_async_t *ant_async,
                                ant_async_event_t *ant_async_event);
ant_async_event_t *get_first_event_from_ant_async(ant_async_t *ant_async);
void remove_first_event_from_ant_async(ant_async_t *ant_async);

// ant_event
ant_async_event_t *create_ant_async_event(int key, void *data);
void destroy_ant_async_event(ant_async_event_t *ant_async_event,
                             gen_fun_t event_queue_item_destroyer);

/** declaration macros **/
#define ANT_ASYNC(type) g_##type##_async
#define ANT_ASYNC_DECL(type) ant_async_t *ANT_ASYNC(type);

#define REGISTER_JS_HANDLER_FUNC(type, event_queue_item_destroyer)             \
  bool register_js_handler_for_##type(int key, jerry_value_t js_handler) {     \
    bool res = add_js_handler_to_ant_async(ANT_ASYNC(type), key, js_handler);  \
    return res;                                                                \
  }
#define REGISTER_JS_HANDLER_FDECL(type)                                        \
  bool register_js_handler_for_##type(int key, jerry_value_t js_handler);
#define UNREGISTER_JS_HANDLER_FUNC(type)                                       \
  bool unregister_js_handler_for_##type(int key) {                             \
    return remove_js_handler_from_ant_async(ANT_ASYNC(type), key);             \
  }
#define UNREGISTER_JS_HANDLER_FDECL(type)                                      \
  bool unregister_js_handler_for_##type(int key);

#define EMIT_ANT_ASYNC_EVENT_FUNC(type)                                        \
  bool emit_ant_async_event_for_##type(int key, void *event_data,              \
                                       bool sync_mode) {                       \
    return emit_ant_async_event(ANT_ASYNC(type), key, event_data, sync_mode);  \
  }
#define EMIT_ANT_ASYNC_EVENT_FDECL(type)                                       \
  bool emit_ant_async_event_for_##type(int key, void *event_data,              \
                                       bool sync_mode)

#define ANT_ASYNC_DECL_FUNCS(type, event_queue_item_destroyer)                 \
  ANT_ASYNC_DECL(type)                                                         \
  REGISTER_JS_HANDLER_FUNC(type, event_queue_item_destroyer)                   \
  UNREGISTER_JS_HANDLER_FUNC(type)                                             \
  EMIT_ANT_ASYNC_EVENT_FUNC(type)
#define ANT_ASYNC_DECL_IN_HEADER(type)                                         \
  REGISTER_JS_HANDLER_FDECL(type);                                             \
  UNREGISTER_JS_HANDLER_FDECL(type);                                           \
  EMIT_ANT_ASYNC_EVENT_FDECL(type);

#define ANT_UV_HANDLER_FUNCTION(type)                                          \
  static void uv_handler_for_##type(uv_async_t *handle)
/** declaration macros (end) **/

/** function-call macros **/
#define INIT_ANT_ASYNC(type, event_queue_data_destroyer)                       \
  ANT_ASYNC(type) = create_ant_async(uv_handler_for_##type,                    \
                                     event_queue_data_destroyer, #type);       \
  insert_ant_async_to_list(ANT_ASYNC(type));
#define REGISTER_JS_HANDLER(type, key, js_handler)                             \
  register_js_handler_for_##type(key, js_handler)

#define EMIT_ANT_ASYNC_EVENT(type, key, event_data, sync_mode)                 \
  emit_ant_async_event_for_##type(key, event_data, sync_mode)

#define GET_FIRST_EVENT_FROM_ANT_ASYNC(type)                                   \
  get_first_event_from_ant_async(ANT_ASYNC(type))
#define GET_JS_HANDLER_FROM_ANT_ASYNC(type, key)                               \
  get_js_handler_from_ant_async(ANT_ASYNC(type), key)
#define REMOVE_FIRST_EVENT_FROM_ANT_ASYNC(type)                                \
  remove_first_event_from_ant_async(ANT_ASYNC(type))
#define UNREGISTER_JS_HANDLER(type, key) unregister_js_handler_for_##type(key)
/** function-call macros (end) **/

#endif /* !defined(__ANT_ASYNC_H__) */
