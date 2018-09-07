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
#include <DebugLog.h>
#include <ExpConfig.h>

#include <mutex>
#include <thread>

namespace sc {
typedef enum {
  kNSStateInitialized = 0,
  kNSStateRunning = 1,
  kNSStateSwitching = 2,
} NSState;

typedef enum {
  kNSModeEnergyAware = 0,  /* WearDrive-like */
  kNSModeLatencyAware = 1, /* Selective Connection Unique */
  kNSModeCapDynamic = 2    /* CoolSpots */
} NSMode;

class SwitchAdapterTransaction {
  /*
   * Switch Adapter Transaction: Order
   * 1. Entry
   *    - NetworkSwitcher.switch_adapters()
   *    - SwitchAdapterTransaction.start()
   * 2-a. Connect/WakeUp Next Data Adapter
   *    - next_data_adapter.connect()
   *       or next_data_adapter.wake_up()
   * 2-b. Callback (for connect request)
   *    - SwitchAdapterTransaction.connect_data_callback()
   * 3-a. Disconnect/Sleep Prev Data Adapter
   *    - prev_adapter.disconnect()
   * 3-b. Callback (for disconnect request)
   *    - SwitchAdapterTransaction.disconnect_callback()
   * 4-a. Connect/WakeUp Next Control Adapter
   *    - next_adapter.connect()
   * 4-b. Callback (for connect request)
   *    - SwitchAdapterTransaction.connect_callback()
   * 5-a. Disconnect/Sleep Prev Control Adapter
   *    - prev_adapter.disconnect()
   *      or prev_adapter.sleep()
   * 5-b. Callback (for disconnect request)
   *    - SwitchAdapterTransaction.disconnect_callback()
   * 6. NetworkSwitcher.done_switch()
   */
public:
  static bool run(int prev_index, int next_index);
  void start(void);
  static void connect_next_data_callback(bool is_success);
  static void disconnect_prev_data_callback(bool is_success);
  static void connect_next_control_callback(bool is_success);
  static void disconnect_prev_control_callback(bool is_success);

protected:
  void done(bool is_success);

  SwitchAdapterTransaction(int prev_index, int next_index) {
    this->mPrevIndex = prev_index;
    this->mNextIndex = next_index;
  }
  static SwitchAdapterTransaction *sOngoing;

  int mPrevIndex;
  int mNextIndex;
};

class ConnectRequestTransaction {
public:
  static bool run(int adapter_id);
  bool start(void);
  static void connect_callback(bool is_success);

protected:
  void done();

  ConnectRequestTransaction(int adapter_id) { this->mAdapterId = adapter_id; }
  static ConnectRequestTransaction *sOngoing;

  int mAdapterId;
};

class ReconnectControlAdapterTransaction {
public:
  static bool run();
  bool start();
  static void disconnect_callback(bool is_success);
  static void connect_callback(bool is_success);

protected:
  void done(bool require_restart);
  // static void on_fail(bool is_restart);

  ReconnectControlAdapterTransaction() {}
  static ReconnectControlAdapterTransaction *sOngoing;
};

class Stats {
public:
  /* Statistics used to print present status */
  float ema_queue_arrival_speed = 0;

  /* Statistics used in CoolSpots Policy */
  int now_total_bandwidth = 0;

  /* Statistics used in Energy-aware & Latency-aware Policy */
  float ema_send_request_size = 0;
  float ema_arrival_time_us = 0;
  int now_queue_data_size = 0;
};

class Core;
class NetworkSwitcher {
public:
  /* Control netwowrk switcher thread */
  void start(void);
  void stop(void);

public:
  /* APIs called by peer through Core */
  void connect_adapter(int adapter_id);
  void sleep_adapter(int adapter_id);
  void wake_up_adapter(int adapter_id);
  void reconnect_control_adapter(void);

private:
  /* Notification of switch done event */
  void done_switch() {
    LOG_VERB("Switch adapter end!");
    NSState state = this->get_state();
    switch (state) {
    case NSState::kNSStateSwitching:
      this->set_state(NSState::kNSStateRunning);
      break;
    case NSState::kNSStateInitialized:
    case NSState::kNSStateRunning:
      break;
    }
  }

private:
  /* Network switcher thread */
  void switcher_thread(void);
  std::thread *mThread;
  bool mSwitcherThreadOn;

  /* Checking statistics and decide switching */
  void get_stats(Stats &stats);
  void print_stats(Stats &stats);
  void check_and_decide_switching(Stats &stats);

private:
  /* Switch adapters */
  bool increase_adapter(void);
  bool decrease_adapter(void);
  bool switch_adapters(int prev_index, int next_index);

private:
  /* Check policy-driven condition of increase/decrease */
  bool check_increase_adapter(const Stats &stats);
  bool check_decrease_adapter(const Stats &stats);

  /* Check condition of increase/decrease */
  bool is_increaseable(void);
  bool is_decreaseable(void);

  /* Get payoff points */
  int get_init_energy_payoff_point(void);
  int get_idle_energy_payoff_point(int avg_arrival_time_us);
  int get_init_latency_payoff_point(void);

  /* Policy-related */
  /* CoolSpots Policy */
  int mBandwidthWhenIncreasing;
  int mDecreasingCheckCount;

  /* Auxiliary member variable for statistics */
  Counter mQueueArrivalSpeed; /* to achieve the ema of queue arrival speed */

public:
  /* State getter */
  NSState get_state(void) {
    std::unique_lock<std::mutex> lck(this->mStateLock);
    return this->mState;
  }

private:
  /* State setter */
  void set_state(NSState new_state) {
    std::unique_lock<std::mutex> lck(this->mStateLock);
    this->mState = new_state;
  }

  /* State */
  NSState mState;
  std::mutex mStateLock;

public:
  /* Mode getter */
  NSMode get_mode(void) {
    std::unique_lock<std::mutex> lck(this->mModeLock);
    NSMode mode = this->mMode;
    return mode;
  }

  /* Mode setter */
  void set_mode(NSMode new_mode) {
    std::unique_lock<std::mutex> lck(this->mModeLock);
    this->mMode = new_mode;
  }

private:
  /* Mode */
  NSMode mMode;
  std::mutex mModeLock;

public:
  /* Singleton */
  static NetworkSwitcher *get_instance(void) {
    if (NetworkSwitcher::singleton == NULL) {
      NetworkSwitcher::singleton = new NetworkSwitcher();
    }
    return NetworkSwitcher::singleton;
  }

private:
  /* Singleton */
  static NetworkSwitcher *singleton;
  NetworkSwitcher(void) {
    this->mSwitcherThreadOn = false;
    this->mThread = NULL;
    this->set_state(NSState::kNSStateInitialized);
    this->mBandwidthWhenIncreasing = 0;
    this->mDecreasingCheckCount = 0;
    this->set_mode(NSMode::kNSModeEnergyAware);
  }

public:
  /*
   * Its private members can be accessed by auxiliary classes
   */
  friend SwitchAdapterTransaction;
  friend ConnectRequestTransaction;
  friend ReconnectControlAdapterTransaction;
};
} /* namespace sc */

#endif /* INC_NETWORK_SWITCHER_H_ */
