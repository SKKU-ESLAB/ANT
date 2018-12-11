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

int NativeResourceAPI::sendPost(std::string &senderUri, std::string &targetUri,
                                std::string &body,
                                ResourceResponseCallback callback) {
  return NativeResourceAPI::sendRequest(ResourceOperationType::POST, senderUri,
                                        targetUri, body, callback);
}
int NativeResourceAPI::sendGet(std::string &senderUri, std::string &targetUri,
                               std::string &body,
                               ResourceResponseCallback callback) {
  return NativeResourceAPI::sendRequest(ResourceOperationType::GET, senderUri,
                                        targetUri, body, callback);
}
int NativeResourceAPI::sendPut(std::string &senderUri, std::string &targetUri,
                               std::string &body,
                               ResourceResponseCallback callback) {
  return NativeResourceAPI::sendRequest(ResourceOperationType::PUT, senderUri,
                                        targetUri, body, callback);
}
int NativeResourceAPI::sendDelete(std::string &senderUri,
                                  std::string &targetUri, std::string &body,
                                  ResourceResponseCallback callback) {
  return NativeResourceAPI::sendRequest(ResourceOperationType::DELETE,
                                        senderUri, targetUri, body, callback);
}

int NativeResourceAPI::sendDiscover(std::string &senderUri,
                                    std::string &targetUri, std::string &body,
                                    ResourceResponseCallback callback) {
  return NativeResourceAPI::sendRequest(ResourceOperationType::DISCOVER,
                                        senderUri, targetUri, body, callback);
}
int NativeResourceAPI::sendSubscribe(std::string &senderUri,
                                     std::string &targetUri, std::string &body,
                                     ResourceResponseCallback callback) {
  return NativeResourceAPI::sendRequest(ResourceOperationType::SUBSCRIBE,
                                        senderUri, targetUri, body, callback);
}
int NativeResourceAPI::sendUnsubscribe(std::string &senderUri,
                                       std::string &targetUri,
                                       std::string &body,
                                       ResourceResponseCallback callback) {
  return NativeResourceAPI::sendRequest(ResourceOperationType::UNSUBSCRIBE,
                                        senderUri, targetUri, body, callback);
}

int NativeResourceAPI::sendRequest(ResourceOperationType::Value opType,
                                   std::string &senderUri,
                                   std::string &targetUri, std::string &body,
                                   ResourceResponseCallback callback) {
  BaseMessage *message =
      MessageFactory::makeResourceRequest(senderUri, targetUri, opType, body);
  LocalResourceManager::singleton()->sendRequest(message, callback);
  return message->getMessageId();
}