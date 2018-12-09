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

// Attach file on message to be sent
BaseMessage* BaseMessage::attachFile(std::string filePath) {
  char filePathBuffer[EXT4_FILE_PATH_LENGTH];
  snprintf(filePathBuffer, EXT4_FILE_PATH_LENGTH, "%s", filePath.c_str());
  std::string fileName(basename(filePathBuffer));

  this->mIsFileAttached = true;
  this->mFileName = fileName;
  return setStoredFilePath(filePath);
}