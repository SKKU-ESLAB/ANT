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
#include "ModelPackageLoader.h"

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
  MLMessage* payload = (MLMessage*)message->getPayload();
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
  originalPayload->getParamsLoadIU(modelPackagePath, params);

  // Model Package Loader: Load a model package and make inference unit
  InferenceUnit* iu = ModelPackageLoader::load(modelPackagePath, params);

  // Insert the Infernce Unit to Inference Unit Directory
  int iuid = this->mInferenceUnitDirectory.insert(iu);

  // Make ACK message
  // TODO: remove setting target URI of AckMessage
  BaseMessage* ackMessage
    = MessageFactory::makeMLAckMessage(APPS_URI, message);
  MLAckMessage* ackPayload = (MLAckMessage*)ackMessage->getPayload();
  ackPayload->setParamsLoadIU(iuid);

  // Send ACK message
  this->mLocalChannel->sendMessage(ackMessage);
}

void MLDaemon::unloadIU(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  int iuid;
  originalPayload->getParamsUnloadIU(iuid);

  // Find inference unit
  InferenceUnit* iu = this->mInferenceUnitDirectory.find(iuid);
  if(iu == NULL) {
    ANT_DBG_ERR("Cannot find inference unit: %d", iuid);
    return;
  }

  // Unload inference unit
  bool unloadRes = iu->unload();
  if(!unloadRes) {
    ANT_DBG_ERR("Cannot unload inference unit: %d", iuid);
    return;
  }

  // Remove inference unit from Inference Unit Directory
  bool removeRes = this->mInferenceUnitDirectory.remove(iuid);
  if(!removeRes) {
    ANT_DBG_ERR("Cannot remove inference unit from directory: %d", iuid);
    return;
  }

  // No ACK message
}

void MLDaemon::getIUs(BaseMessage* message) {
  // No arguments
  
  // Get infernece unit map
  std::map<int, InferenceUnit*>& iuMap = this->mInferenceUnitDirectory.getMap();

  // Make parameters
  ParamIUList* paramIUList = ParamIUList::make();
  std::map<int, InferenceUnit*>::iterator iter;
  for(iter = iuMap.begin();
      iter != iuMap.end();
      iter++) {
    InferenceUnit* iu = iter->second;
    paramIUList->addEntry(iu->getIuid(), iu->getName(), iu->getType());
  }

  // Make ACK message
  BaseMessage* ackMessage
    = MessageFactory::makeMLAckMessage(APPS_URI, message);
  MLAckMessage* ackPayload = (MLAckMessage*)ackMessage->getPayload();
  ackPayload->setParamsGetIUs(paramIUList);

  // Send ACK message
  this->mLocalChannel->sendMessage(ackMessage);
  delete paramIUList;
}

void MLDaemon::setIUInput(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  int iuid;
  std::string inputName;
  std::string sourceUri;
  originalPayload->getParamsSetIUInput(iuid, inputName, sourceUri);

  // Find inference unit
  InferenceUnit* iu = this->mInferenceUnitDirectory.find(iuid);
  if(iu == NULL) {
    ANT_DBG_ERR("Cannot find inference unit: %d", iuid);
    return;
  }

  // Set inference unit input
  bool setInputRes = iu->setInput(inputName, sourceUri);
  if(!setInputRes) {
    ANT_DBG_ERR("Cannot set input of inference unit: %d", iuid);
    return;
  }

  // No ACK message
}

void MLDaemon::startListeningIUOutput(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  int iuid;
  std::string listenerUri;
  originalPayload->getParamsStartListeningIUOutput(iuid, listenerUri);

  // Find inference unit
  InferenceUnit* iu = this->mInferenceUnitDirectory.find(iuid);
  if(iu == NULL) {
    ANT_DBG_ERR("Cannot find inference unit: %d", iuid);
    return;
  }

  // Start listening inference unit output
  bool startListeningOutputRes = iu->startListeningOutput(listenerUri);
  if(!startListeningOutputRes) {
    ANT_DBG_ERR("Cannot start listening output of inference unit: %d", iuid);
    return;
  }

  // ACK message will be sent later
}

void MLDaemon::stopListeningIUOutput(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  int iuid;
  std::string listenerUri;
  originalPayload->getParamsStopListeningIUOutput(iuid, listenerUri);

  // Find inference unit
  InferenceUnit* iu = this->mInferenceUnitDirectory.find(iuid);
  if(iu == NULL) {
    ANT_DBG_ERR("Cannot find inference unit: %d", iuid);
    return;
  }

  // Stop listening inference unit output
  bool stopListeningOutputRes = iu->stopListeningOutput(listenerUri);
  if(!stopListeningOutputRes) {
    ANT_DBG_ERR("Cannot stop listening output of inference unit: %d", iuid);
    return;
  }

  // No ACK message
}

void MLDaemon::startIU(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  int iuid;
  originalPayload->getParamsStartIU(iuid);

  // Find inference unit
  InferenceUnit* iu = this->mInferenceUnitDirectory.find(iuid);
  if(iu == NULL) {
    ANT_DBG_ERR("Cannot find inference unit: %d", iuid);
    return;
  }

  // Start inference unit
  bool startRes = iu->start();
  if(!startRes) {
    ANT_DBG_ERR("Cannot start inference unit: %d", iuid);
    return;
  }

  // No ACK message
}

void MLDaemon::stopIU(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  int iuid;
  originalPayload->getParamsStopIU(iuid);

  // Find inference unit
  InferenceUnit* iu = this->mInferenceUnitDirectory.find(iuid);
  if(iu == NULL) {
    ANT_DBG_ERR("Cannot find inference unit: %d", iuid);
    return;
  }

  // Stop inference unit
  bool stopRes = iu->stop();
  if(!stopRes) {
    ANT_DBG_ERR("Cannot stop inference unit: %d", iuid);
    return;
  }

  // No ACK message
}

void MLDaemon::getIUResourceUsage(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  int iuid;
  originalPayload->getParamsGetIUResourceUsage(iuid);

  // Find inference unit
  InferenceUnit* iu = this->mInferenceUnitDirectory.find(iuid);
  if(iu == NULL) {
    ANT_DBG_ERR("Cannot find inference unit: %d", iuid);
    return;
  }

  // Get resource usage of inference unit
  std::string data = iu->getResourceUsage();
  if(data.empty()) {
    ANT_DBG_ERR("Cannot get resource usage of inference unit: %d", iuid);
    return;
  }

  // Make ACK message
  BaseMessage* ackMessage
    = MessageFactory::makeMLAckMessage(APPS_URI, message);
  MLAckMessage* ackPayload = (MLAckMessage*)ackMessage->getPayload();
  ackPayload->setParamsGetIUResourceUsage(data);

  // Send ACK message
  this->mLocalChannel->sendMessage(ackMessage);
}
