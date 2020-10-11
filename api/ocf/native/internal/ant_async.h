#ifndef __ANT_ASYNC_H__
#define __ANT_ASYNC_H__

#include "internal/hashmap.h"
#include "internal/ll.h"
#include "iotjs_def.h"
#include "iotjs_uv_request.h"
#include "modules/iotjs_module_buffer.h"

#include <stdbool.h>
#include <stdlib.h>

// ANT async handler procedure:
// 1. [External thread]
//    an event occurs -> type_ant_handler() -> uv_async_send()
// 2. [JS thread]
//    uv async event occurs -> type_uv_handler() -> g_type_async.js_handler()

struct ant_async_s {
  uv_async_t uv_async; // libuv async
  map_t handler_map;   // js_handler map
  ll_t *event_queue;
};
typedef struct ant_async_s ant_async_t;

struct ant_async_event_s {
  int key;
  void *data;
};
typedef struct ant_async_event_s ant_async_event_t;

// ant_async list
extern ll_t *g_ant_async_list;
void init_ant_async_list();
void destroy_ant_async_list();
void insert_ant_async_to_list(ant_async_t *async);

// ant_async
ant_async_t *create_ant_async(uv_async_cb uv_handler_fn,
                              gen_fun_t event_queue_data_destroyer);
void destroy_ant_async(ant_async_t *ant_async);
bool add_js_handler_to_ant_async(ant_async_t *ant_async, int key,
                                 jerry_value_t js_handler);
bool remove_js_handler_from_ant_async(ant_async_t *ant_async, int key);
jerry_value_t get_handler_from_ant_async(ant_async_t *ant_async, int key);
bool emit_ant_async_event(ant_async_t *ant_async, int key, void *event);

// event_queue in ant_async
void enqueue_event_to_ant_async(ant_async_t *ant_async, void *event);
void *get_first_event_from_ant_async(ant_async_t *ant_async);
void remove_first_event_from_ant_async(ant_async_t *ant_async);

// ant_event
ant_async_event_t *create_ant_async_event(int key, void *data);
void destroy_ant_async_event(ant_async_event_t *ant_async_event,
                             gen_fun_t event_queue_item_destroyer);

// declaration macros
#define GLOBAL_ANT_ASYNC(type) ant_async_t *g_##type##_async;
#define REGISTER_ANT_ASYNC_HANDLER_FUNC(type, event_queue_item_destroyer)      \
  bool register_js_handler_for_##type(int key, jerry_value_t js_handler) {     \
    bool res = add_js_handler_to_ant_async(g_##type##_async, key, js_handler); \
    return res;                                                                \
  }                                                                            \
  bool unregister_js_handler_for_##type(int key) {                             \
    remove_js_handler_from_ant_async(g_##type##_async, key);                   \
  }
#define EMIT_ANT_ASYNC_EVENT_FUNC(type)                                        \
  static bool emit_ant_async_event_for_##type(int key, void *event) {          \
    return emit_ant_async_event(g_##type##_async, key, event);                 \
  }
#define ANT_UV_HANDLER_FUNCTION(type)                                          \
  static void uv_handler_for_##type(uv_async_t *handle)

// function-call macros
#define INIT_ANT_ASYNC(type, event_queue_data_destroyer)                       \
  g_##type##_async =                                                           \
      create_ant_async(uv_handler_for_##type, event_queue_data_destroyer);
#define REGISTER_ANT_HANDLER(type, key, js_handler)                            \
  register_js_handler_for_##type(key, js_handler)
#define UNREGISTER_ANT_HANDLER(type, jsHandler)                                \
  unregister_js_handler_for_##type(jsHandler)
#define EMIT_ANT_ASYNC_EVENT(type, key, event)                                 \
  emit_ant_async_event_for_##type(key, event)

// deprecated
#define GET_MR_EVENT(type)                                                     \
  ll_get_n(g_##type##_async.event_queue, g_##type##_async.event_queue->len - 1);
#define GET_JS_HANDLER(type, key)                                              \
  get_ant_handler_from_ant_async(g_##type##_async, key)->js_handler
#define DESTROY_EVENTS(type)                                                   \
  do {                                                                         \
    int i;                                                                     \
    int events_count = g_##type##_async.event_queue->len;                      \
    for (i = 0; i < events_count - 1; i++) {                                   \
      ll_remove_first(g_##type##_async.event_queue);                           \
    }                                                                          \
  } while (false)

#endif /* !defined(__ANT_ASYNC_H__) */