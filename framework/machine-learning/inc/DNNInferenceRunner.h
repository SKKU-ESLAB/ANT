/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
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

#ifndef __DNN_INFERENCE_RUNNER_H__
#define __DNN_INFERENCE_RUNNER_H__

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "InferenceRunner.h"

// For Caffe Framework
#define CPU_ONLY
#include <caffe/caffe.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include <iosfwd>
#include <memory>
#include <utility>

// For Connection with Camera Framework
#include <sys/shm.h>
#include <semaphore.h>
#include <errno.h>
#include <dbus/dbus.h>

#define SHM_KEY_FOR_BUFFER 5315
#define CAMERA_DEFAULT_BUF_SIZE CAMERA_480P_BUF_SIZE
#define CAMERA_480P_BUF_SIZE (RAW_480P_WIDTH * RAW_480P_HEIGHT * 3)
#define RAW_480P_WIDTH 640
#define RAW_480P_HEIGHT 480

using namespace caffe;  // NOLINT(build/namespaces)
using std::string;

typedef std::pair<string, float> Prediction;

class DNNInferenceRunner
: public InferenceRunner {
  public:
    // Run model inference with inputData and get outputData
    //   - Input: MLDataUnit* inputData
    //   - Ouptut: MLDataUnit* outputData
    virtual MLDataUnit* run(MLDataUnit* inputData);

    // Get resource usage of inference runner
    virtual std::string getResourceUsage();

  private:
    bool initClassifier(const string& model_file,
        const string& trained_file,
        const string& mean_file,
        const string& label_file);

    std::vector<Prediction> classify(const cv::Mat& img, int N = 5);
    void setMean(const string& mean_file);
    std::vector<float> predict(const cv::Mat& img);
    void wrapInputLayer(std::vector<cv::Mat>* input_channels);
    void preprocess(const cv::Mat& img,
        std::vector<cv::Mat>* input_channels);

    bool initSemaphore();
    bool releaseSemaphore();
    bool initSharedMemorySpace();
    bool releaseSharedMemorySpace();
    IplImage* retrieveFrame();
    bool initDBus();
    bool streamingStart();

    shared_ptr<Net<float> > mNet;
    cv::Size mInputGeometry;
    int mNumChannels;
    cv::Mat mMean;
    std::vector<string> mLabels;

    bool mIsStreaming = false;
    IplImage mFrame;
    unsigned mBufferSize;
    void* mShmPtr;
    int mShmId;
    sem_t* mSem;
    DBusConnection* mConnection;
    DBusError mError;
};

static bool pairCompare(const std::pair<float, int>& lhs,
                        const std::pair<float, int>& rhs);
static std::vector<int> argmax(const std::vector<float>& v, int N);
static void sendDBusMsg(DBusConnection* conn, const char* msg, const char* text);

#endif // !defined(__DNN_INFERENCE_RUNNER_H__)
