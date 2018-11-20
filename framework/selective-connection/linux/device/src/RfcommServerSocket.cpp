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
#include "../inc/RfcommServerSocket.h"

#include "../../common/inc/DebugLog.h"

#include <mutex>
#include <thread>

#include <arpa/inet.h>
#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define VERBOSE_BT_MSG 0

using namespace sc;

bool RfcommServerSocket::open_impl(void) {
  if (this->mServerSocket > 0) {
    ::close(this->mServerSocket);
    this->mServerSocket = 0;
  }

  this->mServerSocket = ::socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
  if (this->mServerSocket < 0) {
    LOG_ERR("open_impl(%s): socket open error (%s)", this->get_name(),
            strerror(errno));
    return false;
  }

  this->mPort = this->bt_dynamic_bind_rc();
  if (this->mPort < 1 || this->mPort > 30) {
    LOG_ERR("open_impl(%s): bt_dynamic_bind_rc error (%s)", this->get_name(),
            strerror(errno));
    return false;
  }

  if (this->bt_register_service() < 0) {
    LOG_ERR("open_impl(%s): bt_register_service error (%s)", this->get_name(),
            strerror(errno));
    return false;
  }

  if (::listen(this->mServerSocket, 1) < 0) {
    LOG_ERR("open_impl(%s): listen error (%s)", this->get_name(), strerror(errno));
    return false;
  }

  if (this->mServerSocket < 0)
    return false;

  struct sockaddr_rc client_addr = {
      0,
  };
  socklen_t opt = sizeof(client_addr);
  LOG_VERB("open_impl(%s): Accepting...", this->get_name());
  this->mClientSocket =
      ::accept(this->mServerSocket, (struct sockaddr *)&client_addr, &opt);
  if (this->mClientSocket < 0) {
    LOG_ERR("open_impl(%s): Accept FAILED (%s)", this->get_name(), strerror(errno));
    return false;
  }
  
  LOG_VERB("open_impl(%s): Accept SUCCESS (fd=%d)", this->get_name(),
            this->mClientSocket);

  return true;
}

int RfcommServerSocket::bt_dynamic_bind_rc(void) {
  int err;
  int port;
  struct sockaddr_rc sockaddr;

  bdaddr_t my_bdaddr_any = {0, 0, 0, 0, 0, 0};
  sockaddr.rc_family = AF_BLUETOOTH;
  sockaddr.rc_bdaddr = my_bdaddr_any;
  sockaddr.rc_channel = (unsigned char)0;

  for (port = 1; port < 31; port++) {
    sockaddr.rc_channel = port;
    err = ::bind(this->mServerSocket, (struct sockaddr *)&sockaddr,
                 sizeof(struct sockaddr_rc));
    if (!err) {
      LOG_DEBUG("%s: port binded: %d", this->get_name(), port);
      return port;
    }

    if (errno == EINVAL)
      break;
  }

  if (port == 31)
    err = -1;

  return err;
}

int RfcommServerSocket::bt_register_service() {
  char service_name[256];
  char service_dsc[256];
  char service_prov[256];
  this->mSdpSession = NULL;
  uuid_t root_uuid, l2cap_uuid, rfcomm_uuid;
  sdp_list_t *l2cap_list = 0, *rfcomm_list = 0, *root_list = 0, *proto_list = 0,
             *access_proto_list = 0;
  sdp_data_t *channel = 0;
  sdp_record_t *record = sdp_record_alloc();

  int res = 0;
  do {
    // Set the general service ID
    sdp_set_service_id(record, this->mServiceUUID);

    // make the service record publicly browsable
    sdp_uuid16_create(&root_uuid, PUBLIC_BROWSE_GROUP);
    root_list = sdp_list_append(0, &root_uuid);
    sdp_set_browse_groups(record, root_list);

    // Set l2cap information
    sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
    l2cap_list = sdp_list_append(0, &l2cap_uuid);
    proto_list = sdp_list_append(0, l2cap_list);

    // Set rfcomm information
    sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
    channel = sdp_data_alloc(SDP_UINT8, &this->mPort);
    rfcomm_list = sdp_list_append(0, &rfcomm_uuid);
    sdp_list_append(rfcomm_list, channel);
    sdp_list_append(proto_list, rfcomm_list);

    // Attach protocol information to service record
    access_proto_list = sdp_list_append(0, proto_list);
    sdp_set_access_protos(record, access_proto_list);

    // Set the name, provider, and description
    sdp_set_info_attr(record, service_name, service_prov, service_dsc);

    // Connect to the local SDP server, register the service record, and
    // disconnect
    bdaddr_t my_bdaddr_any = {0, 0, 0, 0, 0, 0};
    bdaddr_t my_bdaddr_local = {0, 0, 0, 0xff, 0xff, 0xff};
    this->mSdpSession =
        sdp_connect(&my_bdaddr_any, &my_bdaddr_local, SDP_RETRY_IF_BUSY);
    if (NULL == this->mSdpSession) {
      LOG_ERR("%s: SDP server connection error", this->get_name());
      res = -1;
      break;
    }

    res = sdp_record_register(this->mSdpSession, record, 0);

  } while (0);

  sdp_data_free(channel);
  sdp_list_free(l2cap_list, 0);
  sdp_list_free(rfcomm_list, 0);
  sdp_list_free(root_list, 0);
  sdp_list_free(access_proto_list, 0);
  sdp_record_free(record);

  return res;
}

bool RfcommServerSocket::close_impl(void) {
  ::close(this->mClientSocket);
  ::close(this->mServerSocket);
  sdp_close(this->mSdpSession);

  this->mClientSocket = 0;
  this->mServerSocket = 0;
  this->mSdpSession = NULL;

  LOG_VERB("close_impl(%s): DONE", this->get_name());

  return true;
}

int RfcommServerSocket::send_impl(const void *data_buffer, size_t data_length) {
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
#if VERBOSE_BT_MSG != 0
    LOG_DEBUG("send_impl(%s): size=%d", this->get_name(), once_sent_bytes);
#endif
    sent_bytes += once_sent_bytes;
  }

  return sent_bytes;
}

int RfcommServerSocket::receive_impl(void *data_buffer, size_t data_length) {
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
#if VERBOSE_BT_MSG != 0
    LOG_DEBUG("receive_impl(%s): size=%d", this->get_name(), once_received_bytes);
#endif
  }

  if (received_bytes <= 0) {
    LOG_ERR("receive_impl(%s): FAILED (res=%d, fd=%d)", received_bytes,
            this->mClientSocket);
  }

  return received_bytes;
}

int RfcommServerSocket::str2uuid(char *str, uuid_t *uuid) {
  if (strlen(str) != 36)
    return -1;

  uint8_t adv_data[16];

  int strCounter = 0;
  int hexCounter = 0;
  while (strCounter < strlen(str)) {
    if (str[strCounter] == '-') {
      strCounter++;
      continue;
    }

    char hex[3] = {
        0,
    };
    hex[0] = str[strCounter++];
    hex[1] = str[strCounter++];

    adv_data[hexCounter] = strtol(hex, NULL, 16);

    hexCounter++;
  }

  sdp_uuid128_create(uuid, adv_data);

  return 0;
}
