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

#ifndef __ANT_COMPANION_NATIVE_INTERNAL_H__
#define __ANT_COMPANION_NATIVE_INTERNAL_H__

#include <sys/types.h>

void ant_companion_getMyIPAddress_internal(const char *interfaceName,
                                           char *resultIPAddress);

void initANTCompanion(void);

#endif /* !defined(__ANT_COMPANION_NATIVE_INTERNAL_H__) */
