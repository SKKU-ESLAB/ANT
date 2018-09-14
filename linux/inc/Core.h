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

#ifndef INC_COMMUNICATOR_H_
#define INC_COMMUNICATOR_H_

#include <APIInternal.h>
#include <ArrivalTimeCounter.h>
#include <SegmentManager.h>
#include <ServerAdapter.h>

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
};

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
};

/*
 * Control Request Code
 * It is used to classify "control request message" that is transferred to the
 * peer.
 *  - Commands: "Connect", "Sleep", "WakeUp", "Disconnect"
 *  - Acks: "Disconnect"
 *  - Private Data: "Priv"
 */
typedef enum {
  kCtrlReqConnect = 1,
  kCtrlReqSleep = 2,
  kCtrlReqWakeUp = 3,
  kCtrlReqDisconnect = 4,
  kCtrlReqPriv = 10,
  kCtrlReqDisconnectAck = 24
} CtrlReq;

typedef enum {
  kPrivTypeWFDInfo = 1,
  kPrivTypeUnknown = 999
} PrivType;

/* Core State */
typedef enum {
  kCMStateIdle = 0,
  kCMStateStarting = 1,
  kCMStateReady = 2,
  kCMStateStopping = 3
} CMState;

class ControlMessageListener {
public:
  virtual void on_receive_control_message(PrivType priv_type, void *data,
                                          size_t len) = 0;
};

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

  void register_control_adapter(ServerAdapter *adapter);
  void register_data_adapter(ServerAdapter *adapter);

  int send(const void *dataBuffer, uint32_t dataLength);
  int receive(void **pDataBuffer);

public:
  // TODO: Add AdapterPair class
  // data adapter & control adapter need to be tied in pair(couple).

  ServerAdapter *find_adapter_by_id(int adapter_id) {
    ServerAdapter *adapter = NULL;
    adapter = this->find_data_adapter_by_id(adapter_id);
    if (adapter == NULL)
      adapter = this->find_control_adapter_by_id(adapter_id);
    return adapter;
  }

  /* Handling data adapters */
  ServerAdapter *get_data_adapter(int index) {
    std::unique_lock<std::mutex> lck(this->mDataAdaptersLock);
    return this->mDataAdapters.at(index);
  }
  ServerAdapter *get_active_data_adapter() {
    std::unique_lock<std::mutex> lck(this->mDataAdaptersLock);
    assert(this->mActiveAdapterIndex < this->mDataAdapters.size());
    return this->mDataAdapters.at(this->mActiveAdapterIndex);
  }
  ServerAdapter *find_data_adapter_by_id(int adapter_id) {
    std::unique_lock<std::mutex> lck(this->mDataAdaptersLock);
    for (std::vector<ServerAdapter *>::iterator it =
             this->mDataAdapters.begin();
         it != this->mDataAdapters.end(); it++) {
      ServerAdapter *adapter = *it;
      if (adapter->get_id() == adapter_id) {
        return adapter;
      }
    }
    return NULL;
  }
  int get_data_adapter_count(void) {
    std::unique_lock<std::mutex> lck(this->mDataAdaptersLock);
    return this->mDataAdapters.size();
  }

  /* Handling control adapters */
  ServerAdapter *get_control_adapter(int index) {
    std::unique_lock<std::mutex> lck(this->mControlAdaptersLock);
    return this->mControlAdapters.at(index);
  }
  ServerAdapter *get_active_control_adapter() {
    std::unique_lock<std::mutex> lck(this->mControlAdaptersLock);
    assert(this->mActiveAdapterIndex < this->mControlAdapters.size());
    return this->mControlAdapters.at(this->mActiveAdapterIndex);
  }
  ServerAdapter *find_control_adapter_by_id(int adapter_id) {
    std::unique_lock<std::mutex> lck(this->mControlAdaptersLock);
    for (std::vector<ServerAdapter *>::iterator it =
             this->mControlAdapters.begin();
         it != this->mControlAdapters.end(); it++) {
      ServerAdapter *adapter = *it;
      if (adapter->get_id() == adapter_id) {
        return adapter;
      }
    }
    return NULL;
  }
  int get_control_adapter_count(void) {
    std::unique_lock<std::mutex> lck(this->mControlAdaptersLock);
    return this->mControlAdapters.size();
  }

  /* Handling adapter index */
  int get_active_adapter_index(void) { return this->mActiveAdapterIndex; }
  void set_active_adapter_index(int active_control_adapter_index) {
    this->mActiveAdapterIndex = active_control_adapter_index;
  }

public:
  /* Control message handling (External) */
  void send_request_connect(uint16_t adapter_id);
  void send_request_disconnect(uint16_t adapter_id);
  void send_request_disconnect_ack(uint16_t adapter_id);
  void send_request_sleep(uint16_t adapter_id);
  void send_request_wake_up(uint16_t adapter_id);
  void send_noti_private_data(PrivType priv_type, char *private_data_buf,
                              uint32_t private_data_len);

private:
  /* Control message handling (Internal) */
  void send_control_message(const void *dataBuffer, size_t dataLength);
  void send_request(CtrlReq request_code, uint16_t adapter_id);

public:
  /* Handling control message (private data) listener */
  void add_control_message_listener(ControlMessageListener *listener) {
    this->mControlMessageListeners.push_back(listener);
  }
  static void control_adapter_receive_loop(ServerAdapter *adapter);
  static int __control_adapter_receive_loop(ServerAdapter* adapter);

public:
  /* Get statistics */
  int get_total_bandwidth(void) {
    int num_adapters = 0;
    int now_total_bandwidth = 0;
    /* Statistics from control adapter */
    {
      ServerAdapter *adapter = this->get_active_control_adapter();
      int bandwidth_up = adapter->get_bandwidth_up();
      int bandwidth_down = adapter->get_bandwidth_down();
      now_total_bandwidth += (bandwidth_up + bandwidth_down);
      num_adapters++;
    }

    /* Statistics from data adapters */
    int data_adapter_count = this->get_data_adapter_count();
    for (int i = 0; i < data_adapter_count; i++) {
      ServerAdapter *adapter = this->get_data_adapter(i);
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
   *  - N Data Adapters (+ access lock)
   *  - 1 Control Adapter (+ access lock)
   */
  std::vector<ServerAdapter *> mDataAdapters;
  std::vector<ServerAdapter *> mControlAdapters;
  std::mutex mDataAdaptersLock;
  std::mutex mControlAdaptersLock;

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
  /* Control Message Listeners */
  std::vector<ControlMessageListener *> mControlMessageListeners;

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
};
} /* namespace sc */
#endif /* INC_COMMUNICATOR_H_ */
