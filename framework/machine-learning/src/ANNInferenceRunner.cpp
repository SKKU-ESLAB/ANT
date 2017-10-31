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

#include "ANNInferenceRunner.h"

MLDataUnit* ANNInferenceRunner::run(MLDataUnit* inputData) {
  // TODO: implement it
  // Now DNN output is given as dummy data.
  MLTensor* inputTensor = inputData->findTensor("input");
  if(inputTensor == NULL) return NULL;
  void* inputBuffer = inputTensor->bytesValue();
  float input0 = *(((float*)inputBuffer) + 0);
  float input1 = *(((float*)inputBuffer) + 1);
  float input2 = *(((float*)inputBuffer) + 2);
  int outputShape[] = {30};
  MLTensorLayout outputTensorLayout(1, outputShape, MLDataType::Char);

  MLTensor* outputTensor = new MLTensor(outputTensorLayout);
  char outputStr[30];
  snprintf(outputStr, 30, "%.4f-%.4f-%.4f", input0, input1, input2);
  outputTensor->assignData(outputStr);

  MLDataUnit* outputData = new MLDataUnit();
  outputData->insertTensor("output", outputTensor);
  return outputData;
}

// Get resource usage of inference runner
std::string ANNInferenceRunner::getResourceUsage() {
  std::string data("");
  // not implemented yet
  
  return data;
}
