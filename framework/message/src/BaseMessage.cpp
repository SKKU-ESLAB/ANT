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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>

#include "BaseMessage.h"
#include "MessageUtil.h"
#include "cJSON.h"
#include "ANTdbugLog.h"

// encoding to JSON (BaseMessage)
cJSON* BaseMessage::toJSON() {
  cJSON* thisObj = cJSON_CreateObject();
  char tempStr[20];

  // messageId
  sprintf(tempStr, "%d", this->mMessageId);
  cJSON_AddStringToObject(thisObj, BASE_MESSAGE_KEY_MESSAGE_NUM, tempStr);

  // senderUri
  cJSON_AddStringToObject(thisObj, BASE_MESSAGE_KEY_SENDER_URI,
      this->mSenderUri.c_str());

  // uri
  cJSON_AddStringToObject(thisObj, BASE_MESSAGE_KEY_URI, this->mUri.c_str());

  // type
  sprintf(tempStr, "%d", this->mType);
  cJSON_AddStringToObject(thisObj, BASE_MESSAGE_KEY_TYPE, tempStr);

  // isFileAttached
  sprintf(tempStr, "%d", (this->mIsFileAttached) ? 1 : 0);
  cJSON_AddStringToObject(thisObj, BASE_MESSAGE_KEY_IS_FILE_ATTACHED, tempStr);

  // fileName
  if(this->mIsFileAttached) {
    cJSON_AddStringToObject(thisObj, BASE_MESSAGE_KEY_FILE_NAME,
        this->mFileName.c_str());
  }

  // payload
  cJSON_AddItemToObject(thisObj, BASE_MESSAGE_KEY_PAYLOAD,
      this->mPayload->toJSON());
  return thisObj;
}

// encoding to JSON (AppCoreMessage)
cJSON* AppCoreMessage::toJSON() {
  cJSON* thisObj = cJSON_CreateObject();

  // commandType
  char commandTypeStr[20];
  sprintf(commandTypeStr, "%d", this->mCommandType);
  cJSON_AddStringToObject(thisObj, APPCORE_MESSAGE_KEY_COMMAND_TYPE,
      commandTypeStr);

  // payload (AppCoreMessage's)
  if(this->mAppCorePayloadObj != NULL) {
    cJSON_AddItemToObject(thisObj, APPCORE_MESSAGE_KEY_PAYLOAD,
      this->mAppCorePayloadObj);
  }

  return thisObj;
}


// encoding to JSON (AppCoreAckMessage)
cJSON* AppCoreAckMessage::toJSON() {
  cJSON* thisObj = cJSON_CreateObject();

  // commandMessageId
  char commandMessageIdStr[20];
  sprintf(commandMessageIdStr, "%d", this->mCommandMessageId);
  cJSON_AddStringToObject(thisObj,
      APPCORE_ACK_MESSAGE_KEY_COMMAND_MESSAGE_NUM,
      commandMessageIdStr);

  // commandType
  char commandTypeStr[20];
  sprintf(commandTypeStr, "%d", this->mCommandType);
  cJSON_AddStringToObject(thisObj, APPCORE_ACK_MESSAGE_KEY_COMMAND_TYPE,
      commandTypeStr);

  // payload (AppCoreAckMessage's)
  if(this->mAppCoreAckPayloadObj != NULL) {
    cJSON_AddItemToObject(thisObj, APPCORE_ACK_MESSAGE_KEY_PAYLOAD,
        this->mAppCoreAckPayloadObj);
  }

  return thisObj;
}

// encoding to JSON (AppMessage)
cJSON* AppMessage::toJSON() {
  cJSON* thisObj = cJSON_CreateObject();

  // commandType
  char commandTypeStr[20];
  sprintf(commandTypeStr, "%d", this->mCommandType);
  cJSON_AddStringToObject(thisObj, APP_MESSAGE_KEY_COMMAND_TYPE,
      commandTypeStr);

  // payload (AppMessage's)
  if(this->mAppPayloadObj != NULL) {
    cJSON_AddItemToObject(thisObj, APP_MESSAGE_KEY_PAYLOAD,
        this->mAppPayloadObj);
  }

  return thisObj;
}

