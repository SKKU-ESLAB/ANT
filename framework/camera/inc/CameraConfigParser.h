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
