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

#ifndef __P2P_SERVER_H__
#define __P2P_SERVER_H__

#include "../../common/inc/RefCount.h"

#include <thread>
#include <mutex>

#include <stdio.h>

namespace sc {
typedef enum {
  kDisallowed = 0,
  kAllowed = 1
} P2PServerState;

class P2PServer {
public:
  bool allow_discover(void);
  bool disallow_discover(void);

  virtual bool allow_discover_impl(void) = 0;
  virtual bool disallow_discover_impl(void) = 0;

  P2PServerState get_state(void) {
    return this->mState;
  }

  P2PServer(const char* name) {
    this->mState = P2PServerState::kDisallowed;
    snprintf(this->mName, sizeof(this->mName), name);
  }
  ~P2PServer(void) {
  }

protected:
  void set_state(P2PServerState new_state) {
    this->mState = new_state;
  }

  char* get_name() {
    return this->mName;
  }

private:
  P2PServerState mState;
  char mName[256];
}; /* class P2PServer */
} /* namespace sc */

#endif /* !defined(__P2P_SERVER_H__) */