// encoding to JSON (CompanionMessage)
cJSON* CompanionMessage::toJSON() {
  cJSON* thisObj = cJSON_CreateObject();

  // commandType
  char commandTypeStr[20];
  sprintf(commandTypeStr, "%d", this->mCommandType);
  cJSON_AddStringToObject(thisObj, COMPANION_MESSAGE_KEY_COMMAND_TYPE,
      commandTypeStr);

  // payload (CompanionMessage's)
  if(this->mCompanionPayloadObj != NULL) {
    cJSON_AddItemToObject(thisObj, COMPANION_MESSAGE_KEY_PAYLOAD,
        this->mCompanionPayloadObj);
  }

  return thisObj;
}

// encoding to JSON (AppAckMessage)
cJSON* AppAckMessage::toJSON() {
  cJSON* thisObj = cJSON_CreateObject();

  // commandMessageId
  char commandMessageIdStr[20];
  sprintf(commandMessageIdStr, "%d", this->mCommandMessageId);
  cJSON_AddStringToObject(thisObj,
      APP_ACK_MESSAGE_KEY_COMMAND_MESSAGE_NUM,
      commandMessageIdStr);

  // commandType
  char commandTypeStr[20];
  sprintf(commandTypeStr, "%d", this->mCommandType);
  cJSON_AddStringToObject(thisObj, APP_ACK_MESSAGE_KEY_COMMAND_TYPE,
      commandTypeStr);

  // payload (AppAckMessage's)
  if(this->mAppAckPayloadObj != NULL) {
    cJSON_AddItemToObject(thisObj, APP_ACK_MESSAGE_KEY_PAYLOAD,
        this->mAppAckPayloadObj);
  }

  return thisObj;
}

// Attach file on message to be sent
BaseMessage* BaseMessage::attachFile(std::string filePath) {
  char filePathBuffer[EXT4_FILE_PATH_LENGTH];
  snprintf(filePathBuffer, EXT4_FILE_PATH_LENGTH, "%s", filePath.c_str());
  std::string fileName(basename(filePathBuffer));

  this->mIsFileAttached = true;
  this->mFileName = fileName;
  return setStoredFilePath(filePath);
}

// Set command-specific parameters (AppCoreMessage)
void AppCoreMessage::setParamsCompleteLaunchingApp(int pid) {
  cJSON* payloadObj = cJSON_CreateObject();
  char pidStr[20];
  sprintf(pidStr, "%d", pid);
  cJSON_AddStringToObject(payloadObj, "pid", pidStr);

  this->mAppCorePayloadObj = payloadObj;
}

// Get command-specific parameters (AppCoreMessage)
bool AppCoreMessage::getParamsListenAppState(int& appId) { 
  cJSON* appIdObj = cJSON_GetObjectItem(this->mAppCorePayloadObj, "appId");
  RETURN_IF_INVALID_CJSON_OBJ(appIdObj, false);
  appId = atoi(appIdObj->valuestring);

  return true;
}

bool AppCoreMessage::getParamsInstallApp(int& appId,
    std::string& packageFileName) { 
  cJSON* appIdObj = cJSON_GetObjectItem(this->mAppCorePayloadObj, "appId");
  RETURN_IF_INVALID_CJSON_OBJ(appIdObj, false);
  appId = atoi(appIdObj->valuestring);

  cJSON* packageFileNameObj = cJSON_GetObjectItem(this->mAppCorePayloadObj,
      "packageFileName");
  RETURN_IF_INVALID_CJSON_OBJ(packageFileNameObj, false);
  packageFileName.assign(packageFileNameObj->valuestring);

  return true;
}

bool AppCoreMessage::getParamsLaunchApp(int& appId) { 
  cJSON* appIdObj = cJSON_GetObjectItem(this->mAppCorePayloadObj, "appId");
  RETURN_IF_INVALID_CJSON_OBJ(appIdObj, false);
  appId = atoi(appIdObj->valuestring);

  return true;
}

bool AppCoreMessage::getParamsCompleteLaunchingApp(int& pid) { 
  cJSON* pidObj = cJSON_GetObjectItem(this->mAppCorePayloadObj, "pid");
  RETURN_IF_INVALID_CJSON_OBJ(pidObj, false);
  pid = atoi(pidObj->valuestring);

  return true;
}

bool AppCoreMessage::getParamsTerminateApp(int& appId) { 
  cJSON* appIdObj = cJSON_GetObjectItem(this->mAppCorePayloadObj, "appId");
  RETURN_IF_INVALID_CJSON_OBJ(appIdObj, false);
  appId = atoi(appIdObj->valuestring);

  return true;
}

