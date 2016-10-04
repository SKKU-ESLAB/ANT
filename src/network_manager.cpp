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

#include <network_manager.h>

#include <dbug_log.h>
#include <network_adapter.h>
#include <segment_manager.h>

#include <string.h>
#include <thread>
#include <functional>
#include <list>

namespace cm {
static NetworkManager *net_manager = NULL;

NetworkManager *NetworkManager::get_instance() {
  if (net_manager == NULL)
    net_manager = new NetworkManager();

  return net_manager;
}
NetworkManager::NetworkManager(void) {
}

void NetworkManager::install_network_adapter(NetworkAdapter *na) {
  std::unique_lock<std::mutex> lck(lock);
  adapter_list.push_back(na);
}

void NetworkManager::remove_network_adapter(NetworkAdapter *na) {
  std::unique_lock<std::mutex> lck(lock);
  adapter_list.remove(na);
}

} /* namespace cm */
