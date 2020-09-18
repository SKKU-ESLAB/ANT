#ifndef __ANT_STREAM_NATIVE_INTERNAL_H__
#define __ANT_STREAM_NATIVE_INTERNAL_H__

#include <stdbool.h>

bool ant_stream_testPipeline_internal(const char *ipAddress);

void ant_stream_callDbusMethod_internal(const char *inputMessage, char *resultMessage);
void ant_stream_initializeStream_internal();

typedef void (*ant_async_handler)(const char *, unsigned char *, unsigned long);
bool ant_stream_elementConnectSignal_internal(int element_index,
                                              const char *detailed_signal,
                                              ant_async_handler handler);

void initANTStream(void);

#endif /* !defined(__ANT_STREAM_NATIVE_INTERNAL_H__) */