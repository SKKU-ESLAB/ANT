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
  this->mLocalChannel = new LocalChannel(this->mMessageRouter, ML_URI, false);

  // Run DbusChannel and add it to MessageRouter's routing table
  this->mDbusChannel->run();
  this->mMessageRouter->addRoutingEntry(APPS_URI, this->mDbusChannel);

  // LocalChannel: run on main thread
  // Main loop starts to run in LocalChannel::run()
  this->mLocalChannel->setListener(this);
  this->mMessageRouter->addRoutingEntry(ML_URI, this->mLocalChannel);
  this->mLocalChannel->run();
}

// Testing mode
void MLDaemon::runTest(std::string modelName) {
  this->mIsTestMode = true;

  // Run model
  if(modelName.compare("motionclassifier") == 0) {
    // Run motion classifier model
    ANT_DBG_VERB("Test motion classifier loading: BEGIN");
    bool res = this->runModel("motionclassifier");
    ANT_DBG_VERB("Test motion classifier loading: END (%s)",
        (res) ? "Success" : "Fail");
  } else if(modelName.compare("imageclassifier") == 0) {
    // Run image classifier model
    ANT_DBG_VERB("Test image classifier loading: BEGIN");
    bool res = this->runModel("imageclassifier");
    ANT_DBG_VERB("Test image classifier loading: END (%s)",
        (res) ? "Success" : "Fail");
  } else {
    ANT_DBG_ERR("Invalid model name! (%s)", modelName.c_str());
  }

  // Main thread waits until ctrl+c is pressed
  ANT_DBG_VERB("Inference unit thread is running... Press ctrl+c to stop test.");
  do { } while(true);
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
    case MLMessageCommandType::RunModel:
      this->runModel(message);
      break;
    default:
      // Do not handle it
      break;
  }
}

// InferenceUnitOutputListener
void MLDaemon::onInferenceUnitOutput(int iuid,
    std::string listenerUri, MLDataUnit* outputData) {
  if(this->mIsTestMode) {
    // Test mode: print on stdout
    MLTensor* outputTensor = outputData->findTensor("output");
    if(outputTensor == NULL) {
      ANT_DBG_ERR("Cannot find output tensor in output data unit!");
      return;
    }
    std::string outputDataStr(outputTensor->stringValue());
    ANT_DBG_VERB("Output: %s", outputDataStr.c_str());
  } else {
    // Normal mode: notify to app

    // Find from setIUOutput listner list
    std::vector<BaseMessage*>::iterator iter;
    BaseMessage* originalMessage = NULL;
    MLMessage* originalPayload = NULL;
    for(iter = this->mListenIUOutputList.begin();
        iter != this->mListenIUOutputList.end();
        iter++) {
      originalMessage = (*iter);
      originalPayload = (MLMessage*)originalMessage->getPayload();

      MLMessageCommandType::Value commandType
        = originalPayload->getCommandType();
      if(commandType == MLMessageCommandType::StartListeningIUOutput) { 
        // StartListeningIUOutput ACK
        int thisIuid;
        std::string thisListenerUri;
        originalPayload->getParamsStartListeningIUOutput(thisIuid,
            thisListenerUri);

        if((iuid == thisIuid) && (listenerUri == thisListenerUri)) {
          // Make ACK message
          BaseMessage* ackMessage
            = MessageFactory::makeMLAckMessage(this->mLocalChannel->getUri(),
                originalMessage); 
          MLAckMessage* ackPayload = (MLAckMessage*)ackMessage->getPayload();
          ackPayload->setParamsStartListeningIUOutput(outputData);

          // Send ACK message
          this->mLocalChannel->sendMessage(ackMessage);
          return;
        }
      } else if(commandType == MLMessageCommandType::RunModel) { 
        // RunModel ACK
        
        // TODO: modelName comparison
        
        // Get outputData string
        MLTensor* outputTensor = outputData->findTensor("output");
        if(outputTensor == NULL) {
          ANT_DBG_ERR("Cannot find output tensor in output data unit!");
          return;
        }
        std::string outputDataStr(outputTensor->stringValue());
        
        // Make ACK message
        BaseMessage* ackMessage
          = MessageFactory::makeMLAckMessage(this->mLocalChannel->getUri(),
              originalMessage); 
        MLAckMessage* ackPayload = (MLAckMessage*)ackMessage->getPayload();
        ackPayload->setParamsRunModel(outputDataStr);

        // Send ACK message
        this->mLocalChannel->sendMessage(ackMessage);
        return;
      }
    }

    ANT_DBG_WARN("Cannot find listening request! : %d -> %s",
        iuid, listenerUri.c_str());
  }
}

