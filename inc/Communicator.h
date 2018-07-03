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

#include <SegmentManager.h>
#include <ServerAdapter.h>

#include <stdint.h>
#include <vector>
#include <mutex>

namespace cm {

enum CommErr {
  kProtOk = 0,
  kProtErr = -1,
};

/*
 * Control Request Code
 * It is used to classify "control request message" that is transferred to the peer.
 *  - Commands: "Connect", "Disconnect"
 *  - Acks: "Ok", "Fail"
 *  - Private Data: "Priv"
 */
typedef enum {
  kCtrlReqOk = 0,
  kCtrlReqFail = 1,
  kCtrlReqConnect = 2,
  kCtrlReqDisconnect = 3,
  kCtrlReqPriv = 4 
} CtrlReq;

/*
 * Communicator State
 */
typedef enum {
  kCMStateIdle = 0,
  kCMStateStarting = 1,
  kCMStateReady = 2,
  kCMStateConnecting = 3,
  kCMStateDisconnecting = 4,
  kCMStateStopping = 5
} CMState;

/*
 * Transactions
 * Series of asynchronous callbacks, especially used for connection/disconnection callbacks.
 */
class Communicator;
class StartCommunicatorTransaction {
public:
  static bool start(Communicator* caller);
  static void connect_control_adapter_callback(bool is_success);
  static void connect_first_data_adapter_callback(bool is_success);
protected:
  static bool sIsOngoing;
  static Communicator* sCaller;
};

class StopCommunicatorTransaction {
public:
  static bool start(Communicator* caller);
  static void disconnect_control_adapter_callback(bool is_success);
  static void disconnect_data_adapter_callback(bool is_success);
protected:
  static bool sIsOngoing;
  static Communicator* sCaller;
  static int sDataAdaptersCount;
  static std::mutex sDataAdaptersCountLock;
};

class SwitchAdapterTransaction {
  /*
   * Switch Adapter Transaction: Order
   * 1. Communicator.switch_adapters()
   * 2. SwitchAdapterTransaction.start()
   * 3. next_adapter.connect()
   * 4. SwitchAdapterTransaction.connect_callback()
   * 5. prev_adapter.disconnect()
   * 6. SwitchAdapterTransaction.disconnect_callback()
   * 7. NetworkSwitcher.done_switch()
   */
public:
  static bool start(Communicator* caller, int prev_index, int next_index);
  static void connect_callback(bool is_success);
  static void disconnect_callback(bool is_success);

protected:
  static Communicator* sCaller;
  static bool sIsOngoing;
  static int sPrevIndex;
  static int sNextIndex;
};

class ConnectRequestTransaction {
public:
  static bool start(Communicator* caller, int adapter_id);
  static void connect_callback(bool is_success);
protected:
  static Communicator* sCaller;
  static bool sIsOngoing;
  static int sAdapterId;
};

class DisconnectRequestTransaction {
public:
  static bool start(Communicator* caller, int adapter_id);
  static void disconnect_callback(bool is_success);
protected:
  static Communicator* sCaller;
  static bool sIsOngoing;
  static int sAdapterId;
};

class ReconnectControlAdapterTransaction {
public:
  static bool start(Communicator* caller);
  static void disconnect_callback(bool is_success);
  static void connect_callback(bool is_success);
protected:
  static Communicator* sCaller;
  static bool sIsOngoing;
};

class ControlMessageListener {
public:
  virtual void on_receive_control_message(int adapter_id, void* data, size_t len) = 0;
};

class ServerAdapter;

/*
 * Communicator
 */
class Communicator {
public:
  /*
   * APIs
   * These functions are mapped to ones in API.h
   */
  friend StartCommunicatorTransaction;
  friend StopCommunicatorTransaction;
  bool start(void);
  void done_start(bool is_success);
  bool stop(void);
  void done_stop(bool is_success);

  void register_control_adapter(ServerAdapter* adapter);
  void register_data_adapter(ServerAdapter* adapter);

  int send(const void *buf, uint32_t len);
  int receive(void **buf);

  bool increase_adapter(void);
  bool decrease_adapter(void);

  bool is_increaseable(void) {
    std::unique_lock<std::mutex> lck(this->mDataAdaptersLock);
    return ((this->mDataAdapterCount > 1)
        && (this->mActiveDataAdapterIndex < (this->mDataAdapterCount - 1)));
  }
  bool is_decreaseable(void) {
    std::unique_lock<std::mutex> lck(this->mDataAdaptersLock);
    return ((this->mDataAdapterCount > 1)
        && (this->mActiveDataAdapterIndex > 0));
  }

  CMState get_state(void) {
    std::unique_lock<std::mutex> lck(this->mStateLock);
    return this->mState;
  }
  
  /* Handling adapters */
  ServerAdapter* get_data_adapter(int index) {
    std::unique_lock<std::mutex> lck(this->mDataAdaptersLock);
    return this->mDataAdapters.at(index);
  }
  ServerAdapter* find_data_adapter_by_id(int adapter_id) {
    std::unique_lock<std::mutex> lck(this->mDataAdaptersLock);
    for(std::vector<ServerAdapter*>::iterator it = this->mDataAdapters.begin();
        it != this->mDataAdapters.end();
        it++) {
      ServerAdapter* adapter = *it;
      if(adapter->get_id() == adapter_id) {
        return adapter;
      }
    }
    return NULL;
  }
  ServerAdapter* get_control_adapter() {
    return this->mControlAdapter;
  }
  int get_data_adapter_count(void) {
    return this->mDataAdapterCount;
  }

  /* Control message handling */
  void send_control_message(const void *data, size_t len);
  void send_private_control_data(uint16_t adapter_id, char* private_data_buf, uint32_t private_data_len);
  void add_control_message_listener(ControlMessageListener* listener) {
    this->mControlMessageListeners.push_back(listener);
  }
  static void receive_control_message_loop(ServerAdapter* adapter);

  /* Singleton */
  static Communicator* get_instance(void) {
    if (singleton == NULL) {
      singleton = new Communicator();
    }
    return singleton;
  }

  ~Communicator() {
    SegmentManager::get_instance()->free_segment_all();
  }

private:
  /* Main function to switch adapters */
  friend SwitchAdapterTransaction;
  bool switch_adapters(int prev_index, int next_index);

  void set_state(CMState new_state) {
    std::unique_lock<std::mutex> lck(this->mStateLock);
    this->mState = new_state;
  }

  /* Singleton */
  static Communicator* singleton;
  Communicator(void) {
    SegmentManager *sm = SegmentManager::get_instance();
    this->mState = kCMStateIdle;
  }

  CMState mState;
  std::mutex mStateLock;
  
  /*
   * Active Data Adapter Index means the index value indicating
   * 'conencted' or 'connecting' data adapter currently.
   * Only "the current data adapter" is 'connected' or 'connecting',
   * but the others are 'connected(but to-be-disconnected)', 'disconnected' or 'disconnecting'.
   * This index is changed right before increasing or decreasing starts.
   */
  int mActiveDataAdapterIndex = 0;

  /*
   * Adapter List
   *  - N Data Adapters (+ access lock)
   *  - 1 Control Adapter (+ access lock)
   */
  std::vector<ServerAdapter*> mDataAdapters;
  ServerAdapter* mControlAdapter = NULL;
  std::mutex mDataAdaptersLock;
  std::mutex mControlAdapterLock;
  int mDataAdapterCount = 0;

  /* Control Message Listeners */
  std::vector<ControlMessageListener*> mControlMessageListeners;
};

} /* namespace cm */
#endif  /* INC_COMMUNICATOR_H_ */
