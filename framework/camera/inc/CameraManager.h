/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Hayun Lee<lhy920806@gmail.com>
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

#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "Camera.h"
#include "CameraController.h"
#include "CameraCommunicator.h"
#include "CameraConfigParser.h"

class CameraManager {
  public:
    CameraManager();
    ~CameraManager();

    bool start(int argc, char **argv);

    CameraController* getController(void);
    CameraCommunicator* getCommunicator(void);
    CameraConfigParser* getConfigParser(void);
    
    void setTotalCameraNum(int total_camera_num);
    int getTotalCameraNum(void);

    GMainLoop* getGMainLoop()
    { return this->mGMainLoop; }

  private:
    CameraController* mController;
    CameraCommunicator *mCommunicator;
    CameraConfigParser *mConfigParser;

    int mTotalCameraNum;
    GMainLoop *mGMainLoop;
};

DBusHandlerResult msg_dbus_filter(DBusConnection *connection,
    DBusMessage *msg, void *camera_manager);

#endif /* CAMERA_MANAGER_H */
