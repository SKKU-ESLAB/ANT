#ifndef __OCF_RESOURCE_H__
#define __OCF_RESOURCE_H__

#include "iotjs_def.h"
#include "iotjs_uv_request.h"
#include "modules/iotjs_module_buffer.h"
#include "ocf_common.h"

void ocf_resource_init(void);

// OCFResource, OCFRequest
extern const jerry_object_native_info_t ocf_resource_native_info;
extern const jerry_object_native_info_t ocf_request_native_info;

void InitOCFResourceNative(jerry_value_t ocfNative);

#endif /* !defined(__OCF_RESOURCE_H__) */