bool AppCoreMessage::getParamsRemoveApp(int& appId) { 
  cJSON* appIdObj = cJSON_GetObjectItem(this->mAppCorePayloadObj, "appId");
  RETURN_IF_INVALID_CJSON_OBJ(appIdObj, false);
  appId = atoi(appIdObj->valuestring);

  return true;
}

bool AppCoreMessage::getParamsGetFileList(std::string& path) { 
  cJSON* pathObj = cJSON_GetObjectItem(this->mAppCorePayloadObj, "path");
  RETURN_IF_INVALID_CJSON_OBJ(pathObj, false);
  path.assign(pathObj->valuestring);

  return true;
}

bool AppCoreMessage::getParamsGetFile(std::string& path) { 
  cJSON* pathObj = cJSON_GetObjectItem(this->mAppCorePayloadObj, "path");
  RETURN_IF_INVALID_CJSON_OBJ(pathObj, false);
  path.assign(pathObj->valuestring);

  return true;
}

bool AppCoreMessage::getParamsGetAppIcon(int& appId) {
  cJSON* appIdObj = cJSON_GetObjectItem(this->mAppCorePayloadObj, "appId");
  RETURN_IF_INVALID_CJSON_OBJ(appIdObj, false);
  appId = atoi(appIdObj->valuestring);

  return true;
}

cJSON* ParamAppList::toJSON() {
  cJSON* listObj = cJSON_CreateArray();

  std::vector<ParamAppListEntry>::iterator iter;
  for(iter = this->mAppList.begin();
      iter != this->mAppList.end();
      ++iter) {
    cJSON* entryObj = cJSON_CreateObject();
    char tempStr[20];

    // appId
    sprintf(tempStr, "%d", iter->getAppId());
    cJSON_AddStringToObject(entryObj, "appId", tempStr);

    // appName
    std::string appName(iter->getAppName());
    cJSON_AddStringToObject(entryObj, "appName", appName.c_str());

    // isDefaultApp
    sprintf(tempStr, "%d", (iter->isDefaultApp()) ? 1 : 0);
    cJSON_AddStringToObject(entryObj, "isDefaultApp", tempStr);

    cJSON_AddItemToArray(listObj, entryObj);
  }

  return listObj;
}

// Set command-specific parameters (AppCoreAckMessage)
void AppCoreAckMessage::setParamsGetAppList(ParamAppList* appList) {
  cJSON* payloadObj = cJSON_CreateObject();
  cJSON_AddItemToObject(payloadObj, "appList", appList->toJSON());
  this->mAppCoreAckPayloadObj = payloadObj; 
}

void AppCoreAckMessage::setParamsListenAppState(int appId, int appState) {
  cJSON* payloadObj = cJSON_CreateObject();
  char appIdStr[20];
  sprintf(appIdStr, "%d", appId);
  cJSON_AddStringToObject(payloadObj, "appId", appIdStr);

  char appStateStr[20];
  sprintf(appStateStr, "%d", appState);
  cJSON_AddStringToObject(payloadObj, "appState", appStateStr);

  this->mAppCoreAckPayloadObj = payloadObj;
}

void AppCoreAckMessage::setParamsInitializeApp(int appId) {
  cJSON* payloadObj = cJSON_CreateObject();
  char appIdStr[20];
  sprintf(appIdStr, "%d", appId);
  cJSON_AddStringToObject(payloadObj, "appId", appIdStr);

  this->mAppCoreAckPayloadObj = payloadObj;
}

void AppCoreAckMessage::setParamsCompleteLaunchingApp(int appId) {
  cJSON* payloadObj = cJSON_CreateObject();
  char appIdStr[20];
  sprintf(appIdStr, "%d", appId);
  cJSON_AddStringToObject(payloadObj, "appId", appIdStr);

  this->mAppCoreAckPayloadObj = payloadObj;
}

// Get command-specific parameters (AppCoreAckMessage)
bool AppCoreAckMessage::getParamsCompleteLaunchingApp(int& appId) {
  cJSON* appIdObj = cJSON_GetObjectItem(this->mAppCoreAckPayloadObj, "appId");
  RETURN_IF_INVALID_CJSON_OBJ(appIdObj, false);
  appId = atoi(appIdObj->valuestring);

  return true;
}

