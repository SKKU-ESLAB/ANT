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

#include "CompanionMessage.h"
#include "MessageUtil.h"
#include "cJSON.h"
#include "ANTdbugLog.h"

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

// Set command-specific parameters (CompanionMessage)
void CompanionMessage::setParamsSendToCompanion(std::string listenerName,
    std::string data) {
  cJSON* payloadObj = cJSON_CreateObject();
  cJSON_AddStringToObject(payloadObj, "listenerName", listenerName.c_str());
  cJSON_AddStringToObject(payloadObj, "data", data.c_str());

  this->mCompanionPayloadObj = payloadObj;
}