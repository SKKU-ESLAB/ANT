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
#ifndef __LOCAL_RESOURCE_MANAGER_H__
#define __LOCAL_RESOURCE_MANAGER_H__

#include <iostream>
#include <string>
#include <vector>

#include "ANTdbugLog.h"
#include "LocalChannel.h"
#include "MessageRouter.h"
#include "Resource.h"

class LocalResourceManager : public LocalChannelListener {
public:
  static LocalResourceManager *singleton() {
    if (sSingleton == NULL) {
      sSingleton = new LocalResourceManager();
    }
    return sSingleton;
  }

  void initialize(MessageRouter *messageRouter, LocalChannel *localChannel);

  void addLocalResource(Resource *resource);
  void removeLocalResource(Resource *resource);
  void removeLocalResource(std::string uri);
  
  Resource *findLocalResource(std::string uri);

  // Send Request
  void sendRequest(BaseMessage *requestMessage,
                   ResourceResponseCallback callback);
  void sendResponse(BaseMessage *responseMessage);

  // LocalChannelListener: Receive Message
  virtual void onReceivedMessage(BaseMessage *message);

private:
  static LocalResourceManager *sSingleton;

  ~LocalResourceManager() {
    for (std::vector<Resource *>::iterator it = this->mResources.begin();
         it != this->mResources.end(); it++) {
      this->mResources.erase(it);
      Resource *resource = (Resource *)*it;
      this->mMessageRouter->removeRoutingEntry(resource->getUri());
      delete resource;
    }
  }

  ResourceResponseCallback getResponseCallback(int requestMessageId);
  void addResponseCallback(int requestMessageId,
                           ResourceResponseCallback callback);
  void removeResponseCallback(int requestMessageId);

  void addSubscription(std::string targetUri, int requestMessageId);
  int removeSubscription(std::string targetUri);

  MessageRouter *mMessageRouter;
  LocalChannel *mLocalChannel;

  std::vector<Resource *> mResources;
  std::map<int, ResourceResponseCallback> mResponseCallbacks;
  std::map<std::string, int> mSubscriptionList;
};

#endif // !defined(__LOCAL_RESOURCE_MANAGER_H__)