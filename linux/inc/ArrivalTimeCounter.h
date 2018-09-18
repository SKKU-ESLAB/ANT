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

#ifndef __ARRIVAL_TIME_COUNTER_H__
#define __ARRIVAL_TIME_COUNTER_H__

#include "Counter.h"

#include <sys/time.h>

#include <mutex>
#include <vector>

namespace sc {
class ArrivalTimeCounter : public Counter {
public:
  ArrivalTimeCounter(void) : Counter() {
    this->mLastAccessedTS.tv_sec = 0;
    this->mLastAccessedTS.tv_usec = 0;
  }

  void arrive(void) {
    std::unique_lock<std::mutex> lock(this->mArriveLock);
    struct timeval startTS, endTS;
    startTS = this->mLastAccessedTS;
    gettimeofday(&endTS, NULL);

    // Get and store interval
    if (startTS.tv_sec != 0 || startTS.tv_usec != 0) {
      uint64_t endUS = (uint64_t)endTS.tv_sec * 1000 * 1000 + endTS.tv_usec;
      uint64_t startUS =
          (uint64_t)startTS.tv_sec * 1000 * 1000 + startTS.tv_usec;
      int intervalUS = (int)(endUS - startUS);
      this->set_value(intervalUS);
    }

    // Store last accessed timestamp
    this->mLastAccessedTS = endTS;
  }

private:
  std::mutex mArriveLock;

  struct timeval mLastAccessedTS;
}; /* class ArrivalTimeCounter */
} /* namespace sc */

#endif /* !defined(__ARRIVAL_TIME_COUNTER_H__) */
