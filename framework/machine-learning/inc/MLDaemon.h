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
#include "MLMessage.h"
#include "InferenceUnit.h"
#include "InferenceUnitDirectory.h"
#include "InputReaderSet.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>

#define PATH_BUFFER_SIZE 1024

class LocalChannel;

class MLDaemon
: public LocalChannelListener,
  public InferenceUnitOutputListener {

  public:
    MLDaemon() {
    }

    ~MLDaemon() {
      delete this->mMessageRouter;
      delete this->mDbusChannel;
      delete this->mLocalChannel;

      std::vector<BaseMessage*>::iterator iter;
      for(iter = this->mListenIUOutputList.begin();
          iter != this->mListenIUOutputList.end();
          iter++) {
        this->mListenIUOutputList.erase(iter);
        delete (*iter);
      }
    }

    // Main loop
    void run();

    // Initialize
    void initialize();

    // Testing mode
    void runTest(std::string modelName);

    // LocalChannelListener
    virtual void onReceivedMessage(BaseMessage* message);

    // InferenceUnitOutputListener
    virtual void onInferenceUnitOutput(int iuid,
        std::string listenerUri, MLDataUnit* outputData);

  protected:
    // MLFW commands 
    void loadIU(BaseMessage* message);
    void unloadIU(BaseMessage* message);
    void getIUs(BaseMessage* message);
    void setIUInput(BaseMessage* message);
    void startListeningIUOutput(BaseMessage* message);
    void stopListeningIUOutput(BaseMessage* message);
    void startIU(BaseMessage* message);
    void stopIU(BaseMessage* message);
    void getIUResourceUsage(BaseMessage* message);
    void runModel(BaseMessage* message);

    // MLFW internal functions
    int loadIU(std::string modelPackagePath, MLDataUnit params);
    int loadBuiltin(std::string modelName);
    bool unloadIU(int iuid);   
    std::map<int, InferenceUnit*>& getIUs();
    bool setIUInput(int iuid, std::string inputName, std::string sourceUri);
    bool startListeningIUOutput(int iuid, std::string listenerUri);
    bool stopListeningIUOutput(int iuid, std::string listenerUri);
    bool startIU(int iuid);
    bool stopIU(int iuid);
    std::string getIUResourceUsage(int iuid);
    bool runModel(std::string modelName);

    // Input Reader Set
    InputReaderSet* mInputReaderSet = NULL;

    // Inference Unit Directory
    InferenceUnitDirectory mInferenceUnitDirectory;

    // Inference unit output listen request message list
    std::vector<BaseMessage*> mListenIUOutputList;

    // Message framework
    MessageRouter* mMessageRouter = NULL;
    DbusChannel* mDbusChannel = NULL;
    LocalChannel* mLocalChannel = NULL;

    // Test mode
    bool mIsTestMode = false;
};

#endif // !defined(__ML_DAEMON_H__)
