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

#include <ServerAdapter.h>

using namespace cm;

bool Device::turn_on(void) {
  this->set_state(DeviceState::kTurningOn);

  bool res = this->turn_on_impl();

  if(!res) {
    this->set_state(DeviceState::kOff);
  } else {
    this->set_state(DeviceState::kOn);
  }
  return res;
}

bool Device::turn_off(void) {
  this->set_state(DeviceState::kTurningOff);

  bool res = this->turn_off_impl();

  if(!res) {
    this->set_state(DeviceState::kOn);
  } else {
    this->set_state(DeviceState::kOff);
  }
  return res;
}
