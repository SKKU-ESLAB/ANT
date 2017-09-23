/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
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

#include <iostream>
#include <vector>
#include <dirent.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "MLDaemon.h"
#include "ANTdbugLog.h"

// Message FW
#include "BaseMessage.h"
#include "MessageFactory.h"

using namespace std;

#define APPS_URI "/thing/apps"
#define ML_URI "/thing/ml"

// Main loop
void MLDaemon::run() {
  // Initialize MessageRouter and Channels
  this->mMessageRouter = new MessageRouter();
  this->mDbusChannel = new DbusChannel(this->mMessageRouter);
  this->mLocalChannel = new LocalChannel(this->mMessageRouter, false);

  // Run DbusChannel and add it to MessageRouter's routing table
  this->mDbusChannel->run();
  this->mMessageRouter->addRoutingEntry(APPS_URI, this->mDbusChannel);

  // LocalChannel: run on main thread
  // Main loop starts to run in LocalChannel::run()
  this->mLocalChannel->setListener(this);
  this->mMessageRouter->addRoutingEntry(ML_URI, this->mLocalChannel);
  this->mLocalChannel->run();
}

// LocalChannelListener
void MLDaemon::onReceivedMessage(BaseMessage* message) {
  // TODO: implement MLMessage in Message FW
  if(message == NULL) {
    ANT_DBG_ERR("Invalid ML Message");
    return;
  }
  if(message->getType() != BaseMessageType::ML) {
    ANT_DBG_ERR("Not ML Message");
    return;
  }
  MLMessage* payload = (MLMessage*)message->getPayload();
  if(payload == NULL) {
    ANT_DBG_ERR("MLMessage payload does not exist");
    return;
  }

  int commandType = payload->getCommandType();
  switch(commandType) {
    default:
      // Do not handle it
      break;
  }
}

// TODO: MLFW Commands
