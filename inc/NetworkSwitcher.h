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

#include <Core.h>

#include <thread>
#include <mutex>

#define kSegThreshold 512
#define kSegQueueThreshold 50*(kSegThreshold / 512)

namespace cm {
typedef enum {
  kNSStateInitialized = 0,
  kNSStateRunning = 1,
  kNSStateSwitching = 2,
} NSState;

class SwitchAdapterTransaction {
  /*
   * Switch Adapter Transaction: Order
   * 1. NetworkSwitcher.switch_adapters()
   * 2. SwitchAdapterTransaction.start()
   * 3. next_adapter.connect()
   * 4. SwitchAdapterTransaction.connect_callback()
   * 5. prev_adapter.disconnect()
   * 6. SwitchAdapterTransaction.disconnect_callback()
   * 7. NetworkSwitcher.done_switch()
   */
public:
  static bool start(int prev_index, int next_index);
  static void connect_callback(bool is_success);
  static void disconnect_callback(bool is_success);

protected:
  static bool sIsOngoing;
  static int sPrevIndex;
  static int sNextIndex;
};

class ConnectRequestTransaction {
public:
  static bool start(int adapter_id);
  static void connect_callback(bool is_success);
protected:
  static void on_fail(void);

  static bool sIsOngoing;
  static int sAdapterId;
};

class ReconnectControlAdapterTransaction {
public:
  static bool start();
  static void disconnect_callback(bool is_success);
  static void connect_callback(bool is_success);
protected:
  static void on_fail(bool is_restart);

  static bool sIsOngoing;
};

class Core;
class NetworkSwitcher {
  public:
    /* Control netwowrk switcher thread */
    void start(void);
    void stop(void);

    NSState get_state(void) {
      std::unique_lock<std::mutex> lck(this->mStateLock);
      return this->mState;
    }

    void switcher_thread(void);

    /*
     * Connect adapter command.
     * It is called by peer.
     */
    void connect_adapter(int adapter_id);
    void reconnect_control_adapter(void);

    /* Notification of switch done event */
    void done_switch(void) {
      NSState state = this->get_state();
      switch(state) {
        case NSState::kNSStateSwitching:
          this->set_state(NSState::kNSStateRunning);
          break;
        case NSState::kNSStateInitialized:
        case NSState::kNSStateRunning:
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
      this->mSwitcherThreadOn = false;
      this->mThread = NULL;
      this->set_state(NSState::kNSStateInitialized);
      this->mBandwidthWhenIncreasing = 0;
      this->mDecreasingCheckCount = 0;
      this->mActiveDataAdapterIndex = 0;

      for(int i=0; i<METRIC_WINDOW_LENGTH; i++) {
        this->mSendRequestSpeedValues[i] = 0;
        this->mSendQueueDataSizeValues[i] = 0;
        this->mTotalBandwidthNowValues[i] = 0;
      }
      this->mValuesCursor = 0;
    }

    /* Monitoring */
    void monitor(int &avg_send_request_speed,
        int& avg_send_queue_data_size, int& avg_total_bandwidth_now);
    void check_and_handover(int avg_send_request_speed,
        int avg_send_queue_data_size, int avg_total_bandwidth_now);

    bool check_increase_adapter(int send_request_speed, int send_queue_data_size);
    bool check_decrease_adapter(int bandwidth_now, int bandwidth_when_increasing);

    /* Switch adapters */
    bool increase_adapter(void);
    bool decrease_adapter(void);
    bool switch_adapters(int prev_index, int next_index);
    bool is_increaseable(void);
    bool is_decreaseable(void);

  private:
    /*
     * Active Data Adapter Index means the index value indicating
     * 'conencted' or 'connecting' data adapter currently.
     * Only "the current data adapter" is 'connected' or 'connecting',
     * but the others are 'connected(but to-be-disconnected)', 'disconnected' or 'disconnecting'.
     * This index is changed right before increasing or decreasing starts.
     */
    int mActiveDataAdapterIndex;
  public:
    int get_active_data_adapter_index(void) {
      return this->mActiveDataAdapterIndex;
    }

    void set_active_data_adapter_index(int active_data_adapter_index) {
      this->mActiveDataAdapterIndex = active_data_adapter_index;
    }

  private:
    /* Values */
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

    void set_state(NSState new_state) {
      std::unique_lock<std::mutex> lck(this->mStateLock);
      this->mState = new_state;
    }

    std::thread *mThread;

    bool mSwitcherThreadOn;
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
