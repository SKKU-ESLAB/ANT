#ifndef __OCF_COMMON_H__
#define __OCF_COMMON_H__

#define JS_DECLARE_PTR2(JOBJ, TYPE, NAME, TYPE2)                               \
  TYPE *NAME = NULL;                                                           \
  do {                                                                         \
    if (!jerry_get_object_native_pointer(JOBJ, (void **)&NAME,                 \
                                         &TYPE2##_native_info)) {              \
      return JS_CREATE_ERROR(COMMON, "Internal");                              \
    }                                                                          \
  } while (0)

#endif /* !defined(__OCF_COMMON_H__) */