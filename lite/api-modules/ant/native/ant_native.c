#include "internal/ant_native_ml.h"
#include "internal/ant_native_stream.h"
#include "internal/ll.h"

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

ANT_API_VOID_TO_VOID(stream, initializeStream);
ANT_API_STRING_TO_STRING(stream, callDbusMethod);

// TODO: add a function to reset g_uv_async (hardcoded now)
// Async handler order: gstreamer signal -> ant async -> uv async -> js
// uv async handler
bool g_is_async_handler_set = false;
uv_async_t g_uv_async;
typedef struct async_handler_args async_handler_args_t;
struct async_handler_args {
  char *element_name;
  unsigned char *buffer_data;
  unsigned long buffer_size;
};
ll_t *g_async_args_ll;
void async_handler_args_teardown(void *item) {
  async_handler_args_t *args_item;
  args_item = (async_handler_args_t *)item;
  free(args_item->element_name);
  free(args_item->buffer_data);
  free(args_item);
}
// js async handler
jerry_value_t g_js_async_handler;

static void stream_elementConnectSignal_ant_async_handler(
    const char *element_name, unsigned char *data, unsigned long data_size) {
  // ant async handler -> call uv async handler
  if (g_is_async_handler_set) {
    async_handler_args_t *args =
        (async_handler_args_t *)malloc(sizeof(async_handler_args_t));
    args->element_name =
        (char *)malloc(sizeof(char) * (strlen(element_name) + 1));
    snprintf(args->element_name, (strlen(element_name) + 1), "%s",
             element_name);
    args->buffer_data =
        (unsigned char *)malloc(sizeof(unsigned char) * data_size);
    memcpy(args->buffer_data, data, data_size);
    args->buffer_size = data_size;
    ll_insert_last(g_async_args_ll, args);

    uv_async_send(&g_uv_async);
  }
}

static void stream_elementConnectSignal_uv_handler(uv_async_t *handle) {
  // uv async handler -> call js handler
  jerry_value_t js_arg_element_name;
  jerry_value_t js_arg_buffer;
  iotjs_bufferwrap_t *buffer_wrap;
  async_handler_args_t *async_args;
  int last_index;
  last_index = g_async_args_ll->len - 1;
  if (last_index < 0) {
    return;
  }
  async_args = (async_handler_args_t *)ll_get_n(g_async_args_ll, last_index);
  if (async_args == NULL) {
    return;
  }

  js_arg_element_name =
      jerry_create_string((const jerry_char_t *)async_args->element_name);
  js_arg_buffer =
      iotjs_bufferwrap_create_buffer((size_t)async_args->buffer_size);
  buffer_wrap = iotjs_bufferwrap_from_jbuffer(js_arg_buffer);
  iotjs_bufferwrap_copy(buffer_wrap, (const char *)async_args->buffer_data,
                        (size_t)async_args->buffer_size);

  {
    jerry_value_t js_args[] = {js_arg_element_name, js_arg_buffer};
    iotjs_invoke_callback(g_js_async_handler, jerry_create_undefined(), js_args,
                          2);
  }
  jerry_release_value(js_arg_element_name);
  jerry_release_value(js_arg_buffer);

  {
    int i;
    int ll_len = g_async_args_ll->len;
    for (i = 0; i < ll_len - 1; i++) {
      ll_remove_first(g_async_args_ll);
    }
  }
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
    fprintf(stderr,
            "ERROR: JS handler already registered!"
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
    g_async_args_ll = ll_new(async_handler_args_teardown);
    uv_async_init(loop, &g_uv_async, stream_elementConnectSignal_uv_handler);
  }

  // Register ant async handler to the stream thread
  result = ant_stream_elementConnectSignal_internal(
      argElementIndex, iotjs_string_data(&argDetailedSignal),
      stream_elementConnectSignal_ant_async_handler);
  iotjs_string_destroy(&argDetailedSignal);

  return jerry_create_boolean(result);
}

JS_FUNCTION(ant_ml_getMaxOfBuffer) {
  jerry_value_t argBuffer;
  iotjs_string_t argType;

  const char *type;
  iotjs_bufferwrap_t *buffer_wrap;
  size_t buffer_len;

  DJS_CHECK_ARGS(2, object, string);
  argBuffer = JS_GET_ARG(0, object);
  argType = JS_GET_ARG(1, string);

  type = iotjs_string_data(&argType);
  buffer_wrap = iotjs_jbuffer_get_bufferwrap_ptr(argBuffer);
  if (buffer_wrap == NULL) {
    return JS_CREATE_ERROR(TYPE, "Invalid buffer given");
  }
  buffer_len = iotjs_bufferwrap_length(buffer_wrap);

  if (buffer_len <= 0) {
    fprintf(stderr, "Invalid buffer length!: %d\n", buffer_len);
    iotjs_string_destroy(&argType);
    return jerry_create_undefined();
  }

  if (strncmp(type, "uint8", strlen("uint8")) == 0) {
    unsigned char *data_array = (unsigned char *)buffer_wrap->buffer;
    size_t data_len = buffer_len / sizeof(unsigned char);
    int result_max_index;
    unsigned char result_value;
    jerry_value_t ret;

    ant_ml_getMaxOfBuffer_internal_uint8(data_array, data_len,
                                         &result_max_index, &result_value);

    ret = jerry_create_object();
    iotjs_jval_set_property_jval(ret, "max_value",
                                 jerry_create_number(result_value));
    iotjs_jval_set_property_jval(ret, "max_index",
                                 jerry_create_number(result_max_index));
    iotjs_string_destroy(&argType);
    return ret;
  } else if (strncmp(type, "int32", strlen("int32")) == 0) {
    long *data_array = (long *)buffer_wrap->buffer;
    size_t data_len = buffer_len / sizeof(long);
    int result_max_index;
    long result_value;
    jerry_value_t ret;

    ant_ml_getMaxOfBuffer_internal_int32(data_array, data_len,
                                         &result_max_index, &result_value);

    ret = jerry_create_object();
    iotjs_jval_set_property_jval(ret, "max_value",
                                 jerry_create_number(result_value));
    iotjs_jval_set_property_jval(ret, "max_index",
                                 jerry_create_number(result_max_index));
    iotjs_string_destroy(&argType);
    return ret;
  } else if (strncmp(type, "float32", strlen("float32")) == 0) {
    float *data_array = (float *)buffer_wrap->buffer;
    size_t data_len = buffer_len / sizeof(float);
    int result_max_index;
    float result_value;
    jerry_value_t ret;

    ant_ml_getMaxOfBuffer_internal_float32(data_array, data_len,
                                           &result_max_index, &result_value);

    ret = jerry_create_object();
    iotjs_jval_set_property_jval(ret, "max_value",
                                 jerry_create_number(result_value));
    iotjs_jval_set_property_jval(ret, "max_index",
                                 jerry_create_number(result_max_index));
    iotjs_string_destroy(&argType);
    return ret;
  } else {
    fprintf(stderr, "Invalid type for getMaxOfBuffer!: %s\n", type);
    fprintf(stderr, "  - Valid types: uint8, int32, float32\n");
    iotjs_string_destroy(&argType);
    return jerry_create_undefined();
  }
}

jerry_value_t InitANTNative() {
  jerry_value_t antNative = jerry_create_object();
  REGISTER_ANT_API(antNative, stream, initializeStream);
  REGISTER_ANT_API(antNative, stream, callDbusMethod);
  REGISTER_ANT_API(antNative, stream, elementConnectSignal);
  REGISTER_ANT_API(antNative, ml, getMaxOfBuffer);

  initANTStream();

  return antNative;
}