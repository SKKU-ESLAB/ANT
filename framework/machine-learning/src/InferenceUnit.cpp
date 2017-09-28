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

#include "InferenceUnit.h"

bool InferenceUnit::start() {
  pthread_mutex_lock(&this->mThreadRunningMutex);

  // Check if state is "Ready"
  InferenceUnitState::Value state = this->getState();
  if( state != InferenceUnitState::Ready) {
    pthread_mutex_unlock(&this->mThreadRunningMutex);
    return false;
  }

  // Make Inference Unit Thread
  pthread_create(&this->mInferenceUnitThread, NULL,
      InferenceUnit::inferenceLoop, (void *)this);

  pthread_mutex_unlock(&this->mThreadRunningMutex);
  return true;
}

bool InferenceUnit::stop() {
  pthread_mutex_lock(&this->mThreadRunningMutex);

  // Check if state is "Running"
  InferenceUnitState::Value state = this->getState();
  if( state != InferenceUnitState::Running) {
    pthread_mutex_unlock(&this->mThreadRunningMutex);
    return false;
  }

  this->mIsThreadRunning = false;

  pthread_mutex_unlock(&this->mThreadRunningMutex);
  return true;
}

// TODO: I assumed that input buffer & output buffer's size are fixed.
// However, this assumption is not realistic.
// TODO: input/output buffer size should be tuned according to data type
#define INPUT_BUFFER_SIZE 1024
#define OUTPUT_BUFFER_SIZE 1024

void* InferenceUnit::inferenceLoop(void* data) {
  InferenceUnit* self = (InferenceUnit*)data;

  self->mIsThreadRunning = true;

  while(self->mIsThreadRunning) {
    pthread_mutex_lock(&self->mInputMutex);
    std::map<std::string, std::string> inputMap = self->mInputMap;
    pthread_mutex_unlock(&self->mInputMutex);

    // Step 1. Run InputReaders to load inputs
    // InputDataBuffer: Dictionary
    //   (key=string inputName, value=void* inputDataBuffer)
    std::map<std::string, void*> inputDataBuffers;
    std::map<std::string, std::string>::iterator imIter;
    for(imIter = inputMap.begin();
        imIter != inputMap.end();
        ++imIter) {
      std::string inputName(imIter->first);
      std::string sourceUri(imIter->second);

      // TODO: fixed buffer size -> buffer size corresponding to data type
      // Allocate inputBuffer
      char* inputBuffer = new char[INPUT_BUFFER_SIZE];

      // run InputReader
      self->mInputReaderSet->read(sourceUri, inputBuffer);

      // Store input buffer
      inputDataBuffers.insert(std::pair<std::string, void*>(
            inputName, (void*)inputBuffer));
    }

    pthread_mutex_lock(&self->mOutputMutex);
    std::map<std::string, std::string> outputShape = self->mOutputShape;
    pthread_mutex_unlock(&self->mOutputMutex);

    // Make OutputDataBuffer
    std::map<std::string, void*> outputDataBuffers;
    std::map<std::string, std::string>::iterator osIter;
    for(osIter = outputShape.begin();
        osIter != outputShape.end();
        ++osIter) {
      std::string outputName(osIter->first);
      std::string dataType(osIter->second);

      // TODO: fixed buffer size -> buffer size corresponding to data type
      // Allocate outputBuffer
      char* outputBuffer = new char[OUTPUT_BUFFER_SIZE];

      outputDataBuffers.insert(std::pair<std::string, void*>(
            outputName, outputBuffer));
    }

    // Step 2. Run InferenceRunner to do inference
    self->mInferenceRunner->run(inputDataBuffers, self->mInputShape,
        outputDataBuffers);

    // Step 3. Notify output to listener
    // (OutputListener will notify the output to listening apps)
    if(self->mOutputListener != NULL) {
      self->mOutputListener->onInferenceUnitOutput(self->getIuid(),
          outputDataBuffers);
    }

    // Deallocate input buffers and output buffers
    std::map<std::string, void*>::iterator ibIter;
    for(ibIter = inputDataBuffers.begin();
        ibIter != inputDataBuffers.end();
        ++ibIter) {
      char* inputBuffer = (char*)(ibIter->second);
      delete[] inputBuffer;
    }

    std::map<std::string, void*>::iterator obIter;
    for(obIter = outputDataBuffers.begin();
        obIter != outputDataBuffers.end();
        ++obIter) {
      char* outputBuffer = (char*)(obIter->second);
      delete[] outputBuffer;
    }
  }
}

