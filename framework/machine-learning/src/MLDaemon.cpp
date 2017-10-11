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
  if(message == NULL) {
    ANT_DBG_ERR("Invalid ML Message");
    return;
  }
  if(message->getType() != BaseMessageType::ML) {
    ANT_DBG_ERR("Not ML Message");
    return;
  }
  BaseMessage* payload = (MLMessage*)message->getPayload();
  if(payload == NULL) {
    ANT_DBG_ERR("MLMessage payload does not exist");
    return;
  }

  int commandType = payload->getCommandType();
  switch(commandType) {
    case MLMessageCommandType::LoadIU:
      this->loadIU(message);
      break;
    case MLMessageCommandType::UnloadIU:
      this->unloadIU(message);
      break;
    case MLMessageCommandType::GetIUs:
      this->getIUs(message);
      break;
    case MLMessageCommandType::SetIUInput:
      this->setIUInput(message);
      break;
    case MLMessageCommandType::StartListeningIUOutput:
      this->startListeningIUOutput(message);
      break;
    case MLMessageCommandType::StopListeningIUOutput:
      this->stopListeningIUOutput(message);
      break;
    case MLMessageCommandType::StartIU:
      this->startIU(message);
      break;
    case MLMessageCommandType::StopIU:
      this->stopIU(message);
      break;
    case MLMessageCommandType::GetIUResourceUsage:
      this->getIUResourceUsage(message);
      break;
    default:
      // Do not handle it
      break;
  }
}

// MLFW Commands
void MLDaemon::loadIU(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  std::string modelPackagePath;
  MLDataUnit params;
  originalPayload->getParamsLoadIU(modelPacakgePath, params);

  // TODO: implement it
}

void MLDaemon::unloadIU(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  int iuid;
  originalPayload->getParamsUnloadIU(iuid);

  // TODO: implement it
}

void MLDaemon::getIUs(BaseMessage* message) {
  // No arguments

  // TODO: implement it
}

void MLDaemon::setIUInput(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  int iuid;
  std::string inputName;
  std::string sourceUri;
  originalPayload->getParamsSetIUInput(iuid, inputName, sourceUri);

  // TODO: implement it
}

void MLDaemon::startListeningIUOutput(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  int iuid;
  std::string listenerUri;
  originalPayload->getParamsStartListeningIUOutput(iuid, listenerUri);

  // TODO: implement it
}

void MLDaemon::stopListeningIUOutput(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  int iuid;
  std::string listenerUri;
  originalPayload->getParamsStopListeningIUOutput(iuid, listenerUri);

  // TODO: implement it
}

void MLDaemon::startIU(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  int iuid;
  originalPayload->getParamsStartIU(iuid);

  // TODO: implement it
}

void MLDaemon::stopIU(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  int iuid;
  originalPayload->getParamsStopIU(iuid);

  // TODO: implement it
}

void MLDaemon::getIUResourceUsage(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  std::string data;
  originalPayload->getParamsGetIUResourceUsage(data);

  // TODO: implement it
}
