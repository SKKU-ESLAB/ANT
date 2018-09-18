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

#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <thread>
#include <mutex>

#include <stdio.h>

namespace sc {

typedef enum {
  kOff        = 0,
  kTurningOn  = 1,
  kOn         = 2,
  kTurningOff = 3
} DeviceState;

class ServerAdapter;
class Device {
public:
  bool turn_on(void);
  bool turn_off(void);

  virtual bool turn_on_impl(void) = 0;
  virtual bool turn_off_impl(void) = 0;

  DeviceState get_state(void) {
    return this->mState;
  }

  Device(const char* name) {
    this->mState = DeviceState::kOff;
    snprintf(this->mName, sizeof(this->mName), name);
  }

  ~Device() {
  }

protected:
  void set_state(DeviceState new_state) {
    this->mState = new_state;
  }
  
  char* get_name() {
    return this->mName;
  }

private:
  DeviceState mState;
  char mName[256];
}; /* class Device */
} /* namespace sc */

#endif /* !defined(__DEVICE_H__) */
