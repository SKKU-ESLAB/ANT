#ifndef __ANT_ASYNC_INTERNAL_H__
#define __ANT_ASYNC_INTERNAL_H__

typedef void (*ant_handler_v_v)();

// in internal header
#define DECLARE_GLOBAL_ANT_ASYNC_HANDLER(x)                                    \
  ant_handler_v_v g_##x##_ant_handler = NULL;

// in internal source
#define DECLARE_ANT_ASYNC_HANDLER_SETTER(x)                                    \
  void set_##x##_ant_handler(ant_handler_v_v handler);

#define ANT_ASYNC_HANDLER_SETTER(x)                                            \
  void set_##x##_ant_handler(ant_handler_v_v handler) {                        \
    g_##x##_ant_handler = handler;                                             \
  }

#define CALL_ANT_ASYNC_HANDLER(x)                                              \
  do {                                                                         \
    if (g_##x##_ant_handler != NULL) {                                         \
      g_##x##_ant_handler();                                                   \
    }                                                                          \
  } while (false)

#endif /* !defined(__ANT_ASYNC_INTERNAL_H__) */