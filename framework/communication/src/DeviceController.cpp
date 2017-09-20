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

#include "DeviceController.h"

bool BluetoothDeviceController::turnOn() {
  // not implemented
  // RedCarrottt: adding hciconfig control here would be good
  __ENTER__;
  
  this->DeviceController::turnOn();
  __EXIT__;
  return true;
}

bool BluetoothDeviceController::turnOff() {
  // not implemented
  // RedCarrottt: adding hciconfig control here would be good
  __ENTER__;
  
  this->DeviceController::turnOff();
  __EXIT__;
  return true;
}

bool WifiDirectDeviceController::turnOn() {
	__ENTER__;

	char wfdStatStr[256];
  int wfdStat;
	tmpc_get("wifi/wifi-direct/wfd_stat", wfdStatStr, 256);
  wfdStat = atoi(wfdStatStr);
  while(wfdStat == 0) {
    // Reset before initializing Wi-fi Direct
    {
      char command[256]; 
      strcpy(command, getenv("ANT_BIN_DIR"));
      strcat(command, "/ant_p2p_setup.sh stop");
      system(command);
    }

    // Initialize Wi-fi Direct
//    char isInitStr[256];
//    tmpc_get("wifi/wifi-direct/init", isInitStr, 256);
//    int isInit = atoi(isInitStr);
//    if(isInit == 1){
//      CommLog("Wifi Direct has already turned on");
//    } else {
    {
      char command[256];
      strcpy(command, getenv("ANT_BIN_DIR"));
      strcat(command, "/ant_p2p_setup.sh init");
      system(command);
    }

    // Start Wi-fi Direct
    {
      char command[256]; 
      strcpy(command, getenv("ANT_BIN_DIR"));
      strcat(command, "/ant_p2p_setup.sh start");
      system(command);
      CommLog("P2p start!");
    }

    tmpc_get("wifi/wifi-direct/wfd_stat", wfdStatStr, 256);
    wfdStat = atoi(wfdStatStr);
  }

	CommLog("Wifi direct turned on");
  this->DeviceController::turnOn();
	__EXIT__;
  return true;
}

bool WifiDirectDeviceController::turnOff() {
  __ENTER__;
	char command[256]; 
	strcpy(command, getenv("ANT_BIN_DIR"));
	strcat(command, "/ant_p2p_setup.sh stop");
	system(command);
	CommLog("Wifi direct turned off");

  // RedCarrottt: is "ant_p2p_setup.sh deinit" required?

  this->DeviceController::turnOff();
  __EXIT__;
  return true;
}
