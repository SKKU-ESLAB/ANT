/* Copyright 2017-2018 All Rights Reserved.
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *  Eunsoo Park (esevan.park@gmail.com)
 *  Injung Hwang (sinban04@gmail.com)
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

#ifndef DEVICE_INC_TCP_SERVER_OVER_ETH_H_
#define DEVICE_INC_TCP_SERVER_OVER_ETH_H_

#include <network_adapter.h>

#include <dbug_log.h>
#include <sys/socket.h>
#include <arpa/inet.h>

namespace cm {
class TCPServerOverEthAdapter : public NetworkAdapter {
 public:
  TCPServerOverEthAdapter(uint32_t id, int port);

  void set_device_name(char *name);

 private:
  int port;

  int serv_sock, cli_sock;
  struct sockaddr_in saddr, caddr;

  char dev_name[256];

  bool device_on(void);
  bool device_off(void);
  bool make_connection(void);
  bool close_connection(void);
  int send(const void *buf, size_t len);
  int recv(void *buf, size_t len);
  uint16_t get_id(void);
  void on_control_recv(const void *buf, size_t len);
};
}  /* namespace cm */

#endif  // DEVICE_INC_TCP_SERVER_OVER_ETH_H_
