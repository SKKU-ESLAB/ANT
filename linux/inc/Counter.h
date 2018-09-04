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

#include <Counter.h>
#include <DebugLog.h>

#include <assert.h>
#include <mutex>
#include <sys/time.h>

namespace sc {
class Counter {
public:
#define DEFAULT_SIMPLE_MOVING_AVERAGE_LENGTH 10
#define DEFAULT_EXPONENTIAL_MOVING_AVERAGE_WEIGHT 0.9f

  Counter() {
    int simple_moving_average_length = DEFAULT_SIMPLE_MOVING_AVERAGE_LENGTH;
    float exponential_moving_average_weight =
        DEFAULT_EXPONENTIAL_MOVING_AVERAGE_WEIGHT;

    this->mValue = 0;
    this->mPrevValue = 0;
    this->mLastAccessedTS.tv_sec = 0;
    this->mLastAccessedTS.tv_usec = 0;

    /* Simple moving average */
    assert(simple_moving_average_length > 0);
    this->mSmaLength = simple_moving_average_length;
    this->mSimpleHistoryValues = new int[this->mSmaLength];
    for (int i = 0; i < this->mSmaLength; i++) {
      this->mSimpleHistoryValues[i] = 0;
    }
    this->mSimpleHistoryCursor = 0;

    /* Exponential moving average */
    this->mEma = 0.0f;
    this->mEmaWeight = exponential_moving_average_weight;
    assert(exponential_moving_average_weight >= 0 &&
           exponential_moving_average_weight <= 1);
  }

  ~Counter() { delete this->mSimpleHistoryValues; }

  void add(int diff) {
    std::unique_lock<std::mutex> lock(this->mValueLock);
    this->set_value_locked(this->get_value_locked() + diff);
  }

  void sub(int diff) {
    std::unique_lock<std::mutex> lock(this->mValueLock);
    this->set_value_locked(this->get_value_locked() - diff);
  }

  void increase(void) {
    std::unique_lock<std::mutex> lock(this->mValueLock);
    this->set_value_locked(this->get_value_locked() + 1);
  }

  void decrease(void) {
    std::unique_lock<std::mutex> lock(this->mValueLock);
    this->set_value_locked(this->get_value_locked() - 1);
  }

  void set_value(int new_value) {
    std::unique_lock<std::mutex> lock(this->mValueLock);
    this->set_value_locked(new_value);
  }

  int get_value() {
    std::unique_lock<std::mutex> lock(this->mValueLock);
    return this->get_value_locked();
  }

  int get_speed() {
    std::unique_lock<std::mutex> lock(this->mValueLock);
    return this->get_speed_locked();
  };

  int get_sm_average(void) {
    std::unique_lock<std::mutex> lock(this->mValueLock);
    return this->get_sm_average_locked();
  }

  float get_em_average(void) {
    std::unique_lock<std::mutex> lock(this->mValueLock);
    return this->get_em_average_locked();
  }

private:
  void set_value_locked(int new_value) {
    /* Update new value */
    this->mValue = new_value;

    /* Update history for simple moving average */
    this->mSimpleHistoryValues[this->mSimpleHistoryCursor] = new_value;
    this->mSimpleHistoryCursor =
        (this->mSimpleHistoryCursor + 1) % this->mSmaLength;

    /* Update exponential moving average */
    this->mEma = ((float)this->mEma * (1.0f - this->mEmaWeight)) +
                 ((float)this->mValue * this->mEmaWeight);
  }

  int get_value_locked() { return this->mValue; }

  int get_speed_locked() {
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
  }

  int get_sm_average_locked(void) {
    int simple_mavg = 0;
    for (int i = 0; i < this->mSmaLength; i++) {
      simple_mavg += this->mSimpleHistoryValues[i];
    }
    simple_mavg /= this->mSmaLength;
    return simple_mavg;
  }

  float get_em_average_locked(void) { return this->mEma; }

private:
  /* Value */
  std::mutex mValueLock;
  int mValue;

  /* Speed */
  int mPrevValue;
  struct timeval mLastAccessedTS;

  /* Simple moving average (SMA) */
  int *mSimpleHistoryValues; /* History values for simple moving average */
  int mSimpleHistoryCursor;  /* Cursor on history values */
  int mSmaLength;            /* Length for simple moving average */

  /* Exponential moving average (EMA) */
  float mEma;       /* Exponential moving average */
  float mEmaWeight; /* Weight for Exponential moving average */
};
} /* namespace sc */

#endif /* INC_COUNTER_H_ */
