#ifndef __OCF_NATIVE_INTERNAL_H__
#define __OCF_NATIVE_INTERNAL_H__

#include "ant_async_internal.h"

void ocf_adapter_start_internal();
DECLARE_ANT_ASYNC_HANDLER_SETTER(ocf_adapter_onPrepareServer)

void initOCF(void);

#endif /* !defined(__OCF_NATIVE_INTERNAL_H__) */