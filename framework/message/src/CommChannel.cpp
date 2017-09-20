/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
 *              Dongig Sin<dongig@skku.edu>
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

#include <time.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>  
#include <string.h>  

#include <sys/socket.h>  
#include <sys/ioctl.h>  
#include <sys/stat.h>  
#include <netinet/in.h>  
#include <net/if.h>  
#include <arpa/inet.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>

#include "CommChannel.h"
#include "ANTdbugLog.h"
#include "MessageRouter.h"
#include "MessageFactory.h"
#include "BaseMessage.h"
#include "CommPort.h"

#define RAW_STRING_SIZE 1024
#define EXT4_FILE_PATH_LENGTH 4097

void CommChannelState::set(Value newValue) {
  Value oldValue = this->mValue;
  this->mValue = newValue;
  if((this->mStateListener != NULL) && (oldValue != newValue)) {
    this->mStateListener->onCommChannelStateChanged(this->mValue);
  }
}

void CommChannel::run() {
  // Turn on Bluetooth
  this->mBluetoothDeviceController.turnOn();

  // Start CommChannel
  this->start();

  // Run RoutedLoop on separate thread
  this->runRoutedLoop(true);
}

void CommChannel::onCommPortStateChanged(CommPort *port,
    CommPortState::Value state) {
  CommChannelState::Value presentChannelState = this->mState.get();
  if(port == this->mLargeDataPort) {
    // LargeData Port:
    //  - OPENED, CONNECTED -> CLOSED: Go to CONNECTED_DEFAULT
    //  - OPENED -> CONNECTED: Go to CONNECTED_LARGEDATA
    switch(state) {
      case CommPortState::CLOSED:
        if(presentChannelState == CommChannelState::LISTENING_LARGEDATA
            || presentChannelState == CommChannelState::CONNECTED_LARGEDATA) {
          this->mState.set(CommChannelState::CONNECTED_DEFAULT);
        }
        break;
      case CommPortState::CONNECTED:
        if(presentChannelState == CommChannelState::LISTENING_LARGEDATA) {
          this->mState.set(CommChannelState::CONNECTED_LARGEDATA);
        }
        break;
    }
  } else if(port == this->mDefaultPort) {
    // Default Port:
    //  - OPENED, CONNECTED -> CLOSED: Go to IDLE
    //  - OPENED -> CONNECTED: Go to CONNECTED_DEFAULT
     switch(state) {
       case CommPortState::CLOSED:
         if(presentChannelState != CommChannelState::IDLE) {
           this->mState.set(CommChannelState::IDLE);
         }
         break;
       case CommPortState::CONNECTED:
         if(presentChannelState == CommChannelState::LISTENING_DEFAULT) {
           this->mState.set(CommChannelState::CONNECTED_DEFAULT);
         }
         break;
     }
  } else if(port == this->mControlPort) {
    // Control Port:
    //  - OPENED, CONNECTED -> CLOSED: Retry open
     switch(state) {
       case CommPortState::CLOSED:
         if(presentChannelState != CommChannelState::IDLE) {
           bool controlOpenRes = this->mControlPort->openConnection();
           if(!controlOpenRes) {
             ANT_DBG_ERR("CommChannel run: opening control port fail");
           } else {
             this->mControlPort->runListeningThread(
                 this->mControlPortsListener, this->mFileTempDir);
             ANT_DBG_ERR("CommChannel control port closed -> reopened");
           }
         }
         break;
     }
  } else {
    ANT_DBG_ERR("Unknown CommPort's state is changed!");
  }
}

bool CommChannel::start() {
  ANT_DBG_VERB("Start CommChannel");
  
  // Open default/control ports
  bool defaultOpenRes = this->mDefaultPort->openConnection();
  bool controlOpenRes = this->mControlPort->openConnection();
  if(!defaultOpenRes) {
    ANT_DBG_ERR("CommChannel run: opening default port fail");
    return false;
  }
  if(!controlOpenRes) {
    ANT_DBG_ERR("CommChannel run: opening control port fail");
    return false;
  }

  // Run ListeningLoop of default/control ports on separate thread
  this->mDefaultPort->runListeningThread(
      this->mDataPortsListener, this->mFileTempDir);
  this->mControlPort->runListeningThread(
      this->mControlPortsListener, this->mFileTempDir);

  // Channel state goes to LISTENING_DEFAULT
  this->mState.set(CommChannelState::LISTENING_DEFAULT);

  return true;
}

void CommChannel::stop() {
  ANT_DBG_VERB("Stop CommChannel");

  // Close all the ports
  this->mLargeDataPort->closeConnection();
  this->mDefaultPort->closeConnection();
  this->mControlPort->closeConnection();

  // Channel state goes to IDLE
  this->mState.set(CommChannelState::IDLE);
}

bool CommChannel::enableLargeDataMode() {
  // Turn on Wi-fi Direct
  bool wfdTurnOnRes = this->mWifiDirectDeviceController.turnOn();
  if(!wfdTurnOnRes) {
    ANT_DBG_ERR("CommChannel enableLargeData: turning on Wi-fi Direct fail");
    return false;
  }

  // Open largedata port
  bool largeDataOpenRes = this->mLargeDataPort->openConnection();
  if(!largeDataOpenRes) {
    ANT_DBG_ERR("CommChannel enableLargeData: opening largedata port fail");
    return false;
  }

  // Channel state goes to LISTENING_LARGEDATA
  this->mState.set(CommChannelState::LISTENING_LARGEDATA);

  // Run ListeningLoop of largedata port on separate thread
  this->mLargeDataPort->runListeningThread(
      this->mDataPortsListener, this->mFileTempDir);
  return true;
}

