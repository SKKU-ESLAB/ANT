/* Copyright (c) 2017-2018 SKKU ESLAB, and contributors. All rights reserved.
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

#ifndef __COMPANION_MESSAGE_H__
#define __COMPANION_MESSAGE_H__

#include <iostream>
#include <string>
#include <vector>

#include "cJSON.h"

#include "BaseMessage.h"

namespace CompanionMessageCommandType {
  enum Value {
    NotDetermined = 0,
    SendToCompanion = 3 // params: string listenerName, string data
  };
}

#define COMPANION_MESSAGE_KEY_COMMAND_TYPE "commandType"
#define COMPANION_MESSAGE_KEY_PAYLOAD "payload"

// CompanionMessage: message sent to companion device
// - Decoding(makeFromJSON): Java
// - Encoding(make, toJSON): JavaScript
class CompanionMessage: public BaseMessagePayload {
  public:
    friend class MessageFactory;

    ~CompanionMessage() {
      if(this->mCompanionPayloadObj != NULL) {
        cJSON_Delete(this->mCompanionPayloadObj);
      }
    }

    // encoding to JSON
    virtual cJSON* toJSON();

    // Get parameters
    CompanionMessageCommandType::Value getCommandType() {
      return this->mCommandType;
    }
    cJSON* getCompanionPayloadObj() { return this->mCompanionPayloadObj; }

    // Set command-specific parameters
    void setParamsSendToCompanion(std::string listenerName, std::string data);

    void setCompanionPayloadObj(cJSON* companionPayloadObj) {
      this->mCompanionPayloadObj = companionPayloadObj;
    }

  protected:
    // Initializer
    CompanionMessage(CompanionMessageCommandType::Value commandType)
      : mCommandType(commandType), mCompanionPayloadObj(NULL) {
    }

    // JSON-exported values
    CompanionMessageCommandType::Value mCommandType;
    cJSON* mCompanionPayloadObj;
};

#endif // !defined(__COMPANION_MESSAGE_H__)