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

#include <TcpServerSocket.h>

#include <DebugLog.h>

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
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
    LOG_ERR("%s: Socket setup failed", this->get_name());
    return false;
  }

  int err = ::bind(this->mServerSocket, (struct sockaddr *)&server_address,
                   sizeof(server_address));
  if (err < 0) {
    LOG_ERR("%s: Socket bind failed", this->get_name());
    return false;
  }

  err = ::listen(this->mServerSocket, 5);
  if (err < 0) {
    LOG_ERR("%s: Socket listen failed", this->get_name());
    return false;
  }

  struct sockaddr_in client_address;
  memset(&client_address, 0, sizeof(client_address));
  int client_address_len = sizeof(client_address);
  const int kMaxTries = 10;
  for (int tries = 0; tries < kMaxTries; tries++) {
    LOG_VERB("%s: Accepting client... (%d)", this->get_name(), tries);
    this->mClientSocket =
        ::accept(this->mServerSocket, (struct sockaddr *)&client_address,
                 (socklen_t *)&client_address_len);
    if (this->mClientSocket >= 0) {
      return true;
    } else {
      if (errno == EINTR) {
        LOG_WARN("%s: Interrupted system call: Retry to accept...", this->get_name());
      } else {
        LOG_ERR("%s: Accept failed %s", this->get_name(), strerror(errno));
        return false;
      }
    }
  }
  return true;
}

bool TcpServerSocket::close_impl(void) {
  ::close(this->mClientSocket);
  ::close(this->mServerSocket);
  this->mClientSocket = 0;
  this->mServerSocket = 0;

  LOG_VERB("%s: TCP Socket closed", this->get_name());

  return true;
}

int TcpServerSocket:: send_impl(const void *data_buffer, size_t data_length) {
  int sent_bytes = 0;

  if (this->mClientSocket <= 0)
    return -1;

  while (sent_bytes < data_length) {
    int once_sent_bytes =
        ::write(this->mClientSocket, data_buffer, data_length);
    if (once_sent_bytes <= 0) {
      return -1;
    }
#if VERBOSE_WFD_MSG != 0
    LOG_DEBUG("%s: Send: %d", this->get_name(), once_sent_bytes);
#endif
    sent_bytes += once_sent_bytes;
  }

  return sent_bytes;
}

int TcpServerSocket::receive_impl(void *data_buffer, size_t data_length) {
  int received_bytes = 0;

  if (this->mClientSocket <= 0)
    return -1;

  while (received_bytes < data_length) {
    int once_received_bytes =
        ::read(this->mClientSocket, data_buffer, data_length);
    if (once_received_bytes <= 0) {
      return -1;
    }

    received_bytes += once_received_bytes;
#if VERBOSE_WFD_MSG != 0
    LOG_DEBUG("%s: Receive : %d", this->get_name(), once_received_bytes);
#endif
  }

  return received_bytes;
}

void TcpServerSocket::on_change_ip_address(const char *ip_address) {
  this->set_ip_address_raw(inet_addr(ip_address));
}
