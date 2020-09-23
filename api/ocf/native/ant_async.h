#ifndef __ANT_ASYNC_H__
#define __ANT_ASYNC_H__

#include "internal/ll.h"
#include "iotjs_def.h"
#include "iotjs_uv_request.h"
#include "modules/iotjs_module_buffer.h"

typedef struct ant_async ant_async_t;
struct ant_async {
  uv_async_t uv_async;      // libuv async
  jerry_value_t js_handler; // JS async handler
  ll_t *events;
};

#define DECLARE_GLOBAL_ASYNC(x) ant_async_t g_##x##_async;

#define DECLARE_ASYNC_HANDLERS(x)                                              \
  static void x##_ant_handler() { uv_async_send(&g_##x##_async.uv_async); }    \
  static void x##_uv_handler(uv_async_t *handle) {                             \
    iotjs_invoke_callback(g_##x##_async.js_handler, jerry_create_undefined(),  \
                          NULL, 0);                                            \
  }

#define ASYNC_REGISTER(x)                                                      \
  bool x##_async_register(jerry_value_t jsHandler) {                           \
    iotjs_environment_t *env = iotjs_environment_get();                        \
    uv_loop_t *loop = iotjs_environment_loop(env);                             \
    if (g_##x##_async.js_handler != '\0') {                                    \
      fprintf(stderr, "ERROR: JS handler already registered!");                \
      return false;                                                            \
    }                                                                          \
    g_##x##_async.js_handler = jsHandler;                                      \
    uv_async_init(loop, &g_##x##_async.uv_async, x##_uv_handler);              \
    set_##x##_ant_handler(x##_ant_handler);                                    \
    return true;                                                               \
  }

#define CALL_ASYNC_REGISTER(x, jsHandler) x##_async_register(jsHandler)

#endif /* !defined(__ANT_ASYNC_H__) */