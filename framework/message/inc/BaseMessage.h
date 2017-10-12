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

#ifndef __BASE_MESSAGE_H__
#define __BASE_MESSAGE_H__

#include <iostream>
#include <string>
#include <vector>

#include "cJSON.h"

// {
//   // BaseMessage
//   payload: {
//     // BaseMessagePayload (AppCore, AppCoreAck, App, AppAck, Companion, etc.)
//     params: {
//       // **Params
//     }
//   }
// }

class MessageFactory;

class BaseMessagePayload {
  public:
    // encoding to JSON
    virtual cJSON* toJSON() = 0;
};

// Use namespace + enum for readability
namespace BaseMessageType {
  enum Value {
    NotDetermined = 0,
    AppCore = 10,
    AppCoreAck = 11,
    App = 20,
    AppAck = 21,
    Companion = 30,
    ML = 40,
    MLAck = 41
  };
}

#define BASE_MESSAGE_KEY_MESSAGE_NUM "messageId"
#define BASE_MESSAGE_KEY_SENDER_URI "senderUri"
#define BASE_MESSAGE_KEY_URI "uri"
#define BASE_MESSAGE_KEY_TYPE "type"
#define BASE_MESSAGE_KEY_IS_FILE_ATTACHED "isFileAttached"
#define BASE_MESSAGE_KEY_FILE_NAME "fileName"
#define BASE_MESSAGE_KEY_PAYLOAD "payload"

// BaseMessage: the root base message
// - Decoding(makeFromJSON): C++, Java
// - Encoding(make, toJSON): C++, Java
class BaseMessage {
  public:
    friend class MessageFactory;

    // File attachment
    // Attached file will be transferred to target or be received from target
    // in each Channel.
    // Some Channel can refuse to transfer it due to its capability.
    // ex. CommChannel and LocalChnanel can handle attached file, but
    //     DbusChannel cannot.
    
    // Attach file on message to be sent
    BaseMessage* attachFile(std::string filePath);

    // Set local file path when attached file has came
    BaseMessage* setStoredFilePath(std::string storedFilePath) {
      this->mStoredFilePath = storedFilePath;
      return this;
    }
    std::string& getStoredFilePath() {
      return this->mStoredFilePath;
    }

    // encoding to JSON
    cJSON* toJSON();
    char* toJSONString() {
      cJSON* jsonObj = this->toJSON();
      return cJSON_Print(jsonObj);
    }

    // Get parameters
    int getMessageId() { return this->mMessageId; }
    std::string& getSenderUri() { return this->mSenderUri; }
    std::string& getUri() { return this->mUri; }
    BaseMessageType::Value getType() { return this->mType; }
    bool isFileAttached() { return this->mIsFileAttached; }
    std::string& getFileName() { return this->mFileName; }

    // Payload
    void setPayload(BaseMessagePayload* payload) { this->mPayload = payload; }
    BaseMessagePayload* getPayload() { return this->mPayload; }

  protected:
    BaseMessage(int messageId, std::string senderUri, std::string uri,
        BaseMessageType::Value type,
        bool isFileAttached, std::string fileName)
      : mMessageId(messageId), mSenderUri(senderUri), mUri(uri), mType(type),
      mIsFileAttached(isFileAttached), mFileName(fileName),
      mStoredFilePath("") {
    }
    BaseMessage(int messageId, std::string senderUri, std::string uri,
        BaseMessageType::Value type)
      : mMessageId(messageId), mSenderUri(senderUri), mUri(uri), mType(type),
      mIsFileAttached(false), mFileName(""),
      mStoredFilePath("") {
    }

    // JSON-exported values
    int mMessageId;
    std::string mSenderUri;
    std::string mUri;
    BaseMessageType::Value mType;
    bool mIsFileAttached;
    std::string mFileName;
    BaseMessagePayload* mPayload;

    // Internal value
    // StoreFilePath: the local storage path of attached file
    std::string mStoredFilePath;
};

