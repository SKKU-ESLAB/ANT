/* Copyright 2017-2018 All Rights Reserved.
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *
 * [Contact]
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *
 * Licensed under the Apache License, Version 2.0(the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __SC_API_H__
#define __SC_API_H__

#include "Core.h"
#include "ServerAdapter.h"

namespace sc {
typedef void (*StartCallback)(bool is_success);
void start_sc(StartCallback startCallback);

typedef void (*StopCallback)(bool is_success);
void stop_sc(StopCallback stopCallback);

inline void register_adapter(ServerAdapter *adapter) {
  Core::singleton()->register_adapter(adapter);
}

inline int send(const void *dataBuffer, uint32_t dataLength) {
  Core::singleton()->send(dataBuffer, dataLength, false);
}

inline int receive(void **dataBuffer) {
  Core::singleton()->receive(dataBuffer, false);
}

typedef enum {
  kNSModeEnergyAware = 0,  /* WearDrive-like */
  kNSModeLatencyAware = 1, /* Selective Connection Unique */
  kNSModeCapDynamic = 2,    /* CoolSpots */
  kNSModeBTOnly = 3, /* BT-only baseline */
  kNSModeWFDOnly = 4 /* WFD-only baseline */
} NSMode;

void set_switcher_mode(NSMode switcher_mode);
} /* namespace sc */

#endif /* !defined(__SC_API_H__) */