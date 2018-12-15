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

#include <cstring>
#include <pthread.h>

#include "Channel.h"
#include "ANTdbugLog.h"

void Channel::runRoutedLoop(bool isCreateRoutedThread) {
  if(isCreateRoutedThread == true) {
    // Run on a separate thread
    pthread_create(&this->mRoutedThread, NULL, Channel::routedLoop,
        (void *)this);
  } else {
    // Run on this thread
    this->routedLoop(this);
  }
}

void* Channel::routedLoop(void* data) {
  Channel* self = (Channel*)data;

  while(1) {
    pthread_mutex_lock(&self->mWaitMutex);

    // Wait until someone enqueues a message to the RoutedMessageQueue
    int queueSize = self->getQueueSizeLocked();
    while(queueSize <= 0) {
      pthread_cond_wait(&self->mWaitCond, &self->mWaitMutex);
      queueSize = self->getQueueSizeLocked();
    }

    // If any message is in RoutedMessageQueue, handle one message
    BaseMessage* message = self->dequeueRoutedMessageLocked();
    self->onRoutedMessage(message);

    pthread_mutex_unlock(&self->mWaitMutex);
  }
}

void Channel::routeMessage(BaseMessage* message) {
  pthread_mutex_lock(&this->mWaitMutex);

  // Push to mesage queue
  pthread_mutex_lock(&this->mRoutedMessageQueueMutex);
  this->mRoutedMessageQueue.insert(this->mRoutedMessageQueue.begin(), message);
  pthread_mutex_unlock(&this->mRoutedMessageQueueMutex);

  // Wake up RoutedLoop so that the loop can handle the message
  pthread_cond_signal(&this->mWaitCond);

  pthread_mutex_unlock(&this->mWaitMutex);
}

int Channel::getQueueSizeLocked() {
  int queueSize; 
  pthread_mutex_lock(&this->mRoutedMessageQueueMutex);
  queueSize = this->mRoutedMessageQueue.size();
  pthread_mutex_unlock(&this->mRoutedMessageQueueMutex);
  return queueSize;
}

BaseMessage* Channel::dequeueRoutedMessageLocked() {
  pthread_mutex_lock(&this->mRoutedMessageQueueMutex);
  BaseMessage* message = this->mRoutedMessageQueue.back();
  this->mRoutedMessageQueue.pop_back();
  pthread_mutex_unlock(&this->mRoutedMessageQueueMutex);
  return message;
}
