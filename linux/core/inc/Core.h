/* Copyright 2017-2018 All Rights Reserved.
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *  Eunsoo Park (esevan.park@gmail.com)
 *  Injung Hwang (sinban04@gmail.com)
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

#ifndef __SC_CORE_H__
#define __SC_CORE_H__

#include "APIInternal.h"
#include "SegmentManager.h"
#include "ServerAdapter.h"

#include "../../common/inc/ArrivalTimeCounter.h"

#include <mutex>
#include <stdint.h>
#include <vector>

namespace sc {

class Core;
/*
 * Auxiliary Classes (Transactions)
 * Series of asynchronous callbacks, especially used for
 * connection/disconnection callbacks.
 */
class StartCoreTransaction {
public:
  static bool run(Core *caller);
  void start();
  static void connect_first_control_adapter_callback(bool is_success);
  static void connect_first_data_adapter_callback(bool is_success);

private:
  void done(bool is_success);

  StartCoreTransaction(Core *caller) { this->mCaller = caller; }
  static StartCoreTransaction *sOngoing;

  Core *mCaller;
}; /* class StartCoreTransaction */

class StopCoreTransaction {
public:
  static bool run(Core *caller);
  void start();
  static void disconnect_control_adapter_callback(bool is_success);
  static void disconnect_data_adapter_callback(bool is_success);

private:
  void done(bool is_success);

  StopCoreTransaction(Core *caller) { this->mCaller = caller; }
  static StopCoreTransaction *sOngoing;

  Core *mCaller;

  int mControlAdaptersCount;
  std::mutex mControlAdaptersCountLock;

  int mDataAdaptersCount;
  std::mutex mDataAdaptersCountLock;
}; /* class StopCoreTransaction */

/* Core State */
typedef enum {
  kCMStateIdle = 0,
  kCMStateStarting = 1,
  kCMStateReady = 2,
  kCMStateStopping = 3
} CMState; /* enum CMState */

class ServerAdapter;

/*
 * Core
 */
class Core {
public:
  /*
   * APIs
   * These functions are mapped to ones in API.h
   */
  void start();
  void done_start(bool is_success);
  void stop();
  void done_stop(bool is_success);

  void register_adapter(ServerAdapter *adapter);

  int send(const void *dataBuffer, uint32_t dataLength, bool is_control);
  int receive(void **pDataBuffer, bool is_control);

public:
  ServerAdapter *find_adapter_by_id(int adapter_id) {
    std::unique_lock<std::mutex> lck(this->mAdaptersLock);
    for (std::vector<ServerAdapter *>::iterator it =
             this->mAdapters.begin();
         it != this->mAdapters.end(); it++) {
      ServerAdapter *adapter = *it;
      if (adapter->get_id() == adapter_id) {
        return adapter;
      }
    }
    return NULL;
  }

  ServerAdapter *get_adapter(int index) {
    std::unique_lock<std::mutex> lck(this->mAdaptersLock);
    assert(index < this->mAdapters.size());
    return this->mAdapters.at(index);
  }

  ServerAdapter *get_active_adapter() {
    return this->get_adapter(this->get_active_adapter_index());
  }

  int get_adapter_count(void) {
    std::unique_lock<std::mutex> lck(this->mAdaptersLock);
    return this->mAdapters.size();
  }

  /* Handling adapter index */
  int get_active_adapter_index(void) { return this->mActiveAdapterIndex; }
  void set_active_adapter_index(int active_control_adapter_index) {
    this->mActiveAdapterIndex = active_control_adapter_index;
  }

public:
  /* Get statistics */
  int get_total_bandwidth(void) {
    int num_adapters = 0;
    int now_total_bandwidth = 0;
    /* Statistics from adapters */
    int adapter_count = this->get_adapter_count();
    for (int i = 0; i < adapter_count; i++) {
      ServerAdapter *adapter = this->get_adapter(i);
      if (adapter == NULL)
        continue;
      int bandwidth_up = adapter->get_bandwidth_up();
      int bandwidth_down = adapter->get_bandwidth_down();
      now_total_bandwidth += (bandwidth_up + bandwidth_down);
      num_adapters++;
    }
    return now_total_bandwidth;
  }
  float get_ema_send_request_size() {
    return this->mSendRequestSize.get_em_average();
  }
  float get_ema_send_arrival_time() {
    return this->mSendArrivalTime.get_em_average();
  }

public:
  /* State getter */
  CMState get_state(void) {
    std::unique_lock<std::mutex> lck(this->mStateLock);
    return this->mState;
  }

private:
  /* State setter */
  void set_state(CMState new_state) {
    std::unique_lock<std::mutex> lck(this->mStateLock);
    this->mState = new_state;
  }

  /* State */
  CMState mState;
  std::mutex mStateLock;

private:
  /*
   * Adapter List
   *  - N Adapters (+ access lock)
   */
  std::vector<ServerAdapter *> mAdapters;
  std::mutex mAdaptersLock;

  /*
   * Active Adapter Index means the index value indicating
   * 'conencted' or 'connecting' adapter currently.
   * Only "the current adapter" is 'connected' or 'connecting',
   * but the others are 'connected(but to-be-disconnected)', 'disconnected' or
   * 'disconnecting'. This index is changed right before increasing or
   * decreasing starts.
   */
  int mActiveAdapterIndex;

private:
  /* Statistics */
  Counter mSendRequestSize;
  ArrivalTimeCounter mSendArrivalTime;

public:
  /* Singleton */
  static Core *get_instance(void) {
    if (singleton == NULL) {
      singleton = new Core();
    }
    return singleton;
  }

  ~Core() { SegmentManager::get_instance()->free_segment_all(); }

private:
  /* Singleton */
  static Core *singleton;
  Core(void) {
    SegmentManager *sm = SegmentManager::get_instance();
    this->mState = kCMStateIdle;
    this->mActiveAdapterIndex = 0;
  }

public:
  /*
   * Its private members can be accessed by auxiliary classes
   */
  friend StartCoreTransaction;
  friend StopCoreTransaction;
}; /* class Core */
} /* namespace sc */
#endif /* !defined(__SC_CORE_H__) */
