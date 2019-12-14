#include "internal/ant_companion_native_internal.h"

#include "iotjs_def.h"
#include "iotjs_uv_request.h"
#include "modules/iotjs_module_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ANT_API_STRING_TO_STRING(api_name, function_name)                      \
  JS_FUNCTION(ant_##api_name##_##function_name) {                              \
    iotjs_string_t argString;                                                  \
    char result_string[MAX_RESULT_MESSAGE_LENGTH];                             \
    DJS_CHECK_ARGS(1, string);                                                 \
    argString = JS_GET_ARG(0, string);                                         \
    ant_##api_name##_##function_name##_internal(iotjs_string_data(&argString), \
                                                result_string);                \
    iotjs_string_destroy(&argString);                                          \
    return jerry_create_string((const jerry_char_t *)result_string);           \
  }

#define ANT_API_VOID_TO_STRING(api_name, function_name)              \
  JS_FUNCTION(ant_##api_name##_##function_name) {                    \
    char result_string[MAX_RESULT_MESSAGE_LENGTH];                   \
    ant_##api_name##_##function_name##_internal(result_string);      \
    return jerry_create_string((const jerry_char_t *)result_string); \
  }

#define ANT_API_VOID_TO_VOID(api_name, function_name) \
  JS_FUNCTION(ant_##api_name##_##function_name) {     \
    ant_##api_name##_##function_name##_internal();    \
    return jerry_create_undefined();                  \
  }

#define REGISTER_ANT_API(module, api_name, function_name)     \
  iotjs_jval_set_method(module, #api_name "_" #function_name, \
                        ant_##api_name##_##function_name);

ANT_API_STRING_TO_STRING(companion, getMyIPAddress);

jerry_value_t InitANTCompanionNative() {
  jerry_value_t antCompanionNative = jerry_create_object();
  REGISTER_ANT_API(antCompanionNative, companion, getMyIPAddress);

  initANTCompanion();

  return antCompanionNative;
}