// MLFW Commands
void MLDaemon::loadIU(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  std::string modelPackagePath;
  MLDataUnit params;
  originalPayload->getParamsLoadIU(modelPackagePath, params);

  // Internal
  int iuid = this->loadIU(modelPackagePath, params);

  // Make ACK message
  BaseMessage* ackMessage
    = MessageFactory::makeMLAckMessage(this->mLocalChannel->getUri(), message);
  MLAckMessage* ackPayload = (MLAckMessage*)ackMessage->getPayload();
  ackPayload->setParamsLoadIU(iuid);

  // Send ACK message
  this->mLocalChannel->sendMessage(ackMessage);
}

int MLDaemon::loadIU(std::string modelPackagePath, MLDataUnit params) {
  // Model Package Loader: Load a model package and make inference unit
  InferenceUnit* iu = ModelPackageLoader::load(modelPackagePath, params);
  iu->setOutputListener(this);

  // Insert the Infernce Unit to Inference Unit Directory
  int iuid = this->mInferenceUnitDirectory.insert(iu);
  return iuid;
}

void MLDaemon::unloadIU(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  int iuid;
  originalPayload->getParamsUnloadIU(iuid);

  // Internal
  bool res = this->unloadIU(iuid);

  // No ACK message
}

bool MLDaemon::unloadIU(int iuid) {
  // Find inference unit
  InferenceUnit* iu = this->mInferenceUnitDirectory.find(iuid);
  if(iu == NULL) {
    ANT_DBG_ERR("Cannot find inference unit: %d", iuid);
    return false;
  }

  // Unload inference unit
  bool unloadRes = iu->unload();
  if(!unloadRes) {
    ANT_DBG_ERR("Cannot unload inference unit: %d", iuid);
    return false;
  }

  // Remove inference unit from Inference Unit Directory
  bool removeRes = this->mInferenceUnitDirectory.remove(iuid);
  if(!removeRes) {
    ANT_DBG_ERR("Cannot remove inference unit from directory: %d", iuid);
    return false;
  }
  return true;
}

void MLDaemon::getIUs(BaseMessage* message) {
  // No arguments
  
  // Internal
  std::map<int, InferenceUnit*>& iuMap = this->getIUs();

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
    = MessageFactory::makeMLAckMessage(this->mLocalChannel->getUri(), message);
  MLAckMessage* ackPayload = (MLAckMessage*)ackMessage->getPayload();
  ackPayload->setParamsGetIUs(paramIUList);

  // Send ACK message
  this->mLocalChannel->sendMessage(ackMessage);
  delete paramIUList;
}

std::map<int, InferenceUnit*>& MLDaemon::getIUs() {
  // Get infernece unit map
  std::map<int, InferenceUnit*>& iuMap = this->mInferenceUnitDirectory.getMap();
}

void MLDaemon::setIUInput(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  int iuid;
  std::string inputName;
  std::string sourceUri;
  originalPayload->getParamsSetIUInput(iuid, inputName, sourceUri);

  // Internal
  bool res = this->setIUInput(iuid, inputName, sourceUri);

  // No ACK message
}

bool MLDaemon::setIUInput(int iuid, std::string inputName,
    std::string sourceUri) {
  // Find inference unit
  InferenceUnit* iu = this->mInferenceUnitDirectory.find(iuid);
  if(iu == NULL) {
    ANT_DBG_ERR("Cannot find inference unit: %d", iuid);
    return false;
  }

  // Set inference unit input
  bool setInputRes = iu->setInput(inputName, sourceUri);
  if(!setInputRes) {
    ANT_DBG_ERR("Cannot set input of inference unit: %d", iuid);
    return false;
  }
  return true;
}

