#ifndef __OCF_ADAPTER_INTERNAL_H__
#define __OCF_ADAPTER_INTERNAL_H__

#include "ant_async_internal.h"
#include <stdbool.h>

DECLARE_ANT_ASYNC_HANDLER_SETTER(ocf_adapter_onInitialize);
DECLARE_ANT_ASYNC_HANDLER_SETTER(ocf_adapter_onPrepareServer);
DECLARE_ANT_ASYNC_HANDLER_SETTER(ocf_adapter_onPrepareClient);

void ocf_adapter_start_internal();
void ocf_adapter_stop_internal();

bool ocf_adapter_setPlatform_internal(const char *mfg_name);
bool ocf_adapter_addDevice_internal(const char *uri, const char *resource_type,
                                    const char *name, const char *spec_version,
                                    const char *data_model_version);

void initOCFAdapter(void);

#endif /* !defined(__OCF_ADAPTER_INTERNAL_H__) */