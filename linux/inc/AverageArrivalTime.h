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

#ifndef INC_AVERAGE_ARRIVAL_TIME_H_
#define INC_AVERAGE_ARRIVAL_TIME_H_

#include <sys/time.h>

#include <mutex>
#include <vector>

namespace sc {
class AverageArrivalTime {
public:
  AverageArrivalTime(void) {
    this->mLastAccessedTS.tv_sec = 0;
    this->mLastAccessedTS.tv_usec = 0;
  }

  void arrive(void) {
    std::unique_lock<std::mutex> lock(this->mLock);
    struct timeval startTS, endTS;
    startTS = this->mLastAccessedTS;
    gettimeofday(&endTS, NULL);

    // Get and store interval
    if (startTS.tv_sec != 0 || startTS.tv_usec != 0) {
      uint64_t endUS = (uint64_t)endTS.tv_sec * 1000 * 1000 + endTS.tv_usec;
      uint64_t startUS =
          (uint64_t)startTS.tv_sec * 1000 * 1000 + startTS.tv_usec;
      uint64_t intervalUS = endUS - startUS;
      this->mArrivalTimeUSs.insert(this->mArrivalTimeUSs.begin(), intervalUS);
    }

    // Store last accessed timestamp
    this->mLastAccessedTS = endTS;
  }

  uint64_t getAverage(uint64_t windowSizeUS) {
    std::unique_lock<std::mutex> lock(this->mLock);
    std::vector<uint64_t>::iterator it = this->mArrivalTimeUSs.begin();

    uint64_t totalArrivalTimeUS = 0;
    int count = 0;
    while (it != this->mArrivalTimeUSs.end()) {
      uint64_t arrivalTimeUS = *it;
      if (totalArrivalTimeUS < windowSizeUS) {
        // not exceed window size: sum the arrival time
        totalArrivalTimeUS += arrivalTimeUS;
        count++;
        it++;
      } else {
        // exceed window size: erase the entry
        it = this->mArrivalTimeUSs.erase(it);
      }
    }

    uint64_t average = (count != 0) ? (totalArrivalTimeUS / count) : 0;
    return average;
  }

private:
  std::mutex mLock;

  std::vector<uint64_t> mArrivalTimeUSs;
  struct timeval mLastAccessedTS;
};
} /* namespace sc */

#endif /* INC_AVERAGE_ARRIVAL_TIME_H_ */
