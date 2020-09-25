#ifndef __OCF_ADAPTER_INTERNAL_H__
#define __OCF_ADAPTER_INTERNAL_H__

#include "ant_async_internal.h"
#include <stdbool.h>

DECLARE_ANT_ASYNC_HANDLER_SETTER(ocf_adapter_onInitialize);
DECLARE_ANT_ASYNC_HANDLER_SETTER(ocf_adapter_onPrepareServer);
DECLARE_ANT_ASYNC_HANDLER_SETTER(ocf_adapter_onPrepareClient);

void ocf_adapter_start_internal(void);
void ocf_adapter_stop_internal(void);

bool ocf_adapter_setPlatform_internal(const char *mfg_name);
bool ocf_adapter_addDevice_internal(const char *uri, const char *resource_type,
                                    const char *name, const char *spec_version,
                                    const char *data_model_version);
bool ocf_adapter_addResource_internal(void *ocf_resource_nobject);

void ocf_adapter_repStartRootObject_internal(void);
void ocf_adapter_repSetBoolean_internal(const char* key, bool value);
void ocf_adapter_repSetInt_internal(const char* key, int value);
void ocf_adapter_repSetDouble_internal(const char* key, double value);
void ocf_adapter_repSetString_internal(const char* key, const char* value);
void ocf_adapter_repEndRootObject_internal(void);
void ocf_adapter_sendResponse_internal(void* ocf_request_nobject, int status_code);


void initOCFAdapter(void);

#endif /* !defined(__OCF_ADAPTER_INTERNAL_H__) */