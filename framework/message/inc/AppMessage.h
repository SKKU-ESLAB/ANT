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

#ifndef __APP_MESSAGE_H__
#define __APP_MESSAGE_H__

#include <iostream>
#include <string>
#include <vector>

#include "cJSON.h"

#include "BaseMessage.h"

// Use namespace + enum for readability
namespace AppMessageCommandType {
  enum Value {
    NotDetermined = 0,
    Terminate = 1, // params: void
    UpdateAppConfig = 2, // params: string legacyData
                         // (ACK params: bool isSucceed)
  };
}

#define APP_MESSAGE_KEY_COMMAND_TYPE "commandType"
#define APP_MESSAGE_KEY_PAYLOAD "payload"

// AppMessage: message sent to App
// - Decoding(makeFromJSON): C++
// - Encoding(make, toJSON): C++, Java
class AppMessage: public BaseMessagePayload {
  public:
    friend class MessageFactory;

    ~AppMessage() {
      if(this->mAppPayloadObj != NULL) {
        cJSON_Delete(this->mAppPayloadObj);
      }
    }

    // encoding to JSON
    virtual cJSON* toJSON();

    // Get parameters
    AppMessageCommandType::Value getCommandType() { return this->mCommandType; }
    cJSON* getAppPayloadObj() { return this->mAppPayloadObj; }

    // Get command-specific parameters
    bool getParamsUpdateAppConfig(std::string& legacyData);

    // Set command-specific parameters
    void setAppPayloadObj(cJSON* appPayloadObj) {
      this->mAppPayloadObj = appPayloadObj;
    }

  protected:
    // Initializer
    AppMessage(AppMessageCommandType::Value commandType)
      : mCommandType(commandType), mAppPayloadObj(NULL) {
    }

    // JSON-exported values
    AppMessageCommandType::Value mCommandType;
    cJSON* mAppPayloadObj;
};

#define APP_ACK_MESSAGE_KEY_COMMAND_MESSAGE_NUM "commandMessageId"
#define APP_ACK_MESSAGE_KEY_COMMAND_TYPE "commandType"
#define APP_ACK_MESSAGE_KEY_PAYLOAD "payload"

// AppAckMessage: ack message sent from App
// - Decoding(makeFromJSON): Java
// - Encoding(make, toJSON): C++
class AppAckMessage : public BaseMessagePayload {
  public:
    friend class MessageFactory;

    ~AppAckMessage() {
      if(this->mAppAckPayloadObj != NULL) {
        cJSON_Delete(this->mAppAckPayloadObj);
      }
    }

    // encoding to JSON
    virtual cJSON* toJSON();

    // Get parameters
    AppMessageCommandType::Value getCommandType() {
      return this->mCommandType;
    }

    // Set command-specific parameters
    void setParamsUpdateAppConfig(bool isSucceed);

    void setAppAckPayloadObj(cJSON* appAckPayloadObj) {
      this->mAppAckPayloadObj = appAckPayloadObj;
    }

  protected:
    AppAckMessage(int commandMessageId,
        AppMessageCommandType::Value commandType)
      : mCommandMessageId(commandMessageId),
      mCommandType(commandType), mAppAckPayloadObj(NULL) { }

    // JSON-exported values
    int mCommandMessageId;
    AppMessageCommandType::Value mCommandType;
    cJSON* mAppAckPayloadObj;
};

#endif // !defined(__APP_MESSAGE_H__)