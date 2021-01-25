/* Copyright (c) 2017-2021 SKKU ESLAB, and contributors. All rights reserved.
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

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iotjs_def.h>
#include <iotjs_uv_request.h>
#include <modules/iotjs_module_buffer.h>

#include "../../common/native/ant_common.h"
#include "./ant_ml_common.h"
#include "./ant_ml_dfe.h"
#include "./internal/ant_ml_dfe_internal.h"

const jerry_object_native_info_t interpreters_native_info = {
    .free_cb = (jerry_object_native_free_callback_t)interpreters_destroy,
};

JS_FUNCTION(ant_ml_dfeLoad) {
  iotjs_string_t argModelName;
  int argNumFragments;
  DJS_CHECK_ARGS(2, string, number);
  argModelName = JS_GET_ARG(0, string);
  argNumFragments = (int)JS_GET_ARG(1, number);
  const char *modelName = iotjs_string_data(&argModelName);

  void *native_interpreters =
      ant_ml_dfeLoad_internal(modelName, argNumFragments);
  jerry_value_t js_interpreters = jerry_create_object();
  jerry_set_object_native_pointer(js_interpreters, native_interpreters,
                                  &interpreters_native_info);
  IOTJS_ASSERT(jerry_get_object_native_pointer(js_interpreters, NULL,
                                               &interpreters_native_info));

  iotjs_string_destroy(&argModelName);
  return js_interpreters;
}

JS_FUNCTION(ant_ml_dfeExecute) {
  jerry_value_t argInterpreters;
  jerry_value_t argInputTensor;
  int argStartLayerNum;
  int argEndLayerNum;
  DJS_CHECK_ARGS(4, object, object, number, number);
  argInterpreters = JS_GET_ARG(0, object);
  argInputTensor = JS_GET_ARG(1, object);
  argStartLayerNum = (int)JS_GET_ARG(2, number);
  argEndLayerNum = (int)JS_GET_ARG(3, number);

  JS_DECLARE_PTR2(argInterpreters, void, interpreters_nobject, interpreters);
  iotjs_bufferwrap_t *inputTensorBuffer =
      iotjs_bufferwrap_from_jbuffer(argInputTensor);
  void *inputTensorNativeBuffer = (void *)inputTensorBuffer->buffer;
  size_t inputTensorLength = iotjs_bufferwrap_length(inputTensorBuffer);

  void *outputTensor = ant_ml_dfeExecute_internal(
      interpreters_nobject, inputTensorNativeBuffer, inputTensorLength,
      argStartLayerNum, argEndLayerNum);

  size_t outputTensorLength;
  void *outputTensorNativeBuffer = ant_ml_dfeExecute_getOutputBufferWithLength(
      outputTensor, &outputTensorLength);

  jerry_value_t js_outputTensor =
      iotjs_bufferwrap_create_buffer(outputTensorLength);
  iotjs_bufferwrap_t *outputTensorBuffer =
      iotjs_bufferwrap_from_jbuffer(js_outputTensor);
  iotjs_bufferwrap_copy(outputTensorBuffer,
                        (const char *)outputTensorNativeBuffer,
                        outputTensorLength);
  ant_ml_dfeExecute_releaseOutput(outputTensor);

  return js_outputTensor;
}

void InitANTMLDFE(jerry_value_t nativeObj) {
  REGISTER_ANT_API(nativeObj, ant_ml, dfeLoad);
  REGISTER_ANT_API(nativeObj, ant_ml, dfeExecute);
}