bool InferenceUnit::setInput(std::string inputName, std::string sourceUri) {
  // check if state is "Initialized" or "Ready"
  InferenceUnitState::Value state = this->getState();
  if(state != InferenceUnitState::Initialized
      && state != InferenceUnitState::Ready) {
    return false;
  }

  // TODO: check if source URI's type is same as inputShape's dataType

  // set input map
  pthread_mutex_lock(&this->mInputMutex);
  bool foundInputEntry = false;
  std::map<std::string, std::string>::iterator imIter;
  for(imIter = this->mInputMap.begin();
      imIter != this->mInputMap.end();
      ++imIter) {
    std::string entryName(imIter->first);
    if(entryName.compare(inputName) == 0) {
      imIter->second = sourceUri;
      foundInputEntry = true;
      break;
    }
  }
  if(!foundInputEntry) {
    return false;
  }
  pthread_mutex_unlock(&this->mInputMutex);

  // check input & output conenctions and update state
  this->checkConnectionsAndUpdateState();

  return true;
}

bool InferenceUnit::startListeningOutput(std::string listenerUri) {
  // Check if state is "Initialized" or "Ready" or "Running"
  InferenceUnitState::Value state = this->getState();
  if(state != InferenceUnitState::Initialized
      && state != InferenceUnitState::Ready
      && state != InferenceUnitState::Running) {
    return false;
  }

  // Add listener URI to the list
  pthread_mutex_lock(&this->mOutputMutex);
  this->mOutputListenerURIs.push_back(listenerUri);
  pthread_mutex_unlock(&this->mOutputMutex);
  return true;
}

bool InferenceUnit::stopListeningOutput(std::string listenerUri) {
  // Check if state is "Initialized" or "Ready" or "Running"
  InferenceUnitState::Value state = this->getState();
  if(state != InferenceUnitState::Initialized
      && state != InferenceUnitState::Ready
      && state != InferenceUnitState::Running) {
    return false;
  }

  // Remove listener URI from the list
  pthread_mutex_lock(&this->mOutputMutex);
  bool foundOutputEntry = false;
  std::vector<std::string>::iterator olIter;
  for(olIter = this->mOutputListenerURIs.begin();
      olIter != this->mOutputListenerURIs.end();
      ++olIter) {
    std::string entryListenerUri(*olIter);
    if(entryListenerUri.compare(listenerUri) == 0) {
      this->mOutputListenerURIs.erase(olIter);
      foundOutputEntry = true;
      break;
    }
  }
  pthread_mutex_unlock(&this->mOutputMutex);
  if(!foundOutputEntry) {
    return false;
  }

  return true;
}

void InferenceUnit::checkConnectionsAndUpdateState() {
  // TODO:
  // Set "Ready" if inputs & outputs are all connected.
  // Set "Initialized" if one of inputs is disconnected
  //   or no output is connected.
}

void InferenceUnit::setState(InferenceUnitState::Value newState) {
  InferenceUnitState::Value oldState = this->mState;
  this->mState = newState;

  // TODO: Handle state change
  switch(newState) {
    case InferenceUnitState::Initialized:
      break;
    case InferenceUnitState::Ready:
      break;
    case InferenceUnitState::Running:
      break;
    case InferenceUnitState::Destroyed:
      break;
    default:
      break;
  }

  // Notify the state change to state listener
   if(this->mStateListener != NULL) {
     this->mStateListener->onChangedInferenceUnitState(
         this->getIuid(), newState);
   }
}