cJSON* ParamFileList::toJSON() {
  cJSON* listObj = cJSON_CreateArray();

  std::vector<ParamFileListEntry>::iterator iter;
  for(iter = this->mFileList.begin();
      iter != this->mFileList.end();
      ++iter) {
    cJSON* entryObj = cJSON_CreateObject();
    char tempStr[20];

    // fileName
    std::string fileName(iter->getFileName());
    cJSON_AddStringToObject(entryObj, "fileName", fileName.c_str());

    // fileType
    sprintf(tempStr, "%d", iter->getFileType());
    cJSON_AddStringToObject(entryObj, "fileType", tempStr);

    // fileSizeBytes
    sprintf(tempStr, "%d", iter->getFileSizeBytes());
    cJSON_AddStringToObject(entryObj, "fileSizeBytes", tempStr);

    // fileTime
    std::string fileTime(iter->getFileTime());
    cJSON_AddStringToObject(entryObj, "fileTime", fileTime.c_str());

    cJSON_AddItemToArray(listObj, entryObj);
  }

  return listObj;
}

void AppCoreAckMessage::setParamsGetFileList(std::string path,
    ParamFileList* fileList) {
  cJSON* payloadObj = cJSON_CreateObject();
  cJSON_AddStringToObject(payloadObj, "path", path.c_str());
  cJSON_AddItemToObject(payloadObj, "fileList", fileList->toJSON());
  this->mAppCoreAckPayloadObj = payloadObj; 
}

void AppCoreAckMessage::setParamsGetRootPath(std::string rootPath) {
  cJSON* payloadObj = cJSON_CreateObject();
  cJSON_AddStringToObject(payloadObj, "rootPath", rootPath.c_str());
  this->mAppCoreAckPayloadObj = payloadObj; 
}

// Get command-specific parameters (AppMessage)
bool AppMessage::getParamsUpdateAppConfig(std::string& legacyData) {
  cJSON* legacyDataObj = cJSON_GetObjectItem(this->mAppPayloadObj,
      "legacyData");
  RETURN_IF_INVALID_CJSON_OBJ(legacyDataObj, false);
  legacyData.assign(legacyDataObj->valuestring);

  return true;
}

// Set command-specific parameters (AppAckMessage)
void AppAckMessage::setParamsUpdateAppConfig(bool isSucceed) {
  cJSON* payloadObj = cJSON_CreateObject();
  cJSON_AddStringToObject(payloadObj, "isSucceed", (isSucceed) ? "1" : "0");
  this->mAppAckPayloadObj = payloadObj; 
}

// Set command-specific parameters (CompanionMessage)
void CompanionMessage::setParamsSendEventPage(int appId, std::string legacyData,
    bool isNoti) {
  cJSON* payloadObj = cJSON_CreateObject();
  cJSON* legacyDataObj = cJSON_Parse(legacyData.c_str());
  if(legacyDataObj == NULL) {
    ANT_DBG_ERR("SendEventPage: cannot parse legacyData");
  }

  char tempStr[20];
  sprintf(tempStr, "%d", appId);
  cJSON_AddStringToObject(payloadObj, "appId", tempStr);
  cJSON_AddItemToObject(payloadObj, "legacyData", legacyDataObj);
  cJSON_AddStringToObject(payloadObj, "isNoti", (isNoti) ? "1" : "0");

  this->mCompanionPayloadObj = payloadObj;
}

void CompanionMessage::setParamsSendConfigPage(int appId,
    std::string legacyData) {
  cJSON* payloadObj = cJSON_CreateObject();
  cJSON* legacyDataObj = cJSON_Parse(legacyData.c_str());
  if(legacyDataObj == NULL) {
    ANT_DBG_ERR("SendConfigPage: cannot parse legacyData");
  }

  char tempStr[20];
  sprintf(tempStr, "%d", appId);
  cJSON_AddStringToObject(payloadObj, "appId", tempStr);
  cJSON_AddItemToObject(payloadObj, "legacyData", legacyDataObj);

  this->mCompanionPayloadObj = payloadObj;
}
void CompanionMessage::setParamsUpdateSensorData(std::string legacyData) {
  cJSON* payloadObj = cJSON_CreateObject();
  cJSON* legacyDataObj = cJSON_Parse(legacyData.c_str());
  if(legacyDataObj == NULL) {
    ANT_DBG_ERR("UpdateSensorData: cannot parse legacyData");
  }
  cJSON_AddItemToObject(payloadObj, "legacyData", legacyDataObj);

  this->mCompanionPayloadObj = payloadObj;
}
