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

#include "MLTensor.h"
#include "cJSON.h"
#include "MessageUtil.h"

// encoding to JSON
cJSON* MLTensorLayout::toJSON() {
  cJSON* tensorLayoutObj = cJSON_CreateObject();
  // rank
  char rankStr[20];
  sprintf(rankStr, "%d", this->mRank);
  cJSON_AddStringToObject(tensorLayoutObj, "rank", rankStr);

  // shape
  cJSON* shapeObj = cJSON_CreateArray();
  for(int i = 0; i < this->mRank; i++) {
    char shapeEntryStr[20];
    sprintf(shapeEntryStr, "%d", this->mShape[i]);
    cJSON* shapeEntryObj = cJSON_CreateString(shapeEntryStr);
    cJSON_AddItemToArray(shapeObj, shapeEntryObj);
  }

  // dataType
  char dataTypeStr[20];
  sprintf(dataTypeStr, "%d", this->mDataType);
  cJSON_AddStringToObject(tensorLayoutObj, "dataType", dataTypeStr);

  return tensorLayoutObj;
}

// decoding from JSON
MLTensorLayout* MLTensorLayout::makeFromJSON(cJSON* tensorLayoutObj) {
  // rank
  cJSON* rankObj = cJSON_GetObjectItem(tensorLayoutObj, "rank");
  RETURN_IF_INVALID_CJSON_OBJ(rankObj, NULL);
  int rank = atoi(rankObj->valuestring);

  // shape
  int* shape = new int[rank];
  cJSON* shapeObj = cJSON_GetObjectItem(tensorLayoutObj, "shape");
  RETURN_IF_INVALID_CJSON_OBJ(shapeObj, NULL);
  int shapeSize = cJSON_GetArraySize(shapeObj);
  for(int i = 0; i < shapeSize && i < rank; i++) {
    cJSON* shapeEntryObj = cJSON_GetArrayItem(shapeObj, i);
    int shapeEntry = atoi(shapeEntryObj->valuestring);
    shape[i] = shapeEntry;
  }

  // dataType
  cJSON* dataTypeObj = cJSON_GetObjectItem(tensorLayoutObj, "dataType");
  RETURN_IF_INVALID_CJSON_OBJ(dataTypeObj, NULL);
  MLDataType::Value dataType
    = static_cast<MLDataType::Value>(atoi(dataTypeObj->valuestring));

  MLTensorLayout* newTensorLayout = new MLTensorLayout(rank, shape, dataType);
  delete[] shape;
  return newTensorLayout;
}

#define WORD_SIZE (4)
#define DECODED_WORD_SIZE (WORD_SIZE / 2 + 1)

// encoding to JSON
cJSON* MLTensor::toJSON() {
  cJSON* tensorObj = cJSON_CreateObject();

  // layout
  cJSON_AddItemToObject(tensorObj, "layout", this->mLayout.toJSON());

  // buffer
  // Encoding tensor buffer to JSON string(hexa-decimal).
  // This part can spend much time when the size(shape) of tensor is too large
  std::string bufferString;
  for(int i = 0; i < this->mBufferSize; i += WORD_SIZE) {
    // One word(4 bytes) can be represented as 2 hexa-decimal characters.
    char decodedWord[DECODED_WORD_SIZE];
    unsigned int* word = (unsigned int*)((char*)(this->mBuffer) + i);
    snprintf(decodedWord, DECODED_WORD_SIZE, "%02x", word);
    bufferString.append(decodedWord);
  }
  cJSON* bufferObj = cJSON_CreateString(bufferString.c_str());
  cJSON_AddItemToObject(tensorObj, "buffer", bufferObj);

  return tensorObj;
}

// decoding from JSON
MLTensor* MLTensor::makeFromJSON(cJSON* tensorObj) {
  // layout
  cJSON* layoutObj = cJSON_GetObjectItem(tensorObj, "layout");
  MLTensorLayout* layout = MLTensorLayout::makeFromJSON(layoutObj);
  RETURN_IF_NULL(layout, NULL);

  MLTensor* newTensor = new MLTensor(*layout);

  // buffer
  // Decoding JSON string(hexa-decimal) to tensor buffer.
  // This part can spend much time when the size(shape) of tensor is too large
  cJSON* bufferObj = cJSON_GetObjectItem(tensorObj, "buffer");
  char* bufferString = bufferObj->valuestring;
  int bufferStringLength = strlen(bufferString);
  for(int i = 0; i < bufferStringLength; i += DECODED_WORD_SIZE) {
    unsigned int word;
    sscanf(&bufferString[i], "%02x", &word);
    memcpy((void*)((char*)(newTensor->mBuffer) + i),
        &word,
        sizeof(unsigned int));
  }

  return newTensor;
}

bool MLTensorLayout::isMatched(MLTensorLayout* tensorLayout) {
  if(tensorLayout->getRank() != this->getRank()) {
    return false;
  } else if(tensorLayout->getDataType() != this->getDataType()) {
    return false;
  } else {
    int* thisShape = this->getShape();
    int* givenShape = tensorLayout->getShape();
    for(int i = 0; i < this->mRank; i++) {
      if(thisShape[i] != givenShape[i]) {
        return false;
      }
    }
    return true;
  }
}

bool MLTensor::isMatched(MLTensorLayout& targetLayout) {
  MLTensorLayout& thisLayout = this->getLayout();
  return thisLayout.isMatched(&targetLayout);
}

bool MLTensor::isMatched(MLTensor* targetTensor) {
  MLTensorLayout& targetLayout = targetTensor->getLayout();
  return this->isMatched(targetLayout);
}
