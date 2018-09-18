/* Copyright 2017-2018 All Rights Reserved.
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *  
 * [Contact]
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *
 * Licensed under the Apache License, Version 2.0(the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __REF_COUNT_H__
#define __REF_COUNT_H__

#include <mutex>

namespace sc {

class RefCount {
public:
  int get_value(void) {
    std::unique_lock<std::mutex> lck(this->mLock);
    return this->mCount;
  }
  int increase(void) {
    std::unique_lock<std::mutex> lck(this->mLock);
    this->mCount++;
    return this->mCount;
  }
  int decrease(void) {
    std::unique_lock<std::mutex> lck(this->mLock);
    this->mCount--;
    return this->mCount;
  }

  RefCount(void) {
    this->mCount = 0;
  }
protected:
  int mCount;
  std::mutex mLock;
}; /* class RefCount */
} /* namespace sc */
#endif /* !defined(__REF_COUNT_H__) */
