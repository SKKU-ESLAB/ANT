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

#include "../inc/P2PServer.h"

#include "../../common/inc/DebugLog.h"

using namespace sc;

bool P2PServer::allow_discover(void) {
  bool res = this->allow_discover_impl();

  if (!res) {
    LOG_DEBUG("%s: Failed to allow", this->get_name());
    this->set_state(P2PServerState::kDisallowed);
  } else {
    LOG_DEBUG("%s: Successfully allowed", this->get_name());
    this->set_state(P2PServerState::kAllowed);
  }
  return res;
}

bool P2PServer::disallow_discover(void) {
  bool res = this->disallow_discover_impl();

  if (!res) {
    LOG_DEBUG("%s: Failed to disallow", this->get_name());
    this->set_state(P2PServerState::kAllowed);
  } else {
    LOG_DEBUG("%s: Successfully disallowed", this->get_name());
    this->set_state(P2PServerState::kDisallowed);
  }
  return res;
}