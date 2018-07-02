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

#include <P2PServer.h>

using namespace cm;

bool P2PServer::allow(void) {
  bool res = this->allow_impl();

  if(!res) {
    this->set_state(P2PServerState::kDisallowed);
  } else {
    this->set_state(P2PServerState::kAllowed);
  }
  return res;
}

bool P2PServer::disallow(void) {
  bool res = this->disallow_impl();

  if(!res) {
    this->set_state(P2PServerState::kAllowed);
  } else {
    this->set_state(P2PServerState::kDisallowed);
  }
  return res;
}
