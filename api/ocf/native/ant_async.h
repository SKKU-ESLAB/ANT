#ifndef __ANT_ASYNC_H__
#define __ANT_ASYNC_H__

#include "internal/ll.h"
#include "iotjs_def.h"
#include "iotjs_uv_request.h"
#include "modules/iotjs_module_buffer.h"

// ANT async handler procedure:
// 1. [External thread]
//    an event occurs -> x_ant_handler() -> uv_async_send()
// 2. [JS thread]
//    uv async event occurs -> x_uv_handler() -> g_x_async.js_handler()

typedef struct ant_async ant_async_t;
struct ant_async {
  uv_async_t uv_async;      // libuv async
  jerry_value_t js_handler; // JS async handler
  ll_t *events;
};

// TODO: is it okay if only one event is used on uv_async handler?
#define DECLARE_GLOBAL_ASYNC(x) ant_async_t g_##x##_async;

#define DECLARE_ANT_ASYNC_HANDLER(x)                                           \
  static void x##_ant_handler(void *event) {                                   \
    if (event != NULL)                                                         \
      ll_insert_last(g_##x##_async.events, event);                             \
    uv_async_send(&g_##x##_async.uv_async);                                    \
  }
#define UV_ASYNC_HANDLER(x) static void x##_uv_handler(uv_async_t *handle)
#define GET_MR_EVENT(x)                                                        \
  ll_get_n(g_##x##_async.events, g_##x##_async.events->len - 1);
#define GET_JS_HANDLER(x) g_##x##_async.js_handler
#define DESTROY_EVENTS(x)                                                      \
  do {                                                                         \
    int i;                                                                     \
    int events_count = g_##x##_async.events->len;                              \
    for (i = 0; i < events_count - 1; i++) {                                   \
      ll_remove_first(g_##x##_async.events);                                   \
    }                                                                          \
  } while (false)

#define ASYNC_REGISTER(x, args_destroyer)                                      \
  bool x##_async_register(jerry_value_t jsHandler) {                           \
    iotjs_environment_t *env = iotjs_environment_get();                        \
    uv_loop_t *loop = iotjs_environment_loop(env);                             \
    if (g_##x##_async.js_handler == '\0') {                                    \
      g_##x##_async.events = ll_new(args_destroyer);                           \
      uv_async_init(loop, &g_##x##_async.uv_async, x##_uv_handler);            \
      set_##x##_ant_handler(x##_ant_handler);                                  \
    }                                                                          \
    g_##x##_async.js_handler = jsHandler;                                      \
    return true;                                                               \
  }

#define CALL_ASYNC_REGISTER(x, jsHandler) x##_async_register(jsHandler)

#endif /* !defined(__ANT_ASYNC_H__) */