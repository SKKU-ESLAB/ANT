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
#ifndef __RESOURCE_RESPONSE_H__
#define __RESOURCE_RESPONSE_H__

#include <iostream>
#include <string>
#include <vector>

#include "cJSON.h"

#include "BaseMessage.h"
#include "ResourceMessageCommon.h"

#define RESOURCE_RESPONSE_KEY_OP_TYPE "opType"
#define RESOURCE_RESPONSE_KEY_REQUEST_MESSAGE_ID "reqMsgId"
#define RESOURCE_RESPONSE_KEY_STATUS_CODE "statusCode"
#define RESOURCE_RESPONSE_KEY_BODY "body"

// ResourceResponse: message sent to everywhere
// - Decoding(makeFromJSON): C++, Java, JavaScript
// - Encoding(make, toJSON): C++, Java, JavaScript
class ResourceResponse : public BaseMessagePayload {
public:
  friend class MessageFactory;
  ~ResourceResponse() {}

  // encoding to JSON
  virtual cJSON *toJSON();

  // Get parameters
  ResourceOperationType::Value getOpType() { return this->mOpType; }

  int getRequestMessageId() { return this->mRequestMessageId; }

  int getStatusCode() { return this->mStatusCode; }

  std::string &getBody() { return this->mBody; }

  // Set command-specific parameters
  void setBody(std::string &body) { this->mBody.assign(body); }

  void setStatusCode(int statusCode) { this->mStatusCode = statusCode; }

private:
  ResourceResponse(ResourceOperationType::Value opType, int requestMessageId,
                   int statusCode, std::string &body)
      : mOpType(opType), mRequestMessageId(requestMessageId),
        mStatusCode(statusCode), mBody(body) {}

  // JSON-exported values
  ResourceOperationType::Value mOpType;
  int mRequestMessageId;
  int mStatusCode;
  std::string mBody;
};

#endif // !defined(__RESOURCE_RESPONSE_H__)