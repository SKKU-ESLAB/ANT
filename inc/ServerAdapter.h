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

class ServerAdapter {
public:
  bool connect(void);
  bool disconnect(void);
  int send(const void *buf, size_t len);
  int recv(void *buf, size_t len);

  int get_bandwidth_up(void) {
    this->mSendDataSize.get_speed();
  }

  int get_bandwidth_down(void) {
    this->mReceiveDataSize.get_speed();
  }

  ServerAdapterState get_state(void) {
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

  ServerAdapter(char* name,
      Device* device, P2PServer* p2pServer, ServerSocket* serverSocket) {
    this->mState = ServerAdapterState::kDisconnected;
    snprintf(this->mName, sizeof(this->mName), name);
    this->mId = ServerAdapter::sNextId++;
    this->mDevice = device;
    this->mP2pServer = p2pServer;
    this->mServerSocket = serverSocket;
  }

  ~ServerAdapter();

protected:
  void set_state(ServerAdapterState new_state) {
    this->mState = new_state;
  }

  static int sNextId;

  ServerAdapterState mState;
  char mName[256];
  int mId;
  Device* mDevice;
  P2PServer* mP2pServer;
  ServerSocket* mServerSocket;

  /* Statistics */
  Counter mSendDataSize;
  Counter mReceiveDataSize;

private:
  void run_sender(void);
  void run_recver(void);
  void join_threads();

  void return_sending_failed_packet(void *segment);

  std::thread *mSenderThread;
  std::thread *mReceiverThread;
}; /* class ServerAdapter */

} /* namespace cm */

#endif /* !defined(_SERVER_ADAPTER_H_) */