// Register Original BaseMessage
void MLDaemon::startListeningIUOutput(BaseMessage* message) {
  // Get arguments
  BaseMessage* originalMessage = message;
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  int iuid;
  std::string listenerUri;
  originalPayload->getParamsStartListeningIUOutput(iuid, listenerUri);

  // Internal
  bool res = this->startListeningIUOutput(iuid, listenerUri);
  if(!res) {
    return;
  }

  // Check if there has already been duplicated listening request
  std::vector<BaseMessage*>::iterator iter;
  for(iter = this->mListenIUOutputList.begin();
      iter != this->mListenIUOutputList.end();
      iter++) {
    BaseMessage* thisMessage = (*iter);
    MLMessage* thisPayload = (MLMessage*)thisMessage->getPayload();
    MLMessageCommandType::Value commandType = thisPayload->getCommandType();
    if(commandType != MLMessageCommandType::RunModel) {
      continue;
    }
    int thisIuid;
    std::string thisListenerUri;
    thisPayload->getParamsStartListeningIUOutput(thisIuid, thisListenerUri);
    if((iuid == thisIuid) && (listenerUri.compare(thisListenerUri) == 0)) {
      ANT_DBG_ERR("Duplicated listening request is ignored: %d -> %s",
          iuid, listenerUri.c_str());
      return;
    }
  }

  // ACK message will be sent later
  this->mListenIUOutputList.push_back(originalMessage);
}

// Register only ListenerURI
bool MLDaemon::startListeningIUOutput(int iuid, std::string listenerUri) {
  // Find inference unit
  InferenceUnit* iu = this->mInferenceUnitDirectory.find(iuid);
  if(iu == NULL) {
    ANT_DBG_ERR("Cannot find inference unit: %d", iuid);
    return false;
  }

  // Start listening inference unit output
  bool startListeningOutputRes = iu->startListeningOutput(listenerUri);
  if(!startListeningOutputRes) {
    ANT_DBG_ERR("Cannot start listening output of inference unit: %d", iuid);
    return false;
  }
  return true;
}

void MLDaemon::stopListeningIUOutput(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  int iuid;
  std::string listenerUri;
  originalPayload->getParamsStopListeningIUOutput(iuid, listenerUri);

  this->stopListeningIUOutput(iuid, listenerUri);

  // No ACK message
}

bool MLDaemon::stopListeningIUOutput(int iuid, std::string listenerUri) {
  // Find inference unit
  InferenceUnit* iu = this->mInferenceUnitDirectory.find(iuid);
  if(iu == NULL) {
    ANT_DBG_ERR("Cannot find inference unit: %d", iuid);
    return false;
  }

  // Stop listening inference unit output
  bool stopListeningOutputRes = iu->stopListeningOutput(listenerUri);
  if(!stopListeningOutputRes) {
    ANT_DBG_ERR("Cannot stop listening output of inference unit: %d", iuid);
    return false;
  }

  return true;
}

void MLDaemon::startIU(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  int iuid;
  originalPayload->getParamsStartIU(iuid);

  bool res = this->startIU(iuid);

  // No ACK message
}

bool MLDaemon::startIU(int iuid) {
  // Find inference unit
  InferenceUnit* iu = this->mInferenceUnitDirectory.find(iuid);
  if(iu == NULL) {
    ANT_DBG_ERR("Cannot find inference unit: %d", iuid);
    return false;
  }

  // Start inference unit
  bool startRes = iu->start();
  if(!startRes) {
    ANT_DBG_ERR("Cannot start inference unit: %d", iuid);
    return false;
  }

  return true;
}

void MLDaemon::stopIU(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  int iuid;
  originalPayload->getParamsStopIU(iuid);

  bool res = this->stopIU(iuid);

  // No ACK message
}

