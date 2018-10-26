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
  // Check previous state
  P2PServerState state = this->get_state();
  if (state == P2PServerState::kAllowed) {
    LOG_WARN("allow_discover(%s): FAILED - already allowed (%d)", this->get_name(), state);
    return false;
  }

  // Allow discover
  bool res = this->allow_discover_impl();

  // Check result and change state
  if (!res) {
    LOG_ERR("allow_discover(%s): FAILED - impl error", this->get_name());
    this->set_state(P2PServerState::kDisallowed);
  } else {
    LOG_DEBUG("allow_discover(%s): SUCCESS", this->get_name());
    this->set_state(P2PServerState::kAllowed);
  }
  return res;
}

bool P2PServer::disallow_discover(void) {
  // Check previous state
  P2PServerState state = this->get_state();
  if (state == P2PServerState::kDisallowed) {
    LOG_WARN("disallow_discover(%s): FAILED - already disallowed(%d)", this->get_name(), state);
    return false;
  }

  // Disallow discover
  bool res = this->disallow_discover_impl();

  // Check result and change state
  if (!res) {
    LOG_ERR("disallow_discover(%s): FAILED - impl error", this->get_name());
    this->set_state(P2PServerState::kAllowed);
  } else {
    LOG_DEBUG("disallow_discover(%s): SUCCESS", this->get_name());
    this->set_state(P2PServerState::kDisallowed);
  }
  return res;
}