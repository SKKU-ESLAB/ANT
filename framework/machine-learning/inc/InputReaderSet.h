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

#ifndef __INPUT_READER_SET_H__
#define __INPUT_READER_SET_H__

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "InputReader.h"
#include "MLTensor.h"

class InputReaderSet {
  public:
    InputReaderSet();

    // Read input tensor from source
    //   - Child class of InputReader should implement it.
    MLTensor* read(std::string sourceUri);

    InputReader* findBestInputReader(std::string uriString);
    
  protected:
    // Dictinary(key: string sourceUriPattern, value: InputReader)
    std::map<std::string, InputReader*> mInputReaders;
};

#endif // !defined(__INPUT_READER_SET_H__)
