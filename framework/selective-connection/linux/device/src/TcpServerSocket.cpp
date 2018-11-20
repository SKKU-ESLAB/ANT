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

#include "../inc/TcpServerSocket.h"

#include "../../common/inc/DebugLog.h"

#include <arpa/inet.h>
#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define VERBOSE_WFD_MSG 0

using namespace sc;

bool TcpServerSocket::open_impl(void) {
  if (this->mIpAddressRaw == 0) {
    LOG_ERR("IP Address is not set yet");
    return false;
  }

  this->mServerSocket = ::socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = this->mIpAddressRaw;
  server_address.sin_port = htons((uint16_t)this->mPort);
  int reuse = 1;

  if (setsockopt(this->mServerSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse,
                 sizeof(int)) == -1) {
    LOG_ERR("open(%s): setsockopt SO_REUSEADDR error", this->get_name());
    return false;
  }

  int err = ::bind(this->mServerSocket, (struct sockaddr *)&server_address,
                   sizeof(server_address));
  if (err < 0) {
    LOG_ERR("open(%s): bind error", this->get_name());
    return false;
  }

  err = ::listen(this->mServerSocket, 5);
  if (err < 0) {
    LOG_ERR("open_impl(%s): listen error", this->get_name());
    return false;
  }

  struct sockaddr_in client_address;
  memset(&client_address, 0, sizeof(client_address));
  int client_address_len = sizeof(client_address);
  const int kMaxTries = 10;
  for (int tries = 0; tries < kMaxTries; tries++) {
    LOG_VERB("open_impl(%s): Accepting... (trial: %d/%d)", this->get_name(),
             tries + 1, kMaxTries);
    this->mClientSocket =
        ::accept(this->mServerSocket, (struct sockaddr *)&client_address,
                 (socklen_t *)&client_address_len);
    if (this->mClientSocket >= 0) {
      LOG_VERB("open_impl(%s): Accept SUCCESS (fd=%d)", this->get_name(),
               this->mClientSocket);

      return true;
    } else {
      if (errno == EINTR) {
        LOG_WARN("open_impl(%s): Accept FAILED (interrupted) Retry to accept...",
                 this->get_name());
      } else {
        LOG_WARN("open_impl(%s): Accept FAILED (%s)", this->get_name(),
                 strerror(errno));
        return false;
      }
    }
  }

  return false;
}

bool TcpServerSocket::close_impl(void) {
  ::close(this->mClientSocket);
  ::close(this->mServerSocket);
  this->mClientSocket = 0;
  this->mServerSocket = 0;

  LOG_VERB("close_impl(%s): DONE", this->get_name());

  return true;
}

int TcpServerSocket::send_impl(const void *data_buffer, size_t data_length) {
  int sent_bytes = 0;

  if (this->mClientSocket <= 0) {
    LOG_WARN("send_impl(%s): FAILED - socket closed", this->get_name());
    return -1;
  }

  while (sent_bytes < data_length) {
    int once_sent_bytes =
        ::send(this->mClientSocket, data_buffer, data_length, MSG_NOSIGNAL);
    if (once_sent_bytes <= 0) {
      return once_sent_bytes;
    }
#if VERBOSE_WFD_MSG != 0
    LOG_DEBUG("send_impl(%s): size=%d", this->get_name(), once_sent_bytes);
#endif
    sent_bytes += once_sent_bytes;
  }

  return sent_bytes;
}

int TcpServerSocket::receive_impl(void *data_buffer, size_t data_length) {
  int received_bytes = 0;

  if (this->mClientSocket <= 0) {
    LOG_WARN("receive_impl(%s): FAILED - socket closed", this->get_name());
    return -1;
  }

  // Polling
  struct pollfd poll_fd;
  poll_fd.fd = this->mClientSocket;
  poll_fd.events = POLLIN;
  int ret = poll(&poll_fd, 1, 1000);
  if (ret == -1) {
    LOG_ERR("receive_impl(%s): FAILED - polling error", this->get_name());
    return -1;
  } else if (ret == 0) {
    // Receive timeout
    return -999;
  }

  // Read
  while (received_bytes < data_length) {
    int left_data_bytes = data_length - received_bytes;
    int once_received_bytes =
        ::recv(this->mClientSocket, (void*)((uint8_t*)data_buffer + received_bytes), left_data_bytes, 0);
    if (once_received_bytes <= 0) {
      return once_received_bytes;
    }

    received_bytes += once_received_bytes;
#if VERBOSE_WFD_MSG != 0
    LOG_DEBUG("receive_impl(%s): size=%d", this->get_name(), once_received_bytes);
#endif
  }

  if (received_bytes < 0) {
    LOG_ERR("receive_impl(%s): FAILED (fd=%d)", this->get_name(),
            this->mClientSocket);
  }

  return received_bytes;
}

void TcpServerSocket::on_change_ip_address(const char *ip_address) {
  this->set_ip_address_raw(inet_addr(ip_address));
}
