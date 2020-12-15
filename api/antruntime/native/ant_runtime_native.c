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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iotjs_def.h>
#include <iotjs_uv_request.h>
#include <modules/iotjs_module_buffer.h>

#include "../../common/native/ant_common.h"
#include "./internal/ant_runtime_native_internal.h"

ANT_API_VOID_TO_NUMBER(ant_runtime, getPssInKB);

ANT_API_STRING_TO_STRING(ant_runtime, getEnv);

JS_FUNCTION(ant_runtime_downloadFileViaHTTP) {
  iotjs_string_t argFileUrl, argLocalPath;
  DJS_CHECK_ARGS(2, string, string);
  argFileUrl = JS_GET_ARG(0, string);
  argLocalPath = JS_GET_ARG(1, string);

  bool res = ant_runtime_downloadFileViaHTTP_internal(
      iotjs_string_data(&argFileUrl), iotjs_string_data(&argLocalPath));
  iotjs_string_destroy(&argFileUrl);
  iotjs_string_destroy(&argLocalPath);
  return jerry_create_boolean(res);
}

JS_FUNCTION(ant_runtime_unarchive) {
  iotjs_string_t argArchiveFilePath, argTargetDirPath;
  DJS_CHECK_ARGS(2, string, string);
  argArchiveFilePath = JS_GET_ARG(0, string);
  argTargetDirPath = JS_GET_ARG(1, string);

  bool res = ant_runtime_unarchive_internal(
      iotjs_string_data(&argArchiveFilePath), iotjs_string_data(&argTargetDirPath));
  iotjs_string_destroy(&argArchiveFilePath);
  iotjs_string_destroy(&argTargetDirPath);
  return jerry_create_boolean(res);
}

jerry_value_t InitANTRuntimeNative() {
  jerry_value_t antRuntimeNative = jerry_create_object();
  REGISTER_ANT_API(antRuntimeNative, ant_runtime, getPssInKB);
  REGISTER_ANT_API(antRuntimeNative, ant_runtime, getEnv);
  REGISTER_ANT_API(antRuntimeNative, ant_runtime, downloadFileViaHTTP);
  REGISTER_ANT_API(antRuntimeNative, ant_runtime, unarchive);

  initANTRuntime();

  return antRuntimeNative;
}
