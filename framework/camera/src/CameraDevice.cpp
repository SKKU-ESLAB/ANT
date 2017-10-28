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

#include "CameraDevice.h"

// TODO: plan to remove OpenCV decoupling
#include "ANTRawRequest.h"

CameraDevice::CameraDevice(int camera_id, GstElement *main_bin, GMainLoop *loop)
{
  this->mCameraId = camera_id;
  this->mPreRecordingTime = 10;
  this->mRequestList = new std::list<CameraRequest*>;

  this->mDelayedTee = NULL;

  this->mPipeline = gst_pipeline_new(NULL);
  this->mMainBin = main_bin;

  this->opencv_num_users = 0;
  
  GstIterator *iter = gst_bin_iterate_elements(GST_BIN(this->mMainBin));
  GValue item = G_VALUE_INIT;
  bool done = false;
  GstElement *tmp;
  char *name;
  while (!done) {
    switch (gst_iterator_next(iter, &item)) {
      case GST_ITERATOR_OK:
        tmp = (GstElement*)g_value_get_object(&item);
        name = GST_OBJECT_NAME(tmp);
        ANT_LOG_DBG(CAM, name);

        if (!strncmp(name, RAW_TEE_NAME, strlen(RAW_TEE_NAME))) {
          ANT_LOG_DBG(CAM, "Main %d bin has raw tee", camera_id);
          this->mRawTee = tmp;
        }
        else if (!strncmp(name, MAIN_TEE_NAME, strlen(MAIN_TEE_NAME))) {
          ANT_LOG_DBG(CAM, "Main %d bin has main tee", camera_id);
          this->mMainTee = tmp;
        }
        else if (!strncmp(name, H264_TEE_NAME, strlen(H264_TEE_NAME))) {
          ANT_LOG_DBG(CAM, "Main %d bin has h264 tee", camera_id);
          this->mH264Tee = tmp;
        }
        else if (!strncmp(name, TEXT_ELEMENT_NAME, strlen(TEXT_ELEMENT_NAME))) {
          ANT_LOG_DBG(CAM, "Main %d bin has text element", camera_id);
          this->mTextOverlay = tmp;
        }
        else if (!strncmp(name, CLOCK_ELEMENT_NAME, strlen(CLOCK_ELEMENT_NAME))) {
          ANT_LOG_DBG(CAM, "Main %d bin has clock element", camera_id);
          this->mClockOverlay = tmp;
        }

        g_value_reset(&item);
        break;
      case GST_ITERATOR_RESYNC:
        ANT_LOG_WARN(CAM, "Datastructure changed while iterating");
        gst_iterator_resync(iter);
        break;
      case GST_ITERATOR_ERROR:
        ANT_LOG_ERR(CAM, "Gstreamer iterator failed");
        done = TRUE;
        break;
      case GST_ITERATOR_DONE:
        done = TRUE;
        break;
    }
  }
  g_value_unset(&item);
  gst_iterator_free(iter);

  gst_bin_add(GST_BIN(this->mPipeline), this->mMainBin);

  GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(this->mPipeline));
  this->mBusWatchId = gst_bus_add_watch(bus, busCB, loop);
  if (bus != NULL)
    gst_object_unref(bus);

  this->mRequestCount = 0;
}

CameraDevice::~CameraDevice()
{
  if (this->mRequestList != NULL)
    delete this->mRequestList;
  g_source_remove(this->mBusWatchId);
  if (this->mPipeline != NULL)
    gst_object_unref(GST_OBJECT(this->mPipeline));
}

bool CameraDevice::processRequest(CameraRequest *request)
{
  RequestType type = request->getRequestType();

  switch (type) {
    case kRecordingStart:
      return this->recordingStart(request);
    case kRecordingStop:
      return this->recordingStop(request);
    case kSnapshot:
      return this->snapshotStart(request);
    case kStreamingStart:
      return this->streamingStart(request);
    case kStreamingStop:
      return this->streamingStop(request);
    case kPreRecordingInit:
      return this->preRecordingInit(request);
    case kPreRecordingStart:
      return this->preRecordingStart(request);
    case kPreRecordingStop:
      return this->preRecordingStop(request);
    case kOpenCVStart:
      return this->openCVStart(request);
    case kOpenCVStop:
      return this->openCVStop(request);
    case kSensorOverlayStart:
      return this->sensorOverlayStart(request);
    case kSensorOverlayStop:
      return this->sensorOverlayStop(request);
    default:
      ANT_LOG_ERR(CAM, "Invalid request type (%d)", type);
      break;
  }

  return false;
}

