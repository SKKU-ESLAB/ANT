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

using namespace sc;

bool Device::hold_and_turn_on(void) {
  if(this->mRefCount.increase() == 1) {
    this->set_state(DeviceState::kTurningOn);

    bool res = this->turn_on_impl();

    if(!res) {
      this->mRefCount.decrease();
      this->set_state(DeviceState::kOff);
    } else {
      this->set_state(DeviceState::kOn);
    }
    return res;
  } else {
    // Already turn on
    return true;
  }
}

bool Device::release_and_turn_off(void) {
  if(this->mRefCount.decrease() == 0) {
    this->set_state(DeviceState::kTurningOff);

    bool res = this->turn_off_impl();

    if(!res) {
      LOG_DEBUG("%s: Failed to turn off", this->get_name());
      this->mRefCount.increase();
      this->set_state(DeviceState::kOn);
    } else {
      LOG_DEBUG("%s: Successfully turned off", this->get_name());
      this->set_state(DeviceState::kOff);
    }
    return res;
  } else {
    // Not yet turn off
    LOG_DEBUG("%s: Not yet turn off", this->get_name());
    return true;
  }
}
