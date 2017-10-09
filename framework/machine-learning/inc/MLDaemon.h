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

#ifndef __ML_DAEMON_H__
#define __ML_DAEMON_H__

#include "LocalChannel.h"
#include "MessageRouter.h"
#include "DbusChannel.h"
#include "BaseMessage.h"
#include "InferenceUnitParams.h"
#include "InferenceUnit.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>

#define PATH_BUFFER_SIZE 1024

class LocalChannel;

class MLDaemon
: public LocalChannelListener {

  public:
    MLDaemon() {
    }

    ~MLDaemon() {
      delete this->mMessageRouter;
      delete this->mDbusChannel;
      delete this->mLocalChannel;
    }

    // Main loop
    void run();

    // LocalChannelListener
    virtual void onReceivedMessage(BaseMessage* message);

  protected:
    // MLFW commands 
    void loadIU(std::string modelFilePath,
        InferenceUnitParams params);
    void unloadIU(int iuid);
    void getIUs();
    void setIUInput(int iuid,
        std::string inputName, std::string sourceUri);
    void startListeningIUOutput(int iuid,
        std::string listenerUri);
    void stopListeningIUOutput(int iuid,
        std::string listenerUri);
    void startIU(int iuid);
    void stopIU(int iuid);
    void getIUResourceUsage(int iuid);
    
    // Inference Unit Directory (key: int iuid, value: InferenceUnit)
    std::map<int, InferenceUnit*> mInferenceUnitDirectory;

    // Message framework
    MessageRouter* mMessageRouter = NULL;
    DbusChannel* mDbusChannel = NULL;
    LocalChannel* mLocalChannel = NULL;
};

#endif // !defined(__ML_DAEMON_H__)