// Use namespace + enum for readability
namespace AppCoreMessageCommandType {
  enum Value {
    NotDetermined = 0,
    GetAppList = 1, // params: void (ACK params: ParamAppList)
    ListenAppState = 2, // params: int appId
                        // (ACK params: int appId, int appState)
    InitializeApp = 3, // params: void (ACK params: int appId)
    InstallApp = 4, // params: int appId, std::string packageFileName
    LaunchApp = 5, // params: int appId
    CompleteLaunchingApp = 6, // params: int pid
                              // (ACK params: int appId)
    TerminateApp = 7, // params: int appId
    RemoveApp = 8, // params: int appId
    GetFileList = 9, // params: std::string path (ACK params: AckParamFileList)
    GetFile = 10, // params: std::string path (ACK params: void)
    GetRootPath = 11, // params: void (ACK params: std::string rootPath)
    GetAppIcon = 12 // params: int appId (ACK params: void)
  };
}

#define APPCORE_MESSAGE_KEY_COMMAND_TYPE "commandType"
#define APPCORE_MESSAGE_KEY_PAYLOAD "payload"

// AppCoreMessage: message sent to AppCore Framework
// - Decoding(makeFromJSON): C++
// - Encoding(make, toJSON): Java
class AppCoreMessage: public BaseMessagePayload {
  public:
    friend class MessageFactory;

    ~AppCoreMessage() {
      if(this->mAppCorePayloadObj != NULL) {
        cJSON_Delete(this->mAppCorePayloadObj);
      }
    }

    // encoding to JSON
    virtual cJSON* toJSON();

    // Get parameters
    AppCoreMessageCommandType::Value getCommandType() {
      return this->mCommandType;
    }
    cJSON* getAppCorePayloadObj() { return this->mAppCorePayloadObj; }

    // Set command-specific parameters
    void setParamsCompleteLaunchingApp(int pid);
    void setAppCorePayloadObj(cJSON* appCorePayloadObj) {
      this->mAppCorePayloadObj = appCorePayloadObj;
    }

    // Get command-specific parameters
    bool getParamsListenAppState(int& appId);
    bool getParamsInstallApp(int& appId, std::string& packgeFileName);
    bool getParamsLaunchApp(int& appId);
    bool getParamsCompleteLaunchingApp(int& pid);
    bool getParamsTerminateApp(int& appId);
    bool getParamsRemoveApp(int& appId);
    bool getParamsGetFileList(std::string& path);
    bool getParamsGetFile(std::string& path);
    bool getParamsGetAppIcon(int& appId);

  protected:
    // Initializer
    AppCoreMessage(AppCoreMessageCommandType::Value commandType)
      : mCommandType(commandType), mAppCorePayloadObj(NULL) {
    }

    // JSON-exported values
    AppCoreMessageCommandType::Value mCommandType;
    cJSON* mAppCorePayloadObj;
};

class ParamAppListEntry {
  public:
    ParamAppListEntry(int appId, std::string appName, bool isDefaultApp)
      : mAppId(appId), mAppName(appName), mIsDefaultApp(isDefaultApp) { }

    int getAppId() { return this->mAppId; }
    std::string getAppName() { return this->mAppName; }
    bool isDefaultApp() { return this->mIsDefaultApp; }
  private:
    int mAppId;
    std::string mAppName;
    bool mIsDefaultApp;
};

class ParamAppList {
  public:
    // Make
    static ParamAppList* make() {
      return new ParamAppList();
    }

    void addEntry(int appId, std::string appName, bool isDefaultApp) {
      ParamAppListEntry newEntry(appId, appName, isDefaultApp);
      this->mAppList.push_back(newEntry);
    }
    
    // encoding to JSON
    cJSON* toJSON();

    std::vector<ParamAppListEntry>& getList() { return this->mAppList; }

  private:
    ParamAppList() { }
    std::vector<ParamAppListEntry> mAppList;
};

namespace ParamFileListEntryType {
  enum Value {
    Directory = 1,
    File = 2,
    Others = 3
  };
}

class ParamFileListEntry {
  public:
    ParamFileListEntry(std::string fileName,
        ParamFileListEntryType::Value fileType,
        int fileSizeBytes, std::string fileTime)
      : mFileName(fileName), mFileType(fileType), mFileSizeBytes(fileSizeBytes),
      mFileTime(fileTime) { }

