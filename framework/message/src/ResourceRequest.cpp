/* Copyright (c) 2017-2018 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
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

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ResourceRequest.h"
#include "ANTdbugLog.h"
#include "MessageUtil.h"
#include "cJSON.h"

cJSON *ResourceRequest::toJSON() {
  cJSON *thisObj = cJSON_CreateObject();

  // opType
  cJSON_AddNumberToObject(thisObj, RESOURCE_REQUEST_KEY_OP_TYPE,
                          (int)this->getOpType());

  // body
  cJSON_AddStringToObject(thisObj, RESOURCE_REQUEST_KEY_BODY,
                          this->getBody().c_str());

  return thisObj;
}