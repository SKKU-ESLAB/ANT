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

#ifndef INC_NETWORK_SWITCHER_H_
#define INC_NETWORK_SWITCHER_H_

#include <thread>
#include <mutex>

#define kSegThreshold 512
#define kSegQueueThreshold 50*(kSegThreshold / 512)

namespace cm {
typedef enum {
  kNSStateIdle = 0,
  kNSStateNeedControlAdapter = 1,
  kNSStateNeedDataAdapter = 2,
  kNSStateReady = 3,
  kNSStateIncreasing = 4,
  kNSStateDecreasing = 5
} NSState;

class NetworkSwitcher {
  public:
    void run(void);
    NSState get_state(void) {
      std::unique_lock<std::mutex> lck(this->mStateLock);
      return this->mState;
    }

    void run_switcher(void);
    void monitor(int &avg_send_request_speed,
        int& avg_send_queue_data_size, int& avg_total_bandwidth_now);
    void check_and_handover(int avg_send_request_speed,
        int avg_send_queue_data_size, int avg_total_bandwidth_now);

    bool check_increase_adapter(int send_request_speed, int send_queue_data_size);
    bool check_decrease_adapter(int bandwidth_now, int bandwidth_when_increasing);

    void control_adapter_ready(void) {
      std::unique_lock<std::mutex> lck(this->mStateLock);
      switch(this->mState) {
        case kNSStateNeedControlAdapter:
          this->mState = kNSStateNeedDataAdapter;
          break;
        case kNSStateNeedDataAdapter:
        case kNSStateReady:
        case kNSStateIncreasing:
        case kNSStateDecreasing:
          break;
      }
    }

    void done_switch(void) {
      std::unique_lock<std::mutex> lck(this->mStateLock);
      switch(this->mState) {
        case kNSStateIncreasing:
        case kNSStateDecreasing:
          this->mState = kNSStateReady;
          break;
        case kNSStateNeedControlAdapter:
        case kNSStateNeedDataAdapter:
        case kNSStateReady:
          break;
      }
    }

    /* Singleton */
    static NetworkSwitcher* get_instance(void) {
      if(NetworkSwitcher::singleton == NULL) {
        NetworkSwitcher::singleton = new NetworkSwitcher();
      }
      return NetworkSwitcher::singleton;
    }

  private:
#define METRIC_WINDOW_LENGTH 8

    /* Singleton */
    static NetworkSwitcher* singleton;
    NetworkSwitcher(void) {
      this->mThread = NULL;
      this->mState = kNSStateIdle;
      this->mBandwidthWhenIncreasing = 0;
      this->mDecreasingCheckCount = 0;

      for(int i=0; i<METRIC_WINDOW_LENGTH; i++) {
        this->mSendRequestSpeedValues[i] = 0;
        this->mSendQueueDataSizeValues[i] = 0;
        this->mTotalBandwidthNowValues[i] = 0;
      }
      this->mValuesCursor = 0;
    }

    void put_values(int send_request_speed, int send_queue_data_size,
        int total_bandwidth_now) {
      this->mSendRequestSpeedValues[this->mValuesCursor] = send_request_speed;
      this->mSendQueueDataSizeValues[this->mValuesCursor] = send_queue_data_size;
      this->mTotalBandwidthNowValues[this->mValuesCursor] = total_bandwidth_now;
      this->mValuesCursor = (this->mValuesCursor + 1) % METRIC_WINDOW_LENGTH;
    }

    int get_average_send_request_speed() {
      int average = 0;
      for(int i=0; i<METRIC_WINDOW_LENGTH; i++) {
        average += this->mSendRequestSpeedValues[i];
      }
      average = average / METRIC_WINDOW_LENGTH;
      return average;
    }

    int get_average_send_queue_data_size() {
      int average = 0;
      for(int i=0; i<METRIC_WINDOW_LENGTH; i++) {
        average += this->mSendQueueDataSizeValues[i];
      }
      average = average / METRIC_WINDOW_LENGTH;
      return average;
    }

    int get_average_total_bandwidth_now() {
      int average = 0;
      for(int i=0; i<METRIC_WINDOW_LENGTH; i++) {
        average += this->mTotalBandwidthNowValues[i];
      }
      average = average / METRIC_WINDOW_LENGTH;
      return average;
    }

    std::thread *mThread;

    NSState mState;
    std::mutex mStateLock;
    int mBandwidthWhenIncreasing;
    int mDecreasingCheckCount;

    int mSendRequestSpeedValues[METRIC_WINDOW_LENGTH];
    int mSendQueueDataSizeValues[METRIC_WINDOW_LENGTH];
    int mTotalBandwidthNowValues[METRIC_WINDOW_LENGTH];
    int mValuesCursor;
};
} /* namespace cm */

#endif /* INC_NETWORK_SWITCHER_H_ */
