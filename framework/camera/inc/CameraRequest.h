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

#ifndef CAMERA_REQUEST_H
#define CAMERA_REQUEST_H

#include "Camera.h"

class CameraDevice;

typedef enum _RequestType {
  kRecordingStart,
  kRecordingStop,
  kSnapshot,
  kStreamingStart,
  kStreamingStop,
  kPreRecordingInit,
  kPreRecordingStart,
  kPreRecordingStop,
  kOpenCVStart,
  kOpenCVStop,
  kSensorOverlayStart,
  kSensorOverlayStop,
} RequestType;

class CameraRequest {
  public:
    CameraRequest();
    CameraRequest(RequestType type);
    CameraRequest(RequestType type, GstElement *bin);
    ~CameraRequest();
    RequestType getRequestType()
    { return this->mRequestType; }

    void setCameraDevice(CameraDevice *device)
    { this->mCameraDevice = device; }
    CameraDevice* getCameraDevice()
    { return this->mCameraDevice; }

    void setMsgHandle(void *msg_handle)
    { this->mMsgHandle = msg_handle; }
    void* getMsgHandle()
    { return this->mMsgHandle; }
    GstElement* getBin()
    { return this->mBin; }
    
    void setTee(GstElement *tee)
    { this->mTee = tee; }
    void setTeePad(GstPad *tee_pad)
    { this->mTeePad = tee_pad; }
    void setSinkPad(GstPad *sink_pad)
    { this->mSinkPad = sink_pad; }
    void setLastPad(GstPad *last_pad)
    { this->mLastPad = last_pad; }
    GstElement* getTee()
    { return this->mTee; }
    GstPad* getTeePad()
    { return this->mTeePad; }
    GstPad* getSinkPad()
    { return this->mSinkPad; }
    GstPad* getLastPad()
    { return this->mLastPad; }

    void requestTeePad();
    void linkPads();
    unsigned getSeconds();
    GstElement* getLastElement()
    { return this->mLastElement; }
    GstPad* getTeeGhostPad()
    { return this->mTeeGhostPad; }

    GstElement* getElementByName(const char *name);

  private:
    CameraDevice *mCameraDevice;
    std::vector<GstElement*> *mElementVector;
    GstElement *mTee;
    GstElement *mLastElement;
    GstPad *mTeePad;
    GstPad *mTeeGhostPad;
    GstPad *mSinkPad;
    GstPad *mLastPad;
    RequestType mRequestType;
    void *mMsgHandle;
    GstElement *mBin;
};

typedef struct _dbusRequest {
  unsigned pid;
  unsigned camera_id;
  unsigned play_seconds;
  const char *file_path;
  unsigned width;
  unsigned height;
  unsigned fps;
} dbusRequest;

typedef struct _dbusStreamingRequest {
  unsigned pid;
  unsigned camera_id;
  const char *ip_address;
  unsigned port;
} dbusStreamingRequest;

typedef struct _dbusSensorRequest {
  unsigned pid;
  unsigned camera_id;
  const char *sensor_name;
} dbusSensorRequest;

#endif /* CAMERA_REQUEST_H */
