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

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "CommLog.h"
#include "TmpControl.h"

#include "CommPort.h"

#define ANT_WIFI_DIRECT_IP "192.168.49.1"

bool WifiDirectCommPort::openConnection() {
  __ENTER__;
  if(this->isOpened()) {
    CommLog("Wifi direct port has already opened");
    __EXIT__;
    return true;
  }

  // Open socket
  int listenedSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(listenedSocket < 0){
    CommLog("Wifi direct port open error: socket open fail");
    __EXIT__;
    return false;
	}

  // Set socket option
	struct sockaddr_in socketAddress;
	memset(&socketAddress, 0, sizeof(socketAddress));
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_addr.s_addr = inet_addr(ANT_WIFI_DIRECT_IP);
	socketAddress.sin_port = htons(this->mTcpPortNum);
	int reuse = 1;
	if(setsockopt(listenedSocket, SOL_SOCKET, SO_REUSEADDR,
        (char *)&reuse, sizeof(int)) == -1){
    CommLog("Wifi direct port open error: socket option fail");
    __EXIT__;
    return false;
	}

  // Bind
  int bindRes = bind(listenedSocket, (struct sockaddr *)&socketAddress,
      sizeof(socketAddress));
	if(bindRes < 0){
		CommLog("Wifi direct port open error: bind fail (%s)",
        strerror(errno));
    __EXIT__;
    return false;
	}

  // Listen
#define NUM_PENDING_CONNECTIONS 5
  int listenRes = listen(listenedSocket, NUM_PENDING_CONNECTIONS);
	if(listenRes < 0){
    CommLog("Wifi direct port open error: listen fail");
    __EXIT__;
    return false;
	}
	
  // Set socket
	CommLog("Wifi direct port listening success");
  this->setListenedSocket(listenedSocket);

  this->CommPort::openConnection();
  __EXIT__;
  return true;
}

bool WifiDirectCommPort::acceptConnection() {
  __ENTER__;
  // Accept connection
	struct sockaddr_in clientAddress;
	int clientAddressLength = sizeof(clientAddress);
	CommLog("Wifi direct port is now accepting: port = %d", this->mTcpPortNum);
  int newSocket = accept(this->getListenedSocket(),
      (struct sockaddr *)&clientAddress,
      (socklen_t *)&clientAddressLength);
	if(newSocket < 0) {
		CommLog("Wifi direct port open error: accept fail (%s)", strerror(errno));
    this->closeConnection();
    __EXIT__;
		return false;
	}

  // Set new accpeted socket
  // socket: NULL -> listened socket -> accepted socket
  this->setSocket(newSocket);

	CommLog("Wifi direct port opening complete: client address = %s \n",\
			inet_ntoa(*(struct in_addr *)&clientAddress.sin_addr));
  
  this->CommPort::acceptConnection();
  __EXIT__;
  return true;
}

void WifiDirectCommPort::closeConnection() {
  __ENTER__;

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
