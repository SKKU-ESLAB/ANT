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

#ifndef __INFERENCE_UNIT_H__
#define __INFERENCE_UNIT_H__

#include <pthread.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "InputReaderSet.h"
#include "InferenceRunner.h"
#include "OutputNotifier.h"

namespace InferenceUnitState {
  enum Value {
    Initialized = 1,
    Ready = 2,
    Running = 3,
    Destroyed = 4
  };
}

namespace InferenceUnitType {
  enum Value {
    DNN = 1,
    ANN = 2,
    SVM = 3,
    KNN = 4
  };
}

class InferenceUnitOutputListener {
  public:
    virtual void onInferenceUnitOutput(int iuid,
        std::map<std::string, void*> outputDataBuffers);
};

class InferenceUnitStateListener {
  public:
    virtual void onChangedInferenceUnitState(int iuid,
        InferenceUnitState::Value newState) = 0;
};

class InferenceUnit {
  public:
    // Inference Unit Thread
    static void* inferenceLoop(void*);

    // Commands
    bool run();
    bool setInput(std::string inputName, std::string sourceUri);
    bool startListeningOutput(std::string listenerUri);
    bool stopListeningOutput(std::string listenerUri);

    // Getters
    int getIuid() { return this->mIuid; }
    InferenceUnitState::Value getState() { return this->mState; }
    std::string getModelPackagePath() { return this->mModelPackagePath; }
    InferenceUnitType::Value getType() { return this->mType; }
    std::map<std::string, std::string> getInputShape() {
      return this->mInputShape;
    }
    std::map<std::string, std::string> getOutputShape() {
      return this->mOutputShape;
    }
    int getPid() { return this->mPid; }
    
    // Setters

    // State Listener
    void setStateListener(InferenceUnitStateListener* stateListener) {
      this->mStateListener = stateListener;
    }

    // Output Listener
    void setOutputListener(InferenceUnitOutputListener* outputListener) {
      this->mOutputListener = outputListener;
    }

  protected:
    // Users should create a child of InferenceUnit.
    InferenceUnit(int iuid, std::string modelPackagePath) 
    : mIuid(iuid), mState(InferenceUnitState::Initialized),
    mModelPackagePath(modelPackagePath), mPid(-1),
    mFieldsMutex(PTHREAD_MUTEX_INITIALIZER) {
      // NOTICE: Its child classes should create its own InferenceRunner
      //         in the child's constructor function.
      this->mInputReaderSet = new InputReaderSet();
    }

    // Check input & output connections and update state
    //   - Set "Ready" if inputs & outputs are all connected.
    //   - Set "Initialized" if one of inputs is disconnected
    //       or no output is connected.
    void checkConnectionsAndUpdateState();

    // State change
    void setState(InferenceUnitState::Value newState);

    // Fields determined at Initializd state //
    // Inference unit ID (IUID)
    int mIuid;
    // Inference unit's state
    InferenceUnitState::Value mState;
    // Inference unit's model package path (it should be absolute path)
    std::string mModelPackagePath;
    InferenceUnitType::Value mType;
    // Input shape (key: String name, value: String dataType)
    std::map<std::string, std::string> mInputShape;
    // Output shape (key: String name, value: String dataType)
    std::map<std::string, std::string> mOutputShape;

    // Fields which of key is determined at Initialized state,
    //   and value is determined at Initialized & Ready state
    // Input map (key: String name, value: String sourceURI)
    std::map<std::string, std::string> mInputMap;
    // Output listener URI list
    std::vector<std::string> mOutputListenerURIs;

    // Fields determined at Running state //
    // process ID (initialized as -1 at Initialized state)
    int mPid;

    // State listener //
    InferenceUnitStateListener* mStateListener = NULL;

    // Output listener //
    InferenceUnitOutputListener* mOutputListener = NULL;

    // Input Reader Set //
    InputReaderSet* mInputReaderSet = NULL;

    // Inference Runner //
    InferenceRunner* mInferenceRunner = NULL;

    // Inference Unit Thread //
    pthread_t mInferenceUnitThread;

    pthread_mutex_t mFieldsMutex;
};

#endif // !defined(__INFERENCE_UNIT_H__)
