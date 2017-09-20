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
