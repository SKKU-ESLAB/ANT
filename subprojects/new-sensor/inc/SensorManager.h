/* Copyright (c) 2017-2018 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
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

#ifndef __SENSOR_MANAGER_H__
#define __SENSOR_MANAGER_H__

#include "BaseMessage.h"
#include "DbusChannel.h"
#include "LocalChannel.h"
#include "MessageRouter.h"
#include "NativeResourceAPI.h"

#include <iostream>
#include <vector>

#define PATH_BUFFER_SIZE 1024

class LocalChannel;

class SensorManager {

public:
  static SensorManager* singleton() {
    if(SensorManager::sSingleton == NULL) {
      SensorManager::sSingleton = new SensorManager();
    }
    return SensorManager::sSingleton;
  }

  ~SensorManager() {
    delete this->mMessageRouter;
    delete this->mDbusChannel;
    delete this->mLocalChannel;
  }

  // Main loop
  void run();

  // Resource request callback
  static void onGetMainResource(BaseMessage *requestMessage,
                                BaseMessage *responseMessage);

  // TODO: hard-coded
  static void onGetAccSensorResource(BaseMessage *requestMessage,
                                     BaseMessage *responseMessage);

protected:
  SensorManager() {}
  static SensorManager* sSingleton;

  // Sensor Commands
  void onGetSensorList(ResourceResponse *response);
  void onGet(ResourceResponse *response, std::string sensorName);

  // Message framework
  MessageRouter *mMessageRouter = NULL;
  DbusChannel *mDbusChannel = NULL;
  LocalChannel *mLocalChannel = NULL;

  // Resource interface
  Resource *mMainResource = NULL;
  Resource *mSensorResource = NULL;
};

#endif // !defined(__SENSOR_MANAGER_H__)