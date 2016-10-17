/* Copyright 2016 Eunsoo Park (esevan.park@gmail.com). All rights reserved
 * 
 * Contact: Eunsoo Park (esevan.park@gmail.com)
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

#include <tcp_server_over_wfd.h>
#include <wifi_control.h>

namespace cm {
TCPServerOverWfdAdapter::TCPServerOverWfdAdapter(uint32_t id, int port) {
  this->port = port;
  this->id = id;
}

void TCPServerOverWfdAdapter::set_device_name(char *name) {
  snprintf(dev_name, "%s", name);
}

uint32_t TCPServerOverWfdAdapter::get_id(void) {
  return id;
}

bool TCPServerOverWfdAdapter::device_on(void) {
  int ret = wifi::wifi_direct_server_up();

  return ret==0;
}

bool TCPServerOverWfdAdapter::device_off(void) {
  int ret = wifi::wifi_direct_server_down();

  return ret==0;
}

bool TCPServerOverWfdAdapter::make_connection(void) {

}

}
