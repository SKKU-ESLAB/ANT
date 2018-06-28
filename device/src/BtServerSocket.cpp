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
#include <BtP2pServer.h>
#include <Util.h>

#include <counter.h>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

using namespace cm;

bool BtServerSocket::open_impl(void) {
  if (this->mServerSocket > 0) {
    close(this->mServerSocket);
    this->mServerSocket = 0;
  }

  this->mServerSocket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
  if (this->mServerSocket < 0) {
    LOG_ERR("Bluetooth socket open failed(%s)", strerror(errno));
    return false;
  }

  this->mPort = this->bt_dynamic_bind_rc();
  if (this->mPort < 1 || this->mPort > 30) {
    LOG_ERR("Bluetooth socket bind failed(%s)", strerror(errno));
    return false;
  }

  if (this->bt_register_service() < 0) {
    LOG_ERR("Bluetooth sdp session creation failed(%s)", strerror(errno));
    return false;
  }

  if (listen(this->mServerSocket, 1) < 0) {
    LOG_ERR("Listening failed(%s)", strerror(errno));
    return false;
  }

  if (this->mServerSocket < 0) return false;
  
  struct sockaddr_rc client_addr = {0, };
  socklen_t opt = sizeof(client_addr);
  LOG_VERB("Bluetooth accept...");
  this->mClientSocket = accept(this->mServerSocket, (struct sockaddr *)&client_addr, &opt);
  if (this->mClientSocket < 0) {
    LOG_ERR("Bluetooth accept failed(%s)", strerror(errno));
    return false;
  }
  
  return true;
}

int BtServerSocket::bt_dynamic_bind_rc(void) {
  int err;
  int port;
  struct sockaddr_rc sockaddr;

  bdaddr_t my_bdaddr_any = {0, 0, 0, 0, 0, 0};
  sockaddr.rc_family = AF_BLUETOOTH;
  sockaddr.rc_bdaddr = my_bdaddr_any;
  sockaddr.rc_channel = (unsigned char) 0;

  for (port = 1; port < 31; port++) {
    sockaddr.rc_channel = port;
    err = bind(this->mServerSocket, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr_rc));
    if (!err) {
      LOG_VERB("BT port binded : %d", port);
      return port;
    }

    if (errno == EINVAL) break;
  }

  if (port == 31) err = -1;

  return err;
}

int BtServerSocket::bt_register_service() {
  char service_name[256];
  char service_dsc[256];
  char service_prov[256];
  sdp_session_t *sdp_session = 0;
  uuid_t root_uuid, l2cap_uuid, rfcomm_uuid;
  sdp_list_t *l2cap_list = 0,
             *rfcomm_list = 0,
             *root_list = 0,
             *proto_list = 0,
             *access_proto_list = 0;
  sdp_data_t *channel = 0;
  sdp_record_t *record = sdp_record_alloc();

  int res = 0;
  do {
    //Set the general service ID
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
    sdp_session = sdp_connect(&my_bdaddr_any, &my_bdaddr_local, SDP_RETRY_IF_BUSY);
    if (NULL == sdp_session) {
      LOG_ERR("Cannot connect to bluetooth sdp server");
      res = -1;
      break;
    }

    res = sdp_record_register(sdp_session, record, 0);

  } while (0);

  sdp_data_free(channel);
  sdp_list_free(l2cap_list, 0);
  sdp_list_free(rfcomm_list, 0);
  sdp_list_free(root_list, 0);
  sdp_list_free(access_proto_list, 0);
  sdp_record_free(record);

  return res;
}

bool BtServerSocket::close_impl(void) {
  close(this->mClientSocket);
  close(this->mServerSocket);

  this->mClientSocket = 0;
  this->mServerSocket = 0;

  return true;
}

int BtServerSocket::send_impl(const void *data_buffer, size_t data_length) {
  int sent_bytes = 0;

  if (cli_sock <= 0) {
    LOG_WARN("Socket closed\n");
    return -1;
  }

  while (sent_bytes < data_length) {
    int once_sent_bytes = write(cli_sock, data_buffer, data_length);
    if (once_sent_bytes <= 0) {
      LOG_WARN("Cli sock closed");
      return -1;
    }
    LOG_DEBUG("BT %d] send: %d\n", this->mPort, once_sent_bytes);
    sent_bytes += once_sent_bytes;
  }

  return sent_bytes;
}

int BtServerSocket::receive_impl(void *data_buffer, size_t data_length) {
  int received_bytes = 0;

  if (cli_sock <= 0)
    return -1;

  while (received_bytes < data_length) {
    int once_received_bytes = read(cli_sock, data_buffer, data_length);
    if (once_received_bytes <= 0) {
      LOG_WARN("Cli sock closed");
      return -1;
    }

    received_bytes += once_received_bytes;
    LOG_DEBUG("BT %d] receive : %d\n", this->mPort, once_received_bytes);
  }

  return received_bytes;
}

int BtServerSocket::str2uuid(char *str, uuid_t *uuid) {
  if (strlen(str) != 36)
    return -1;
  
  uint8_t adv_data[16];

  int strCounter = 0;
  int hexCounter = 0;
  while (strCounter<strlen(str)) {
    if (str[strCounter] == '-') {
      strCounter++;
      continue;
    }

    char hex[3] = {0,};
    hex[0] = str[strCounter++];
    hex[1] = str[strCounter++];

    adv_data[hexCounter] = strtol(hex, NULL, 16);

    hexCounter++;
  }

  sdp_uuid128_create(uuid, adv_data);

  return 0;
}