void CommChannel::disableLargeDataMode() {
  // Stop ListeningLoop of largedata port
  this->mLargeDataPort->stopListeningThread();

  // Turn on Wi-fi Direct
  bool wfdTurnOffRes = this->mWifiDirectDeviceController.turnOff();
  if(!wfdTurnOffRes) {
    ANT_DBG_ERR("CommChannel disableLargeData: turning off Wi-fi Direct fail");
  }
  
  // Channel state goes to CONNECTED_DEFAULT
  this->mState.set(CommChannelState::CONNECTED_DEFAULT);
}

void CommChannel::onRoutedMessage(BaseMessage* message) {
  // RedCarrottt: It works for only single companion.

  // Send message as raw string 
  const char* rawString = message->toJSONString();

  CommChannelState::Value state = this->mState.get();
  CommPort* targetPort = NULL;
  switch(state) {
    case CommChannelState::IDLE:
    case CommChannelState::LISTENING_DEFAULT:
      ANT_DBG_ERR("CommChannel has not initialized!");
      return;
      break;
    case CommChannelState::CONNECTED_DEFAULT:
    case CommChannelState::LISTENING_LARGEDATA:
      targetPort = this->mDefaultPort;
      break;
    case CommChannelState::CONNECTED_LARGEDATA:
      targetPort = this->mLargeDataPort;
      break;
    default:
      ANT_DBG_ERR("Unknown CommChannel state");
      return;
      break;
  }

  bool sendRes;
  if(message->isFileAttached()) {
    sendRes = targetPort->sendRawMessage(std::string(rawString),
        message->getStoredFilePath());
  } else {
    sendRes = targetPort->sendRawMessage(std::string(rawString));
  }

  if(!sendRes) {
    ANT_DBG_ERR("Sending message error");
  }
}

void CommChannel::onReceivedMessage(BaseMessage* message) {
  // Route message to the target of the base message
  this->mMessageRouter->routeMessage(message);
}

int CommChannel::getIpAddress(const char* interfaceName, char* ipAddr) {
  int sockfd; 
  char _ipAddr[40]; 
  struct ifreq ifr; 

  strncpy(ifr.ifr_name, interfaceName, IFNAMSIZ); 
  sockfd = socket(AF_INET, SOCK_STREAM, 0); 

  if(ioctl(sockfd, SIOCGIFADDR, &ifr) < 0) {
    perror("ioctl"); 
    return -1; 
  } 
  
  inet_ntop(AF_INET, ifr.ifr_addr.sa_data + 2, _ipAddr,
      sizeof(struct sockaddr)); 

  strncpy(ipAddr, _ipAddr, strlen(_ipAddr));
  return 0;
}

void DataPortsListener::onReceivedRawMessage(std::string messageData,
    std::string filePath) {
  // Raw message from default port or largedata port
  ANT_DBG_VERB("Received raw message from CommChannel: %s / %s",
      messageData.c_str(), filePath.c_str());

  // Parse rawString into base message
  BaseMessage* message = MessageFactory::makeMessageFromJSONString(
      messageData.c_str());
  if(message == NULL) {
    ANT_DBG_ERR("Received message is not base message!: %s",
        messageData.c_str());
    return;
  }
  if(filePath.length() > 0) {
    message->attachFile(filePath);
  }

  this->mOwner->onReceivedMessage(message);
}

void ControlPortsListener::onReceivedRawMessage(std::string messageData,
    std::string filePath) {
  this->mOwner->onReceivedControlMessage(messageData);
}

#define WLAN_P2P_INTERFACE_NAME "p2p-wlan0-0"
#define WLAN_INTERFACE_NAME "wlan0"
#define ETHERNET_INTERFACE_NAME "eth0"

void CommChannel::onReceivedControlMessage(std::string messageData) {
  // Raw message from control port 
  ANT_DBG_VERB("Received control message for CommChannel: %s",
      messageData.c_str());

  if(messageData.compare("on") == 0) {
    // Enable largedata mode
    bool enableLargeDataRes = this->enableLargeDataMode();
    if(enableLargeDataRes) {
      // If succeed to enable large data mode,
      // send my IP address and WFD name to the client

      // Get my IP address
      char buffer[1024];
      int getIpRes;
      getIpRes = this->getIpAddress(WLAN_P2P_INTERFACE_NAME, buffer);
      if(getIpRes < 0) {
        getIpRes = this->getIpAddress(WLAN_INTERFACE_NAME, buffer);
      }
      if(getIpRes < 0) {
        getIpRes = this->getIpAddress(ETHERNET_INTERFACE_NAME, buffer);
      }
      if(getIpRes < 0) {
        ANT_DBG_ERR("Cannot get IP address & WFD name");
        return;
      }

      // Get my WFD name
      char* wfdName = getenv("ANT_WFD_NAME");
      if(wfdName == NULL) {
        ANT_DBG_ERR("Cannot get Wi-fi Direct name");
        return;
      }
      strncat(buffer, "\n", strlen("\n"));
      strncat(buffer, wfdName, strlen(wfdName));

      std::string wfdServerInfo(buffer);
      this->mControlPort->sendRawMessage(wfdServerInfo);
    }
  } else if(messageData.compare("off") == 0) {
    // Disable largedata mode
    this->disableLargeDataMode();
  } else {
    ANT_DBG_ERR("Unknown message has arrived via control port: %s",
        messageData.c_str());
  }
}