bool CameraDevice::recordingStart(CameraRequest *request)
{
  GstElement *recording_bin = request->getBin();
  gst_bin_add(GST_BIN(this->mPipeline), recording_bin);

  if (this->mH264Tee == NULL) {
    ANT_LOG_ERR(CAM, "Main bin have to include Tee name: %s", H264_TEE_NAME);
    return false;
  }

  dbusRequest *msg_handle = (dbusRequest *)request->getMsgHandle();
  const char* file_path = msg_handle->file_path;
  g_object_set(G_OBJECT(request->getLastElement()), "location", file_path, NULL);
      
  request->setTee(this->mH264Tee);
  request->requestTeePad();
  request->linkPads();
  
  this->pushRequest(request);

  g_timeout_add_seconds(request->getSeconds(), timeOutCB, (void*)request);

  return true;
}

bool CameraDevice::recordingStop(CameraRequest *request)
{
  return true;
}

bool CameraDevice::snapshotStart(CameraRequest *request)
{
  GstElement *snapshot_bin = request->getBin();
  gst_bin_add(GST_BIN(this->mPipeline), snapshot_bin);

  request->setTee(this->mMainTee);
  request->requestTeePad();
  request->linkPads();

  g_object_set(G_OBJECT(request->getLastElement()), "emit-signals", true,
      "drop", true, "max-buffers", 1, "wait-on-eos", true, NULL);

  // TODO: time set
  g_signal_connect(request->getLastElement(), "new-sample",
      G_CALLBACK(jpegBufferFromSinkCB), (gpointer)request);

  this->pushRequest(request);

  return true;
}

bool CameraDevice::streamingStart(CameraRequest *request)
{
  GstElement *streaming_bin = request->getBin();
  gst_bin_add(GST_BIN(this->mPipeline), streaming_bin);

  dbusStreamingRequest *msg_handle = (dbusStreamingRequest *)request->getMsgHandle();
  const char* ip_address = msg_handle->ip_address;
  unsigned port = msg_handle->port;
  g_object_set(G_OBJECT(request->getLastElement()), "host", ip_address,
      "port", port, NULL);

  request->setTee(this->mH264Tee);
  request->requestTeePad();
  request->linkPads();
  
  this->pushRequest(request);

  return true;
}

bool CameraDevice::streamingStop(CameraRequest *request)
{
  dbusStreamingRequest *msg_handle = (dbusStreamingRequest *)request->getMsgHandle();
  deleteRequest(msg_handle, kStreamingStop);
  return true;
}

bool CameraDevice::preRecordingInit(CameraRequest *request)
{
  GstElement *pre_recording_init_bin = request->getBin();
  gst_bin_add(GST_BIN(this->mPipeline), pre_recording_init_bin);

  if (this->mDelayedTee == NULL)
    this->mDelayedTee = request->getLastElement();
  else {
    ANT_LOG_ERR(CAM, "Pre-recording initialization already performed");
    return false;
  }
  
  this->mDelayedValve = request->getElementByName("valve");
  if (this->mDelayedValve == NULL) {
    ANT_LOG_ERR(CAM, "Pre-recording requires a 'valve' element");
    return false;
  }
  g_object_set(G_OBJECT(this->mDelayedValve), "drop", true, NULL);

  if (this->mH264Tee == NULL) {
    ANT_LOG_ERR(CAM, "Main bin have to include Tee name: %s", H264_TEE_NAME);
    return false;
  }
  request->setTee(this->mH264Tee);
  request->requestTeePad();
  request->linkPads();
  
  this->pushRequest(request);
  
  return true;
}

bool CameraDevice::preRecordingStart(CameraRequest *request)
{
  GstElement *pre_recording_bin = request->getBin();
  gst_bin_add(GST_BIN(this->mPipeline), pre_recording_bin);

  if (this->mDelayedTee == NULL) {
    ANT_LOG_ERR(CAM, "Initialization of pre-recording should be performed first.");
    return false;
  }

  dbusRequest *msg_handle = (dbusRequest *)request->getMsgHandle();
  const char* file_path = msg_handle->file_path;
  g_object_set(G_OBJECT(request->getLastElement()), "location", file_path, NULL);

  g_object_set(G_OBJECT(this->mDelayedValve), "drop", false, NULL);

  request->setTee(this->mDelayedTee);
  request->requestTeePad();
  request->linkPads();
  
  this->pushRequest(request);

  g_timeout_add_seconds(request->getSeconds(), timeOutCB, (void*)request);

  return true;
}

