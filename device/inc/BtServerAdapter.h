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

#ifndef _BT_SERVER_ADAPTER_H_
#define _BT_SERVER_ADAPTER_H_

#include <counter.h>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <stdio.h>

namespace cm {
  class BtServerAdapter : ServerAdapter {
  public:
    BtServerAdapter(char* name) : ServerAdapter(name) { 
      BtDevice* device = BtDevice::getSingleton();
      BtP2pServer* p2pServer = new BtP2pServer();
      BtServerSocket* serverSocket = new BtServerSocket();
      this->initialize(device, p2pServer, serverSocket);
    }

    ~BtServerAdapter() {
    }

  protected:
  };
} /* namespace cm */

#endif /* !defined(_BT_SERVER_ADAPTER_H_) */
