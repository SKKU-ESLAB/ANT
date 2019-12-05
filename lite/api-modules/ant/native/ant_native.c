#include "stream/ant_native_stream.h"

#include "iotjs_def.h"
#include <stdio.h>

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

#define ANT_API_VOID_TO_VOID(api_name, function_name)                          \
  JS_FUNCTION(ant_##api_name##_##function_name) {                              \
    ant_##api_name##_##function_name##_internal();                             \
    return jerry_create_undefined();                                           \
  }

#define REGISTER_ANT_API(module, api_name, function_name)                      \
  iotjs_jval_set_method(module, #api_name "_" #function_name,                  \
                        ant_##api_name##_##function_name);

ANT_API_VOID_TO_VOID(stream, initializeStream);
ANT_API_STRING_TO_STRING(stream, callDbusMethod);

// TODO: add a function to reset g_js_handler
bool g_is_js_handler_set = false;
jerry_value_t g_js_handler = 0;

static void stream_elementConnectSignal_handler(const char *message) {
  if (g_is_js_handler_set) {
    jerry_value_t js_args[1];
    js_args[0] = jerry_create_string((const jerry_char_t *)message);
    iotjs_invoke_callback(g_js_handler, jerry_create_undefined(), js_args, 1);
    jerry_release_value(js_args[0]);
  }
}

JS_FUNCTION(ant_stream_elementConnectSignal) {
  iotjs_string_t argString;
  jerry_value_t argHandler;
  bool result;
  DJS_CHECK_ARGS(2, string, function);
  argString = JS_GET_ARG(0, string);
  argHandler = JS_GET_ARG(1, function);

  // TODO: Hardcoding: unique handler
  if (g_is_js_handler_set) {
    fprintf(stderr, "ERROR: JS handler already registered!"
                    " (unique handler constraint)");
    return jerry_create_boolean(false);
  }
  g_js_handler = argHandler;
  g_is_js_handler_set = true;

  result = ant_stream_elementConnectSignal_internal(
      iotjs_string_data(&argString), &stream_elementConnectSignal_handler);
  iotjs_string_destroy(&argString);
  return jerry_create_boolean(result);
}

jerry_value_t InitANTNative() {
  jerry_value_t antNative = jerry_create_object();
  REGISTER_ANT_API(antNative, stream, initializeStream);
  REGISTER_ANT_API(antNative, stream, callDbusMethod);
  REGISTER_ANT_API(antNative, stream, elementConnectSignal);

  initANTStream();

  return antNative;
}