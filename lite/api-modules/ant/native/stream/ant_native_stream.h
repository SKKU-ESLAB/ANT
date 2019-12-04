#ifndef __ANT_NATIVE_STREAM_H__
#define __ANT_NATIVE_STREAM_H__

#include <stdbool.h>

bool ant_stream_testPipeline_internal(const char *ipAddress);
void ant_stream_callDbusMethod_internal(const char *inputMessage,
                                        char *resultMessage);

void initANTStream(void);

#endif /* !defined(__ANT_NATIVE_STREAM_H__) */