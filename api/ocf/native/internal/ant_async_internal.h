#ifndef __ANT_ASYNC_INTERNAL_H__
#define __ANT_ASYNC_INTERNAL_H__

typedef void (*ant_async_handler)(void *);

// in internal header
#define DECLARE_GLOBAL_ANT_ASYNC_HANDLER(x)                                    \
  ant_async_handler g_##x##_ant_handler = NULL;

// in internal source
#define DECLARE_ANT_ASYNC_HANDLER_SETTER(x)                                    \
  void set_##x##_ant_handler(ant_async_handler handler)

#define ANT_ASYNC_HANDLER_SETTER(x)                                            \
  void set_##x##_ant_handler(ant_async_handler handler) {                      \
    g_##x##_ant_handler = handler;                                             \
  }

#define CALL_ANT_ASYNC_HANDLER(x, event)                                       \
  do {                                                                         \
    if (g_##x##_ant_handler != NULL) {                                         \
      g_##x##_ant_handler(event);                                              \
    }                                                                          \
  } while (false)

#endif /* !defined(__ANT_ASYNC_INTERNAL_H__) */