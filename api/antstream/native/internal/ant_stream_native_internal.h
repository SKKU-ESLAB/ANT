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

#ifndef __ANT_STREAM_NATIVE_INTERNAL_H__
#define __ANT_STREAM_NATIVE_INTERNAL_H__

#include <stdbool.h>

bool ant_stream_testPipeline_internal(const char *ipAddress);

void ant_stream_callDbusMethod_internal(const char *inputMessage, char *resultMessage);
void ant_stream_initializeStream_internal();

typedef void (*ant_async_handler)(const char *, unsigned char *, unsigned long);
bool ant_stream_elementConnectSignal_internal(int element_index,
                                              const char *detailed_signal,
                                              ant_async_handler handler);

void initANTStream(void);

#endif /* !defined(__ANT_STREAM_NATIVE_INTERNAL_H__) */