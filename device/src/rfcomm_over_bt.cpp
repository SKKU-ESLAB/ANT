/* Copyright 2017 Eunsoo Park (esevan.park@gmail.com). All rights reserved
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

#include <rfcomm_over_bt.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

namespace cm {
RfcommServerOverBt::RfcommServerOverBt(uint16_t id, char *svc_uuid) {
  this->dev_id = id;

  str2uuid(svc_uuid, &(this->svc_uuid));
  set_controllable();
}
bool RfcommServerOverBt::device_on() {
  return true;
}
bool RfcommServerOverBt::device_off() {
  return true;
}
bool RfcommServerOverBt::make_connection() {
  int res = bt_open();
  if (res < 0) return false;

  struct sockaddr_rc cli_addr = {0, };
  socklen_t opt = sizeof(cli_addr);

  OPEL_DBG_LOG("Bluetooth accept...");
  cli_sock = accept(serv_sock, (struct sockaddr *)&cli_addr, &opt);
  if (cli_sock < 0) {
    OPEL_DBG_WARN("Bluetooth accept failed(%s)", strerror(errno));
    return false;
  }

  return true;
}

int RfcommServerOverBt::str2uuid(char *str, uuid_t *uuid) {
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

int RfcommServerOverBt::uuid2strn(uuid_t *uuid, char *str, int len) {
  return sdp_uuid2strn(uuid, str, len);
}

int RfcommServerOverBt::bt_register_service() {
  char service_name[256];
  char service_dsc[256];
  char service_prov[256];
  sdp_session_t *t_session = 0;
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
    sdp_set_service_id(record, svc_uuid);

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
    channel = sdp_data_alloc(SDP_UINT8, &port);
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
    t_session = sdp_connect(&my_bdaddr_any, &my_bdaddr_local, SDP_RETRY_IF_BUSY);
    if (NULL == t_session) {
      OPEL_DBG_ERR("Cannot connect to bluetooth sdp server");
      res = -1;
      break;
    }

    res = sdp_record_register(t_session, record, 0);

  } while (0);

  sdp_data_free(channel);
  sdp_list_free(l2cap_list, 0);
  sdp_list_free(rfcomm_list, 0);
  sdp_list_free(root_list, 0);
  sdp_list_free(access_proto_list, 0);
  sdp_record_free(record);

  return res;
}

int RfcommServerOverBt::bt_dynamic_bind_rc() {
  int err;
  int tmp_port;
  struct sockaddr_rc sockaddr;

  bdaddr_t my_bdaddr_any = {0, 0, 0, 0, 0, 0};
  sockaddr.rc_family = AF_BLUETOOTH;
  sockaddr.rc_bdaddr = my_bdaddr_any;
  sockaddr.rc_channel = (unsigned char) 0;

  for (tmp_port = 1; tmp_port < 31; tmp_port++) {
    sockaddr.rc_channel = tmp_port;
    err = bind(serv_sock, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr_rc));
    if (!err) {
      return tmp_port;
    }

    if (errno == EINVAL) break;
  }

  if (tmp_port == 31) err = -1;

  return err;
}

int RfcommServerOverBt::bt_open() {
  if (serv_sock > 0) {
    if (session != NULL) return serv_sock;

    close(serv_sock);
    serv_sock = 0;
  } else if (session != NULL) {
    sdp_close(session);
    session = NULL;
  }

  serv_sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
  if (serv_sock < 0) {
    OPEL_DBG_WARN("Bluetooth socket open failed(%s)", strerror(errno));
    return -1;
  }

  port = bt_dynamic_bind_rc();
  if (port < 1 || port > 30) {
    OPEL_DBG_WARN("Bluetooth socket bind failed(%s)", strerror(errno));
    return -1;
  }

  if (bt_register_service() < 0) {
    OPEL_DBG_WARN("Bluetooth sdp session creation failed(%s)", strerror(errno));
    return -1;
  }

  if (listen(serv_sock, 1) < 0) {
    OPEL_DBG_WARN("Listening failed(%s)", strerror(errno));
    return -1;
  }
  
  return serv_sock;
}

int RfcommServerOverBt::send(const void *buf, size_t len) {
  int sent = 0;

  if (cli_sock <= 0)
    return -1;

  while (sent < len) {
    int sent_bytes = write(cli_sock, buf, len);
    if (sent_bytes <= 0) {
      OPEL_DBG_WARN("Cli sock closed");
      return -1;
    }

    sent += sent_bytes;
  }

  return sent;
}

int RfcommServerOverBt::recv(void *buf, size_t len) {
  int recved = 0;

  if (cli_sock <= 0)
    return -1;

  while (recved < len) {
    int recv_bytes = read(cli_sock, buf, len);
    if (recv_bytes <= 0) {
      OPEL_DBG_WARN("Cli sock closed");
      return -1;
    }

    recved += recv_bytes;
  }

  return recved;
}

uint16_t RfcommServerOverBt::get_id() {
  return dev_id;
}

void RfcommServerOverBt::on_control_recv(const void *buf, size_t len) {
  return;
}

bool RfcommServerOverBt::close_connection() {
  close(cli_sock);
  close(serv_sock);

  cli_sock = 0;
  serv_sock = 0;

  return true;
}
}