    std::string getFileName() { return this->mFileName; }
    ParamFileListEntryType::Value getFileType() { return this->mFileType; }
    int getFileSizeBytes() { return this->mFileSizeBytes; }
    std::string getFileTime() { return this->mFileTime; }

  private:
    std::string mFileName;
    ParamFileListEntryType::Value mFileType;
    int mFileSizeBytes;
    std::string mFileTime;
};

class ParamFileList {
  public:
    // Make
    static ParamFileList* make() {
      return new ParamFileList();
    }

    void addEntry(std::string fileName, ParamFileListEntryType::Value fileType,
        int fileSizeBytes, std::string fileTime) {
      ParamFileListEntry newEntry(fileName, fileType, fileSizeBytes, fileTime);
      this->mFileList.push_back(newEntry);
    }
    
    // encoding to JSON
    cJSON* toJSON();

    std::vector<ParamFileListEntry>& getList() { return this->mFileList; }

  private:
    ParamFileList() { }
    std::vector<ParamFileListEntry> mFileList;
};

#define APPCORE_ACK_MESSAGE_KEY_COMMAND_MESSAGE_NUM "commandMessageId"
#define APPCORE_ACK_MESSAGE_KEY_COMMAND_TYPE "commandType"
#define APPCORE_ACK_MESSAGE_KEY_PAYLOAD "payload"

// AppCoreAckMessage: ack message sent from AppCore Framework
// - Decoding(makeFromJSON): Java
// - Encoding(make, toJSON): C++
class AppCoreAckMessage : public BaseMessagePayload {
  public:
    friend class MessageFactory;

    ~AppCoreAckMessage() {
      if(this->mAppCoreAckPayloadObj != NULL) {
        cJSON_Delete(this->mAppCoreAckPayloadObj);
      }
    }

    // encoding to JSON
    virtual cJSON* toJSON();

    // Get parameters
    AppCoreMessageCommandType::Value getCommandType() {
      return this->mCommandType;
    }

    // Set command-specific parameters
    void setParamsGetAppList(ParamAppList* appList);
    void setParamsListenAppState(int appId, int appState);
    void setParamsInitializeApp(int appId);
    void setParamsCompleteLaunchingApp(int appId);
    void setParamsGetRootPath(std::string rootPath);
    void setParamsGetFileList(std::string path, ParamFileList* fileList);

    void setAppCoreAckPayloadObj(cJSON* appCoreAckPayloadObj) {
      this->mAppCoreAckPayloadObj = appCoreAckPayloadObj;
    }

    // Get command-specific parameters
    bool getParamsCompleteLaunchingApp(int& appId);

  protected:
    AppCoreAckMessage(int commandMessageId,
        AppCoreMessageCommandType::Value commandType)
      : mCommandMessageId(commandMessageId),
      mCommandType(commandType), mAppCoreAckPayloadObj(NULL) { }

    // JSON-exported values
    int mCommandMessageId;
    AppCoreMessageCommandType::Value mCommandType;
    cJSON* mAppCoreAckPayloadObj;
};

// Use namespace + enum for readability
namespace AppMessageCommandType {
  enum Value {
    NotDetermined = 0,
    Terminate = 1, // params: void
    UpdateAppConfig = 2, // params: string legacyData
                         // (ACK params: bool isSucceed)
  };
}

#define APP_MESSAGE_KEY_COMMAND_TYPE "commandType"
#define APP_MESSAGE_KEY_PAYLOAD "payload"

// AppMessage: message sent to App
// - Decoding(makeFromJSON): C++
// - Encoding(make, toJSON): C++, Java
class AppMessage: public BaseMessagePayload {
  public:
    friend class MessageFactory;

    ~AppMessage() {
      if(this->mAppPayloadObj != NULL) {
        cJSON_Delete(this->mAppPayloadObj);
      }
    }

    // encoding to JSON
    virtual cJSON* toJSON();

