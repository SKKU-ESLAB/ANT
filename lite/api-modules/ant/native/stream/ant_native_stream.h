#ifndef __ANT_NATIVE_STREAM_H__
#define __ANT_NATIVE_STREAM_H__

#include <stdbool.h>

// TODO: hardcoded result message length
#define MAX_RESULT_MESSAGE_LENGTH 100

#define ANT_API_INTERNAL_DEF_STRING_TO_STRING(api_name, function_name)         \
  void ant_##api_name##_##function_name##_internal(const char *inputMessage,   \
                                                   char *resultMessage);
#define ANT_API_INTERNAL_DEF_VOID_TO_VOID(api_name, function_name)             \
  void ant_##api_name##_##function_name##_internal();

bool ant_stream_testPipeline_internal(const char *ipAddress);

ANT_API_INTERNAL_DEF_STRING_TO_STRING(stream, callDbusMethod);
ANT_API_INTERNAL_DEF_VOID_TO_VOID(stream, initializeStream);

typedef void (*native_handler)(const char *);
bool ant_stream_elementConnectSignal_internal(const char *argString,
                                              native_handler handler);

void initANTStream(void);

#endif /* !defined(__ANT_NATIVE_STREAM_H__) */