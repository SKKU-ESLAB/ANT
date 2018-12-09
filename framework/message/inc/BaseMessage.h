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

#ifndef __BASE_MESSAGE_H__
#define __BASE_MESSAGE_H__

#include <iostream>
#include <string>
#include <vector>

#include "cJSON.h"

// {
//   // BaseMessage
//   payload: {
//     // BaseMessagePayload (AppCore, AppCoreAck, App, AppAck, Companion, etc.)
//     params: {
//       // **Params
//     }
//   }
// }

class MessageFactory;

class BaseMessagePayload {
  public:
    // encoding to JSON
    virtual cJSON* toJSON() = 0;
};

// Use namespace + enum for readability
namespace BaseMessageType {
  enum Value {
    NotDetermined = 0,
    AppCore = 10,
    AppCoreAck = 11,
    App = 20,
    AppAck = 21,
    Companion = 30,
    ML = 40,
    MLAck = 41
  };
}

#define BASE_MESSAGE_KEY_MESSAGE_NUM "messageId"
#define BASE_MESSAGE_KEY_SENDER_URI "senderUri"
#define BASE_MESSAGE_KEY_URI "uri"
#define BASE_MESSAGE_KEY_TYPE "type"
#define BASE_MESSAGE_KEY_IS_FILE_ATTACHED "isFileAttached"
#define BASE_MESSAGE_KEY_FILE_NAME "fileName"
#define BASE_MESSAGE_KEY_PAYLOAD "payload"

// BaseMessage: the root base message
// - Decoding(makeFromJSON): C++, Java
// - Encoding(make, toJSON): C++, Java
class BaseMessage {
  public:
    friend class MessageFactory;

    // File attachment
    // Attached file will be transferred to target or be received from target
    // in each Channel.
    // Some Channel can refuse to transfer it due to its capability.
    // ex. CommChannel and LocalChnanel can handle attached file, but
    //     DbusChannel cannot.
    
    // Attach file on message to be sent
    BaseMessage* attachFile(std::string filePath);

    // Set local file path when attached file has came
    BaseMessage* setStoredFilePath(std::string storedFilePath) {
      this->mStoredFilePath = storedFilePath;
      return this;
    }
    std::string& getStoredFilePath() {
      return this->mStoredFilePath;
    }

    // encoding to JSON
    cJSON* toJSON();
    char* toJSONString() {
      cJSON* jsonObj = this->toJSON();
      return cJSON_Print(jsonObj);
    }

    // Get parameters
    int getMessageId() { return this->mMessageId; }
    std::string& getSenderUri() { return this->mSenderUri; }
    std::string& getUri() { return this->mUri; }
    BaseMessageType::Value getType() { return this->mType; }
    bool isFileAttached() { return this->mIsFileAttached; }
    std::string& getFileName() { return this->mFileName; }

    // Payload
    void setPayload(BaseMessagePayload* payload) { this->mPayload = payload; }
    BaseMessagePayload* getPayload() { return this->mPayload; }

  protected:
    BaseMessage(int messageId, std::string senderUri, std::string uri,
        BaseMessageType::Value type,
        bool isFileAttached, std::string fileName)
      : mMessageId(messageId), mSenderUri(senderUri), mUri(uri), mType(type),
      mIsFileAttached(isFileAttached), mFileName(fileName),
      mStoredFilePath("") {
    }
    BaseMessage(int messageId, std::string senderUri, std::string uri,
        BaseMessageType::Value type)
      : mMessageId(messageId), mSenderUri(senderUri), mUri(uri), mType(type),
      mIsFileAttached(false), mFileName(""),
      mStoredFilePath("") {
    }

    // JSON-exported values
    int mMessageId;
    std::string mSenderUri;
    std::string mUri;
    BaseMessageType::Value mType;
    bool mIsFileAttached;
    std::string mFileName;
    BaseMessagePayload* mPayload;

    // Internal value
    // StoreFilePath: the local storage path of attached file
    std::string mStoredFilePath;
};

#endif // !defined(__BASE_MESSAGE_H__)