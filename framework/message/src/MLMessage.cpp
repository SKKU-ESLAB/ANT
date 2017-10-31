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

#include <string>

#include "MLMessage.h"
#include "MessageUtil.h"
#include "cJSON.h"
#include "ANTdbugLog.h"

// encoding to JSON (MLMessage)
cJSON* MLMessage::toJSON() {
  cJSON* thisObj = cJSON_CreateObject();

  // commandType
  char commandTypeStr[20];
  sprintf(commandTypeStr, "%d", this->mCommandType);
  cJSON_AddStringToObject(thisObj, ML_MESSAGE_KEY_COMMAND_TYPE,
      commandTypeStr);

  // payload (MLMessage's)
  if(this->mMLPayloadObj != NULL) {
    cJSON_AddItemToObject(thisObj, ML_MESSAGE_KEY_PAYLOAD,
        this->mMLPayloadObj);
  }

  return thisObj;
}

// encoding to JSON (MLAckMessage)
cJSON* MLAckMessage::toJSON() {
  cJSON* thisObj = cJSON_CreateObject();

  // commandMessageId
  char commandMessageIdStr[20];
  sprintf(commandMessageIdStr, "%d", this->mCommandMessageId);
  cJSON_AddStringToObject(thisObj,
      ML_ACK_MESSAGE_KEY_COMMAND_MESSAGE_NUM,
      commandMessageIdStr);

  // commandType
  char commandTypeStr[20];
  sprintf(commandTypeStr, "%d", this->mCommandType);
  cJSON_AddStringToObject(thisObj, ML_ACK_MESSAGE_KEY_COMMAND_TYPE,
      commandTypeStr);

  // payload (MLAckMessage's)
  if(this->mMLAckPayloadObj != NULL) {
    cJSON_AddItemToObject(thisObj, ML_ACK_MESSAGE_KEY_PAYLOAD,
        this->mMLAckPayloadObj);
  }

  return thisObj;
}

// encoding to JSON (ParamIUList)
cJSON* ParamIUList::toJSON() {
  cJSON* listObj = cJSON_CreateArray();

  std::vector<ParamIUListEntry>::iterator iter;
  for(iter = this->mIUList.begin();
      iter != this->mIUList.end();
      ++iter) {
    cJSON* entryObj = cJSON_CreateObject();
    char tempStr[20];

    // iuid 
    sprintf(tempStr, "%d", iter->getIuid());
    cJSON_AddStringToObject(entryObj, "iuid", tempStr);

    // iuName
    std::string iuName(iter->getIuName());
    cJSON_AddStringToObject(entryObj, "iuName", iuName.c_str());

    // iuType
    sprintf(tempStr, "%d", iter->getIuType());
    cJSON_AddStringToObject(entryObj, "iuType", tempStr);

    cJSON_AddItemToArray(listObj, entryObj);
  }

  return listObj;
}

// decoding from JSON (ParamIUList)
bool ParamIUList::setFromJSON(cJSON* iuListObj) {
  int iuListSize = cJSON_GetArraySize(iuListObj);

  for(int i = 0; i < iuListSize; i++) {
    cJSON* iuListEntryObj = cJSON_GetArrayItem(iuListObj, i);

    // iuid
    cJSON* iuidObj = cJSON_GetObjectItem(iuListEntryObj, "iuid");
    RETURN_IF_INVALID_CJSON_OBJ(iuidObj, false);
    int iuid = atoi(iuidObj->valuestring);

    // iuName
    cJSON* iuNameObj = cJSON_GetObjectItem(iuListEntryObj, "iuName");
    RETURN_IF_INVALID_CJSON_OBJ(iuNameObj, false);
    std::string iuName(iuNameObj->valuestring);

    // iuType
    cJSON* iuTypeObj = cJSON_GetObjectItem(iuListEntryObj, "iuType");
    RETURN_IF_INVALID_CJSON_OBJ(iuTypeObj, false);
    int iuType = atoi(iuTypeObj->valuestring);
    
    ParamIUListEntry entry(iuid, iuName, iuType);
    this->mIUList.push_back(entry);
  }

  return true;
}

