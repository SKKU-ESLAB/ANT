#ifndef __OCF_NATIVE_INTERNAL_H__
#define __OCF_NATIVE_INTERNAL_H__

void ocf_adapter_start_internal();

typedef void (*ant_handler_v_v)();
void ocf_adapter_onPrepareServer_internal(ant_handler_v_v handler);

void initOCF(void);

#endif /* !defined(__OCF_NATIVE_INTERNAL_H__) */