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
