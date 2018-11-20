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

#ifndef __COUNTER_H__
#define __COUNTER_H__

#include "DebugLog.h"

#include <assert.h>
#include <map>
#include <mutex>
#include <sys/time.h>
#include <thread>

namespace sc {
class Counter {
public:
#define DEFAULT_SIMPLE_MOVING_AVERAGE_LENGTH 12
#define DEFAULT_EXPONENTIAL_MOVING_AVERAGE_WEIGHT 0.9f
#define PREV_VALUES_SIZE 10

  Counter() {
    int simple_moving_average_length = DEFAULT_SIMPLE_MOVING_AVERAGE_LENGTH;
    float exponential_moving_average_weight =
        DEFAULT_EXPONENTIAL_MOVING_AVERAGE_WEIGHT;

    this->mValue = 0;

    /* Speed */
    for (int i = 0; i < PREV_VALUES_SIZE; i++) {
      this->mPrevValues[i] = 0;
      this->mPrevValueTSs[i] = 0;
    }

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

  void update_speed() {
    std::unique_lock<std::mutex> lock(this->mValueLock);
    this->update_speed_locked();
  }

  int get_sm_average(void) {
    std::unique_lock<std::mutex> lock(this->mValueLock);
    return this->get_sm_average_locked();
  }

  float get_em_average(void) {
    std::unique_lock<std::mutex> lock(this->mValueLock);
    return this->get_em_average_locked();
  }

  void start_measure_speed(void) {
    this->mSpeedThread = new std::thread(std::bind(&Counter::speed_thread_loop, this));
    this->mSpeedThread->detach();
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

  void speed_thread_loop() {
    while(1) {
      this->update_speed();
      usleep(250 * 1000);
    }
  }

  int get_speed_locked() {
    int startPointer = (this->mPrevValuePointer + 1) % PREV_VALUES_SIZE;
    int endPointer = (this->mPrevValuePointer) % PREV_VALUES_SIZE;

    int startValue = this->mPrevValues[startPointer];
    int endValue = this->mPrevValues[endPointer];
    uint64_t startTStv = this->mPrevValueTSs[startPointer];
    uint64_t endTStv = this->mPrevValueTSs[endPointer];

    int speed;
    if (endTStv == 0 || startTStv == 0) {
      speed = 0;
    } else {
      speed = (int)(((float)(endValue - startValue)) /
                    ((float)(endTStv - startTStv) / 1000000));
    }

    return speed;
  }

  void update_speed_locked() {
    struct timeval nowTStv;
    gettimeofday(&nowTStv, NULL);

    this->mPrevValuePointer = (this->mPrevValuePointer + 1) % PREV_VALUES_SIZE;
    this->mPrevValues[this->mPrevValuePointer] = this->get_value_locked();
    this->mPrevValueTSs[this->mPrevValuePointer] =
        (uint64_t)nowTStv.tv_sec * 1000 * 1000 + nowTStv.tv_usec;
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
  int mPrevValues[PREV_VALUES_SIZE];
  uint64_t mPrevValueTSs[PREV_VALUES_SIZE];
  int mPrevValuePointer;
  struct timeval mLastAccessedTS;
  std::thread* mSpeedThread;

  /* Simple moving average (SMA) */
  int *mSimpleHistoryValues; /* History values for simple moving average */
  int mSimpleHistoryCursor;  /* Cursor on history values */
  int mSmaLength;            /* Length for simple moving average */

  /* Exponential moving average (EMA) */
  float mEma;       /* Exponential moving average */
  float mEmaWeight; /* Weight for Exponential moving average */
};                  /* class Counter */
} /* namespace sc */

#endif /* !defined(__COUNTER_H__) */
