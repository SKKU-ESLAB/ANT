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

#ifndef __SERVER_ADAPTER_H__
#define __SERVER_ADAPTER_H__

#include "Device.h"
#include "P2PServer.h"
#include "ReceiverThread.h"
#include "SenderThread.h"
#include "ServerAdapterState.h"
#include "ServerSocket.h"

#include "../../common/inc/Counter.h"

#include "../../configs/ExpConfig.h"

#include <condition_variable>
#include <mutex>
#include <vector>

#include <stdint.h>
#include <stdio.h>

namespace sc {

typedef void (*ConnectCallback)(ServerAdapter *adapter, bool is_success);
typedef void (*DisconnectCallback)(ServerAdapter *adapter, bool is_success);

class ServerAdapterStateListener;
class ReceiverThread;
class SenderThread;

class ServerAdapter {
public:
  /* Basic APIs
   *  - launch/finish sender/receiver threads
   *  - connect, disconnect, sleep, wake up
   *  - send, receive
   */
  /* Basic APIs related to launching/finishing sender/receiver threads */
  void launch_threads(void);
  void finish_thread(void);

  /* Basic APIs related to connection/sleeping */
  void connect(ConnectCallback callback, bool is_send_request);
  void disconnect_on_command(DisconnectCallback callback);
  void disconnect_on_peer_command(DisconnectCallback callback,
                                  uint32_t peer_last_seq_no_control,
                                  uint32_t peer_last_seq_no_data);
  void disconnect_on_failure(DisconnectCallback callback);
  void sleep(DisconnectCallback callback, bool is_send_request);
  void wake_up(ConnectCallback callback, bool is_send_request);
  void connect_or_wake_up(ConnectCallback callback, bool is_send_request);

  /* Basic APIs related to data transmission */
  int send(void *buf, size_t len);
  int receive(void *buf, size_t len);

private:
  /* Connect */
  void connect_internal(ConnectCallback callback);
  bool __connect_internal(void);
  ConnectCallback mConnectCallback = NULL;

  /* Disconnect */
  void disconnect_internal(DisconnectCallback callback);
  bool __disconnect_internal(ServerAdapterState oldState);
  DisconnectCallback mDisconnectCallback = NULL;

  /* Wake up */
  void wake_up_internal(void);

private:
  /* Sender Thread */
  SenderThread *mSenderThread = NULL;
  friend SenderThread;

  /* Receiver Thread */
  ReceiverThread *mReceiverThread = NULL;

public:
  /* Statistics getters */
  int get_bandwidth_up() {
    this->mSendDataSize.get_speed();
  }
  int get_bandwidth_down() {
    this->mReceiveDataSize.get_speed();
  }

private:
  /* Statistics */
  Counter mSendDataSize;
  Counter mReceiveDataSize;

public:
  /* Attribute getters */
  char *get_name(void) { return this->mName; }
  int get_id(void) { return this->mId; }
  bool is_disconnecting_on_purpose(void) {
    return this->mIsDisconnectingOnPurpose;
  }
  bool is_disconnecting_on_purpose_peer(void) {
    return this->mIsDisconnectingOnPurposePeer;
  }

  /* Component getters */
  Device *get_device(void) { return this->mDevice; }
  P2PServer *get_p2p_server(void) { return this->mP2PServer; }
  ServerSocket *get_server_socket(void) { return this->mServerSocket; }

  /* Attribute setters */
  void start_disconnecting_on_purpose() {
    this->mIsDisconnectingOnPurpose = true;
  }

  void wait_for_disconnecting_on_purpose_peer(void) {
    std::unique_lock<std::mutex> lck(this->mWaitForDisconnectAckLock);
    this->mWaitForDisconnectAckCond.wait(lck);
  }

  void peer_knows_disconnecting_on_purpose() {
    this->mIsDisconnectingOnPurposePeer = true;
    this->mWaitForDisconnectAckCond.notify_all();
  }

private:
  /* Attribute setters */
  void finish_disconnecting_on_purpose() {
    this->mIsDisconnectingOnPurpose = false;
    this->mIsDisconnectingOnPurposePeer = false;
  }

private:
  /* Attributes */
  char mName[256];

  /*
   * TODO: ID is now defined by user. However, the ID should be maintained by
   * system finally.
   */
  int mId;
  bool mIsDisconnectingOnPurpose;     /* Disconnecting on purpose by a device */
  bool mIsDisconnectingOnPurposePeer; /* Peer knows disconnecting on purpose */
  std::mutex mWaitForDisconnectAckLock;
  std::condition_variable mWaitForDisconnectAckCond;

  /* Components */
  Device *mDevice = NULL;
  P2PServer *mP2PServer = NULL;
  ServerSocket *mServerSocket = NULL;

  /* State Listeners */
  std::vector<ServerAdapterStateListener *> mStateListeners;

private:
  /* EXP: Measure sender thread loop's time interval */
#ifdef EXP_MEASURE_INTERVAL_SENDER
private:
  int mSendCount = 0;
  /* Milliseconds */
  int mIntervals[4] = {
      0,
  };
#endif

public:
  /* State getter */
  ServerAdapterState get_state(void) {
    std::unique_lock<std::mutex> lck(this->mStateLock);
    return this->mState;
  }

  /* Add state listener */
  void listen_state(ServerAdapterStateListener *listener) {
    std::unique_lock<std::mutex> lck(this->mStateLock);

    if (listener == NULL)
      return;
    this->mStateListeners.push_back(listener);
  }

  static std::string server_adapter_state_to_string(ServerAdapterState state) {
    char const *const state_string[] = {
        "Disconnected",  "Connecting", "Active",  "Disconnecting",
        "GoingSleeping", "Sleeping",   "WakingUp"};
    int state_index = (int)state;
    if (state_index >= ServerAdapterState::kASNum || state_index < 0) {
      return std::string("");
    } else {
      return std::string(state_string[state_index]);
    }
  }

private:
  /* State setter */
  void set_state(ServerAdapterState new_state);

  /* State */
  ServerAdapterState mState;
  std::mutex mStateLock;

public:
  /* Constructor */
  ServerAdapter(int id, const char *name) {
    this->mState = ServerAdapterState::kDisconnected;
    snprintf(this->mName, sizeof(this->mName), "%s", name);
    this->mId = id;
    this->mIsDisconnectingOnPurpose = false;
    this->mIsDisconnectingOnPurposePeer = false;

    this->mSendDataSize.start_measure_speed();
    this->mReceiveDataSize.start_measure_speed();
  }

  ~ServerAdapter() {
    if (this->mP2PServer != NULL) {
      delete this->mP2PServer;
    }
    if (this->mServerSocket != NULL) {
      delete this->mServerSocket;
    }
  }

protected:
  /* Initializer called by child classes */
  void initialize(Device *device, P2PServer *p2pServer,
                  ServerSocket *serverSocket) {
    this->mDevice = device;
    this->mP2PServer = p2pServer;
    this->mServerSocket = serverSocket;
  }
}; /* class ServerAdapter */
} /* namespace sc */

#endif /* !defined(__SERVER_ADAPTER_H__) */