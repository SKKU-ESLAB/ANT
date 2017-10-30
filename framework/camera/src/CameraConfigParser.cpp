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

#include "CameraConfigParser.h"

CameraConfigParser::CameraConfigParser()
{
}

CameraConfigParser::~CameraConfigParser()
{
  if (this->mCameraConfigCjson)
    cJSON_Delete(this->mCameraConfigCjson);
}

bool CameraConfigParser::readCameraConfig(char *filepath)
{
  FILE *fp;
  char *buffer;
  int num_bytes;

  fp = fopen(filepath, "r");
  if (fp == NULL) {
    ANT_LOG_ERR(CAM, "%s is not exist", filepath);
    return false;
  }

  fseek(fp, 0L, SEEK_END);
  num_bytes = ftell(fp);
  fseek(fp, 0L, SEEK_SET);

  buffer = (char *) calloc(num_bytes, sizeof(char));
  if (buffer == NULL) {
    ANT_LOG_ERR(CAM, "Failed to allocate buffer");
    return false;
  }

  fread(buffer, sizeof(char), num_bytes, fp);
  this->mCameraConfigCjson = cJSON_Parse(buffer);
  if (this->mCameraConfigCjson == NULL) {
    ANT_LOG_ERR(CAM, "Failed to parse camera configuration file");
    return false;
  }

  free(buffer);
  fclose(fp);
  return true;
}

int CameraConfigParser::readyCameraConfig()
{
  cJSON *root = this->mCameraConfigCjson;
  int camera_num = cJSON_GetObjectItem(root, "camera_num")->valueint;
  this->mRecordingConfig = cJSON_GetObjectItem(root, "recording")->valuestring;
  this->mSnapshotConfig = cJSON_GetObjectItem(root, "snapshot")->valuestring;
  this->mStreamingConfig = cJSON_GetObjectItem(root, "streaming")->valuestring;
  this->mPreRecordingInitConfig = cJSON_GetObjectItem(root, "pre_recording_init")->valuestring;
  this->mPreRecordingConfig = cJSON_GetObjectItem(root, "pre_recording")->valuestring;
  this->mOpenCVConfig = cJSON_GetObjectItem(root, "opencv")->valuestring;

  ANT_LOG_DBG(CAM, "Number of camera = %d", camera_num);
  ANT_LOG_DBG(CAM, "Recording = %s", this->mRecordingConfig);
  ANT_LOG_DBG(CAM, "Snapshot = %s", this->mSnapshotConfig);
  ANT_LOG_DBG(CAM, "Streaming = %s", this->mStreamingConfig);
  ANT_LOG_DBG(CAM, "Pre-Recording Init = %s", this->mPreRecordingInitConfig);
  ANT_LOG_DBG(CAM, "Pre-Recording = %s", this->mPreRecordingConfig);
  ANT_LOG_DBG(CAM, "OpenCV = %s", this->mOpenCVConfig);

  return camera_num;
}

GstElement* CameraConfigParser::getMainBinById(int camera_id)
{
  cJSON *main = cJSON_GetObjectItem(this->mCameraConfigCjson, "main");
  char s_camera_id[32];
  sprintf(s_camera_id, "%d", camera_id);

  char *main_bin = cJSON_GetObjectItem(main, s_camera_id)->valuestring;
  ANT_LOG_DBG(CAM, "main_%d = %s", camera_id, main_bin);
  return gst_parse_bin_from_description(main_bin, TRUE, NULL);
}

GstElement* CameraConfigParser::getRecordingBin()
{
  return gst_parse_bin_from_description(this->mRecordingConfig, TRUE, NULL);
}

GstElement* CameraConfigParser::getSnapshotBin()
{
  return gst_parse_bin_from_description(this->mSnapshotConfig, TRUE, NULL);
}

GstElement* CameraConfigParser::getStreamingBin()
{
  return gst_parse_bin_from_description(this->mStreamingConfig, TRUE, NULL);
}

GstElement* CameraConfigParser::getPreRecordingInitBin()
{
  return gst_parse_bin_from_description(this->mPreRecordingInitConfig, TRUE, NULL);
}

GstElement* CameraConfigParser::getPreRecordingBin()
{
  return gst_parse_bin_from_description(this->mPreRecordingConfig, TRUE, NULL);
}

GstElement* CameraConfigParser::getOpenCVBin()
{
  return gst_parse_bin_from_description(this->mOpenCVConfig, TRUE, NULL);
}
