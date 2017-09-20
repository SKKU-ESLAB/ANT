#include "CameraController.h"

CameraController::CameraController()
{
  this->mDeviceVector = new std::vector<CameraDevice*>;
}

CameraController::~CameraController()
{
  if (this->mDeviceVector != NULL)
    delete this->mDeviceVector;
}

CameraDevice* CameraController::getDeviceById(int camera_id)
{
  std::vector<CameraDevice*>::iterator iter, iter_end;
  iter = this->mDeviceVector->begin();
  iter_end = this->mDeviceVector->end();
  for (iter; iter != iter_end; iter++) {
    CameraDevice *device = (CameraDevice*)(*iter);
    if (device->getCameraId() == camera_id)
      return device;
  }

  ANT_LOG_WARN(CAM, "Cannot find device (id=%d)");
  return NULL;
}

CameraDevice* CameraController::createDevice(int camera_id,
    GstElement *main_bin, GMainLoop *loop)
{
  CameraDevice *device = new CameraDevice(camera_id, main_bin, loop);
  if (device == NULL) {
    ANT_LOG_ERR(CAM, "Camera device not created");
    return NULL;
  }

  this->mDeviceVector->push_back(device);
  return device;
}
