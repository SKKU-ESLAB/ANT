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

#include <segment_manager.h>
#include <network_manager.h>
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
  bool start(Communicator* caller, int prev_index, int next_index);
  void connect_callback(bool is_success);
  void disconnect_callback(bool is_success);

  SwitchAdapterTransaction() {
  }

protected:
  static bool sIsOngoing;
  static int sPrevIndex;
  static int sNextIndex;
};

class ConnectTransaction {
public:
  bool start(Communicator* caller, int adapter_id);
  void connect_callback(bool is_success);
};

class DisonnectTransaction {
public:
  bool start(Communicator* caller, int adapter_id);
  void disconnect_callback(bool is_success);
};

class ControlMessageListener {
public:
  virtual void on_receive_control_message(int adapter_id, void* data, size_t len);
};

class ServerAdapter;
class Communicator {
public:
  /**
   * If data size is big, it is recommanded to use following
   * libraries in a thread
   */
  int send(const void *buf, uint32_t len);

  /**
   * @param len: IN buffer length
   * @param buf: OUT buffer read
   * @return: Received bytes(<0 if error)
   */
  int receive(void **buf);

  void register_control_adapter(ServerAdapter* adapter);
  void register_data_adapter(ServerAdapter* adapter);

  void start(void);
  void stop(void);

  bool increase_adapter(void);
  bool decrease_adapter(void);
  
  ServerAdapter* get_data_adapter(int index) {
    return this->mDataAdapters.at(index);
  }
  ServerAdapter* get_control_adapter() {
    return this->mControlAdapter;
  }

  void send_control_data(const void *data, size_t len);

  void finalize(void) {
    SegmentManager::get_instance()->free_segment_all();
  }

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

private:
  bool switch_adapters(int prev_index, int next_index);

  /* Singleton */
  static Communicator* singleton;
  Communicator(void) {
    SegmentManager *sm = SegmentManager::get_instance();
    NetworkManager *nm = NetworkManager::get_instance();
  }
  
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

  /* Control Message Listeners */
  std::vector<ControlMessageListener*> mControlMessageListeners;
};

} /* namespace cm */
#endif  /* INC_COMMUNICATOR_H_ */
