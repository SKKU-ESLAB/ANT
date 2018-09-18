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

#ifndef __BT_P2P_SERVER_H__
#define __BT_P2P_SERVER_H__

#include "../../core/inc/P2PServer.h"

#include <mutex>
#include <thread>

#include <stdio.h>

namespace sc {
class BtP2PServer : public P2PServer {
public:
  virtual bool allow_discover_impl(void);
  virtual bool disallow_discover_impl(void);

  BtP2PServer(void) : P2PServer("BT") {}
  ~BtP2PServer(void) {}

protected:
}; /* class BtP2PServer */
} /* namespace sc */

#endif /* !defined(__BT_P2P_SERVER_H__) */