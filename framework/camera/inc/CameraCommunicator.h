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

#ifndef CAMERA_COMMUNICATOR_H
#define CAMERA_COMMUNICATOR_H

#include "Camera.h"
#include "CameraController.h"
#include "CameraConfigParser.h"

#include "ANTRawRequest.h"

class CameraCommunicator {
  public:
    CameraCommunicator();
    ~CameraCommunicator();

    bool initCommunication(void *cam,
        DBusHandlerResult (*filter)(DBusConnection*, DBusMessage*, void*));

    DBusHandlerResult recordingStart(DBusMessage *msg,
        CameraController *controller, CameraConfigParser *config_parser);
    DBusHandlerResult recordingStop(DBusMessage *msg,
        CameraController *controller, CameraConfigParser *config_parser);
    DBusHandlerResult snapshotStart(DBusMessage *msg,
        CameraController *controller, CameraConfigParser *config_parser);
    DBusHandlerResult streamingStart(DBusMessage *msg,
        CameraController *controller, CameraConfigParser *config_parser);
    DBusHandlerResult streamingStop(DBusMessage *msg,
        CameraController *controller, CameraConfigParser *config_parser);
    DBusHandlerResult preRecordingInit(DBusMessage *msg,
        CameraController *controller, CameraConfigParser *config_parser);
    DBusHandlerResult preRecordingStart(DBusMessage *msg,
        CameraController *controller, CameraConfigParser *config_parser);
    DBusHandlerResult preRecordingStop(DBusMessage *msg,
        CameraController *controller, CameraConfigParser *config_parser);
    DBusHandlerResult openCVStart(DBusMessage *msg,
        CameraController *controller, CameraConfigParser *config_parser);
    DBusHandlerResult openCVStop(DBusMessage *msg,
        CameraController *controller, CameraConfigParser *config_parser);
    DBusHandlerResult sensorOverlayStart(DBusMessage *msg,
        CameraController *controller, CameraConfigParser *config_parser);
    DBusHandlerResult sensorOverlayStop(DBusMessage *msg,
        CameraController *controller, CameraConfigParser *config_parser);

  private:
    DBusConnection *mDBusConnection;
    DBusError mDBusError;
};

#endif /* CAMERA_COMMUNICATOR_H */
