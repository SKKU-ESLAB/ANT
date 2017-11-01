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

#ifndef __SENSOR_INPUT_READER_H__
#define __SENSOR_INPUT_READER_H__

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "InputReader.h"

class SensorInputReader
: public InputReader {
  public:
    // Read input data from source
    //   - Input: String sourceUri
    //   - Output: MLTensor* inputTensor
    virtual MLTensor* read(std::string sourceUri);

    // Get the tensor layout of inputData which this InputReader produces
    //   - Output: MLTensorLayout layout
    //     - It will be used to check if the layout is same as the required.
    virtual MLTensorLayout getLayout();
};

#endif // !defined(__SENSOR_INPUT_READER_H__)
