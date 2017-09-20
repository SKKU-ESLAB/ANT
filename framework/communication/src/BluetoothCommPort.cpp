/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
 *              Eunsoo Park<esevan.park@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include "CommLog.h"

#include "CommPort.h"

#define MIN_BLUETOOTH_PORT 1
#define MAX_BLUETOOTH_PORT 30
bool BluetoothCommPort::openConnection() {
  __ENTER__;

  if(this->isOpened()) {
    CommLog("Bluetooth port has already opened");
    __EXIT__;
    return true;
  }

  // Open socket
  int listenedSocket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
  if(listenedSocket < 0) {
    CommLog("Bluetooth port open error: socket open fail (%s)",
        strerror(errno));
    __EXIT__;
    return false;
  }

  // Dynamic bind
  // Dynamically bind available bluetooth port to the socket
  int bluetoothPort = this->bindDynamically(listenedSocket);
  if(MIN_BLUETOOTH_PORT < 1 || MAX_BLUETOOTH_PORT > 30) {
    CommLog("Bluetooth port open error: dynamic bind failed");
    __EXIT__;
    return false;
  } else {
    CommLog("Bound BT socket %d to BT port %d", listenedSocket, bluetoothPort);
  }

  // Register a bluetooth service of the bluetooth port
  sdp_session_t *sdpSession = this->registerBluetoothService(bluetoothPort);
  if(sdpSession == NULL) {
    CommLog("SDP session creation failed");
    __EXIT__;
    return false;
  }

  // Listen bluetooth socket 
#define NUM_PENDING_CONNECTIONS 1
  int listenRes = listen(listenedSocket, NUM_PENDING_CONNECTIONS);
  if(listenRes < 0) {
    CommLog("Listening failed");
    __EXIT__;
    return false;
  }

  // Set new SDP session and listened socket
  this->mSdpSession = sdpSession;
  this->setListenedSocket(listenedSocket);

	CommLog("Bluetooth port listening success");
  this->CommPort::openConnection();
  __EXIT__;
  return true;
}

bool BluetoothCommPort::acceptConnection() {
  __ENTER__;
  // Client address = any address
  struct sockaddr_rc clientAddress = {0, };
  socklen_t optionLength = sizeof(clientAddress);

  // Accept connection
  int newSocket = accept(this->getListenedSocket(),
      (struct sockaddr *)&clientAddress,
      &optionLength);
  if(newSocket < 0){
    CommLog("Accept Failed");
    this->closeConnection();
    __EXIT__;
    return false;
  }

  // Set new accpeted socket
  // socket: NULL -> listened socket -> accepted socket
  this->setSocket(newSocket);

  CommLog("Bluetooth port accepted");

  this->CommPort::acceptConnection();
  __EXIT__;
  return true;
}

void BluetoothCommPort::closeConnection() {
  __ENTER__;

  if(this->mSdpSession != NULL){
    sdp_close(this->mSdpSession);
    this->mSdpSession = NULL;
  }
  if(this->getSocket() != COMM_PORT_SOCKET_UNINITIALIZED){
    close(this->getSocket());
    this->setSocket(COMM_PORT_SOCKET_UNINITIALIZED);
  }
  if(this->getListenedSocket() != COMM_PORT_SOCKET_UNINITIALIZED) {
    close(this->getListenedSocket());
    this->setListenedSocket(COMM_PORT_SOCKET_UNINITIALIZED);
  }

  this->CommPort::closeConnection();
  __EXIT__;
}

int BluetoothCommPort::bindDynamically(int socket) {
  __ENTER__;
	int bindError;
	int newPort;
	struct sockaddr_rc sockaddr;

	bdaddr_t my_baddr_any = { 0, 0, 0, 0, 0, 0 };
	sockaddr.rc_family = AF_BLUETOOTH;
	sockaddr.rc_bdaddr = my_baddr_any;
	sockaddr.rc_channel = (unsigned char) 0;

	for (newPort = MIN_BLUETOOTH_PORT; newPort <= MAX_BLUETOOTH_PORT; newPort++) {
		sockaddr.rc_channel = newPort;
		bindError = bind(socket, (struct sockaddr *)&sockaddr,
        sizeof(struct sockaddr_rc));
		if(!bindError) {
      __EXIT__;
			return newPort;
		}
		if(bindError == EINVAL) {
      __EXIT__;
      return bindError;
		}
	}
  __EXIT__;
  return -1;
}

sdp_session_t* BluetoothCommPort::registerBluetoothService(int bluetoothPort) {
	__ENTER__;
	char service_name[256];
	char service_dsc[1024];
	char service_prov[256];
	sdp_session_t *t_session = 0;
	char *uuid_char_arr;
	int i;

	uuid_t root_uuid, l2cap_uuid, rfcomm_uuid, svc_uuid;
	sdp_list_t *l2cap_list = 0,
			   *rfcomm_list = 0,
			   *root_list = 0,
			   *proto_list = 0,
			   *access_proto_list = 0;
	sdp_data_t *channel = 0;

  strcpy(service_name, this->getPortName().c_str());
  strcpy(service_dsc, this->getPortName().c_str());
  strcpy(service_prov, this->getPortName().c_str());
  uuid_char_arr = this->mUUID;

	sdp_record_t *record = sdp_record_alloc();

	// set the general service ID
	sdp_uuid128_create(&svc_uuid, uuid_char_arr);
	sdp_set_service_id(record, svc_uuid);

	// make the service record publicly browsable
	sdp_uuid16_create(&root_uuid, PUBLIC_BROWSE_GROUP);
	root_list = sdp_list_append(0, &root_uuid);
	sdp_set_browse_groups(record, root_list);

	// set l2cap information
	sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
	l2cap_list = sdp_list_append(0, &l2cap_uuid);
	proto_list = sdp_list_append(0, l2cap_list);

	// set rfcomm information
	sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
	channel = sdp_data_alloc(SDP_UINT8, &bluetoothPort);
	rfcomm_list = sdp_list_append(0, &rfcomm_uuid);
	sdp_list_append(rfcomm_list, channel);
	sdp_list_append(proto_list, rfcomm_list);

	// attach protocol information to service record
	access_proto_list = sdp_list_append(0, proto_list);
	sdp_set_access_protos(record, access_proto_list);

	// set the name, provider, and description
	sdp_set_info_attr(record, service_name, service_prov, service_dsc);

	// connect to the local SDP server, register the service record, and disconnect
	bdaddr_t my_bdaddr_any = {0, 0, 0, 0, 0, 0};
	bdaddr_t my_bdaddr_local = {0, 0, 0, 0xff, 0xff, 0xff};
	t_session = sdp_connect(&my_bdaddr_any, &my_bdaddr_local, SDP_RETRY_IF_BUSY);
	if(NULL == t_session){
		sdp_data_free(channel);
		sdp_list_free(l2cap_list, 0);
		sdp_list_free(rfcomm_list, 0);
		sdp_list_free(root_list, 0);
		sdp_list_free(access_proto_list, 0);
		sdp_record_free(record); //Let me check later
		__EXIT__;
		return NULL;
	}

	int err = sdp_record_register(t_session, record, 0);

	sdp_data_free( channel );
	sdp_list_free( l2cap_list, 0 );
	sdp_list_free( rfcomm_list, 0 );
	sdp_list_free( root_list, 0 );
	sdp_list_free( access_proto_list, 0 );
	sdp_record_free( record ); //Let me check later
  
	__EXIT__;
	return t_session;
}
