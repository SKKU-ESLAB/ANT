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

#ifndef _BT_DEVICE_H_
#define _BT_DEVICE_H_

#include <Device.h>

#include <thread>
#include <mutex>

#include <stdio.h>

namespace cm {

class BtDevice : public Device {
public:
  virtual bool turn_on_impl(void);
  virtual bool turn_off_impl(void);

  static BtDevice* getSingleton() {
    if(BtDevice::sSingleton == NULL) {
      BtDevice::sSingleton = new BtDevice();
    }
    return BtDevice::sSingleton;
  }

  static BtDevice* sSingleton;

  ~BtDevice(void) {
  }

protected:
  BtDevice() : Device("Bluetooth") {
  }

}; /* class BtDevice */

} /* namespace cm */

#endif /* !defined(_BT_DEVICE_H_) */