bool CameraDevice::preRecordingStop(CameraRequest *request)
{
  return true;
}

bool CameraDevice::openCVStart(CameraRequest *request)
{
  ANTRawRequest *raw_request = ANTRawRequest::getInstance(kOpenCVStart, NULL);

  GstElement *opencv_bin = request->getBin();
  gst_bin_add(GST_BIN(this->mPipeline), opencv_bin);

  request->setTee(this->mRawTee);
  request->requestTeePad();
  request->linkPads();

  g_object_set(G_OBJECT(raw_request->getLastElement()), "emit-signals", true,
      "drop", true, "max-buffers", 1, "wait-on-eos", true, NULL);

  raw_request->initializeSemAndSharedMemory();

  // TODO: time set
  g_signal_connect(request->getLastElement(), "new-sample",
      G_CALLBACK(openCVBufferFromSinkCB), (gpointer)request);

  if (this->mRequestCount == 0) {
    ANT_LOG_DBG(CAM, "Get pipeline not started");
    gst_element_set_state(this->mPipeline, GST_STATE_READY);
    gst_element_set_state(this->mPipeline, GST_STATE_PLAYING);
  }
  else {
    ANT_LOG_DBG(CAM, "Get pipeline already started");
    gst_element_sync_state_with_parent(request->getBin());
  }
  this->mRequestCount++;

  return true;
}

bool CameraDevice::openCVStop(CameraRequest *request)
{
  GstPad *sink_pad;

  ANTRawRequest *raw_request = ANTRawRequest::getInstance(kOpenCVStart, NULL);
  this->decreaseOpenCVNumUsers();
  if (this->getOpenCVNumUsers() > 0) {
    ANT_LOG_DBG(CAM, "OpenCV Bin is still in use");
    delete request;
    return true;
  }
  ANT_LOG_DBG(CAM, "OpenCV Bin is no longer used");
  
  timeOutCB(raw_request);

  raw_request->uninitializeSemAndSharedMemory();

  this->mRequestCount--;
  if (this->mRequestCount == 0) {
    ANT_LOG_DBG(CAM, "No request in pipeline");
    gst_element_set_state(this->mPipeline, GST_STATE_READY);
  }

  delete request;

  return true;
}

bool CameraDevice::sensorOverlayStart(CameraRequest *request)
{
  return true;
}

bool CameraDevice::sensorOverlayStop(CameraRequest *request)
{
  return true;
}

int CameraDevice::getCameraId()
{
  return this->mCameraId;
}

void CameraDevice::pushRequest(CameraRequest *request)
{
  this->mRequestList->push_back(request);

  if (this->mRequestCount == 0) {
    ANT_LOG_DBG(CAM, "Get pipeline not started");
    gst_element_set_state(this->mPipeline, GST_STATE_READY);
    gst_element_set_state(this->mPipeline, GST_STATE_PLAYING);
  }
  else {
    ANT_LOG_DBG(CAM, "Get pipeline already started");
    gst_element_sync_state_with_parent(request->getBin());
  }
  this->mRequestCount++;
}

bool CameraDevice::deleteRequest(CameraRequest *request)
{
  ANT_LOG_ERR(CAM, "deleteRequest Start");
  std::list<CameraRequest*>::iterator iter, iter_end;
  iter = this->mRequestList->begin();
  iter_end = this->mRequestList->end();
  for (iter; iter != iter_end; iter++) {
    CameraRequest *req = (CameraRequest*)(*iter);
    if (request == req) {
      this->mRequestList->erase(iter);
      delete req;

      this->mRequestCount--;
      ANT_LOG_DBG(CAM, "Current number of requests: %d", this->mRequestCount);
      if (this->mRequestCount == 0) {
        ANT_LOG_DBG(CAM, "No request in pipeline");
        gst_element_set_state(this->mPipeline, GST_STATE_READY);
      }
      return true;
    }
  }

  ANT_LOG_WARN(CAM, "Cannot find request");
  return false;
}

