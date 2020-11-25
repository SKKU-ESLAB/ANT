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

#include "../../common/native/ant_common.h"
#include "internal/ant_runtime_native_internal.h"

#include "iotjs_def.h"
#include "iotjs_uv_request.h"
#include "modules/iotjs_module_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ANT_API_VOID_TO_NUMBER(ant_runtime, getPssInKB);

jerry_value_t InitANTRuntimeNative() {
  jerry_value_t antRuntimeNative = jerry_create_object();
  REGISTER_ANT_API(antRuntimeNative, ant_runtime, getPssInKB);

  initANTRuntime();

  return antRuntimeNative;
}