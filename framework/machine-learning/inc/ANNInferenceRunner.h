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

#ifndef __ANN_INFERENCE_RUNNER_H__
#define __ANN_INFERENCE_RUNNER_H__

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "InferenceRunner.h"

class ANNInferenceRunner
: public InferenceRunner {
  public:
    // Run model inference with inputData and get outputData
    //   - Input: MLDataUnit* inputData
    //   - Ouptut: MLDataUnit* outputData
    virtual MLDataUnit* run(MLDataUnit* inputData);

    // Get resource usage of inference runner
    virtual std::string getResourceUsage();
};

#endif // !defined(__ANN_INFERENCE_RUNNER_H__)
