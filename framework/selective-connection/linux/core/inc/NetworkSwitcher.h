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

#ifndef __NETWORK_SWITCHER_H__
#define __NETWORK_SWITCHER_H__

#include "Core.h"
#include "ServerAdapter.h"

#include "../../common/inc/DebugLog.h"

#include "../../configs/ExpConfig.h"

#include <mutex>
#include <stdint.h>
#include <sys/time.h>
#include <thread>
#include <vector>

namespace sc {
class Core;
class NetworkSwitcher;

/* Transctions in NetworkSwitcher */
class SwitchAdapterTransaction {
  /*
   * Switch Adapter Transaction: Order
   * 0. Entry
   *    - NetworkSwitcher.switch_adapters()
   *    - SwitchAdapterTransaction.start()
   * 1-a. Connect/WakeUp Next Adapter
   *    - next_adapter.connect_or_wake_up()
   * 1-b. Callback (for connect request)
   *    - SwitchAdapterTransaction.connect_next_adapter_callback()
   * 2-a. Sleep Prev Adapter
   *    - prev_adapter.sleep()
   * 2-b. Callback (for sleep request)
   *    - SwitchAdapterTransaction.sleep_prev_adapter_callback()
   * 3-a. Disconnect Prev Adapter
   *    - prev_adapter.disconnect()
   * 3-b. Callback (for disconnect request)
   *    - SwitchAdapterTransaction.disconnect_prev_adapter_callback()
   * 4. SwitchAdapterTransaction.done()
   *    NetworkSwitcher.done_switch()
   */
public:
  static bool run(int prev_index, int next_index);
  void start(void);
  static void connect_next_adapter_callback(ServerAdapter *adapter,
                                            bool is_success);
  static void sleep_prev_adapter_callback(ServerAdapter *adapter,
                                          bool is_success);
  static void disconnect_prev_adapter_callback(ServerAdapter *adapter,
                                               bool is_success);

protected:
  void done(bool is_success);

  SwitchAdapterTransaction(int prev_index, int next_index) {
    this->mPrevIndex = prev_index;
    this->mNextIndex = next_index;
  }
  static SwitchAdapterTransaction *sOngoing;

  int mPrevIndex;
  int mNextIndex;

protected:
  /*
   * Connect -> Sleep -> Disconnect
   * Connect Latency
   * Sleep Latency
   * Disconnect Latency
   */
  void setConnectStartTS(void) { gettimeofday(&this->mConnectStartTS, NULL); }
  void setSleepStartTS(void) { gettimeofday(&this->mSleepStartTS, NULL); }
  void setDisconnectStartTS(void) { gettimeofday(&this->mDisconnectStartTS, NULL); }
  void setDisconnectEndTS(void) { gettimeofday(&this->mDisconnectEndTS, NULL); }
  uint64_t getTotalSwitchLatency(void) {
    uint64_t start = (uint64_t)this->mConnectStartTS.tv_sec * 1000 * 1000 +
                     this->mConnectStartTS.tv_usec;
    uint64_t end = (uint64_t)this->mDisconnectEndTS.tv_sec * 1000 * 1000 +
                   this->mDisconnectEndTS.tv_usec;
    return (end - start);
  }

  uint64_t getConnectLatency(void) {
    uint64_t start = (uint64_t)this->mConnectStartTS.tv_sec * 1000 * 1000 +
                     this->mConnectStartTS.tv_usec;
    uint64_t end = (uint64_t)this->mSleepStartTS.tv_sec * 1000 * 1000 +
                   this->mSleepStartTS.tv_usec;
    return (end - start);
  }

  uint64_t getSleepLatency(void) {
    uint64_t start = (uint64_t)this->mSleepStartTS.tv_sec * 1000 * 1000 +
                     this->mSleepStartTS.tv_usec;
    uint64_t end = (uint64_t)this->mDisconnectStartTS.tv_sec * 1000 * 1000 +
                   this->mDisconnectStartTS.tv_usec;
    return (end - start);
  }

  uint64_t getDisconnectLatency(void) {
    uint64_t start = (uint64_t)this->mDisconnectStartTS.tv_sec * 1000 * 1000 +
                     this->mDisconnectStartTS.tv_usec;
    uint64_t end = (uint64_t)this->mDisconnectEndTS.tv_sec * 1000 * 1000 +
                   this->mDisconnectEndTS.tv_usec;
    return (end - start);
  }

  struct timeval mConnectStartTS;
  struct timeval mSleepStartTS;
  struct timeval mDisconnectStartTS;
  struct timeval mDisconnectEndTS;
}; /* class SwitchAdapterTransaction */

class ConnectRequestTransaction {
public:
  static bool run(int adapter_id);
  bool start(void);
  static void connect_callback(ServerAdapter *adapter, bool is_success);

protected:
  void done();

  ConnectRequestTransaction(int adapter_id) { this->mAdapterId = adapter_id; }
  static ConnectRequestTransaction *sOngoing;

  int mAdapterId;
}; /* class ConnectRequestTransaction */

class ReconnectAdapterTransaction {
public:
  static bool run(ServerAdapter *targetAdapter);
  bool start();
  static void disconnect_callback(ServerAdapter *adapter, bool is_success);
  static void connect_callback(ServerAdapter *adapter, bool is_success);

protected:
  void done(bool require_restart);

  ReconnectAdapterTransaction(ServerAdapter *targetAdapter) {
    this->mTargetAdapter = targetAdapter;
  }
  static ReconnectAdapterTransaction *sOngoing;

  ServerAdapter *mTargetAdapter;
}; /* class ReconnectAdapterTransaction */

/* Netweork Switcher State */
typedef enum {
  kNSStateReady = 0,
  kNSStateSwitching = 1,
} NSState;

/* Network Switcher */
class NetworkSwitcher {
public:
  /* APIs called by peer through Core */
  void connect_adapter_by_peer(int adapter_id);
  void disconnect_adapter_by_peer(int adapter_id, uint32_t last_seq_no_control,
                                  uint32_t last_seq_no_data);
  void sleep_adapter_by_peer(int adapter_id);
  void wake_up_adapter_by_peer(int adapter_id);

  /* APIs called by receiver/sender loop */
  void reconnect_adapter(ServerAdapter *adapter,
                         bool retry_if_already_switching);

  /* APIs called by NetworkMonitor */
  bool switch_adapters(int prev_index, int next_index);

private:
  /* Notification of switch done event */
  void done_switch();

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
  /* Singleton */
  static NetworkSwitcher *singleton(void) {
    if (NetworkSwitcher::sSingleton == NULL) {
      NetworkSwitcher::sSingleton = new NetworkSwitcher();
    }
    return NetworkSwitcher::sSingleton;
  }

private:
  /* Singleton */
  static NetworkSwitcher *sSingleton;
  NetworkSwitcher() { this->set_state(NSState::kNSStateReady); }

public:
  /* Its private members can be accessed by auxiliary classes */
  friend SwitchAdapterTransaction;
  friend ConnectRequestTransaction;
  friend ReconnectAdapterTransaction;
}; /* class NetworkSwitcher */
} /* namespace sc */

#endif /* !defined(__NETWORK_SWITCHER_H__) */