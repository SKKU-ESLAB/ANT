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

#ifndef __BT_SERVER_ADAPTER_H__
#define __BT_SERVER_ADAPTER_H__

#include "BtDevice.h"
#include "BtP2PServer.h"
#include "RfcommServerSocket.h"

#include "../../core/inc/ServerAdapter.h"

#include "../../configs/ExpConfig.h"

#include <mutex>
#include <thread>

#include <stdio.h>

namespace sc {
class BtServerAdapter : public ServerAdapter {
public:
  ~BtServerAdapter(void) {}

  /* Singleton */
  static BtServerAdapter *singleton(int id, const char *name,
                                       const char *service_uuid) {
    if (sSingleton == NULL) {
      sSingleton = new BtServerAdapter(id, name, service_uuid);
    }
    return sSingleton;
  }

private:
  /* Singleton */
  static BtServerAdapter *sSingleton;

  /* Constructor */
  BtServerAdapter(int id, const char *name, const char *service_uuid)
      : ServerAdapter(id, name) {
    BtDevice *device = new BtDevice();
    BtP2PServer *p2pServer = new BtP2PServer();
    RfcommServerSocket *serverSocket =
        new RfcommServerSocket(name, service_uuid);
    this->initialize(device, p2pServer, serverSocket);
  }
}; /* class BtServerAdapter */
} /* namespace sc */

#endif /* !defined(__BT_SERVER_ADAPTER_H__) */