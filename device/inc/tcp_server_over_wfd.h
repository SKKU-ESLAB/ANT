/* Copyright 2017 All Rights Reserved.
*  Eunsoo Park (esevan.park@gmail.com)
*  Injung Hwang (sinban04@gmail.com)
*  
* [Contact]
*  Eunsoo Park (esevan.park@gmail.com)
*  Injung Hwang (sinban04@gmail.com)
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

#include <sys/socket.h>
#include <arpa/inet.h>
#include <mutex>
#include <condition_variable>

namespace cm {
class TCPServerOverWfdAdapter : public NetworkAdapter {
 public:
  TCPServerOverWfdAdapter(uint32_t id, int port, char *dev_name);

 private:
  int port;
  int cli_sock, serv_sock;
  struct sockaddr_in saddr, caddr;

  std::mutex dev_wait_lock;
  std::condition_variable dev_connected;

  FILE *fp;
  int sent_data;

  char dev_name[256];


  uint16_t get_id(void);
  bool dev_connected_wait(void);
  bool device_on(void);
  bool device_off(void);
  bool make_connection(void);
  bool close_connection(void);
  int send(const void *buf, size_t len);
  int recv(void *buf, size_t len);

  void on_control_recv(const void *buf, size_t len);
};
}  /* namespace cm */

#endif  // DEVICE_INC_TCP_SERVER_OVER_WFD_H_
