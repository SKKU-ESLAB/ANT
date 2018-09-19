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

#include "../inc/Device.h"

#include "../inc/ServerAdapter.h"

using namespace sc;

bool Device::turn_on(void) {
  // Check previous state
  DeviceState state = this->get_state();
  if (state == DeviceState::kOn || state == DeviceState::kTurningOn) {
    LOG_ERR("%s: Failed to turn on - Already turned on (%d)", this->get_name(),
            state);
    return false;
  }

  // State change
  this->set_state(DeviceState::kTurningOn);

  // Turn on the device
  bool res = this->turn_on_impl();

  // Result check
  if (!res) {
    LOG_DEBUG("%s: Failed to turn on", this->get_name());
    this->set_state(DeviceState::kOff);
  } else {
    LOG_DEBUG("%s: Successfully turned on", this->get_name());
    this->set_state(DeviceState::kOn);
  }

  return res;
}

bool Device::turn_off(void) {
  // Check previous state
  DeviceState state = this->get_state();
  if (state == DeviceState::kOff || state == DeviceState::kTurningOff) {
    LOG_ERR("%s: Failed to turn off - Already turned off (%d)",
            this->get_name(), state);
    return false;
  }

  // State change
  this->set_state(DeviceState::kTurningOff);

  // Turn off the device
  bool res = this->turn_off_impl();
  
  // Result check
  if (!res) {
    LOG_DEBUG("%s: Failed to turn off", this->get_name());
    this->set_state(DeviceState::kOn);
  } else {
    LOG_DEBUG("%s: Successfully turned off", this->get_name());
    this->set_state(DeviceState::kOff);
  }

  return res;
}