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

#ifndef __OCF_RESOURCE_H__
#define __OCF_RESOURCE_H__

#include <iotjs_def.h>
#include <iotjs_uv_request.h>
#include <modules/iotjs_module_buffer.h>

#include "./ocf_common.h"

void ocf_resource_init(void);

// OCFResource, OCFRequest
extern const jerry_object_native_info_t ocf_resource_native_info;
extern const jerry_object_native_info_t ocf_request_native_info;

void InitOCFResourceNative(jerry_value_t ocfNative);

#endif /* !defined(__OCF_RESOURCE_H__) */
