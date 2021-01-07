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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iotjs_def.h>
#include <iotjs_uv_request.h>

#include "../../common/native/ant_common.h"
#include "./internal/coap_internal.h"

JS_FUNCTION(coap_response_end) { return jerry_create_undefined(); }
JS_FUNCTION(coap_response_get) { return jerry_create_undefined(); }

JS_FUNCTION(coap_request_onEvent) { return jerry_create_undefined(); }

jerry_value_t InitCoAPNative() {
  // Initialize sub-objects of CoAP API
  jerry_value_t coapNative = jerry_create_object();

  REGISTER_ANT_API(coapNative, coap_response, end);
  REGISTER_ANT_API(coapNative, coap_response, get);
  REGISTER_ANT_API(coapNative, coap_request, onEvent);
  return coapNative;
}
