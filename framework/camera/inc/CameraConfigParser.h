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

#ifndef CAMERA_CONFIG_PARSER_H
#define CAMERA_CONFIG_PARSER_H

#include "Camera.h"
#include "cJSON.h"

class CameraConfigParser {
  public:
    CameraConfigParser();
    ~CameraConfigParser();

    bool readCameraConfig(char *filepath);
    int readyCameraConfig();

    GstElement* getMainBinById(int camera_id);
    GstElement* getRecordingBin();
    GstElement* getSnapshotBin();
    GstElement* getStreamingBin();
    GstElement* getPreRecordingInitBin();
    GstElement* getPreRecordingBin();
    
    GstElement* getOpenCVBin();
    GstElement* getFaceDetectBin();

  public:
    cJSON *mCameraConfigCjson;
    char *mRecordingConfig;
    char *mSnapshotConfig;
    char *mStreamingConfig;
    char *mPreRecordingInitConfig;
    char *mPreRecordingConfig;
    
    char *mOpenCVConfig;
    char *mFaceDetectConfig;
};

#endif /* CAMERA_CONFIG_PARSER_H */
