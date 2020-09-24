#ifndef __OCF_ADAPTER_INTERNAL_H__
#define __OCF_ADAPTER_INTERNAL_H__

#include "ant_async_internal.h"

void ocf_adapter_start_internal();
void ocf_adapter_stop_internal();

DECLARE_ANT_ASYNC_HANDLER_SETTER(ocf_adapter_onPrepareServer);
DECLARE_ANT_ASYNC_HANDLER_SETTER(ocf_adapter_onPrepareClient);

void initOCF(void);

#endif /* !defined(__OCF_ADAPTER_INTERNAL_H__) */