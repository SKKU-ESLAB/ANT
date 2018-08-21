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

#ifndef INC_COUNTER_H_
#define INC_COUNTER_H_

#include <sys/time.h>

#include <mutex>

namespace sc {
class Counter {
  public:
    Counter() {
      this->mValue = 0;
      this->mPrevValue = 0;
      this->mLastAccessedTS.tv_sec = 0;
      this->mLastAccessedTS.tv_usec = 0;
    }

    void add(int diff) {
      std::unique_lock<std::mutex> lock(this->mLock);
      this->mValue = this->mValue + diff;
    }

    void sub(int diff) {
      std::unique_lock<std::mutex> lock(this->mLock);
      this->mValue = this->mValue - diff;
    }

    void increase(void) {
      std::unique_lock<std::mutex> lock(this->mLock);
      this->mValue++;
    }
    
    void decrease(void) {
      std::unique_lock<std::mutex> lock(this->mLock);
      this->mValue--;
    }

    int get_size() {
      std::unique_lock<std::mutex> lock(this->mLock);
      return this->mValue;
    }

    int get_speed() {
      std::unique_lock<std::mutex> lock(this->mLock);
      int speed;
      struct timeval startTS, endTS;
      startTS = this->mLastAccessedTS;
      gettimeofday(&endTS, NULL);

      if(startTS.tv_sec == 0 && startTS.tv_usec == 0) {
        speed = 0;
      } else {
        uint64_t end = (uint64_t)endTS.tv_sec * 1000 * 1000 + endTS.tv_usec;
        uint64_t start = (uint64_t)startTS.tv_sec * 1000 * 1000 + startTS.tv_usec;
        uint64_t interval = end - start;

        if(start != 0 && interval != 0) {
          speed = (int)((float)(this->mValue - this->mPrevValue) / ((float)interval / (1000 * 1000)));
        } else {
          speed = 0;
        }
      }
      this->mPrevValue = this->mValue;
      this->mLastAccessedTS = endTS;
      return speed;
    };
  private:
    std::mutex mLock;

    int mValue;
    int mPrevValue;
    struct timeval mLastAccessedTS;
};
} /* namespace sc */

#endif /* INC_COUNTER_H_ */
