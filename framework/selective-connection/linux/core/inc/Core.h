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
#include "ControlMessageReceiver.h"
#include "ControlMessageSender.h"
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
  static void connect_first_adapter_callback(ServerAdapter *adapter,
                                             bool is_success);

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
  static void disconnect_adapter_callback(ServerAdapter *adapter,
                                          bool is_success);

private:
  void done(bool is_success);

  StopCoreTransaction(Core *caller) { this->mCaller = caller; }
  static StopCoreTransaction *sOngoing;

  Core *mCaller;

  int mAdaptersCount;
  std::mutex mAdaptersCountLock;
}; /* class StopCoreTransaction */

/* Core State */
typedef enum {
  kCoreStateIdle = 0,
  kCoreStateStarting = 1,
  kCoreStateReady = 2,
  kCoreStateStopping = 3
} CoreState; /* enum CoreState */

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
  /* Control Message Receiver/Sender Getter */
  ControlMessageSender *get_control_sender(void) {
    return this->mControlMessageSender;
  }
  ControlMessageReceiver *get_control_receiver(void) {
    return this->mControlMessageReceiver;
  }

private:
  /* Control Message Receiver/Sender */
  ControlMessageSender *mControlMessageSender;
  ControlMessageReceiver *mControlMessageReceiver;

public:
  /* Get statistics */
  int get_total_bandwidth() {
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

  float get_ema_send_rtt() { return this->mSendRTT.get_em_average(); }

  void set_send_rtt(int send_rtt) { this->mSendRTT.set_value(send_rtt); }

  float get_ema_media_rtt() { return this->mMediaRTT.get_em_average(); }

  void set_media_rtt(int media_rtt) { this->mMediaRTT.set_value(media_rtt); }

private:
  /* Statistics */
  Counter mSendRTT;
  Counter mMediaRTT;

public:
  /* State getter */
  CoreState get_state(void) {
    std::unique_lock<std::mutex> lck(this->mStateLock);
    return this->mState;
  }

private:
  /* State setter */
  void set_state(CoreState new_state) {
    std::unique_lock<std::mutex> lck(this->mStateLock);
    this->mState = new_state;
  }

  /* State */
  CoreState mState;
  std::mutex mStateLock;

public:
  /* Adapter list handling */
  ServerAdapter *find_adapter_by_id(int adapter_id) {
    std::unique_lock<std::mutex> lck(this->mAdaptersLock);
    for (std::vector<ServerAdapter *>::iterator it = this->mAdapters.begin();
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

private:
  /* Adapter list */
  std::vector<ServerAdapter *> mAdapters;
  std::mutex mAdaptersLock;
  int mActiveAdapterIndex;

private:
  /* Statistics */
  Counter mSendRequestSize;
  ArrivalTimeCounter mSendArrivalTime;

public:
  /* Singleton */
  static Core *singleton(void) {
    if (sSingleton == NULL) {
      sSingleton = new Core();
    }
    return sSingleton;
  }

  ~Core() { SegmentManager::singleton()->free_segment_all(); }

private:
  /* Singleton */
  static Core *sSingleton;
  Core(void) {
    SegmentManager *sm = SegmentManager::singleton();
    this->mState = kCoreStateIdle;
    this->mActiveAdapterIndex = 0;
    this->mControlMessageReceiver = new ControlMessageReceiver();
    this->mControlMessageSender = new ControlMessageSender();
  }

public:
  /* Its private members can be accessed by auxiliary classes */
  friend StartCoreTransaction;
  friend StopCoreTransaction;
}; /* class Core */
} /* namespace sc */
#endif /* !defined(__SC_CORE_H__) */
