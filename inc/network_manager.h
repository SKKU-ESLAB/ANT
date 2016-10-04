/* Copyright 2015-2016 CISS, and contributors. All rights reserved
 * 
 * Contact: Eunsoo Park <esevan.park@gmail.com>
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

#ifndef INC_NETWORK_MANAGER_H_
#define INC_NETWORK_MANAGER_H_
#include <network_adapter.h>
#include <segment_manager.h>
#include <protocol_manager.h>

#include <stdint.h>
#include <list>
#include <mutex>
#include <condition_variable>


namespace cm {
typedef enum {
  kNetSend = 0,
  kNetRecv = 1
} kNetQueueType;

class NetworkManager {
 public:
  static NetworkManager *get_instance(void);

  void install_network_adapter(NetworkAdapter *na);
  void remove_network_adapter(NetworkAdapter *na);
 private:
  NetworkManager(void);

  std::mutex lock;
  std::list<NetworkAdapter *> adapter_list;
};
} /* namespace cm */
#endif  /* INC_NETWORK_MANAGER_H_ */
