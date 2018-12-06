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

// For ARM Compute Library
#include "arm_compute/graph.h"
#include "support/ToolchainSupport.h"
#include "utils/CommonGraphOptions.h"
#include "utils/GraphUtils.h"
#include "utils/Utils.h"
#include "utils/ImageLoader.h"

// For Connection with Camera Framework
#include <sys/shm.h>
#include <semaphore.h>
#include <errno.h>
#include <dbus/dbus.h>
#include <unistd.h>

#define SHM_KEY_FOR_BUFFER 5315
//#define CAMERA_DEFAULT_BUF_SIZE CAMERA_480P_BUF_SIZE
#define CAMERA_480P_BUF_SIZE (RAW_480P_WIDTH * RAW_480P_HEIGHT * 3)
#define RAW_480P_WIDTH 640
#define RAW_480P_HEIGHT 480

#define CAMERA_DEFAULT_BUF_SIZE IMAGENET_INPUT_BUF_SIZE
#define IMAGENET_INPUT_BUF_SIZE (227*227*3)

using std::string;
using namespace arm_compute;
using namespace arm_compute::utils;
using namespace arm_compute::graph::frontend;
using namespace arm_compute::graph_utils;

typedef std::pair<string, float> Prediction;

class DNNInferenceRunner
: public InferenceRunner {
  public:
    DNNInferenceRunner() : mCommonOpts(mCmdParser), mGraph(0, "AlexNet") {}

    // Run model inference with inputData and get outputData
    //   - Input: MLDataUnit* inputData
    //   - Ouptut: MLDataUnit* outputData
    virtual MLDataUnit* run(MLDataUnit* inputData);

    // Get resource usage of inference runner
    virtual std::string getResourceUsage();

    void* getShmPtr()
    { return this->mShmPtr; }
    sem_t* getSem()
    { return this->mSem;}
    unsigned char* getFrame()
    { return this->mFrame; }
    unsigned getBufferSize()
    { return this->mBufferSize; }


  private:
    bool initClassifier();

    bool initSemaphore();
    bool releaseSemaphore();
    bool initSharedMemorySpace();
    bool releaseSharedMemorySpace();
    //IplImage* retrieveFrame();
    bool initDBus();
    bool streamingStart();

    //shared_ptr<Net<float> > mNet;
    Stream             mGraph;
    CommandLineParser  mCmdParser;
    CommonGraphOptions mCommonOpts;
    CommonGraphParams  mCommonParams;
    int mNumChannels;
    std::vector<string> mLabels;

    bool mIsStreaming = false;
    unsigned char* mFrame;
    unsigned mBufferSize;
    void* mShmPtr;
    int mShmId;
    sem_t* mSem;
    DBusConnection* mConnection;
    DBusError mError;
};

static void sendDBusMsg(DBusConnection* conn, const char* msg, const char* text);

/* Shared memory accessor class */
class ShmAccessor final : public graph::ITensorAccessor
{
public:
  ShmAccessor(DNNInferenceRunner* runner = nullptr, bool bgr = true, std::unique_ptr<IPreprocessor> preprocessor = nullptr);
  ShmAccessor(ShmAccessor &&) = default;

  bool access_tensor(ITensor &tensor) override;

private:
  DNNInferenceRunner* _runner;
  const bool                          _bgr;
  std::unique_ptr<IPreprocessor>      _preprocessor;
  std::unique_ptr<IImageDataFeeder>   _feeder;
};

inline std::unique_ptr<graph::ITensorAccessor> get_shm_input_accessor(
    const arm_compute::utils::CommonGraphParams &graph_parameters,
    std::unique_ptr<IPreprocessor> preprocessor = nullptr,
    DNNInferenceRunner* runner = nullptr,
    bool bgr = true)
{
  return arm_compute::support::cpp14::make_unique<ShmAccessor>(std::move(runner), bgr, std::move(preprocessor));
}


#endif // !defined(__DNN_INFERENCE_RUNNER_H__)
