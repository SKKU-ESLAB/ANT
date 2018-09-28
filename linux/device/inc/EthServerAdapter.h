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

#ifndef __ETH_SERVER_ADAPTER_H__
#define __ETH_SERVER_ADAPTER_H__

#include "TcpServerSocket.h"

#include "../../core/inc/ServerAdapter.h"

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <mutex>
#include <thread>

#include <stdio.h>

namespace sc {
class EthDevice : public Device {
public:
  virtual bool turn_on_impl(void) { return true; }
  virtual bool turn_off_impl(void) { return true; }

  static EthDevice *getSingleton(void) {
    if (EthDevice::sSingleton == NULL) {
      EthDevice::sSingleton = new EthDevice();
    }
    return EthDevice::sSingleton;
  }

protected:
  static EthDevice *sSingleton;
  EthDevice() : Device("Eth") {}
}; /* class EthDevice */

class EthP2PServer : public P2PServer {
public:
  virtual bool allow_discover_impl(void) { return true; }
  virtual bool disallow_discover_impl(void) { return true; }

  EthP2PServer() : P2PServer("Eth") {}
}; /* class EthP2PServer */

class EthServerAdapter : public ServerAdapter {
public:
  EthServerAdapter(int id, const char *name, int port)
      : ServerAdapter(id, name) {
    EthDevice *device = EthDevice::getSingleton();
    EthP2PServer *p2pServer = new EthP2PServer();
    TcpServerSocket *serverSocket = new TcpServerSocket(name, port);
    serverSocket->set_ip_address_raw(htonl(INADDR_ANY));
    this->initialize(device, p2pServer, serverSocket);
  }

  ~EthServerAdapter(void) {}

protected:
}; /* class EthServerAdapter */
} /* namespace sc */

#endif /* !defined(__ETH_SERVER_ADAPTER_H__) */