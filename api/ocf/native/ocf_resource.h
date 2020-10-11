#ifndef __OCF_RESOURCE_H__
#define __OCF_RESOURCE_H__

#include "iotjs_def.h"
#include "iotjs_uv_request.h"
#include "modules/iotjs_module_buffer.h"

#define JS_DECLARE_PTR2(JOBJ, TYPE, NAME, TYPE2)                               \
  TYPE *NAME = NULL;                                                           \
  do {                                                                         \
    if (!jerry_get_object_native_pointer(JOBJ, (void **)&NAME,                 \
                                         &TYPE2##_native_info)) {              \
      return JS_CREATE_ERROR(COMMON, "Internal");                              \
    }                                                                          \
  } while (0)

void ocf_resource_init(void);

// OCFResource, OCFRequest
extern const jerry_object_native_info_t ocf_resource_native_info;
extern const jerry_object_native_info_t ocf_request_native_info;

void InitOCFResourceNative(jerry_value_t ocfNative);

#endif /* !defined(__OCF_RESOURCE_H__) */