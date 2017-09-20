#ifndef CAMERA_DEVICE_H
#define CAMERA_DEVICE_H

#include "Camera.h"
#include "CameraRequest.h"
#include <gst/app/gstappsink.h>

#define RAW_TEE_NAME   "RAW_TEE"
#define MAIN_TEE_NAME  "MAIN_TEE"
#define H264_TEE_NAME  "H264_TEE"
#define TEXT_ELEMENT_NAME  "TEXT"
#define CLOCK_ELEMENT_NAME "CLOCK"

class CameraDevice {
  public:
    CameraDevice(int camera_id, GstElement *main_bin, GMainLoop *loop);
    ~CameraDevice();
    bool processRequest(CameraRequest *request);
    bool recordingStart(CameraRequest *request);
    bool recordingStop(CameraRequest *request);
    bool snapshotStart(CameraRequest *request);
    bool streamingStart(CameraRequest *request);
    bool streamingStop(CameraRequest *request);
    bool preRecordingInit(CameraRequest *request);
    bool preRecordingStart(CameraRequest *request);
    bool preRecordingStop(CameraRequest *request);
    bool openCVStart(CameraRequest *request);
    bool openCVStop(CameraRequest *request);
    bool sensorOverlayStart(CameraRequest *request);
    bool sensorOverlayStop(CameraRequest *request);

    GstElement* getPipeline()
    { return this->mPipeline; }
    int getCameraId();

    void pushRequest(CameraRequest *request);
    bool deleteRequest(CameraRequest *request);
    bool deleteRequest(dbusRequest *msg_handle, RequestType type);
    bool deleteRequest(dbusStreamingRequest *msg_handle, RequestType type);

    GstElement* getH264Tee()
    { return this->mH264Tee; }
    GstElement* getDelayedValve()
    { return this->mDelayedValve; }
    GstElement* getOpenCVValve()
    { return this->mOpenCVValve; }

    unsigned getOpenCVNumUsers()
    { return this->opencv_num_users; }
    void increaseOpenCVNumUsers()
    { this->opencv_num_users++; }
    void decreaseOpenCVNumUsers()
    { this->opencv_num_users--; }

  private:
    int mCameraId;
    int mPreRecordingTime;
    GstElement *mPipeline;
    GstElement *mMainBin;
    GstElement *mRawTee;
    GstElement *mMainTee;
    GstElement *mH264Tee;
    GstElement *mDelayedTee;
    GstElement *mTextOverlay;
    GstElement *mClockOverlay;
    GstElement *mDelayedValve;
    GstElement *mOpenCVValve;
    guint mBusWatchId;
    std::list<CameraRequest*> *mRequestList;
    int mRequestCount;

    unsigned opencv_num_users;
};

static gboolean busCB(GstBus *bus, GstMessage *message, gpointer user_data);
static gboolean timeOutCB(gpointer _request);
static GstPadProbeReturn padProbeCB(GstPad *pad, GstPadProbeInfo *info,
    gpointer _request);
static GstPadProbeReturn eventProbeCB(GstPad *pad, GstPadProbeInfo *info,
    gpointer _request);

static unsigned fileWrite(const char *file_path, char *buffer, unsigned size);
static GstFlowReturn jpegBufferFromSinkCB(GstElement *ele, gpointer _request);

static GstFlowReturn openCVBufferFromSinkCB(GstElement *ele, gpointer _request);

#endif /* CAMERA_DEVICE_H */
