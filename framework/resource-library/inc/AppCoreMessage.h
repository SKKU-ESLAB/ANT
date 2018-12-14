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

#ifndef __APP_CORE_MESSAGE_H__
#define __APP_CORE_MESSAGE_H__

#include <iostream>
#include <string>
#include <vector>

#include "cJSON.h"

#include "BaseMessage.h"

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

#endif // !defined(__APP_CORE_MESSAGE_H__)