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

#include "LocalResourceManager.h"
#include "MessageFactory.h"
#include "ResourceRequest.h"
#include "ResourceResponse.h"

LocalResourceManager *LocalResourceManager::sSingleton = NULL;

void LocalResourceManager::addLocalResource(Resource *resource) {
  this->mResources.push_back(resource);
  this->mMessageRouter->addRoutingEntry(resource->getUri(),
                                        this->mLocalChannel);
}

void LocalResourceManager::removeLocalResource(Resource *resource) {
  for (std::vector<Resource *>::iterator it = this->mResources.begin();
       it != this->mResources.end(); it++) {
    Resource *foundResource = (Resource *)*it;
    if (resource == foundResource) {
      this->mResources.erase(it);
      return;
    }
  }
}

void LocalResourceManager::removeLocalResource(std::string uri) {
  for (std::vector<Resource *>::iterator it = this->mResources.begin();
       it != this->mResources.end(); it++) {
    Resource *foundResource = (Resource *)*it;
    std::string resourceUri(foundResource->getUri());
    if (uri.compare(resourceUri) == 0) {
      this->mResources.erase(it);
      return;
    }
  }
}

Resource *LocalResourceManager::findLocalResource(std::string uri) {
  for (std::vector<Resource *>::iterator it = this->mResources.begin();
       it != this->mResources.end(); it++) {
    Resource *resource = (Resource *)*it;
    std::string resourceUri(resource->getUri());
    if (uri.compare(resourceUri) == 0) {
      return resource;
    }
  }
  return NULL;
}

// Send Request
void LocalResourceManager::sendRequest(BaseMessage *requestMessage,
                                       ResourceResponseCallback callback) {
  if (this->mLocalChannel == NULL) {
    ANT_DBG_ERR("LocalResourceManager is not initialized!");
    return;
  }

  // Register Response Callback
  ResourceRequest *request = (ResourceRequest *)requestMessage->getPayload();
  ResourceOperationType::Value opType = request->getOpType();
  switch (opType) {
  case ResourceOperationType::POST:
  case ResourceOperationType::GET:
  case ResourceOperationType::PUT:
  case ResourceOperationType::DELETE:
  case ResourceOperationType::DISCOVER: {
    this->addResponseCallback(requestMessage->getMessageId(), callback);
    break;
  }
  case ResourceOperationType::SUBSCRIBE: {
    this->addResponseCallback(requestMessage->getMessageId(), callback);
    this->addSubscription(requestMessage->getUri(),
                          requestMessage->getMessageId());
    break;
  }
  case ResourceOperationType::UNSUBSCRIBE: {
    int requestMessageId = this->removeSubscription(requestMessage->getUri());
    if (requestMessageId >= 0) {
      this->removeResponseCallback(requestMessageId);
    }
    break;
  }
  case ResourceOperationType::NotDetermined:
  default: {
    ANT_DBG_ERR("Invalid opType: %d", opType);
    return;
  }
  }

  // Send Message
  this->mLocalChannel->sendMessage(requestMessage);
}

// Send Response
void LocalResourceManager::sendResponse(BaseMessage *responseMessage) {
  if (this->mLocalChannel == NULL) {
    ANT_DBG_ERR("LocalResourceManager is not initialized!");
    return;
  }

  // Send Message
  this->mLocalChannel->sendMessage(responseMessage);
}

// LocalChannelListener: Receive Message
void LocalResourceManager::onReceivedMessage(BaseMessage *receivedMessage) {
  Resource *resource = this->findLocalResource(receivedMessage->getUri());
  if (resource == NULL) {
    // Not found local resource - ignore the message
    return;
  }

  BaseMessageType::Value messageType = receivedMessage->getType();
  switch (messageType) {
  case BaseMessageType::ResourceRequest: {
    ResourceRequest *request = (ResourceRequest *)receivedMessage->getPayload();
    ResourceOperationType::Value opType = request->getOpType();

    BaseMessage *responseMessage =
        MessageFactory::makeResourceResponse(receivedMessage);
    ResourceRequestCallback callback = resource->getRequestCallback(opType);
    if (callback != NULL) {
      // Execute callback
      callback(receivedMessage, responseMessage);
    } else {
      // Method not allowed
      ResourceResponse *response =
          (ResourceResponse *)responseMessage->getPayload();
      response->setStatusCode(RESOURCE_STATUS_METHOD_NOT_ALLOWED);
    }
    // Send response message
    this->sendResponse(responseMessage);
    break;
  }
  case BaseMessageType::ResourceResponse: {
    ResourceResponse *response =
        (ResourceResponse *)receivedMessage->getPayload();
    ResourceOperationType::Value opType = response->getOpType();
    int requestMessageId = response->getRequestMessageId();

    // Execute callback
    ResourceResponseCallback callback =
        this->getResponseCallback(requestMessageId);
    if (callback != NULL) {
      callback(receivedMessage);
      switch (opType) {
      case ResourceOperationType::POST:
      case ResourceOperationType::GET:
      case ResourceOperationType::PUT:
      case ResourceOperationType::DELETE:
      case ResourceOperationType::DISCOVER: {
        this->removeResponseCallback(requestMessageId);
        break;
      }
      case ResourceOperationType::SUBSCRIBE:
      case ResourceOperationType::UNSUBSCRIBE:
      case ResourceOperationType::NotDetermined:
      default: {
        // Do nothing
        break;
      }
      }
    }
    break;
  }
  default: {
    // No resource request nor response - ignore the message
    return;
  }
  }
}

ResourceResponseCallback
LocalResourceManager::getResponseCallback(int requestMessageId) {
  std::map<int, ResourceResponseCallback>::iterator found =
      this->mResponseCallbacks.find(requestMessageId);
  if (found != this->mResponseCallbacks.end()) {
    ResourceResponseCallback callback = *(found->second);
    return callback;
  } else {
    return NULL;
  }
}

void LocalResourceManager::addResponseCallback(
    int requestMessageId, ResourceResponseCallback callback) {
  this->mResponseCallbacks.insert(
      std::pair<int, ResourceResponseCallback>(requestMessageId, callback));
}

void LocalResourceManager::removeResponseCallback(int requestMessageId) {
  std::map<int, ResourceResponseCallback>::iterator found =
      this->mResponseCallbacks.find(requestMessageId);
  if (found != this->mResponseCallbacks.end()) {
    this->mResponseCallbacks.erase(found);
  }
}

void LocalResourceManager::addSubscription(std::string targetUri,
                                           int requestMessageId) {
  this->mSubscriptionList.insert(
      std::pair<std::string, int>(targetUri, requestMessageId));
}

int LocalResourceManager::removeSubscription(std::string targetUri) {
  std::map<std::string, int>::iterator found =
      this->mSubscriptionList.find(targetUri);
  if (found == this->mSubscriptionList.end()) {
    return -1;
  }

  int requestMessageId = found->second;
  this->mSubscriptionList.erase(found);

  return requestMessageId;
}