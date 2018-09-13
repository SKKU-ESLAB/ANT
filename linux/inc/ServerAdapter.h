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

#ifndef _SERVER_ADAPTER_H_
#define _SERVER_ADAPTER_H_

#include <Counter.h>
#include <ExpConfig.h>

#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

#include <stdio.h>

#include <Device.h>
#include <P2PServer.h>
#include <ServerSocket.h>

namespace sc {

typedef enum {
  kDisconnected = 0,
  kConnecting = 1,
  kActive = 2,
  kDisconnecting = 3,
  kGoingSleeping = 4,
  kSleeping = 5,
  kWakingUp = 6
} ServerAdapterState;

class ServerAdapter;
class ServerAdapterStateListener {
public:
  virtual void onUpdateServerAdapterState(ServerAdapter *adapter,
                                          ServerAdapterState old_state,
                                          ServerAdapterState new_state) = 0;
};

typedef void (*ConnectCallback)(bool is_success);
typedef void (*DisconnectCallback)(bool is_success);

typedef void (*ReceiveLoop)(ServerAdapter *adapter);

class ServerAdapter {
public:
  /* Basic APIs
   *  - connect, disconnect, sleep, wake up
   *  - send, receive
   *  - enable sender thread, enable receiver thread
   */
  /* Basic APIs related to connection/sleeping */
  void connect(ConnectCallback callback, bool is_send_request);
  void disconnect(DisconnectCallback callback, bool is_send_request);
  void sleep(DisconnectCallback callback, bool is_send_request);
  void wake_up(ConnectCallback callback, bool is_send_request);
  void connect_or_wake_up(ConnectCallback callback, bool is_send_request);
  void disconnect_or_sleep(DisconnectCallback callback, bool is_send_request);

  /* Basic APIs related to data transmission */
  int send(const void *buf, size_t len);
  int receive(void *buf, size_t len);

  /* Basic APIs related to handling sender/receiver threads */
  void enable_sender_thread() {
    this->mSenderThreadEnabled = true;
    return;
  }
  void enable_receiver_thread(ReceiveLoop receive_loop) {
    if (receive_loop == NULL) {
      this->mReceiveLoop = ServerAdapter::data_adapter_receive_loop;
    } else {
      this->mReceiveLoop = receive_loop;
    }
    this->mReceiverThreadEnabled = true;
    return;
  }

  /* Basic APIs called in receiver loops */
  bool is_receiver_loop_on(void) { return mReceiverLoopOn; }

private:
  /* Connect Thread */
  void connect_thread(void);
  bool __connect_thread(void);
  std::thread *mConnectThread = NULL;
  ConnectCallback mConnectCallback = NULL;

  /* Disconnect Thread */
  void disconnect_thread(void);
  bool __disconnect_thread(void);
  std::thread *mDisconnectThread = NULL;

  /* Sender Thread */
  void sender_thread(void);
  void data_adapter_send_loop(void);
  std::thread *mSenderThread = NULL;
  bool mSenderThreadEnabled = false;
  bool mSenderLoopOn = false;
  bool mSenderSuspended = false;
  std::mutex mSenderSuspendedMutex;
  std::condition_variable mSenderSuspendedCond;
  std::mutex mWaitSenderThreadMutex;
  std::condition_variable mWaitSenderThreadCond;

  /* Receiver Thread */
  void receiver_thread(void);
  static void data_adapter_receive_loop(ServerAdapter *adapter);
  std::thread *mReceiverThread = NULL;
  ReceiveLoop mReceiveLoop = NULL;
  bool mReceiverThreadEnabled = false;
  bool mReceiverLoopOn = false;
  DisconnectCallback mDisconnectCallback = NULL;
  std::mutex mWaitReceiverThreadMutex;
  std::condition_variable mWaitReceiverThreadCond;

public:
  /* Statistics getters */
  int get_bandwidth_up(void) { this->mSendDataSize.get_speed(); }
  int get_bandwidth_down(void) { this->mReceiveDataSize.get_speed(); }

private:
  /* Statistics */
  Counter mSendDataSize;
  Counter mReceiveDataSize;

public:
  /* Attribute getters */
  char *get_name(void) { return this->mName; }
  int get_id(void) { return this->mId; }
  bool is_sleeping_allowed(void) { return this->mIsSleepingAllowed; }
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
  bool mIsSleepingAllowed;
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
#if EXP_MEASURE_INTERVAL_SENDER != 0
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

private:
  /* State setter */
  void set_state(ServerAdapterState new_state) {
    ServerAdapterState old_state;

    {
      std::unique_lock<std::mutex> lck(this->mStateLock);
      old_state = this->mState;
      this->mState = new_state;
    }

    for (std::vector<ServerAdapterStateListener *>::iterator it =
             this->mStateListeners.begin();
         it != this->mStateListeners.end(); it++) {
      ServerAdapterStateListener *listener = (*it);
      listener->onUpdateServerAdapterState(this, old_state, new_state);
    }
  }

  /* State */
  ServerAdapterState mState;
  std::mutex mStateLock;

public:
  /* Constructor */
  ServerAdapter(int id, const char *name) {
    this->mState = ServerAdapterState::kDisconnected;
    snprintf(this->mName, sizeof(this->mName), name);
    this->mId = id;
    this->mIsSleepingAllowed = false;
    this->mIsDisconnectingOnPurpose = false;
    this->mIsDisconnectingOnPurposePeer = false;
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
                  ServerSocket *serverSocket, bool is_sleeping_allowed) {
    this->mDevice = device;
    this->mP2PServer = p2pServer;
    this->mServerSocket = serverSocket;
    this->mIsSleepingAllowed = is_sleeping_allowed;
  }
}; /* class ServerAdapter */

} /* namespace sc */
#endif /* !defined(_SERVER_ADAPTER_H_) */