bool CameraDevice::deleteRequest(dbusRequest *msg_handle, RequestType type)
{
  RequestType target_type;
  if (type == kRecordingStop)
    target_type = kRecordingStart;
  else if (type == kStreamingStop)
    target_type = kStreamingStop;
  else if (type == kPreRecordingStop)
    target_type = kPreRecordingStart;
  else if (type == kOpenCVStop)
    target_type = kOpenCVStart;
  else {
    ANT_LOG_WARN(CAM, "Given request type is inappropriate (%d)", type);
    return false;
  }

  std::list<CameraRequest*>::iterator iter, iter_end;
  iter = this->mRequestList->begin();
  iter_end = this->mRequestList->end();
  for (iter; iter != iter_end; iter++) {
    CameraRequest *req = (CameraRequest*)(*iter);
    if (req->getRequestType() == target_type) {
      dbusRequest *tmp = (dbusRequest*)req->getMsgHandle();
      if (msg_handle->pid == tmp->pid && msg_handle->camera_id == tmp->camera_id &&
          !strncmp(msg_handle->file_path, tmp->file_path, strlen(tmp->file_path))) {
        ANT_LOG_DBG(CAM, "PID:%u, Camera ID:%u, File Path:%s", tmp->pid, tmp->camera_id, tmp->file_path);
        this->mRequestList->erase(iter);
        delete req;

        this->mRequestCount--;
        if (this->mRequestCount == 0) {
          ANT_LOG_DBG(CAM, "No request in pipeline");
          gst_element_set_state(this->mPipeline, GST_STATE_READY);
        }
        return true;
      }
    }
  }

  ANT_LOG_WARN(CAM, "Cannot find request");
  return false;
}

bool CameraDevice::deleteRequest(dbusStreamingRequest *msg_handle, RequestType type)
{
  RequestType target_type;
  if (type == kStreamingStop)
    target_type = kStreamingStart;
  else {
    ANT_LOG_WARN(CAM, "Given request type is inappropriate (%d)", type);
    return false;
  }

  std::list<CameraRequest*>::iterator iter, iter_end;
  iter = this->mRequestList->begin();
  iter_end = this->mRequestList->end();
  for (iter; iter != iter_end; iter++) {
    CameraRequest *req = (CameraRequest*)(*iter);
    if (req->getRequestType() == target_type) {
      dbusStreamingRequest *tmp = (dbusStreamingRequest*)req->getMsgHandle();
      if (msg_handle->pid == tmp->pid && msg_handle->camera_id == tmp->camera_id &&
          !strncmp(msg_handle->ip_address, tmp->ip_address, strlen(tmp->ip_address))) {
        ANT_LOG_DBG(CAM, "PID:%u, Camera ID:%u, IP Addr:%s", tmp->pid, tmp->camera_id, tmp->ip_address);
        this->mRequestList->erase(iter);
        delete req;

        this->mRequestCount--;
        if (this->mRequestCount == 0) {
          ANT_LOG_DBG(CAM, "No request in pipeline");
          gst_element_set_state(this->mPipeline, GST_STATE_READY);
        }
        return true;
      }
    }
  }

  ANT_LOG_WARN(CAM, "Cannot find request");
  return false;
}

static gboolean busCB(GstBus *bus, GstMessage *message, gpointer user_data)
{
  GError *err;
  gchar *debug_info;

  switch (GST_MESSAGE_TYPE(message)) {
    case GST_MESSAGE_ERROR:
      gst_message_parse_error(message, &err, &debug_info);
      ANT_LOG_ERR(CAM, "Error received from element %s: %s",
          GST_OBJECT_NAME(message->src), err->message);
      ANT_LOG_ERR(CAM, "Debugging information: %s",
          ((debug_info) ? debug_info : "none"));
      g_clear_error(&err);
      g_free(debug_info);
      break;
    case GST_MESSAGE_WARNING:
      break;
    case GST_MESSAGE_EOS:
      ANT_LOG_DBG(CAM, "End-Of-Stream reached");
      break;
  }
  return true;
}

static gboolean timeOutCB(gpointer _request)
{
  ANT_LOG_DBG(CAM, "Time out callback invoked");

  CameraRequest *request = (CameraRequest*)_request;
  GstPad *tee_ghost_pad = request->getTeeGhostPad();
  GstPad *sink_pad = request->getSinkPad();

  if (request->getRequestType() == kPreRecordingStart)
    g_object_set(G_OBJECT(request->getCameraDevice()->getDelayedValve()), "drop", true, NULL);

  gst_pad_add_probe(tee_ghost_pad, GST_PAD_PROBE_TYPE_BLOCK_DOWNSTREAM,
      eventProbeCB, _request, NULL);

  gst_pad_unlink(tee_ghost_pad, sink_pad);
  gst_pad_send_event(sink_pad, gst_event_new_eos());
  gst_object_unref(sink_pad);

  return false;
}

