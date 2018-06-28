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

#ifndef _ETH_SERVER_ADAPTER_H_
#define _ETH_SERVER_ADAPTER_H_

#include <ServerAdapter.h>
#include <TcpServerSocket.h>

#include <counter.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <stdio.h>

namespace cm {

class EthDevice : Device {
public:
  virtual bool turn_on_impl(void) {
    return true;
  }
  virtual bool turn_off_impl(void) {
    return true;
  }
  EthDevice() : Device("Ethernet") {
  }
};

class EthP2pServer : P2pServer {
public:
  virtual bool allow_impl(void) {
    return true;
  }
  virtual bool disallow_impl(void) {
    return true;
  }
};

class EthServerAdapter : ServerAdapter {
public:
  EthServerAdapter(int id, char* name, int port) : ServerAdapter(id, name) { 
    EthDevice* device = EthDevice::getSingleton();
    EthP2pServer* p2pServer = new EthP2pServer();
    TcpServerSocket* serverSocket = new TcpServerSocket(port);
    serverSocket->set_ip_address_raw(htonl(INADDR_ANY));
    this->initialize(device, p2pServer, serverSocket);
  }

  ~EthServerAdapter(void) {
  }

protected:
}; /* class EthServerAdapter */

} /* namespace cm */

#endif /* !defined(_ETH_SERVER_ADAPTER_H_) */
