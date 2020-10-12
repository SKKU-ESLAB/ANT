#ifndef __OCF_ADAPTER_H__
#define __OCF_ADAPTER_H__

#include "iotjs_def.h"
#include "iotjs_uv_request.h"
#include "ocf_common.h"

void ocf_adapter_init(void);

void InitOCFAdapterNative(jerry_value_t ocfNative);

#endif /* !defined(__OCF_ADAPTER_H__) */