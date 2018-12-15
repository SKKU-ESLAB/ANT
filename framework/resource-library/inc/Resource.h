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
#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <iostream>
#include <string>
#include <vector>

#include "BaseMessage.h"
#include "ResourceMessageCommon.h"

typedef void (*ResourceRequestCallback)(BaseMessage *requestMessage,
                                        BaseMessage *responseMessage);
typedef void (*ResourceResponseCallback)(BaseMessage *responseMessage);

class Resource {
public:
  Resource(std::string &uri) : mUri(uri) {}
  void setOnPost(ResourceRequestCallback postRequestCallback) {
    this->mPostRequestCallback = postRequestCallback;
  }
  void setOnGet(ResourceRequestCallback getRequestCallback) {
    this->mGetRequestCallback = getRequestCallback;
  }
  void setOnPut(ResourceRequestCallback putRequestCallback) {
    this->mPutRequestCallback = putRequestCallback;
  }
  void setOnDelete(ResourceRequestCallback deleteRequestCallback) {
    this->mDeleteRequestCallback = deleteRequestCallback;
  }
  void setOnSubscribe(ResourceRequestCallback subscribeRequestCallback) {
    this->mSubscribeRequestCallback = subscribeRequestCallback;
  }
  void setOnUnsubscribe(ResourceRequestCallback unsubscribeRequestCallback) {
    this->mUnsubscribeRequestCallback = unsubscribeRequestCallback;
  }

  std::string &getUri() { return this->mUri; }

  ResourceRequestCallback
  getRequestCallback(ResourceOperationType::Value opType) {
    ResourceRequestCallback callback = NULL;
    switch (opType) {
    case ResourceOperationType::POST: {
      callback = this->mPostRequestCallback;
      break;
    }
    case ResourceOperationType::GET: {
      callback = this->mGetRequestCallback;
      break;
    }
    case ResourceOperationType::PUT: {
      callback = this->mPutRequestCallback;
      break;
    }
    case ResourceOperationType::DELETE: {
      callback = this->mDeleteRequestCallback;
      break;
    }
    case ResourceOperationType::DISCOVER: {
      callback = NULL; // TODO: onDiscover: report to resource directory
      break;
    }
    case ResourceOperationType::SUBSCRIBE: {
      callback = this->mSubscribeRequestCallback;
      break;
    }
    case ResourceOperationType::UNSUBSCRIBE: {
      callback = this->mUnsubscribeRequestCallback;
      break;
    }
    case ResourceOperationType::NotDetermined:
    default: {
      callback = NULL;
      break;
    }
    }
    return callback;
  }

private:
  std::string mUri;
  ResourceRequestCallback mPostRequestCallback = NULL;
  ResourceRequestCallback mGetRequestCallback = NULL;
  ResourceRequestCallback mPutRequestCallback = NULL;
  ResourceRequestCallback mDeleteRequestCallback = NULL;
  ResourceRequestCallback mSubscribeRequestCallback = NULL;
  ResourceRequestCallback mUnsubscribeRequestCallback = NULL;

  // TODO: onDiscover: report to resource directory
};

#endif // !defined(__RESOURCE_H__)