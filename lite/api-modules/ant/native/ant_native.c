#include "stream/ant_native_stream.h"

#include "iotjs_def.h"

#define ANT_API_STRING_TO_STRING(api_name, function_name)                      \
  JS_FUNCTION(ant_##api_name##_##function_name) {                              \
    iotjs_string_t argMessage;                                                 \
    char result_string[100];                                                   \
    DJS_CHECK_ARGS(1, string);                                                 \
    argMessage = JS_GET_ARG(0, string);                                        \
    ant_##api_name##_##function_name##_internal(                               \
        iotjs_string_data(&argMessage), result_string);                        \
    iotjs_string_destroy(&argMessage);                                         \
    return jerry_create_string((const jerry_char_t *)result_string);           \
  }

#define ANT_API_VOID_TO_VOID(api_name, function_name)                          \
  JS_FUNCTION(ant_##api_name##_##function_name) {                              \
    ant_##api_name##_##function_name##_internal();                             \
    return jerry_create_undefined());                                          \
  }

#define REGISTER_ANT_API(module, api_name, function_name)                      \
  iotjs_jval_set_method(##module##, "##api_name##_##function_name##",          \
                        ant_##api_name##_##function_name##);

ANT_API_VOID_TO_VOID(stream, initializeStream);
ANT_API_STRING_TO_STRING(stream, callDbusMethod);

jerry_value_t InitANTNative() {
  jerry_value_t antNative = jerry_create_object();
  REGISTER_ANT_API(antNative, stream, initializeStream);
  REGISTER_ANT_API(antNative, stream, callDbusMethod);

  initANTStream();

  return antNative;
}