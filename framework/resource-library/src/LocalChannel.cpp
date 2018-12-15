/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
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

#include <pthread.h>

#include "ANTdbugLog.h"
#include "BaseMessage.h"
#include "LocalChannel.h"

void LocalChannel::run() {
  // Run RoutedLoop
  this->runRoutedLoop(this->mIsCreateRoutedThread);
}

void LocalChannel::onRoutedMessage(BaseMessage *message) {
  for (std::vector<LocalChannelListener *>::iterator it =
           this->mListeners.begin();
       it != this->mListeners.end(); it++) {
    LocalChannelListener *listener = *it;
    if (listener != NULL) {
      listener->onReceivedMessage(message);
    }
  }
}

void LocalChannel::sendMessage(BaseMessage *message) {
  // Pass the given message to MessageRouter
  this->mMessageRouter->routeMessage(this, message);
}
