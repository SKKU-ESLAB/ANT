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

#ifndef __SERVER_ADAPTER_STATE_LISTENER_H__
#define __SERVER_ADAPTER_STATE_LISTENER_H__

#include "ServerAdapter.h"
#include "ServerAdapterState.h"

namespace sc {
class ServerAdapter;
class ServerAdapterStateListener {
public:
  virtual void onUpdateServerAdapterState(ServerAdapter *adapter,
                                          ServerAdapterState old_state,
                                          ServerAdapterState new_state) = 0;
}; /* class ServerAdapterStateListener */
} /* namespace sc */

#endif /* !defined(__SERVER_ADAPTER_STATE_LISTENER_H__) */