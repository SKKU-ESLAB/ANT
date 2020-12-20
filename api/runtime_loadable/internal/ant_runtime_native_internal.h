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

#ifndef __ANT_RUNTIME_NATIVE_INTERNAL_H__
#define __ANT_RUNTIME_NATIVE_INTERNAL_H__

#include <stdbool.h>

int ant_runtime_getPssInKB_internal();
void ant_runtime_getEnv_internal(const char *envKey, char *resultEnvValue);

bool ant_runtime_downloadFileViaHTTP_internal(const char *url,
                                              const char *local_path);
bool ant_runtime_unarchive_internal(const char *archiveFilePath,
                                    const char *local_path);

void initANTRuntime(void);

#endif /* !defined(__ANT_RUNTIME_NATIVE_INTERNAL_H__) */