bool MLDaemon::stopIU(int iuid) {
  // Find inference unit
  InferenceUnit* iu = this->mInferenceUnitDirectory.find(iuid);
  if(iu == NULL) {
    ANT_DBG_ERR("Cannot find inference unit: %d", iuid);
    return false;
  }

  // Stop inference unit
  bool stopRes = iu->stop();
  if(!stopRes) {
    ANT_DBG_ERR("Cannot stop inference unit: %d", iuid);
    return false;
  }

  return true;
}

void MLDaemon::getIUResourceUsage(BaseMessage* message) {
  // Get arguments
  MLMessage* originalPayload = (MLMessage*)message->getPayload();
  int iuid;
  originalPayload->getParamsGetIUResourceUsage(iuid);

  // Internal
  std::string data(this->getIUResourceUsage(iuid));

  // Make ACK message
  BaseMessage* ackMessage
    = MessageFactory::makeMLAckMessage(this->mLocalChannel->getUri(), message);
  MLAckMessage* ackPayload = (MLAckMessage*)ackMessage->getPayload();
  ackPayload->setParamsGetIUResourceUsage(data);

  // Send ACK message
  this->mLocalChannel->sendMessage(ackMessage);
}

std::string MLDaemon::getIUResourceUsage(int iuid) {
  std::string data("");
  // Find inference unit
  InferenceUnit* iu = this->mInferenceUnitDirectory.find(iuid);
  if(iu == NULL) {
    ANT_DBG_ERR("Cannot find inference unit: %d", iuid);
    return data;
  }

  // Get resource usage of inference unit
  data.assign(iu->getResourceUsage());
  if(data.empty()) {
    ANT_DBG_ERR("Cannot get resource usage of inference unit: %d", iuid);
    return data;
  }
  return data;
}

void MLDaemon::runModel(BaseMessage* message) {
  // Get arguments
  BaseMessage* originalMessage = message;
  MLMessage* originalPayload = (MLMessage*)originalMessage->getPayload();
  std::string modelName;
  originalPayload->getParamsRunModel(modelName);

  // Internal
  bool res = this->runModel(modelName);
  if(!res) {
    ANT_DBG_ERR("Cannot find model %s!", modelName.c_str());
  }

  // Check if there has already been duplicated listening request
  std::vector<BaseMessage*>::iterator iter;
  for(iter = this->mListenIUOutputList.begin();
      iter != this->mListenIUOutputList.end();
      iter++) {
    BaseMessage* thisMessage = (*iter);
    MLMessage* thisPayload = (MLMessage*)thisMessage->getPayload();
    MLMessageCommandType::Value commandType = thisPayload->getCommandType();
    if(commandType != MLMessageCommandType::RunModel) {
      continue;
    }
    std::string thisModelName;
    thisPayload->getParamsRunModel(thisModelName);
    if(modelName.compare(thisModelName) == 0) {
      ANT_DBG_ERR("Duplicated listening request is ignored: %s",
          modelName.c_str());
      return;
    }
  }
}

#define PATH_BUFFER_SIZE 1024
bool MLDaemon::runModel(std::string modelName) {
  // Load a built-in model and create a new inference unit
  int iuid = this->loadBuiltin(modelName);
  
  // Set input & output connection
  if(modelName.compare("motionclassifier") == 0) {
    // Set input: accelerometer
    this->setIUInput(iuid, "input", "/thing/sensor"); 

    // Set dummy output: no listener app process
    this->startListeningIUOutput(iuid, "/null");
  } else if(modelName.compare("imageclassifier") == 0) {
    // Set input: camera
    this->setIUInput(iuid, "input", "/thing/camera"); 

    // Set dummy output: no listener app process
    this->startListeningIUOutput(iuid, "/null");
  }

  // Start the inference unit
  this->startIU(iuid);
}

int MLDaemon::loadBuiltin(std::string modelName) {
  // Model Package Loader: Load a built-in model and make inference unit
  InferenceUnit* iu = ModelPackageLoader::loadBuiltin(modelName);
  iu->setOutputListener(this);

  // Insert the Infernce Unit to Inference Unit Directory
  int iuid = this->mInferenceUnitDirectory.insert(iu);
  return iuid;
}
