/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
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

#ifndef __MESSAGE_UTIL_H__
#define __MESSAGE_UTIL_H__

#include "ANTdbugLog.h"
#include "cJSON.h"

#define RETURN_IF_NULL(a, ret) \
  if((a) == NULL) { \
    ANT_DBG_ERR("JSON handling error: null pointer"); \
    return ret; \
  }

#define RETURN_IF_INVALID_CJSON_OBJ(a, ret) \
  if((a) == NULL) { \
    ANT_DBG_ERR("JSON handling error: %s", cJSON_GetErrorPtr()); \
    return ret; \
  }

#define EXT4_FILE_PATH_LENGTH 4097

#endif // !defined(__MESSAGE_UTIL_H__)
