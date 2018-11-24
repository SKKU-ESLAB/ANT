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

#ifndef __WFD_SERVER_ADAPTER_H__
#define __WFD_SERVER_ADAPTER_H__

#include "TcpServerSocket.h"
#include "WfdDevice.h"
#include "WfdP2PServer.h"

#include "../../core/inc/ServerAdapter.h"

#include <mutex>
#include <thread>

#include <stdio.h>

namespace sc {
class WfdServerAdapter : public ServerAdapter {
public:
  ~WfdServerAdapter(void) {}

  /* Singleton */
  static WfdServerAdapter *singleton(int id, const char *name, int port,
                                        const char *wfd_device_name) {
    if (sSingleton == NULL) {
      sSingleton = new WfdServerAdapter(id, name, port, wfd_device_name);
    }
    return sSingleton;
  }

protected:
  /* Singleton */
  static WfdServerAdapter *sSingleton;

  /* Constructor */
  WfdServerAdapter(int id, const char *name, int port,
                   const char *wfd_device_name)
      : ServerAdapter(id, name) {
    WfdDevice *device = new WfdDevice();
    WfdP2PServer *p2pServer = new WfdP2PServer(wfd_device_name, (void *)this);
    TcpServerSocket *serverSocket = new TcpServerSocket(name, port);
    p2pServer->add_wfd_ip_address_listener(serverSocket);
    this->initialize(device, p2pServer, serverSocket);
  }
}; /* class WfdServerAdapter */
} /* namespace sc */

#endif /* !defined(__WFD_SERVER_ADAPTER_H__) */
