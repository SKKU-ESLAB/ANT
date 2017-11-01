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

#include "InputReaderSet.h"
#include "ANTdbugLog.h"
#include "SensorInputReader.h"
#include "CameraInputReader.h"

#define SENSOR_INPUT_URI "/thing/sensor"
#define CAMERA_INPUT_URI "/thing/camera"

InputReaderSet::InputReaderSet() {
  // TODO: resolve hard-coded input readers
  std::string kSensorInputURI(SENSOR_INPUT_URI);
  std::string kCameraInputURI(CAMERA_INPUT_URI);
  this->mInputReaders.insert(std::pair<std::string, InputReader*>(
        kSensorInputURI, (InputReader*)new SensorInputReader()));
  this->mInputReaders.insert(std::pair<std::string, InputReader*>(
        kCameraInputURI, (InputReader*)new CameraInputReader()));
}

MLTensor* InputReaderSet::read(std::string sourceUri) {
  InputReader* inputReader = this->findBestInputReader(sourceUri);
  if(inputReader == NULL) {
    ANT_DBG_WARN("Cannot find input reader for URI %s", sourceUri.c_str());
    return NULL;
  }

  MLTensor* inputTensor = inputReader->read(sourceUri);
  return inputTensor;
}

InputReader* InputReaderSet::findBestInputReader(std::string uriString) {
  std::string givenURI(uriString);

  // Find all the target entry of given URI 
  InputReader* targetInputReader = NULL;
  std::string targetUriString("");
  std::map<std::string, InputReader*>::iterator irIter;
  for(irIter = this->mInputReaders.begin();
      irIter != this->mInputReaders.end();
      ++irIter) {
    std::string entryUri(irIter->first);
    InputReader* entryInputReader = irIter->second;

    size_t foundPos = givenURI.find(entryUri);
    // Select the best matching target
    // At least, the pattern should be matched from the beginning of URI
    if(foundPos == 0) {
      // If target is not determined, keep it as a target.
      // If the matching length of this entry is longer than present target,
      // change the target.
      if((targetUriString.empty())
          || (entryUri.size() < targetUriString.size())) {
        targetInputReader = entryInputReader;
        targetUriString.assign(entryUri);
      }
    }
  }
  return targetInputReader;
}