// Get command-specific parameters (MLMessage)
bool MLMessage::getParamsLoadIU(std::string& modelPackagePath,
    MLDataUnit& parameters) {
  cJSON* modelPackagePathObj = cJSON_GetObjectItem(this->mMLPayloadObj,
      "modelPackagePath");
  RETURN_IF_INVALID_CJSON_OBJ(modelPackagePathObj, false);
  modelPackagePath.assign(modelPackagePathObj->valuestring);

  cJSON* parametersObj = cJSON_GetObjectItem(this->mMLPayloadObj,
      "parametersObj");
  RETURN_IF_INVALID_CJSON_OBJ(parametersObj, false);
  bool settingRes = parameters.setFromJSON(parametersObj);
  if(!settingRes) {
    return false;
  }

  return true;
}

bool MLMessage::getParamsUnloadIU(int& iuid) {
  cJSON* iuidObj = cJSON_GetObjectItem(this->mMLPayloadObj, "iuid");
  RETURN_IF_INVALID_CJSON_OBJ(iuidObj, false);
  iuid = atoi(iuidObj->valuestring);

  return true;
}

bool MLMessage::getParamsSetIUInput(int& iuid, std::string& inputName,
    std::string& sourceUri) {
  cJSON* iuidObj = cJSON_GetObjectItem(this->mMLPayloadObj, "iuid");
  RETURN_IF_INVALID_CJSON_OBJ(iuidObj, false);
  iuid = atoi(iuidObj->valuestring);

  cJSON* inputNameObj = cJSON_GetObjectItem(this->mMLPayloadObj, "inputName");
  RETURN_IF_INVALID_CJSON_OBJ(inputNameObj, false);
  inputName.assign(inputNameObj->valuestring);

  cJSON* sourceUriObj = cJSON_GetObjectItem(this->mMLPayloadObj, "sourceUri");
  RETURN_IF_INVALID_CJSON_OBJ(sourceUriObj, false);
  sourceUri.assign(sourceUriObj->valuestring);

  return true;
}

bool MLMessage::getParamsStartListeningIUOutput(int& iuid,
    std::string& listenerUri) {
  cJSON* iuidObj = cJSON_GetObjectItem(this->mMLPayloadObj, "iuid");
  RETURN_IF_INVALID_CJSON_OBJ(iuidObj, false);
  iuid = atoi(iuidObj->valuestring);

  cJSON* listenerUriObj = cJSON_GetObjectItem(this->mMLPayloadObj,
      "listenerUri");
  RETURN_IF_INVALID_CJSON_OBJ(listenerUriObj, false);
  listenerUri.assign(listenerUriObj->valuestring);

  return true;
}

bool MLMessage::getParamsStopListeningIUOutput(int& iuid,
    std::string& listenerUri) {
  cJSON* iuidObj = cJSON_GetObjectItem(this->mMLPayloadObj, "iuid");
  RETURN_IF_INVALID_CJSON_OBJ(iuidObj, false);
  iuid = atoi(iuidObj->valuestring);

  cJSON* listenerUriObj = cJSON_GetObjectItem(this->mMLPayloadObj,
      "listenerUri");
  RETURN_IF_INVALID_CJSON_OBJ(listenerUriObj, false);
  listenerUri.assign(listenerUriObj->valuestring);

  return true;
}

bool MLMessage::getParamsStartIU(int& iuid) {
  cJSON* iuidObj = cJSON_GetObjectItem(this->mMLPayloadObj, "iuid");
  RETURN_IF_INVALID_CJSON_OBJ(iuidObj, false);
  iuid = atoi(iuidObj->valuestring);

  return true;
}

