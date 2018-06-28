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

#ifndef _WFD_SERVER_SOCKET_H_
#define _WFD_SERVER_SOCKET_H_

#include <ServerSocket.h>

#include <counter.h>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

namespace cm {

class WfdServerSocket : ServerSocket {
public:
  virtual bool open_impl(void);
  virtual bool close_impl(void);
  virtual int send_impl(const void *data_buffer, size_t data_length);
  virtual int receive_impl(void *data_buffer, size_t data_length);

  void set_ip_address(const char* ip_address) {
    snprintf(this->mIpAddress, 256, "%s", ip_address);
  }

  uuid_t get_service_uuid(void) {
    return this->mServiceUUID;
  }

  WfdServerSocket(int port, const char* wfd_device_name) {
    this->mPort = port;
    snprintf(this->mWfdDeviceName, 256, "%s", wfd_device_name);
  }

  ~WfdServerSocket(void) {
  }

protected:
  int mPort;
  char mIpAddress[256] = { NULL, };
  char mWfdDeviceName[256] = { NULL, };

  int mServerSocket;
  int mClientSocket;

private:
}; /* class WfdServerSocket */

} /* namespace cm */

#endif /* !defined(_WFD_SERVER_SOCKET_H_) */
