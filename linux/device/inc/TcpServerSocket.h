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

#ifndef __TCP_SERVER_SOCKET_H__
#define __TCP_SERVER_SOCKET_H__

#include "WfdIpAddressListener.h"

#include "../../core/inc/ServerSocket.h"

#include <mutex>
#include <thread>

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

namespace sc {
class ServerSocket;
class WfdIpAddressListener;
class TcpServerSocket : public ServerSocket, public WfdIpAddressListener {
public:
  virtual bool open_impl(void);
  virtual bool close_impl(void);
  virtual int send_impl(const void *data_buffer, size_t data_length);
  virtual int receive_impl(void *data_buffer, size_t data_length);

  virtual void on_change_ip_address(const char *ip_address);

  void set_ip_address_raw(unsigned long ip_address_raw) {
    this->mIpAddressRaw = ip_address_raw;
  }

  TcpServerSocket(const char *name, int port) : ServerSocket(name) {
    this->mPort = port;
    this->mIpAddressRaw = 0;
  }

  ~TcpServerSocket(void) {}

protected:
  int mPort;
  int mIpAddressRaw;

  int mServerSocket;
  int mClientSocket;

private:
}; /* class TcpServerSocket */
} /* namespace sc */

#endif /* !defined(__TCP_SERVER_SOCKET_H__) */