static GstPadProbeReturn padProbeCB(GstPad *pad, GstPadProbeInfo *info,
    gpointer _request)
{
  ANT_LOG_DBG(CAM, "Pad probe callback invoked");

  CameraRequest *request = (CameraRequest*)_request;
  GstPad *sink_pad = request->getSinkPad();
  GstPad *last_pad = request->getLastPad();

  gst_pad_remove_probe(pad, GST_PAD_PROBE_INFO_ID(info));

  gst_pad_add_probe(last_pad, (GstPadProbeType) (GST_PAD_PROBE_TYPE_BLOCK |
      GST_PAD_PROBE_TYPE_EVENT_DOWNSTREAM), eventProbeCB, _request, NULL);

  gst_pad_send_event(sink_pad, gst_event_new_eos());

  return GST_PAD_PROBE_OK;
}

static GstPadProbeReturn eventProbeCB(GstPad *pad, GstPadProbeInfo *info,
    gpointer _request)
{
  if (GST_EVENT_TYPE(GST_PAD_PROBE_INFO_DATA(info)) != GST_EVENT_EOS)
    return GST_PAD_PROBE_PASS;

  ANT_LOG_DBG(CAM, "EOS reached to last pad");

  CameraRequest *request = (CameraRequest*)_request;
  CameraDevice *device = request->getCameraDevice();

  GstElement *pipeline = device->getPipeline();
  GstElement *request_bin = request->getBin();
  gst_element_set_state(request_bin, GST_STATE_NULL);
  gst_bin_remove(GST_BIN(pipeline), request_bin);

  gst_element_release_request_pad(request->getTee(), request->getTeePad());

  device->deleteRequest(request);

  return GST_PAD_PROBE_DROP;
}

static unsigned fileWrite(const char *file_path, char *buffer, unsigned size)
{
  FILE *fp;
  int ret = 0;

  fp = fopen(file_path, "w+");  // TODO: Why w+??
  if (!fp) {
    ANT_LOG_ERR(CAM, "File open failed: %s", file_path);
    return ret;
  }

  ret = fwrite((char*)buffer, sizeof(char), size, fp);
  if (ret != size) {
    ANT_LOG_ERR(CAM, "File write failed: %s", file_path);
    ret = 0;
  }

  if (fp)
    fclose(fp);
  return ret;
}

static GstFlowReturn jpegBufferFromSinkCB(GstElement *ele, gpointer _request)
{
  GstSample *sample;
  GstBuffer *buffer;
  GstMapInfo map;
  CameraRequest *request = (CameraRequest*)_request;
  dbusRequest *msg_handle = (dbusRequest*)request->getMsgHandle();

  sample = gst_app_sink_pull_sample(GST_APP_SINK(ele));
  buffer = gst_sample_get_buffer(sample);
  if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
    if (map.size != fileWrite(msg_handle->file_path,
          (char*)map.data, (unsigned)map.size)) {
      ANT_LOG_ERR(CAM, "Fail to write");
      return GST_FLOW_OK;
    }
  }

  timeOutCB(_request);
  return GST_FLOW_EOS;
}


static GstFlowReturn openCVBufferFromSinkCB(GstElement *ele, gpointer _request)
{
  GstSample *sample;
  GstBuffer *buffer;
  GstMapInfo map;
  ANTRawRequest *raw_request = (ANTRawRequest*)_request;

  char *buffer_ptr = (char*)raw_request->getBufferPtr();
  unsigned* buffer_size_ptr = (unsigned*)raw_request->getBufferSizePtr();
  sem_t *sem = raw_request->getSemaphore();

  sample = gst_app_sink_pull_sample(GST_APP_SINK(ele));
  buffer = gst_sample_get_buffer(sample);

  if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
    sem_wait(sem);
    *buffer_size_ptr = map.size;
    memcpy((char*)buffer_ptr, (char*)map.data, map.size);
    gst_buffer_unmap(buffer, &map);
    sem_post(sem);
  }
  gst_buffer_unref(buffer);

  return GST_FLOW_OK;
}
