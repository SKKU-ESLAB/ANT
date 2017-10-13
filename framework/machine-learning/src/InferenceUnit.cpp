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
#include "ANTdbugLog.h"

bool InferenceUnit::unload() {
  InferenceUnitState::Value state = this->getState();
  if(state != InferenceUnitState::Initialized) {
    ANT_DBG_ERR("Invalid state!: %s", state);
    return false;
  }

  this->setState(InferenceUnitState::Destroyed);
  return true;
}

bool InferenceUnit::start() {
  pthread_mutex_lock(&this->mThreadRunningMutex);

  // Check if state is "Ready"
  InferenceUnitState::Value state = this->getState();
  if(state != InferenceUnitState::Ready) {
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
  if(state != InferenceUnitState::Running) {
    pthread_mutex_unlock(&this->mThreadRunningMutex);
    return false;
  }

  this->mIsThreadRunning = false;

  pthread_mutex_unlock(&this->mThreadRunningMutex);
  return true;
}

void* InferenceUnit::inferenceLoop(void* data) {
  InferenceUnit* self = (InferenceUnit*)data;

  self->mIsThreadRunning = true;

  while(self->mIsThreadRunning) {
    pthread_mutex_lock(&self->mInputMutex);
    std::map<std::string, std::string> inputMap = self->mInputMap;

    // Step 1. Run InputReaders to load inputs
    // InputDataBuffer: Dictionary
    //   (key=string inputName, value=void* inputDataBuffer)
    MLDataUnit* inputData = new MLDataUnit();
    std::map<std::string, std::string>::iterator imIter;
    for(imIter = inputMap.begin();
        imIter != inputMap.end();
        ++imIter) {
      std::string inputName(imIter->first);
      std::string sourceUri(imIter->second);

      // Run InputReader
      MLTensor* inputTensor = self->mInputReaderSet->read(sourceUri);
      
      // Verify InputTensor
      MLTensorLayout* givenTensorLayout
        = self->mInputLayout->findTensorLayout(inputName);
      if(inputTensor == NULL) {
        ANT_DBG_ERR("Input tensor is NULL! : %s(%s)",
            inputName.c_str(), sourceUri.c_str());
        pthread_mutex_unlock(&self->mInputMutex);
        delete inputData;
        self->stop();
        return NULL;
      } else if(givenTensorLayout == NULL
          || !inputTensor->isMatched(*givenTensorLayout)) {
        ANT_DBG_ERR("Input tensor layout is not matched! : %s(%s)",
            inputName.c_str(), sourceUri.c_str());
        pthread_mutex_unlock(&self->mInputMutex);
        delete inputData;
        self->stop();
        return NULL;
      }

      // Store input buffer
      inputData->insertTensor(inputName, inputTensor);
    }
    pthread_mutex_unlock(&self->mInputMutex);

    // Step 2. Run InferenceRunner to do inference
    MLDataUnit* outputData = self->mInferenceRunner->run(inputData);

    // Verify OutputData
    if(outputData == NULL) {
        ANT_DBG_ERR("OutputData is NULL!");
        delete inputData;
        delete outputData;
        self->stop();
        return NULL;
    }
    pthread_mutex_lock(&self->mOutputMutex);
    MLDataUnitLayout* outputLayout = self->mOutputLayout;
    if(!outputData->isMatched(outputLayout)) {
        ANT_DBG_ERR("OutputData data unit layout is not matched!");
        pthread_mutex_unlock(&self->mOutputMutex);
        delete inputData;
        delete outputData;
        self->stop();
        return NULL;
    }
    pthread_mutex_unlock(&self->mOutputMutex);

    // Step 3. Notify output to listener
    // (OutputListener will notify the output to listening apps)
    pthread_mutex_lock(&self->mOutputMutex);
    if(self->mOutputListener != NULL) {
      self->mOutputListener->onInferenceUnitOutput(self->getIuid(),
          outputData);
    }
    pthread_mutex_unlock(&self->mOutputMutex);

    // Deallocate input data and output data
    delete inputData;
    delete outputData;
  }
}

bool InferenceUnit::setInput(std::string inputName, std::string sourceUri) {
  // Check if state is "Initialized" or "Ready"
  InferenceUnitState::Value state = this->getState();
  if(state != InferenceUnitState::Initialized
      && state != InferenceUnitState::Ready) {
    return false;
  }

  // Find InputReader
  InputReader* inputReader
    = this->mInputReaderSet->findBestInputReader(sourceUri);

  // Check if source URI's TensorLayout is matched with inputShape's one
  MLTensorLayout sourceTensorLayout = inputReader->getLayout();
  MLTensorLayout* iuInputTensorLayout
    = this->mInputLayout->findTensorLayout(inputName);
  if(iuInputTensorLayout == NULL
      || !sourceTensorLayout.isMatched(iuInputTensorLayout)) {
    ANT_DBG_ERR("Input tensor layout is not matched! : %s(%s)",
        inputName.c_str(), sourceUri.c_str());
    return false;
  }

  // Set input map
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

  // Check input & output conenctions and update state
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

std::string InferenceUnit::getResourceUsage() {
  std::string data("");

  if(this->mInferenceRunner == NULL) {
    ANT_DBG_ERR("Inference runner is not defined! : %d", this->getIuid());
  } else {
    data.assign(this->mInferenceRunner->getResourceUsage());
  }
  return data;
}

// Check if the inference unit is ready to run
// - Set "Ready" if inputs & outputs are all connected.
// - Set "Initialized" if one of inputs is disconnected
//     or no output is connected.
void InferenceUnit::checkConnectionsAndUpdateState() {
  InferenceUnitState::Value state = this->getState();
  if(state != InferenceUnitState::Initialized
      && state != InferenceUnitState::Ready) {
    // This funciton runs only when the state is Initialized or Ready.
    return;
  }

  if(this->checkConnections()) {
    this->setState(InferenceUnitState::Ready);
  } else {
    this->setState(InferenceUnitState::Initialized);
  }
}

bool InferenceUnit::checkConnections() {
  // Check input map
  std::map<std::string, std::string>::iterator imIter; 
  for(imIter = this->mInputMap.begin();
      imIter != this->mInputMap.end();
      imIter++) {
    std::string inputName(imIter->first);
    std::string sourceURI(imIter->second);
    if(inputName.empty() || sourceURI.empty()) {
      return false;
    }
  }

  // Check output map
  if(this->mOutputListenerURIs.empty()) {
    return false;
  }

  return true;
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
