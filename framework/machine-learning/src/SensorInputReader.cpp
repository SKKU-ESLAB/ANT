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

#include <time.h>
#include <stdlib.h>

#include "SensorInputReader.h"

MLTensor* SensorInputReader::read(std::string sourceUri) {
  // TODO: implement it
  // Now sensor data is given as dummy data.
  MLTensor* inputTensor = new MLTensor(this->getLayout());
  srand(time(NULL));
  int randData = rand() % 1000;
  float data[3];
  data[0] = randData*0.001f; data[1] = randData*0.0001f; data[2] = randData*0.001f;
  inputTensor->assignData(data);
  return inputTensor;
}

MLTensorLayout SensorInputReader::getLayout() {
  // Input layout : float[3] input
  int inputShape[] = {3};
  MLTensorLayout inputTensorLayout(1, inputShape, MLDataType::Float);
  return inputTensorLayout;
}
