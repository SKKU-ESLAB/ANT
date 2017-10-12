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

#ifndef __ML_DATA_UNIT_H__
#define __ML_DATA_UNIT_H__

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "MLTensor.h"
#include "cJSON.h"

class MLDataUnitLayout {
  public:
    MLTensorLayout* findTensorLayout(std::string tensorName) {
      std::map<std::string, MLTensorLayout>::iterator it
        = this->mTensorLayoutMap.find(tensorName);
      if(it == this->mTensorLayoutMap.end())
        return NULL;
      else
        return &(it->second);
    }

    void insertTensorLayout(std::string tensorName,
        MLTensorLayout tensorLayout) {
      this->mTensorLayoutMap.insert(
          std::pair<std::string, MLTensorLayout>(tensorName, tensorLayout));
    }

    std::map<std::string, MLTensorLayout>& getMap() {
      return this->mTensorLayoutMap;
    }

  protected:
    std::map<std::string, MLTensorLayout> mTensorLayoutMap;
};

class MLDataUnit {
  public:
    MLDataUnit() {
    }

    ~MLDataUnit() {
      std::map<std::string, MLTensor*>::iterator it;
      for(it = this->mTensorMap.begin();
          it != this->mTensorMap.end();
          it++) {
        MLTensor* tensor = it->second;
        delete tensor;
      }
    }
    
    // decoding from JSON
    bool setFromJSON(const char* jsonString);
    bool setFromJSON(cJSON* dataUnitObj);

    // encoding to JSON
    cJSON* toJSON();

    // Handle tensor map
    MLTensor* findTensor(std::string tensorName) {
      std::map<std::string, MLTensor*>::iterator it
        = this->mTensorMap.find(tensorName);
      if(it == this->mTensorMap.end())
        return NULL;
      else
        return it->second;
    }

    void insertTensor(std::string tensorName, MLTensor* tensor) {
      this->mTensorMap.insert(
          std::pair<std::string, MLTensor*>(tensorName, tensor));
    }

    bool isMatched(MLDataUnitLayout* dataUnitLayout);

    std::map<std::string, MLTensor*>& getMap() {
      return this->mTensorMap;
    }

  protected:
    std::map<std::string, MLTensor*> mTensorMap;
};

#endif // !defined(__ML_DATA_UNIT_H__)
