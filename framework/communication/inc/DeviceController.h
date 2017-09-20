/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
 *              Eunsoo Park<esevan.park@gmail.com>
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

#ifndef __DEVICE_CONTROLLER_H__
#define __DEVICE_CONTROLLER_H__

#include <vector>

class DeviceController {
  public:
    DeviceController()
      : mIsTurnedOn(false) { }

    virtual bool turnOn() {
      this->mIsTurnedOn = true;
      return 0;
    }

    virtual bool turnOff() {
      this->mIsTurnedOn = false;
      return 0;
    }

    bool isTurnedOn() { return this->mIsTurnedOn; }

  protected:
    bool mIsTurnedOn;
};

class BluetoothDeviceController : DeviceController {
  public:
    virtual bool turnOn();
    virtual bool turnOff();
  protected:
};

class WifiDirectDeviceController : DeviceController {
  public:
    virtual bool turnOn();
    virtual bool turnOff();
  protected:
};

#endif /* !defined(__DEVICE_CONTROLLER_H__) */