bool MLMessage::getParamsStopIU(int& iuid) {
  cJSON* iuidObj = cJSON_GetObjectItem(this->mMLPayloadObj, "iuid");
  RETURN_IF_INVALID_CJSON_OBJ(iuidObj, false);
  iuid = atoi(iuidObj->valuestring);

  return true;
}

bool MLMessage::getParamsGetIUResourceUsage(int& iuid) {
  cJSON* iuidObj = cJSON_GetObjectItem(this->mMLPayloadObj, "iuid");
  RETURN_IF_INVALID_CJSON_OBJ(iuidObj, false);
  iuid = atoi(iuidObj->valuestring);

  return true;
}

bool MLMessage::getParamsRunModel(std::string& modelName) {
  cJSON* modelNameObj = cJSON_GetObjectItem(this->mMLPayloadObj,
      "modelName");
  RETURN_IF_INVALID_CJSON_OBJ(modelNameObj, false);
  modelName.assign(modelNameObj->valuestring);

  return true;
}

// Set command-specific parameters (MLMessage)
void MLMessage::setParamsLoadIU(std::string modelPackagePath,
    MLDataUnit* parameters) {
  cJSON* payloadObj = cJSON_CreateObject();
  cJSON_AddStringToObject(payloadObj, "modelPackagePath",
      modelPackagePath.c_str());
  cJSON_AddItemToObject(payloadObj, "parameters", parameters->toJSON());
  
  this->mMLPayloadObj = payloadObj;
}

void MLMessage::setParamsUnloadIU(int iuid) {
  cJSON* payloadObj = cJSON_CreateObject();
  char iuidStr[20];
  sprintf(iuidStr, "%d", iuid);
  cJSON_AddStringToObject(payloadObj, "iuid", iuidStr);

  this->mMLPayloadObj = payloadObj;
}

void MLMessage::setParamsSetIUInput(int iuid, std::string inputName,
    std::string sourceUri) {
  cJSON* payloadObj = cJSON_CreateObject();
  char iuidStr[20];
  sprintf(iuidStr, "%d", iuid);
  cJSON_AddStringToObject(payloadObj, "iuid", iuidStr);
  cJSON_AddStringToObject(payloadObj, "inputName", inputName.c_str());
  cJSON_AddStringToObject(payloadObj, "sourceUri", sourceUri.c_str());

  this->mMLPayloadObj = payloadObj;
}

void MLMessage::setParamsStartListeningIUOutput(int iuid,
    std::string listenerUri) {
  cJSON* payloadObj = cJSON_CreateObject();
  char iuidStr[20];
  sprintf(iuidStr, "%d", iuid);
  cJSON_AddStringToObject(payloadObj, "iuid", iuidStr);
  cJSON_AddStringToObject(payloadObj, "listenerUri", listenerUri.c_str());

  this->mMLPayloadObj = payloadObj;
}

void MLMessage::setParamsStopListeningIUOutput(int iuid,
    std::string listenerUri) {
  cJSON* payloadObj = cJSON_CreateObject();
  char iuidStr[20];
  sprintf(iuidStr, "%d", iuid);
  cJSON_AddStringToObject(payloadObj, "iuid", iuidStr);
  cJSON_AddStringToObject(payloadObj, "listenerUri", listenerUri.c_str());

  this->mMLPayloadObj = payloadObj;
}

void MLMessage::setParamsStartIU(int iuid) {
  cJSON* payloadObj = cJSON_CreateObject();
  char iuidStr[20];
  sprintf(iuidStr, "%d", iuid);
  cJSON_AddStringToObject(payloadObj, "iuid", iuidStr);

  this->mMLPayloadObj = payloadObj;
}

void MLMessage::setParamsStopIU(int iuid) {
  cJSON* payloadObj = cJSON_CreateObject();
  char iuidStr[20];
  sprintf(iuidStr, "%d", iuid);
  cJSON_AddStringToObject(payloadObj, "iuid", iuidStr);

  this->mMLPayloadObj = payloadObj;
}

