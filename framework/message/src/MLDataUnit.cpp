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

#include "MLDataUnit.h"
#include "cJSON.h"
#include "MessageUtil.h"
#include "ANTdbugLog.h"

#define RETURN_IF_INVALID_CJSON_OBJ(a, ret) \
  if((a) == NULL) { \
    ANT_DBG_ERR("JSON handling error: %s", cJSON_GetErrorPtr()); \
    return ret; \
  }

// decoding from JSON
bool MLDataUnit::setFromJSON(const char* jsonString) {
  // Parse rawString in JSON
  cJSON* thisObj = cJSON_Parse(jsonString);
  RETURN_IF_INVALID_CJSON_OBJ(thisObj, NULL);
  return this->setFromJSON(thisObj);
}

bool MLDataUnit::setFromJSON(cJSON* dataUnitObj) {
  cJSON* tensorMapObj = dataUnitObj;
  int tensorMapSize = cJSON_GetArraySize(tensorMapObj);

  for(int i = 0; i < tensorMapSize; i++) {
    cJSON* tensorMapObj = cJSON_GetArrayItem(tensorMapObj, i);

    // name
    cJSON* nameObj = cJSON_GetObjectItem(tensorMapObj, "name");
    RETURN_IF_INVALID_CJSON_OBJ(nameObj, false);
    std::string name(nameObj->valuestring);

    // tensor
    cJSON* tensorObj = cJSON_GetObjectItem(tensorMapObj, "tensor");
    RETURN_IF_INVALID_CJSON_OBJ(tensorObj, false);
    MLTensor* tensor = MLTensor::makeFromJSON(tensorObj);
    RETURN_IF_NULL(tensor, false);

    this->insertTensor(name, tensor);
  }

  return true;
}

// encoding to JSON
cJSON* MLDataUnit::toJSON() {
  cJSON* tensorMapObj = cJSON_CreateArray();

  std::map<std::string, MLTensor*>::iterator it;
  for(it = this->mTensorMap.begin();
      it != this->mTensorMap.end();
      it++) {
    std::string name(it->first);
    MLTensor* tensor = it->second;

    cJSON* entryObj = cJSON_CreateObject();

    // name 
    cJSON_AddStringToObject(entryObj, "name", name.c_str());

    // tensor
    cJSON_AddItemToObject(entryObj, "tensor", tensor->toJSON());

    cJSON_AddItemToArray(tensorMapObj, entryObj);
  }

  return tensorMapObj;
}

bool MLDataUnit::isMatched(MLDataUnitLayout* dataUnitLayout) {
  std::map<std::string, MLTensor*>::iterator it;
  for(it = this->mTensorMap.begin();
      it != this->mTensorMap.end();
      it++) {
    std::string tensorName = it->first;
    MLTensor* tensor = it->second;
    MLTensorLayout& tensorLayout = tensor->getLayout();

    MLTensorLayout* givenTensorLayout
      = dataUnitLayout->findTensorLayout(tensorName);

    if(givenTensorLayout == NULL
        || !givenTensorLayout->isMatched(&tensorLayout)) {
      return false;
    }
  }
  return true;
}
