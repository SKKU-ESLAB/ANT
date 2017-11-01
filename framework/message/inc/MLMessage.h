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

#ifndef __ML_MESSAGE_H__
#define __ML_MESSAGE_H__

#include <iostream>
#include <string>
#include <vector>

#include "BaseMessage.h"
#include "MLDataUnit.h"

class MessageFactory;

// Use namespace + enum for readability
namespace MLMessageCommandType {
  enum Value {
    NotDetermined = 0,
    LoadIU = 1, // params: string modelPackagePath, MLDataUnit parameters
                //         (ACK params: int iuid)
    UnloadIU = 2, // params: int iuid
    GetIUs = 3, // params: void (ACK params: list<IUListEntry> iuList)
    SetIUInput = 4, // params: int iuid, string inputName, string sourceUri
    StartListeningIUOutput = 5, // params: int iuid, string listenerUri
                                //         (ACK params: MLDataUnit outputData)
    StopListeningIUOutput = 6, // params: int iuid, string listenerUri
    StartIU = 7, // params: int iuid
    StopIU = 8, // params: int iuid
    GetIUResourceUsage = 9, // params: string data
    RunModel = 10 // params: string modelName (ACK params: String outputData)
  };
}

#define ML_MESSAGE_KEY_COMMAND_TYPE "commandType"
#define ML_MESSAGE_KEY_PAYLOAD "payload"

// MLMessage: message sent to machine learning daemon
// - Decoding(makeFromJSON): C++
// - Encoding(make, toJSON): C++
class MLMessage: public BaseMessagePayload {
  public:
    friend class MessageFactory;

    ~MLMessage() {
      if(this->mMLPayloadObj != NULL) {
        cJSON_Delete(this->mMLPayloadObj);
      }
    }

    // encoding to JSON
    virtual cJSON* toJSON();

    // Get parameters
    MLMessageCommandType::Value getCommandType() { return this->mCommandType; }
    cJSON* getMLPayloadObj() { return this->mMLPayloadObj; }

    // Get command-specific parameters
    bool getParamsLoadIU(std::string& modelPackagePath, MLDataUnit& parameters);
    bool getParamsUnloadIU(int& iuid);
    // No parameter for GetIUs
    bool getParamsSetIUInput(int& iuid,
        std::string& inputName, std::string& sourceUri);
    bool getParamsStartListeningIUOutput(int& iuid, std::string& listenerUri);
    bool getParamsStopListeningIUOutput(int& iuid, std::string& listenerUri);
    bool getParamsStartIU(int& iuid);
    bool getParamsStopIU(int& iuid);
    bool getParamsGetIUResourceUsage(int& iuid);
    bool getParamsRunModel(std::string& modelName);

    // Set command-specific parameters
    void setMLPayloadObj(cJSON* mlPayloadObj) {
      this->mMLPayloadObj = mlPayloadObj;
    }
    void setParamsLoadIU(std::string modelPackagePath, MLDataUnit* parameters);
    void setParamsUnloadIU(int iuid);
    // No parameter for GetIUs
    void setParamsSetIUInput(int iuid,
        std::string inputName, std::string sourceUri);
    void setParamsStartListeningIUOutput(int iuid, std::string listenerUri);
    void setParamsStopListeningIUOutput(int iuid, std::string listenerUri);
    void setParamsStartIU(int iuid);
    void setParamsStopIU(int iuid);
    void setParamsGetIUResourceUsage(int iuid);
    void setParamsRunModel(std::string& modelName);

  protected:
    // Initializer
    MLMessage(MLMessageCommandType::Value commandType)
      : mCommandType(commandType), mMLPayloadObj(NULL) {
    }

    // JSON-exported values
    MLMessageCommandType::Value mCommandType;
    cJSON* mMLPayloadObj;
};

class ParamIUListEntry {
  public:
    ParamIUListEntry(int iuid, std::string iuName, int iuType)
      : mIuid(iuid), mIuName(iuName), mIuType(iuType) { }

    int getIuid() { return this->mIuid; }
    std::string getIuName() { return this->mIuName; }
    int getIuType() { return this->mIuType; }

  private:
    int mIuid;
    std::string mIuName;
    int mIuType;
};

class ParamIUList {
  public:
    // Make
    static ParamIUList* make() {
      return new ParamIUList();
    }

    bool setFromJSON(cJSON* iuListObj);

    void addEntry(int iuid, std::string iuName, int iuType) {
      ParamIUListEntry newEntry(iuid, iuName, iuType);
      this->mIUList.push_back(newEntry);
    }

    // encoding to JSON
    cJSON* toJSON();

    std::vector<ParamIUListEntry>& getList() { return this->mIUList; }

  private:
    ParamIUList() { }
    std::vector<ParamIUListEntry> mIUList;
};

#define ML_ACK_MESSAGE_KEY_COMMAND_MESSAGE_NUM "commandMessageId"
#define ML_ACK_MESSAGE_KEY_COMMAND_TYPE "commandType"
#define ML_ACK_MESSAGE_KEY_PAYLOAD "payload"

// MLAckMessage: ack message sent from machine learning daemon
// - Decoding(makeFromJSON): C++
// - Encoding(make, toJSON): C++
class MLAckMessage : public BaseMessagePayload {
  public:
    friend class MessageFactory;

    ~MLAckMessage() {
      if(this->mMLAckPayloadObj != NULL) {
        cJSON_Delete(this->mMLAckPayloadObj);
      }
    }

    // encoding to JSON
    virtual cJSON* toJSON();

    // Get parameters
    MLMessageCommandType::Value getCommandType() {
      return this->mCommandType;
    }

    // Get command-specific parameters
    bool getParamsLoadIU(int& iuid);
    // No ack message for UnloadIU
    bool getParamsGetIUs(ParamIUList& iuList);
    // No ack message for SetIUInput 
    bool getParamsStartListeningIUOutput(MLDataUnit& outputData);
    // No ack message for StopListeningIUOutput
    // No ack message for StartIU
    // No ack message for StopIU
    bool getParamsGetIUResourceUsage(std::string& data);
    bool getParamsRunModel(std::string outputData);

    // Set command-specific parameters
    void setMLAckPayloadObj(cJSON* mlAckPayloadObj) {
      this->mMLAckPayloadObj = mlAckPayloadObj;
    }
    void setParamsLoadIU(int iuid);
    // No ack message for UnloadIU
    void setParamsGetIUs(ParamIUList* iuList);
    // No ack message for SetIUInput 
    void setParamsStartListeningIUOutput(MLDataUnit* outputData);
    // No ack message for StopListeningIUOutput
    // No ack message for StartIU
    // No ack message for StopIU
    void setParamsGetIUResourceUsage(std::string data);
    void setParamsRunModel(std::string outputData);

  protected:
    MLAckMessage(int commandMessageId,
        MLMessageCommandType::Value commandType)
      : mCommandMessageId(commandMessageId),
      mCommandType(commandType), mMLAckPayloadObj(NULL) { }

    // JSON-exported values
    int mCommandMessageId;
    MLMessageCommandType::Value mCommandType;
    cJSON* mMLAckPayloadObj;
};

#endif // !definedd(__ML_MESSAGE_H__)
