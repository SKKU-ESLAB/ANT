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

#ifndef INC_SPEED_METRIC_H_
#define INC_SPEED_METRIC_H_

#include <sys/time.h>

#include <mutex>

namespace cm {
class Counter {
  public:
    Counter() {
      this->mSize = 0;
      this->mPrevSize = 0;
      this->mLastAccessedTS.tv_sec = 0;
      this->mLastAccessedTS.tv_usec = 0;
    }

    void add(uint32_t diff) {
      std::unique_lock<std::mutex> lock(this->mLock);
      this->mSize = this->mSize + diff;
    }

    void sub(uint32_t diff) {
      std::unique_lock<std::mutex> lock(this->mLock);
      this->mSize = this->mSize - diff;
    }

    void increase(void) {
      std::unique_lock<std::mutex> lock(this->mLock);
      this->mSize++;
    }
    
    void decrease(void) {
      std::unique_lock<std::mutex> lock(this->mLock);
      this->mSize--;
    }

    uint32_t get_size() {
      std::unique_lock<std::mutex> lock(this->mLock);
      return this->mSize;
    }

    uint64_t get_speed() {
      std::unique_lock<std::mutex> lock(this->mLock);
      uint64_t speed;
      struct timeval startTS, endTS;
      startTS = this->mLastAccessedTS;
      gettimeofday(&endTS, NULL);

      if(startTS.tv_sec == 0 && startTS.tv_usec == 0) {
        speed = 0;
      } else {
        uint64_t interval
          = ((uint64_t)endTS.tv_sec * 1000 * 1000 + endTS.tv_usec)
          - ((uint64_t)startTS.tv_sec * 1000 * 1000 + startTS.tv_usec);

        if(interval != 0) {
          speed = (uint64_t)((float)(this->mSize - this->mPrevSize) / (float)(interval / (1000 * 1000)));
        } else {
          speed = 0;
        }
      }
      this->mPrevSize = this->mSize;
      this->mLastAccessedTS = endTS;
      return speed;
    };
  private:
    std::mutex mLock;

    uint32_t mSize;
    uint32_t mPrevSize;
    struct timeval mLastAccessedTS;
};
} /* namespace cm */

#endif /* INC_SPEED_METRIC_H_ */
