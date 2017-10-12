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

#include <map>

#include "InferenceUnit.h"

class InferenceUnitDirectory {
  public:
    InferenceUnitDirectory()
    : mNextIuid(0) {
    }
    int insert(InferenceUnit* iu) {
      int iuid = this->mNextIuid;
      this->mIuMap.insert(
          std::pair<int, InferenceUnit*>(iuid, iu));
      this->mNextIuid++;
      return iuid;
    }

    bool remove(int iuid) {
      std::map<int, InferenceUnit*>::iterator it = this->mIuMap.find(iuid);
      if(it == this->mIuMap.end()) {
        return false;
      } else {
        this->mIuMap.erase(it);
        return true;
      }
    }

    InferenceUnit* find(int iuid) {
      std::map<int, InferenceUnit*>::iterator it = this->mIuMap.find(iuid);
      if(it == this->mIuMap.end()) {
        return NULL;
      } else {
        return it->second;
      }
    }

    std::map<int, InferenceUnit*>& getMap() {
      return this->mIuMap;
    }

  protected:
    // Inference Unit Directory (key: int iuid, value: InferenceUnit)
    std::map<int, InferenceUnit*> mIuMap;
    int mNextIuid;
};