void MLMessage::setParamsGetIUResourceUsage(int iuid) {
  cJSON* payloadObj = cJSON_CreateObject();
  char iuidStr[20];
  sprintf(iuidStr, "%d", iuid);
  cJSON_AddStringToObject(payloadObj, "iuid", iuidStr);

  this->mMLPayloadObj = payloadObj;
}

void MLMessage::setParamsRunModel(std::string& modelName) {
  cJSON* payloadObj = cJSON_CreateObject();
  cJSON_AddStringToObject(payloadObj, "modelName", modelName.c_str());

  this->mMLPayloadObj = payloadObj;
}

// Get command-specific parameters (MLAckMessage)
bool MLAckMessage::getParamsLoadIU(int& iuid) {
  cJSON* iuidObj = cJSON_GetObjectItem(this->mMLAckPayloadObj, "iuid");
  RETURN_IF_INVALID_CJSON_OBJ(iuidObj, false);
  iuid = atoi(iuidObj->valuestring);

  return true;
}

bool MLAckMessage::getParamsGetIUs(ParamIUList& iuList) {
  cJSON* iuListObj = cJSON_GetObjectItem(this->mMLAckPayloadObj, "iuList");
  RETURN_IF_INVALID_CJSON_OBJ(iuListObj, false);
  iuList.setFromJSON(iuListObj);

  return true;
}

bool MLAckMessage::getParamsStartListeningIUOutput(MLDataUnit& outputData) {
  cJSON* outputDataObj = cJSON_GetObjectItem(this->mMLAckPayloadObj,
      "outputData");
  RETURN_IF_INVALID_CJSON_OBJ(outputDataObj, false);
  outputData.setFromJSON(outputDataObj);
  
  return true;
}

bool MLAckMessage::getParamsGetIUResourceUsage(std::string& data) {
  cJSON* dataObj = cJSON_GetObjectItem(this->mMLAckPayloadObj, "data");
  RETURN_IF_INVALID_CJSON_OBJ(dataObj, false);
  data.assign(dataObj->valuestring);

  return true;
}

bool MLAckMessage::getParamsRunModel(std::string outputData) {
  cJSON* outputDataObj = cJSON_GetObjectItem(this->mMLAckPayloadObj, "outputData");
  RETURN_IF_INVALID_CJSON_OBJ(outputDataObj, false);
  outputData.assign(outputDataObj->valuestring);

  return true;
}

// Set command-specific parameters (MLAckMessage)
void MLAckMessage::setParamsLoadIU(int iuid) {
  cJSON* payloadObj = cJSON_CreateObject();
  char iuidStr[20];
  sprintf(iuidStr, "%d", iuid);
  cJSON_AddStringToObject(payloadObj, "iuid", iuidStr);

  this->mMLAckPayloadObj = payloadObj;
}

void MLAckMessage::setParamsGetIUs(ParamIUList* iuList) {
  cJSON* payloadObj = cJSON_CreateObject();
  cJSON_AddItemToObject(payloadObj, "iuList", iuList->toJSON());

  this->mMLAckPayloadObj = payloadObj;
}

void MLAckMessage::setParamsStartListeningIUOutput(MLDataUnit* outputData) {
  cJSON* payloadObj = cJSON_CreateObject();
  cJSON_AddItemToObject(payloadObj, "outputData", outputData->toJSON());

  this->mMLAckPayloadObj = payloadObj;
}

void MLAckMessage::setParamsGetIUResourceUsage(std::string data) {
  cJSON* payloadObj = cJSON_CreateObject();
  cJSON_AddStringToObject(payloadObj, "data", data.c_str());

  this->mMLAckPayloadObj = payloadObj;
}

void MLAckMessage::setParamsRunModel(std::string outputData) {
  cJSON* payloadObj = cJSON_CreateObject();
  cJSON_AddStringToObject(payloadObj, "outputData", outputData.c_str());

  this->mMLAckPayloadObj = payloadObj;
}
