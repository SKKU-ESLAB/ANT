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
#ifndef __RESOURCE_REQUEST_H__
#define __RESOURCE_REQUEST_H__

#include <iostream>
#include <string>
#include <vector>

#include "cJSON.h"

#include "BaseMessage.h"
#include "ResourceMessageCommon.h"  

#define RESOURCE_REQUEST_KEY_OP_TYPE "opType"
#define RESOURCE_REQUEST_KEY_BODY "body"

// ResourceRequest: message sent to everywhere
// - Decoding(makeFromJSON): C++, Java, JavaScript
// - Encoding(make, toJSON): C++, Java, JavaScript
class ResourceRequest : public BaseMessagePayload {
public:
  friend class MessageFactory;
  ~ResourceRequest() {}

  // encoding to JSON
  virtual cJSON *toJSON();

  // Get parameters
  ResourceOperationType::Value getOpType() {
    return this->mOpType;
  }

  std::string &getBody() { return this->mBody; }

  // Set command-specific parameters
  void setBody(std::string &body) { this->mBody.assign(body); }

private:
  ResourceRequest(ResourceOperationType::Value opType, std::string &body)
      : mOpType(opType), mBody(body) {}

  // JSON-exported values
  ResourceOperationType::Value mOpType;
  std::string mBody;
};

#endif // !defined(__RESOURCE_REQUEST_H__)