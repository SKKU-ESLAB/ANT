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

#include <tcp_server_over_eth.h>

#include <unistd.h>
#include <string.h>

namespace cm {
TCPServerOverEthAdapter::TCPServerOverEthAdapter(uint32_t id, int port) {
  int err;
  this->port = port;
  this->dev_id = id;
  net_dev_type = kWifi;

  cli_sock = 0;

  snprintf(dev_name, sizeof(dev_name), "TCPServer/Eth");

  serv_sock = socket(AF_INET, SOCK_STREAM, 0);
  assert(serv_sock >= 0);

  memset(&saddr, 0, sizeof(saddr));

  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = htonl(INADDR_ANY);
  saddr.sin_port = htons((unsigned short)port);
  int reuse = 1;

  if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse,
                 sizeof(int)) == -1) {
    LOG_ERR("Socket setup failed");
    return;
  }

  err = bind(serv_sock, (struct sockaddr *)&saddr, sizeof(saddr));
  assert (err >= 0);

  err = listen(serv_sock, 5);
  assert (err >= 0);

  set_controllable();
}

void TCPServerOverEthAdapter::set_device_name(char *name) {
  snprintf(dev_name, sizeof(dev_name), "%s", name);
}

uint16_t TCPServerOverEthAdapter::get_id(void) {
  return dev_id;
}

bool TCPServerOverEthAdapter::device_on(void) {
  return true;
}

bool TCPServerOverEthAdapter::device_off(void) {
  return true;
}

bool TCPServerOverEthAdapter::make_connection(void) {
  int caddr_len = sizeof(caddr);

  LOG_VERB("Eth %d] Accepting...client",port);
  cli_sock = accept(serv_sock, (struct sockaddr *)&caddr, (socklen_t *)&caddr_len);
  if (cli_sock < 0) {
    LOG_ERR("Eth %d] Accept failed", port);
    return false;
  }

  LOG_VERB("Eth %d] Accepted", port);
  LOG_VERB("Eth %d] TCP Client connected", port);

  return true;
}

int TCPServerOverEthAdapter::send_impl(const void *buf, size_t len) {
  int sent = 0;
 
  if (cli_sock <= 0)
    return -1;

  while (sent < len) {
    int sent_bytes = write(cli_sock, buf, len);
    if (sent_bytes <= 0) {
      LOG_WARN("Eth %d] Cli sock closed",port);
      return -1;
    }

    sent += sent_bytes;
    LOG_DEBUG("Eth %d] sent the data: %d\n", port, sent_bytes);
  }
 
  return sent;
}

int TCPServerOverEthAdapter::recv_impl(void *buf, size_t len) {
  int recved = 0;

  if (cli_sock <= 0)
    return -1;

  while (recved < len) {
    int recv_bytes = read(cli_sock, buf, len);
    if (recv_bytes <= 0) {
      LOG_WARN("Cli sock closed");
      return -1;
    }

    recved += recv_bytes;
  }

  return recved;
}

void TCPServerOverEthAdapter::on_control_recv(const void *buf, size_t len) {
  char *msg = new char[len+1];

  msg[len] = 0;
  memcpy(msg, buf, len);
  LOG_DEBUG("Got : %s", msg);

  delete[] msg;

  return;
}

bool TCPServerOverEthAdapter::close_connection() {
  /* Close only client socket, not server socket */
  close(cli_sock);
  cli_sock = 0; 
  return true;
}

}
