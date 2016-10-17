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

#ifndef DEVICE_INC_TCP_SERVER_OVER_WFD_H_
#define DEVICE_INC_TCP_SERVER_OVER_WFD_H_

#include <network_adapter.h>

#include <dbug_log.h>

namespace cm {
class TCPServerOverWfdAdapter : public NetworkAdapter {
 public:
  TCPServerOverWfdAdapter(uint32_t id, int port);

  void set_device_name(char *name);

 private:
  int port;
  uint32_t id;

  char dev_name[256];

  uint32_t get_id(void);
  bool device_on(void);
  bool device_off(void);
  bool make_connection(void);
  bool close_connection(void);
  bool send(const void *buf, size_t len);
  bool recv(void *buf, size_t len);
};
}  /* namespace cm */

#endif  // DEVICE_INC_TCP_SERVER_OVER_WFD_H_
