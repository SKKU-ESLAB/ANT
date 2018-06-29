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

#include <counter.h>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <stdio.h>

#include <ServerSocket.h>
#include <P2PServer.h>
#include <Device.h>

namespace cm {

typedef enum {
  kDisconnected = 0,
  kConnecting = 1,
  kConnected = 2,
  kDisconnecting = 3
} ServerAdapterState;

class ServerAdapterStateListener {
  virtual void onUpdateServerAdapterState(ServerAdapter* adapter,
      ServerAdapterState old_state, ServerAdapterState new_state) = 0;
};

typedef void (*ConnectCallback)(bool is_success);
typedef void (*DisconnectCallback)(bool is_success);

class ServerAdapter {
public:
  bool connect(ConnectCallback callback);
  bool disconnect(DisconnectCallback callback);
  int send(const void *buf, size_t len);
  int receive(void *buf, size_t len);

  int get_bandwidth_up(void) {
    this->mSendDataSize.get_speed();
  }

  int get_bandwidth_down(void) {
    this->mReceiveDataSize.get_speed();
  }

  ServerAdapterState get_state(void) {
    std::unique_lock<std::mutex> lck(this->mStateLock);

    return this->mState;
  }

  char* get_name(void) {
    return this->mName;
  }

  int get_id(void) {
    return this->mId;
  }

  Device* get_device(void) {
    return this->mDevice;
  }

  Device* get_p2p_server(void) {
    return this->mP2pServer;
  }

  ServerSocket* get_server_socket(void) {
    return this->mServerSocket;
  }

  void listen_state(ServerAdapterStateListener* listener) {
    std::unique_lock<std::mutex> lck(this->mStateLock);

    if(listener == NULL) return;
    this->mStateListeners.push_back(listener);
  }

  ServerAdapter(int id, char* name) {
    this->mState = ServerAdapterState::kDisconnected;
    snprintf(this->mName, sizeof(this->mName), name);
    this->mId = id; 
  }

  ~ServerAdapter() {
    if(this->mDevice !== NULL) {
      delete this->mDevice;
    }
    if(this->mP2pServer !== NULL) {
      delete this->mP2pServer;
    }
    if(this->mServerSocket !== NULL) {
      delete this->mServerSocket;
    }
  }

protected:
  void send_ctrl_msg(const void *buf, int len);

  void initialize(Device* device, P2PServer* p2pServer, ServerSocket* serverSocket) {
    this->mDevice = device;
    this->mP2pServer = p2pServer;
    this->mServerSocket = serverSocket;
  }

  void set_state(ServerAdapterState new_state) {
    std::unique_lock<std::mutex> lck(this->mStateLock);

    ServerAdapterState old_state = this->mState;
    this->mState = new_state;

    for(std::vector::iterator it = this->mStateListeners.begin();
        it != this->mStateListners.end();
        it++) {
      ServerAdapterStateListener* listener = (*it);
      listener->onUpdateServerAdapterState(this, old_state, new_state);
    }
  }

  ServerAdapterState mState;
  std::mutex mStateLock;
  char mName[256];
  int mId;
  Device* mDevice = NULL;
  P2PServer* mP2pServer = NULL;
  ServerSocket* mServerSocket = NULL;

  /* Statistics */
  Counter mSendDataSize;
  Counter mReceiveDataSize;

  /* State Listeners */
  std::vector<ServerAdapterStateListener*> mStateListeners;

  /* Callback */
  ConnectCallback mConnectCallback = NULL;
  DisconnectCallback mDisconnectCallback = NULL;

private:
  void connect_thread(void);
  void disconnect_thread(void);
  void sender_thread(void);
  void receiver_thread(void);

  std::thread *mSenderThread = NULL;
  std::thread *mReceiverThread = NULL;
  bool mSenderThreadOn = false;
  bool mReceiverThreadOn = false;
}; /* class ServerAdapter */

} /* namespace cm */

#endif /* !defined(_SERVER_ADAPTER_H_) */
