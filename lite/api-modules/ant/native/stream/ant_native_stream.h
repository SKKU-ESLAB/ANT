#ifndef __ANT_NATIVE_STREAM_H__
#define __ANT_NATIVE_STREAM_H__

#include <stdbool.h>

#define MAX_RESULT_MESSAGE_LENGTH 100

#define ANT_API_INTERNAL_DEF_STRING_TO_STRING(api_name, function_name)         \
  void ant_##api_name##_##function_name##_internal(const char *inputMessage,   \
                                                   char *resultMessage);
#define ANT_API_INTERNAL_DEF_VOID_TO_VOID(api_name, function_name)             \
  void ant_##api_name##_##function_name##_internal();

bool ant_stream_testPipeline_internal(const char *ipAddress);

ANT_API_INTERNAL_DEF_STRING_TO_STRING(stream, callDbusMethod);
ANT_API_INTERNAL_DEF_VOID_TO_VOID(stream, initializeStream);

void initANTStream(void);

#endif /* !defined(__ANT_NATIVE_STREAM_H__) */