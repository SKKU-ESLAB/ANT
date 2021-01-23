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
#include "./ant_ml_dfe.h"
#include "./internal/ant_ml_dfe_internal.h"

JS_FUNCTION(ant_ml_dfeLoad) {
  iotjs_string_t argModelName;
  int argNumFragments;
  DJS_CHECK_ARGS(2, string, number);
  argModelName = JS_GET_ARG(0, string);
  argNumFragments = (int)JS_GET_ARG(1, number);

  // TODO: call internal function
  ant_ml_dfeLoad_internal();

  iotjs_string_destroy(&argModelName);

  return jerry_create_undefined();
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

  // TODO: call internal function
  ant_ml_dfeExecute_internal();

  return jerry_create_undefined();
}

void InitANTMLDFE(jerry_value_t nativeObj) {
  REGISTER_ANT_API(nativeObj, ant_ml, dfeLoad);
  REGISTER_ANT_API(nativeObj, ant_ml, dfeExecute);

  initANTMLDFEInternal();

  return nativeObj;
}
