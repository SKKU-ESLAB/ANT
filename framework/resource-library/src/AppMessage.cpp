/* Copyright (c) 2017-2018 SKKU ESLAB, and contributors. All rights reserved.
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

#include "AppMessage.h"
#include "MessageUtil.h"
#include "cJSON.h"
#include "ANTdbugLog.h"

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