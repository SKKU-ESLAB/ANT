#include "stream/ant_native_stream.h"

#include "iotjs_def.h"
#include "iotjs_uv_request.h"
#include "modules/iotjs_module_buffer.h"

#include <stdio.h>
#include <stdlib.h>

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

// TODO: add a function to reset g_uv_async (hardcoded now)
// Async handler order: gstreamer signal -> ant async -> uv async -> js
#define ASYNC_NAME_LENGTH 100
// uv async handler
bool g_is_async_handler_set = false;
uv_async_t g_uv_async;
char g_async_name[ASYNC_NAME_LENGTH];
unsigned char *g_async_data;
unsigned long g_async_data_size;
// js async handler
jerry_value_t g_js_async_handler;

static void stream_elementConnectSignal_ant_async_handler(
    const char *element_name, unsigned char *data, unsigned long data_size) {
  // ant async handler -> call uv async handler
  if (g_is_async_handler_set) {
    g_async_data = (unsigned char *)malloc(sizeof(unsigned char) * data_size);
    memcpy(g_async_data, data, data_size);
    uv_async_send(&g_uv_async);
  }
}

static void stream_elementConnectSignal_uv_handler(uv_async_t *handle) {
  // uv async handler -> call js handler
  jerry_value_t js_args[2];
  iotjs_bufferwrap_t *buffer_wrap;

  js_args[0] = jerry_create_string((const jerry_char_t *)g_async_name);

  js_args[1] = iotjs_bufferwrap_create_buffer((size_t)g_async_data_size);
  buffer_wrap = iotjs_bufferwrap_from_jbuffer(js_args[1]);
  iotjs_bufferwrap_copy(buffer_wrap, (const char *)g_async_data,
                        (size_t)g_async_data_size);

  iotjs_invoke_callback(g_js_async_handler, jerry_create_undefined(), js_args,
                        2);
  jerry_release_value(js_args[0]);
  jerry_release_value(js_args[1]);
  free(g_async_data);
}

JS_FUNCTION(ant_stream_elementConnectSignal) {
  int argElementIndex;
  iotjs_string_t argDetailedSignal;
  jerry_value_t argHandler;
  bool result;
  DJS_CHECK_ARGS(3, number, string, function);
  argElementIndex = JS_GET_ARG(0, number);
  argDetailedSignal = JS_GET_ARG(1, string);
  argHandler = JS_GET_ARG(2, function);

  if (g_is_async_handler_set) {
    fprintf(stderr, "ERROR: JS handler already registered!"
                    " (unique handler constraint)");
    return jerry_create_boolean(false);
  }
  g_is_async_handler_set = true;

  // Register JS async handler
  g_js_async_handler = argHandler;

  // Register uv async handler
  // TODO: Hardcoding: unique handler
  {
    iotjs_environment_t *env = iotjs_environment_get();
    uv_loop_t *loop = iotjs_environment_loop(env);
    uv_async_init(loop, &g_uv_async, stream_elementConnectSignal_uv_handler);
  }

  // Register ant async handler to the stream thread
  result = ant_stream_elementConnectSignal_internal(
      argElementIndex, iotjs_string_data(&argDetailedSignal),
      stream_elementConnectSignal_ant_async_handler);
  iotjs_string_destroy(&argDetailedSignal);

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