/* Copyright (c) 2017-2020 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <iotjs_def.h>
#include <iotjs_uv_request.h>
#include <modules/iotjs_module_buffer.h>

#include "../../common/native/ant_common.h"
#include "./internal/ant_stream_native_internal.h"
#include "./internal/ll.h"

ANT_API_VOID_TO_VOID(ant_stream, initializeStream);
ANT_API_STRING_TO_STRING(ant_stream, callDbusMethod);

// TODO(RedCarrottt): add a function to reset g_uv_async (hardcoded now)
// Async handler order: gstreamer signal -> ant async -> uv async -> js
// uv async handler
bool g_is_async_handler_set = false;
uv_async_t g_uv_async;
typedef struct async_handler_args async_handler_args_t;
struct async_handler_args {
  char *element_name;
  unsigned char *buffer_data;
  uint32_t buffer_size;
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
    const char *element_name, unsigned char *data, uint32_t data_size) {
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
    fprintf(stderr, "ERROR: JS handler already registered!"
                    " (unique handler constraint)");
    return jerry_create_boolean(false);
  }
  g_is_async_handler_set = true;

  // Register JS async handler
  g_js_async_handler = argHandler;

  // Register uv async handler
  // TODO(RedCarrottt): Hardcoding: unique handler
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

jerry_value_t InitANTStreamNative() {
  jerry_value_t antStreamNative = jerry_create_object();
  REGISTER_ANT_API(antStreamNative, ant_stream, initializeStream);
  REGISTER_ANT_API(antStreamNative, ant_stream, callDbusMethod);
  REGISTER_ANT_API(antStreamNative, ant_stream, elementConnectSignal);

  initANTStream();

  return antStreamNative;
}
