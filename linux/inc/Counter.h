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

#include <assert.h>
#include <mutex>
#include <sys/time.h>

namespace sc {
class Counter {
public:
  Counter(int simple_moving_average_length,
          float exponential_moving_average_weight) {
    this->mValue = 0;
    this->mPrevValue = 0;
    this->mLastAccessedTS.tv_sec = 0;
    this->mLastAccessedTS.tv_usec = 0;

    /* Simple moving average */
    assert(simple_moving_average_length > 0);
    this->mSmaLength = simple_moving_average_length;
    this->mHistoryValues = new int[this->mSmaLength];
    for (int i = 0; i < this->mSmaLength; i++) {
      this->mHistoryValues[i] = 0;
    }
    this->mHistoryCursor = 0;

    /* Exponential moving average */
    this->mEma = 0;
    this->mEmaWeight = exponential_moving_average_weight;
    assert(exponential_moving_average_weight >= 0 &&
           exponential_moving_average_weight <= 1);
  }

#define DEFAULT_SIMPLE_MOVING_AVERAGE_LENGTH 10
#define DEFAULT_EXPONENTIAL_MOVING_AVERAGE_WEIGHT 0.9
  Counter()
      : Counter(DEFAULT_SIMPLE_MOVING_AVERAGE_LENGTH,
                DEFAULT_EXPONENTIAL_MOVING_AVERAGE_WEIGHT) {}

  ~Counter() { delete this->mHistoryValues; }

  void add(int diff) {
    std::unique_lock<std::mutex> lock(this->mLock);
    this->set_value(this->mValue + diff);
  }

  void sub(int diff) {
    std::unique_lock<std::mutex> lock(this->mLock);
    this->set_value(this->mValue - diff);
  }

  void increase(void) {
    std::unique_lock<std::mutex> lock(this->mLock);
    this->set_value(this->mValue + 1);
  }

  void decrease(void) {
    std::unique_lock<std::mutex> lock(this->mLock);
    this->set_value(this->mValue - 1);
  }

  void set_value(int new_value) {
    std::unique_lock<std::mutex> lock(this->mLock);

    /* Update new value */
    this->mValue = new_value;

    /* Update history for simple moving average */
    this->mHistoryValues[this->mHistoryCursor] = new_value;
    this->mHistoryCursor = (this->mHistoryCursor + 1) % this->mSmaLength;

    /* Update exponential moving average */
    this->mEma = (this->mEma * (1 - this->mEmaWeight)) +
                 (this->mValue * this->mEmaWeight);
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

    if (startTS.tv_sec == 0 && startTS.tv_usec == 0) {
      speed = 0;
    } else {
      uint64_t end = (uint64_t)endTS.tv_sec * 1000 * 1000 + endTS.tv_usec;
      uint64_t start = (uint64_t)startTS.tv_sec * 1000 * 1000 + startTS.tv_usec;
      uint64_t interval = end - start;

      if (start != 0 && interval != 0) {
        speed = (int)((float)(this->mValue - this->mPrevValue) /
                      ((float)interval / (1000 * 1000)));
      } else {
        speed = 0;
      }
    }
    this->mPrevValue = this->mValue;
    this->mLastAccessedTS = endTS;
    return speed;
  };

  int get_sm_average(void) {
    int simple_mavg = 0;
    for (int i = 0; i < this->mSmaLength; i++) {
      simple_mavg += this->mHistoryValues[i];
    }
    simple_mavg /= this->mSmaLength;
    return simple_mavg;
  }

  int get_em_average(void) { return this->mEma; }

private:
  std::mutex mLock;

  int mValue;
  int mPrevValue;
  struct timeval mLastAccessedTS;

  /* Simple moving average (SMA) */
  int *mHistoryValues; /* History values for simple moving average */
  int mHistoryCursor;  /* Cursor on history values */
  int mSmaLength;      /* Length for simple moving average */

  /* Exponential moving average (EMA) */
  int mEma;         /* Exponential moving average */
  float mEmaWeight; /* Weight for Exponential moving average */
};
} /* namespace sc */

#endif /* INC_COUNTER_H_ */
