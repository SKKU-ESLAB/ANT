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
#ifndef __NATIVE_RESOURCE_API_H__
#define __NATIVE_RESOURCE_API_H__

#include <iostream>
#include <string>
#include <vector>

#include "Resource.h"
#include "ResourceRequest.h"
#include "ResourceResponse.h"

class NativeResourceAPI {
public:
  static void initialize(MessageRouter *messageRouter,
                         LocalChannel *localChannel);

  static void registerResource(Resource *resource);
  static Resource *findLocalResource(std::string &uri);

  static void sendPost(Resource* sender, std::string &targetUri,
                       std::string &body, ResourceResponseCallback callback);
  static void sendGet(Resource* sender, std::string &targetUri,
                      std::string &body, ResourceResponseCallback callback);
  static void sendPut(Resource* sender, std::string &targetUri,
                      std::string &body, ResourceResponseCallback callback);
  static void sendDelete(Resource* sender, std::string &targetUri,
                         std::string &body, ResourceResponseCallback callback);

  static void sendDiscover(Resource* sender, std::string &targetUri,
                           std::string &body,
                           ResourceResponseCallback callback);
  static void sendSubscribe(Resource* sender, std::string &targetUri,
                            std::string &body,
                            ResourceResponseCallback callback);
  static void sendUnsubscribe(Resource* sender, std::string &targetUri,
                              std::string &body,
                              ResourceResponseCallback callback);

private:
};

#endif // !defined(__NATIVE_RESOURCE_API_H__)