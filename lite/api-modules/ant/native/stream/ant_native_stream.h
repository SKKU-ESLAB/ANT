#ifndef __ANT_NATIVE_STREAM_H__
#define __ANT_NATIVE_STREAM_H__

#include <stdbool.h>

bool ant_stream_testPipeline_internal(const char *ipAddress);
bool ant_stream_testMessage_internal(const char *message);

#endif /* !defined(__ANT_NATIVE_STREAM_H__) */