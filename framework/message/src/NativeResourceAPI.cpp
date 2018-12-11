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

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ANTdbugLog.h"
#include "LocalResourceManager.h"
#include "MessageFactory.h"
#include "MessageUtil.h"
#include "NativeResourceAPI.h"

void NativeResourceAPI::initialize(MessageRouter *messageRouter,
                                   LocalChannel *localChannel) {
  LocalResourceManager::singleton()->initialize(messageRouter, localChannel);
}

void NativeResourceAPI::registerResource(Resource *resource) {
  LocalResourceManager::singleton()->addLocalResource(resource);
}

Resource *NativeResourceAPI::findLocalResource(std::string &uri) {
  return LocalResourceManager::singleton()->findLocalResource(uri);
}

void NativeResourceAPI::sendPost(Resource *sender, std::string &targetUri,
                                 std::string &body,
                                 ResourceResponseCallback callback) {
  BaseMessage *message = MessageFactory::makeResourceRequest(
      sender->getUri(), targetUri, ResourceOperationType::POST, body);
  LocalResourceManager::singleton()->sendRequest(message, callback);
}
void NativeResourceAPI::sendGet(Resource *sender, std::string &targetUri,
                                std::string &body,
                                ResourceResponseCallback callback) {
  BaseMessage *message = MessageFactory::makeResourceRequest(
      sender->getUri(), targetUri, ResourceOperationType::GET, body);
  LocalResourceManager::singleton()->sendRequest(message, callback);
}
void NativeResourceAPI::sendPut(Resource *sender, std::string &targetUri,
                                std::string &body,
                                ResourceResponseCallback callback) {
  BaseMessage *message = MessageFactory::makeResourceRequest(
      sender->getUri(), targetUri, ResourceOperationType::PUT, body);
  LocalResourceManager::singleton()->sendRequest(message, callback);
}
void NativeResourceAPI::sendDelete(Resource *sender, std::string &targetUri,
                                   std::string &body,
                                   ResourceResponseCallback callback) {
  BaseMessage *message = MessageFactory::makeResourceRequest(
      sender->getUri(), targetUri, ResourceOperationType::DELETE, body);
  LocalResourceManager::singleton()->sendRequest(message, callback);
}

void NativeResourceAPI::sendDiscover(Resource *sender, std::string &targetUri,
                                     std::string &body,
                                     ResourceResponseCallback callback) {
  BaseMessage *message = MessageFactory::makeResourceRequest(
      sender->getUri(), targetUri, ResourceOperationType::DISCOVER, body);
  LocalResourceManager::singleton()->sendRequest(message, callback);
}
void NativeResourceAPI::sendSubscribe(Resource *sender, std::string &targetUri,
                                      std::string &body,
                                      ResourceResponseCallback callback) {
  BaseMessage *message = MessageFactory::makeResourceRequest(
      sender->getUri(), targetUri, ResourceOperationType::SUBSCRIBE, body);
  LocalResourceManager::singleton()->sendRequest(message, callback);
}
void NativeResourceAPI::sendUnsubscribe(Resource *sender,
                                        std::string &targetUri,
                                        std::string &body,
                                        ResourceResponseCallback callback) {
  BaseMessage *message = MessageFactory::makeResourceRequest(
      sender->getUri(), targetUri, ResourceOperationType::UNSUBSCRIBE, body);
  LocalResourceManager::singleton()->sendRequest(message, callback);
}