    // Get parameters
    AppMessageCommandType::Value getCommandType() { return this->mCommandType; }
    cJSON* getAppPayloadObj() { return this->mAppPayloadObj; }

    // Get command-specific parameters
    bool getParamsUpdateAppConfig(std::string& legacyData);

    // Set command-specific parameters
    void setAppPayloadObj(cJSON* appPayloadObj) {
      this->mAppPayloadObj = appPayloadObj;
    }

  protected:
    // Initializer
    AppMessage(AppMessageCommandType::Value commandType)
      : mCommandType(commandType), mAppPayloadObj(NULL) {
    }

    // JSON-exported values
    AppMessageCommandType::Value mCommandType;
    cJSON* mAppPayloadObj;
};

#define APP_ACK_MESSAGE_KEY_COMMAND_MESSAGE_NUM "commandMessageId"
#define APP_ACK_MESSAGE_KEY_COMMAND_TYPE "commandType"
#define APP_ACK_MESSAGE_KEY_PAYLOAD "payload"

// AppAckMessage: ack message sent from App
// - Decoding(makeFromJSON): Java
// - Encoding(make, toJSON): C++
class AppAckMessage : public BaseMessagePayload {
  public:
    friend class MessageFactory;

    ~AppAckMessage() {
      if(this->mAppAckPayloadObj != NULL) {
        cJSON_Delete(this->mAppAckPayloadObj);
      }
    }

    // encoding to JSON
    virtual cJSON* toJSON();

    // Get parameters
    AppMessageCommandType::Value getCommandType() {
      return this->mCommandType;
    }

    // Set command-specific parameters
    void setParamsUpdateAppConfig(bool isSucceed);

    void setAppAckPayloadObj(cJSON* appAckPayloadObj) {
      this->mAppAckPayloadObj = appAckPayloadObj;
    }

  protected:
    AppAckMessage(int commandMessageId,
        AppMessageCommandType::Value commandType)
      : mCommandMessageId(commandMessageId),
      mCommandType(commandType), mAppAckPayloadObj(NULL) { }

    // JSON-exported values
    int mCommandMessageId;
    AppMessageCommandType::Value mCommandType;
    cJSON* mAppAckPayloadObj;
};

namespace CompanionMessageCommandType {
  enum Value {
    NotDetermined = 0,
    SendEventPage = 1, // params: int appId, string legacyData, boolean isNoti
    SendConfigPage = 2, // params: int appId, string legacyData
    UpdateSensorData = 3 // params: string legacyData
  };
}

#define COMPANION_MESSAGE_KEY_COMMAND_TYPE "commandType"
#define COMPANION_MESSAGE_KEY_PAYLOAD "payload"

// CompanionMessage: message sent to companion device
// - Decoding(makeFromJSON): Java
// - Encoding(make, toJSON): JavaScript
class CompanionMessage: public BaseMessagePayload {
  public:
    friend class MessageFactory;

    ~CompanionMessage() {
      if(this->mCompanionPayloadObj != NULL) {
        cJSON_Delete(this->mCompanionPayloadObj);
      }
    }

    // encoding to JSON
    virtual cJSON* toJSON();

    // Get parameters
    CompanionMessageCommandType::Value getCommandType() {
      return this->mCommandType;
    }
    cJSON* getCompanionPayloadObj() { return this->mCompanionPayloadObj; }

    // Set command-specific parameters
    void setParamsSendEventPage(int appId, std::string legacyData, bool isNoti);
    void setParamsSendConfigPage(int appId, std::string legacyData);
    void setParamsUpdateSensorData(std::string legacyData);

    void setCompanionPayloadObj(cJSON* companionPayloadObj) {
      this->mCompanionPayloadObj = companionPayloadObj;
    }

  protected:
    // Initializer
    CompanionMessage(CompanionMessageCommandType::Value commandType)
      : mCommandType(commandType), mCompanionPayloadObj(NULL) {
    }

    // JSON-exported values
    CompanionMessageCommandType::Value mCommandType;
    cJSON* mCompanionPayloadObj;
};

#endif // !defined(__BASE_MESSAGE_H__)
