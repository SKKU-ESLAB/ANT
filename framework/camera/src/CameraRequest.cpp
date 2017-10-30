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

#include "CameraRequest.h"

CameraRequest::CameraRequest()
{

}

CameraRequest::CameraRequest(RequestType type)
{
  this->mRequestType = type;
}

CameraRequest::CameraRequest(RequestType type, GstElement *bin)
{
  this->mElementVector = new std::vector<GstElement*>;
  this->mRequestType = type;
  this->mBin = bin;
  this->mSinkPad = gst_element_get_static_pad(bin, "sink");

  GstIterator *iter = gst_bin_iterate_elements(GST_BIN(this->mBin));
  GValue item = G_VALUE_INIT;
  bool done = false;
  GstElement *tmp;
  while (!done) {
    switch (gst_iterator_next(iter, &item)) {
      case GST_ITERATOR_OK:
        tmp = (GstElement*)g_value_get_object(&item);
        if (!strncmp(GST_OBJECT_NAME(tmp), "sink", strlen("sink")))
          this->mLastElement = tmp;
        this->mElementVector->push_back(tmp);
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

  if (this->mLastElement == NULL)
    ANT_LOG_WARN(CAM, "This request bin has not set sink element");
  this->mLastPad = gst_element_get_static_pad(this->mLastElement, "sink");
}

CameraRequest::~CameraRequest()
{
  if (this->mSinkPad != NULL) {
    gst_pad_unlink(this->mTeeGhostPad, this->mSinkPad);
    gst_pad_send_event(this->mSinkPad, gst_event_new_eos());
    gst_object_unref(this->mSinkPad);

    gst_element_set_state(this->mBin, GST_STATE_NULL);
    gst_bin_remove(GST_BIN(GST_ELEMENT_PARENT(this->mBin)), this->mBin);
  }
}

void CameraRequest::requestTeePad()
{
  if (this->mTee == NULL)
    ANT_LOG_ERR(CAM, "Tee is not set");
  else
    this->mTeePad = gst_element_get_request_pad(this->mTee, "src_%u");
}

void CameraRequest::linkPads()
{
  GstPadLinkReturn ret;
  
  this->mTeeGhostPad = gst_ghost_pad_new(NULL, this->mTeePad);
  gst_pad_set_active(this->mTeeGhostPad, TRUE);
  gst_element_add_pad(GST_ELEMENT_PARENT(this->mTee), this->mTeeGhostPad);
  ret = gst_pad_link(this->mTeeGhostPad, this->mSinkPad);
  if (ret != GST_PAD_LINK_OK)
    ANT_LOG_ERR(CAM, "Pad link failed (%d)", ret);
}

unsigned CameraRequest::getSeconds()
{
  if (this->mRequestType == kRecordingStart ||
      this->mRequestType == kPreRecordingStart) {
    dbusRequest *msg_handle = (dbusRequest*)this->mMsgHandle;
    return msg_handle->play_seconds;
  }
  return 0;
}

GstElement* CameraRequest::getElementByName(const char *name)
{
  assert(name != NULL);

  std::vector<GstElement*>::iterator iter, iter_end;
  iter = this->mElementVector->begin();
  iter_end = this->mElementVector->end();
  for (iter; iter != iter_end; iter++) {
    GstElement *tmp = (GstElement*)(*iter);
    if (!strncmp(GST_OBJECT_NAME(tmp), name, strlen(name)))
      return tmp;
  }
  ANT_LOG_ERR(CAM, "Cannot find element by element name");
  return NULL;
}
