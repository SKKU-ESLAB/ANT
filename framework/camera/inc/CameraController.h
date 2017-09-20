#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "Camera.h"
#include "CameraDevice.h"

class CameraController {
  public:
    CameraController();
    ~CameraController();
    CameraDevice* getDeviceById(int camera_id);
    CameraDevice* createDevice(int camera_id, GstElement *main_bin,
        GMainLoop *loop);

  private:
    std::vector<CameraDevice*> *mDeviceVector;
};

#endif /* CAMERA_CONTROLLER_H */
