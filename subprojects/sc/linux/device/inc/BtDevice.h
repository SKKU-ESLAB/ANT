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

#ifndef __BT_DEVICE_H__
#define __BT_DEVICE_H__

#include "../../core/inc/Device.h"

#include <mutex>
#include <thread>

#include <stdio.h>

namespace sc {
class BtDevice : public Device {
public:
  virtual bool turn_on_impl(void);
  virtual bool turn_off_impl(void);

  BtDevice() : Device("Bluetooth") {}
  ~BtDevice(void) {}

protected:
}; /* class BtDevice */
} /* namespace sc */

#endif /* !defined(__BT_DEVICE_H__) */
