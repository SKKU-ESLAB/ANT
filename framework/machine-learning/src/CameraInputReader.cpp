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

#include "CameraInputReader.h"

MLTensor* CameraInputReader::read(std::string sourceUri) {
  // TODO: implement it
  // Now camera data is given as dummy data.
  MLTensor* inputTensor = new MLTensor(this->getLayout());
  float data[224*224];
  memset((void*)data, 0.5, 224*224);
  inputTensor->assignData(data);
  return inputTensor;
}

MLTensorLayout CameraInputReader::getLayout() {
  // Input layout : float[224][224] input
  int inputShape[] = {224, 224};
  MLTensorLayout inputTensorLayout(2, inputShape, MLDataType::Float);
  return inputTensorLayout;
}
