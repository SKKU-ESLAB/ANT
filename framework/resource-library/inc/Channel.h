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

#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <pthread.h>
#include <iostream>
#include <vector>

#include "MessageRouter.h"
#include "BaseMessage.h"

class Channel;
class MessageRouter;

// Channel: Base class of all the child Channel classes
// Each Channel contains and runs one RoutedLoop.
class Channel {
  public:
    Channel(MessageRouter* messageRouter, const char* name)
      : mMessageRouter(messageRouter),
      mName(name) {
    }

    // run():
    // Just execute run() in order to use Channel.
    // Child Channel classes SHOULD IMPLEMENT its own run() function and CALL
    // runRoutedLoop() function. 
    virtual void run() = 0;

    // runRoutedLoop():
    // 'isCreateRoutedThread' determines whether RoutedLoop runs on a separate
    // thread or not. Child Channel classes can use more sub-loops for the
    // implementation of polling or sub event loop.
    void runRoutedLoop(bool isCreateRoutedThread);

    // routeMessage():
    // Enqueue a message to the RoutedMessageQueue then make it handled by
    // RoutedLoop.
    void routeMessage(BaseMessage* message);

    // getName():
    // Get this channel's name for readability
    std::string& getName() { return this->mName; }

  protected:
    // VIRTUAL onRoutedMessage():
    // Child Channel class can handle a message that has routed to the Channel.
    virtual void onRoutedMessage(BaseMessage* message) = 0;

    // Routed Loop:
    // RoutedLoop reads RoutedMessageQueue when the queue is updated.
    static void* routedLoop(void*);

    // Locked functions (mRoutedMessageQueueMutex)
    int getQueueSizeLocked();
    BaseMessage* dequeueRoutedMessageLocked();

    // Message Router:
    // Channels can a route request for a message to MessageRouter,
    // and MessageRouter routes the message to other Channel.
    MessageRouter* mMessageRouter;

    // Channel name
    // It is referenced by MessageRouter for readability
    std::string mName;

    // Routed Message Queue:
    // When MessageRouter routes a message to Channel, the message is inserted
    // into RoutedMessageQueue of Channel. The message will be handled in
    // the Channel's RoutedLoop.
    std::vector<BaseMessage*> mRoutedMessageQueue;
    pthread_mutex_t mRoutedMessageQueueMutex = PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_t mWaitMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t mWaitCond = PTHREAD_COND_INITIALIZER;

    // RoutedThread (Enabled when isCreateRoutedThread is true)
    pthread_t mRoutedThread;
};

#endif // !defined(__CHANNEL_H__)
