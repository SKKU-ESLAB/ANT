/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
 *              Dongig Sin<dongig@skku.edu>
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

#include <iostream>
#include <vector>
#include <dirent.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

extern "C" {
#include "miniunz.h"
}

#include "AppCore.h"
#include "ANTdbugLog.h"
#include "AppCoreMessage.h"
#include "AppMessage.h"
#include "MessageFactory.h"
#include "AppList.h"

using namespace std;

bool AppCore::initializeDirs() {
  // Initialize user app's, system app's directory path
  struct stat st = {0};
  char* dirString;

  // Apps Dir
  dirString = getenv("ANT_APPS_DIR");
  if(dirString != NULL) {
    // User Apps Dir
    snprintf(this->mUserAppsDir, PATH_BUFFER_SIZE, "%s/%s",
        dirString, "user");
    if(stat(this->mUserAppsDir, &st) == -1) {
      mkdir(this->mUserAppsDir, 0755);
    }
    ANT_DBG_VERB("UserAppsDir=%s", this->mUserAppsDir);

    // System Apps Dir
    snprintf(this->mSystemAppsDir, PATH_BUFFER_SIZE, "%s/%s",
        dirString, "system");
    if(stat(this->mSystemAppsDir, &st) == -1) {
      mkdir(this->mSystemAppsDir, 0755);
    }
    ANT_DBG_VERB("SystemAppsDir=%s", this->mSystemAppsDir);

    // App List DB Dir
    snprintf(this->mAppListDBPath, PATH_BUFFER_SIZE, "%s/%s",
        dirString, "AppListDB.sqlite");
    ANT_DBG_VERB("AppListDB=%s", this->mAppListDBPath);
  } else {
    ANT_DBG_ERR("Cannot read ANT_APPS_DIR");
    return false;
  }

  // Data Dir
  dirString = getenv("ANT_DATA_DIR");
  if(dirString != NULL) {
    snprintf(this->mDataDir, PATH_BUFFER_SIZE, "%s", dirString);
    if(stat(this->mDataDir, &st) == -1) {
      mkdir(this->mDataDir, 0755);
    }
  } else {
    ANT_DBG_ERR("Cannot read ANT_DATA_DIR");
    return false;
  }
  ANT_DBG_VERB("DataDir=%s", this->mDataDir);

  // Temp Dir
  dirString = getenv("ANT_TEMP_DIR");
  if(dirString != NULL) {
    snprintf(this->mTempDir, PATH_BUFFER_SIZE, "%s", dirString);
    if(stat(this->mTempDir, &st) == -1) {
      mkdir(this->mTempDir, 0755);
    }
  } else {
    // TODO: For now, we use ${ANT_DATA_DIR}/temp as ${ANT_TEMP_DIR} as now.
    // In the future, we should make ${ANT_TEMP_DIR} a compulsory value.
    dirString = getenv("ANT_DATA_DIR");
    if(dirString != NULL) {
      snprintf(this->mTempDir, PATH_BUFFER_SIZE, "%s/temp", dirString);
      ANT_DBG_WARN("Cannot read ANT_TEMP_DIR, so ANT_TEMP_DIR is set as %s",
          this->mTempDir);
      if(stat(this->mTempDir, &st) == -1) {
        mkdir(this->mTempDir, 0755);
      }
    } else {
      ANT_DBG_ERR("Cannot read ANT_DATA_DIR, %s",
          "so we cannot determine ANT_TEMP_DIR");
      return false;
    }
  }
  ANT_DBG_VERB("TempDir=%s", this->mTempDir);
  return true;
}

#define COMPANION_DEVICE_URI "/comp0"
#define APPS_URI "/thing/apps"
#define APPCORE_URI "/thing/appcore"

// Main loop
void AppCore::run() {
  // Initialize directories
  if(this->initializeDirs() == false) {
    ANT_DBG_ERR("Cannot find at least one ANT environment variable.");
    return;
  }

  // Initialize AppList
  this->mAppList = AppList::initializeFromDB(
      this->mAppListDBPath, this->mSystemAppsDir);

  // Initialize MessageRouter and Channels
  this->mMessageRouter = new MessageRouter();
  this->mDbusChannel = new DbusChannel(this->mMessageRouter);
  this->mCommChannel = new CommChannel(this->mMessageRouter,
      this->getTempDir());
  this->mLocalChannel = new LocalChannel(this->mMessageRouter, APPCORE_URI,
      false);

  // Run DbusChannel and add it to MessageRouter's routing table
  this->mDbusChannel->run();
  this->mMessageRouter->addRoutingEntry(APPS_URI, this->mDbusChannel);

  // Run CommChannel
  // CommChannel will be added to routing table when a remote device is
  // connected
  this->mCommChannel->setStateListener(this);
  this->mCommChannel->run();

  // LocalChannel: run on main thread
  // Main loop starts to run in LocalChannel::run()
  this->mLocalChannel->addListener(this);
  this->mMessageRouter->addRoutingEntry(APPCORE_URI, this->mLocalChannel);
  this->mLocalChannel->run();
}

// Signal handler
void AppCore::onSignalSIGCHLD() {
  int status;
  int pid;

  pid = wait(&status);
  if (pid > 0) {
    // Handle 3rd-party App Termination Event on the main thread
    this->completeTerminatingApp(pid);
  }
}

// Appcore Commands
void AppCore::onReceivedMessage(BaseMessage* message) {
  if(message == NULL) {
    ANT_DBG_ERR("Invalid AppCore Message");
    return;
  }
  if(message->getType() != BaseMessageType::AppCore) {
    ANT_DBG_ERR("Not AppCore Message");
    return;
  }
  AppCoreMessage* payload = (AppCoreMessage*)message->getPayload();
  if(payload == NULL) {
    ANT_DBG_ERR("AppCoreMessage payload does not exist");
    return;
  }

  int commandType = payload->getCommandType();
  switch(commandType) {
    case AppCoreMessageCommandType::GetAppList:
      ANT_DBG_VERB("AppCore command: GetAppList (%d)", commandType);
      this->getAppList(message);
      break;
    case AppCoreMessageCommandType::ListenAppState:
      ANT_DBG_VERB("AppCore command: ListenAppState (%d)", commandType);
      this->listenAppState(message);
      break;
    case AppCoreMessageCommandType::InitializeApp:
      ANT_DBG_VERB("AppCore command: InitializeApp (%d)", commandType);
      this->initializeApp(message);
      break;
    case AppCoreMessageCommandType::InstallApp:
      ANT_DBG_VERB("AppCore command: InstallApp (%d)", commandType);
      this->installApp(message);
      break;
    case AppCoreMessageCommandType::LaunchApp:
      ANT_DBG_VERB("AppCore command: LaunchApp (%d)", commandType);
      this->launchApp(message);
      break;
    case AppCoreMessageCommandType::CompleteLaunchingApp:
      ANT_DBG_VERB("AppCore command: CompleteLaunchingApp (%d)", commandType);
      this->completeLaunchingApp(message);
      break;
    case AppCoreMessageCommandType::TerminateApp:
      ANT_DBG_VERB("AppCore command: TerminateApp (%d)", commandType);
      this->terminateApp(message);
      break;
    case AppCoreMessageCommandType::RemoveApp:
      ANT_DBG_VERB("AppCore command: RemoveApp (%d)", commandType);
      this->removeApp(message);
      break;
    case AppCoreMessageCommandType::GetFileList:
      ANT_DBG_VERB("AppCore command: GetFileList (%d)", commandType);
      this->getFileList(message);
      break;
    case AppCoreMessageCommandType::GetFile:
      ANT_DBG_VERB("AppCore command: GetFile (%d)", commandType);
      this->getFile(message);
      break;
    case AppCoreMessageCommandType::GetRootPath:
      ANT_DBG_VERB("AppCore command: GetRootPath (%d)", commandType);
      this->getRootPath(message);
      break;
    case AppCoreMessageCommandType::GetAppIcon:
      ANT_DBG_VERB("AppCore command: GetAppIcon (%d)", commandType);
      this->getAppIcon(message);
      break;
    default:
        // Do not handle it
      break;
  }
}

// It supports only single companion device
void AppCore::onCommChannelStateChanged(CommChannelState::Value state) {
  switch(state) {
    case CommChannelState::IDLE:
      // Remove routing entry for CommChannel
      this->mMessageRouter->removeRoutingEntry(COMPANION_DEVICE_URI);

      // Retry to connect CommChannel
      this->mCommChannel->run();
      break;
    case CommChannelState::LISTENING_DEFAULT:
      // ignore
      this->mMessageRouter->addRoutingEntry(COMPANION_DEVICE_URI,
          this->mCommChannel);
      break;
    case CommChannelState::CONNECTED_DEFAULT:
      // Add routing entry for CommChannel
      break;
    case CommChannelState::LISTENING_LARGEDATA:
    case CommChannelState::CONNECTED_LARGEDATA:
    default:
      // ignore
      break;
  }
}

void AppCore::onChangedState(int appId, AppState::Value newState) {
  // Check if the app is one of which state is changed
  std::vector<BaseMessage*>::iterator iter;
  for(iter = this->mListenAppStateMessageList.begin();
      iter != this->mListenAppStateMessageList.end();
      iter++) {
    BaseMessage* originalMessage = (*iter);
    AppCoreMessage* originalPayload
      = (AppCoreMessage*)originalMessage->getPayload();
    int thisAppId = -1;
    originalPayload->getParamsListenAppState(thisAppId);
    if(thisAppId == appId) {
      // Make ACK message
      BaseMessage* ackMessage
        = MessageFactory::makeAppCoreAckMessage(this->mLocalChannel->getUri(),
            originalMessage); 
      AppCoreAckMessage* ackPayload = (AppCoreAckMessage*)ackMessage->getPayload();
      ackPayload->setParamsListenAppState(appId, newState);

      // Send ACK message
      this->mLocalChannel->sendMessage(ackMessage);
      return;
    }
  }
}

void AppCore::getAppList(BaseMessage* message) {
  // No arguments
  int messageId = message->getMessageId();

  // Make AppList parameter
  ParamAppList* paramAppList = ParamAppList::make();
  std::vector<App*>& apps = this->mAppList->getApps();
  std::vector<App*>::iterator iter;
  for(iter = apps.begin();
      iter != apps.end();
      iter++) {
    int appId = (*iter)->getId();
    std::string appName((*iter)->getName());
    bool isDefaultApp = (*iter)->isDefaultApp();
    paramAppList->addEntry(appId, appName, isDefaultApp);
  }

  // Make ACK message
  BaseMessage* ackMessage
    = MessageFactory::makeAppCoreAckMessage(this->mLocalChannel->getUri(),
        message); 
  AppCoreAckMessage* ackPayload = (AppCoreAckMessage*)ackMessage->getPayload();
  ackPayload->setParamsGetAppList(paramAppList);

  // Send ACK message
  this->mLocalChannel->sendMessage(ackMessage);
  delete paramAppList;
}

void AppCore::listenAppState(BaseMessage* message) {
  // Get arguments
  AppCoreMessage* originalPayload
    = (AppCoreMessage*)message->getPayload();
  int appId = -1;
  originalPayload->getParamsListenAppState(appId);

  // Check if there has already been listener of the app
  std::vector<BaseMessage*>::iterator iter;
  for(iter = this->mListenAppStateMessageList.begin();
      iter != this->mListenAppStateMessageList.end();
      iter++) {
    BaseMessage* originalMessage = (*iter);
    AppCoreMessage* originalPayload =
      (AppCoreMessage*)originalMessage->getPayload();
    int thisAppId = -1;
    originalPayload->getParamsListenAppState(thisAppId);
    if(thisAppId == appId) {
      return;
    }
  }
  
  // If there is no listener, register listener and add to message list
  App* app = this->mAppList->getByAppId(appId);
  if(app == NULL) {
    ANT_DBG_ERR("App does not exist in the app list!");
    return;
  }
  app->setStateListener(this);
  this->mListenAppStateMessageList.push_back(message);
}

void AppCore::initializeApp(BaseMessage* message) {
  // No arguments
  
  // Add to on-memory app list
  App* app = new App();
  this->mAppList->add(app);

  // Update state
  int appId = this->mAppList->getAndIncreaseNextAppId();
  app->finishInitializing(appId);

  // Make ACK message
  BaseMessage* ackMessage
    = MessageFactory::makeAppCoreAckMessage(this->mLocalChannel->getUri(),
        message); 
  AppCoreAckMessage* ackPayload = (AppCoreAckMessage*)ackMessage->getPayload();
  ackPayload->setParamsInitializeApp(app->getId());

  // Send ACK message
  this->mLocalChannel->sendMessage(ackMessage);
}

void AppCore::installApp(BaseMessage* message) {
  // Get arguments
  std::string packageFilePathObj(message->getStoredFilePath());
  char packageFilePath[PATH_BUFFER_SIZE];
  strncpy(packageFilePath, packageFilePathObj.c_str(),
      PATH_BUFFER_SIZE);
  AppCoreMessage* payload = (AppCoreMessage*)message->getPayload();
  int appId;
  std::string packageFileName;
  payload->getParamsInstallApp(appId, packageFileName);

  // Find app for the appId
  App* app = this->mAppList->getByAppId(appId);
  if(app == NULL) {
    ANT_DBG_ERR("App does not exist in the app list!");
    return;
  }

  // Check app's state
  AppState::Value appState = app->getState();
  switch(appState) {
    case AppState::Initialized:
      {
        // Update state
        app->startInstalling();

        // Make app package directory and determine appPackageDirPath
        // truncate opk extension
        char appPackageDirName[PATH_BUFFER_SIZE];
        strncpy(appPackageDirName,
            packageFileName.c_str(),
            packageFileName.length() - 4); 
        appPackageDirName[packageFileName.length() - 4] = '\0';

        // ${ANT_APPS_DIR}/user/${APP_NAME}
        char appPackageDirPath[PATH_BUFFER_SIZE];
        snprintf(appPackageDirPath, PATH_BUFFER_SIZE, "%s/%s",
            this->mUserAppsDir, appPackageDirName); 

        struct stat st = {0};
        if(stat(appPackageDirPath, &st) == -1) {
          mkdir(appPackageDirPath, 0755);
        } else {
          ANT_DBG_ERR("There has already been app package directory!");
          app->failInstalling();
        }

        // Archive app package
        char _commandUnzip0[] = "";
        char _commandUnzip2[] = "-d";
        char* commandUnzip[4] = {
          _commandUnzip0, packageFilePath, _commandUnzip2, appPackageDirPath
        };
        do_unzip(4, commandUnzip);
        sync();

        // Remove app package file
        if (remove(packageFilePath) == -1){
          ANT_DBG_WARN("Cannot remove app package file: %s\n",
              packageFilePath);
          app->failInstalling();
        }

        // Parse app manifest file
        char manifestFilePath[PATH_BUFFER_SIZE];
        snprintf(manifestFilePath, PATH_BUFFER_SIZE, "%s/%s",
            appPackageDirPath, "manifest.xml");
        bool settingSuccess = app->setFromManifest(manifestFilePath);
        if(!settingSuccess) {
          app->failInstalling();
          return;
        }

        // Update state
        app->successInstalling(appPackageDirPath);

        // Flush the app information to database
        this->mAppList->flush(app);
      }
      break;
    case AppState::Initializing:
      ANT_DBG_ERR("App initialization is not finished.");
      break;
    case AppState::Installing:
      ANT_DBG_ERR("The app has already been being installed.");
      break;
    case AppState::Ready:
    case AppState::Launching:
    case AppState::Running:
    case AppState::Terminating:
      ANT_DBG_ERR("The app has already been installed.");
      break;
    case AppState::Removing:
    case AppState::Removed:
      ANT_DBG_ERR("The app is removed.");
      break;
    default:
      ANT_DBG_ERR("Invalid app state: %d", appState);
      break;
  }
}

void AppCore::removeApp(BaseMessage* message) {
  // Get arguments
  int appId;
  AppCoreMessage* payload = (AppCoreMessage*)message->getPayload();
  if(payload->getParamsTerminateApp(appId) == false) {
    ANT_DBG_ERR("Invalid AppCoreMessage! (commandType: %d)",
        payload->getCommandType());
    return;
  }

  // Find app for the appId
  App* app = this->mAppList->getByAppId(appId);
  if(app == NULL) {
    ANT_DBG_ERR("App does not exist in the app list!");
    return;
  }

  // Check app's state
  AppState::Value appState = app->getState();
  switch(appState) {
    case AppState::Ready:
      {
        // Update state
        app->startRemoving();

        char commandRm[PATH_BUFFER_SIZE];
        snprintf(commandRm, PATH_BUFFER_SIZE,
            "rm -rf %s", app->getPackagePath().c_str());

        // Update state
        app->finishRemoving();

        // Remove from app list
        this->mAppList->remove(app);
      }
      break;
    case AppState::Initializing:
    case AppState::Initialized:
    case AppState::Installing:
      ANT_DBG_ERR("The app is not installed.");
      break;
    case AppState::Launching:
    case AppState::Running:
    case AppState::Terminating:
      ANT_DBG_ERR("The app has already launched.");
      break;
    case AppState::Removing:
    case AppState::Removed:
      ANT_DBG_ERR("The app has already been removed.");
      break;
    default:
      ANT_DBG_ERR("Invalid app state: %d", appState);
      break;
  }
}

void AppCore::launchApp(BaseMessage* message) {
  // Get arguments
  int appId;
  AppCoreMessage* payload = (AppCoreMessage*)message->getPayload();
  if(payload->getParamsLaunchApp(appId) == false) {
    ANT_DBG_ERR("Invalid AppCoreMessage! (commandType: %d)",
        payload->getCommandType());
    return;
  }

  // Find app for the appId
  App* app = this->mAppList->getByAppId(appId);
  if(app == NULL) {
    ANT_DBG_ERR("App does not exist in the app list!");
    return;
  }

  // Check app's state
  AppState::Value appState = app->getState();
  switch(appState) {
    case AppState::Ready:
      {
        pid_t pid;
        pid = fork();

        if (pid > 0) {
          // parent for managing child's PID & mainstream
          // Update state
          app->startLaunching(pid);
        } else if(pid == 0) {
          char mainJSFilePath[PATH_BUFFER_SIZE];
          snprintf(mainJSFilePath, PATH_BUFFER_SIZE, "%s/%s",
              app->getPackagePath().c_str(), app->getMainJSFileName().c_str());
          if(this->mIsDebugApp) {
            // Child for executing the application
            char gdbCommand[] = "gdb";
            char gdbOptionRun[] = "-ex=r";
            char gdbArgs[] = "--args";
            char nodeCommand[] = "node";
            char* fullPath[] = {gdbCommand, gdbOptionRun, gdbArgs,
              nodeCommand, mainJSFilePath, NULL};	
            ANT_DBG_VERB("Launch app: %s with gdb debugging", mainJSFilePath);
            execvp(gdbCommand, fullPath);
          } else {
            // Child for executing the application
            char nodeCommand[] = "node";
            char* fullPath[] = {nodeCommand, mainJSFilePath, NULL};	
            ANT_DBG_VERB("Launch app: %s", mainJSFilePath);
            execvp(nodeCommand, fullPath);
          }
        } else {
          ANT_DBG_ERR("Fail to fork app process\n");
        }
      }
      break;
    case AppState::Initializing:
    case AppState::Initialized:
    case AppState::Installing:
      ANT_DBG_ERR("The app is not installed.");
      break;
    case AppState::Launching:
    case AppState::Running:
      ANT_DBG_ERR("The app has already been launched.");
      break;
    case AppState::Terminating:
      ANT_DBG_ERR("The app is being terminated.");
      break;
    case AppState::Removing:
    case AppState::Removed:
      ANT_DBG_ERR("The app is being removed.");
      break;
    default:
      ANT_DBG_ERR("Invalid app state: %d", appState);
      break;
  }
}

void AppCore::completeLaunchingApp(BaseMessage* message) {
  // Get arguments
  int pid;
  AppCoreMessage* payload = (AppCoreMessage*)message->getPayload();
  if(payload->getParamsCompleteLaunchingApp(pid) == false) {
    ANT_DBG_ERR("Invalid AppCoreMessage! (commandType: %d)",
        payload->getCommandType());
    return;
  }

  // Find app for the pid
  App* app = this->mAppList->getByPid(pid);
  if(app == NULL) {
    ANT_DBG_ERR("App does not exist in the app list!");
    return;
  }

  // Check app's state
  AppState::Value appState = app->getState();
  switch(appState) {
    case AppState::Launching:
      {
        // Update state
        app->successLaunching();

        // Make ACK message
        BaseMessage* ackMessage
          = MessageFactory::makeAppCoreAckMessage(this->mLocalChannel->getUri(),
              message); 
        AppCoreAckMessage* ackPayload
          = (AppCoreAckMessage*)ackMessage->getPayload();
        ackPayload->setParamsCompleteLaunchingApp(app->getId());

        // Send ACK message
        this->mLocalChannel->sendMessage(ackMessage);
      }
      break;
    case AppState::Initializing:
    case AppState::Initialized:
    case AppState::Installing:
    case AppState::Ready:
    case AppState::Running:
    case AppState::Terminating:
    case AppState::Removing:
    case AppState::Removed:
      ANT_DBG_ERR("The app is not being launched");
      break;
    default:
      ANT_DBG_ERR("Invalid app state: %d", appState);
      break;
  }
}

void AppCore::terminateApp(BaseMessage* message) {
  // Get arguments
  int appId;
  AppCoreMessage* payload = (AppCoreMessage*)message->getPayload();
  if(payload->getParamsTerminateApp(appId) == false) {
    ANT_DBG_ERR("Invalid AppCoreMessage! (commandType: %d)",
        payload->getCommandType());
    return;
  }

  // Find app for the appId
  App* app = this->mAppList->getByAppId(appId);
  if(app == NULL) {
    ANT_DBG_ERR("App does not exist in the app list!");
    return;
  }

  // Check app's state
  AppState::Value appState = app->getState();
  switch(appState) {
    case AppState::Launching:
      {
        // Update state
        app->startTerminating();

        // Terminate app by force, since the app is not ready
        // to listen terminate message
        kill((pid_t)app->getPid(), SIGKILL);
      }
      break;
    case AppState::Running:
      {
        // Update state
        app->startTerminating();

        // Make terminate message
        char appURI[PATH_BUFFER_SIZE];
        snprintf(appURI, PATH_BUFFER_SIZE, "%s/%d", APPS_URI, app->getId());
        BaseMessage* appMessage
          = MessageFactory::makeAppMessage(this->mLocalChannel->getUri(),
              appURI, AppMessageCommandType::Terminate);

        // Send the terminate message
        this->mLocalChannel->sendMessage(appMessage);
      }
      break;
    case AppState::Initializing:
    case AppState::Initialized:
    case AppState::Installing:
    case AppState::Ready:
    case AppState::Removing:
    case AppState::Removed:
      ANT_DBG_WARN("Application is not running!");
      break;
    case AppState::Terminating:
      ANT_DBG_WARN("Application has already been terminating.");
      break;
    default:
      ANT_DBG_ERR("Invalid app state: %d", appState);
      break;
  }
}

void AppCore::completeTerminatingApp(int pid) {
  // Find app for the pid
  App* app = this->mAppList->getByPid(pid);
  if(app == NULL) {
    return;
  }

  // Check app's state
  AppState::Value appState = app->getState();
  switch(appState) {
    case AppState::Ready:
    case AppState::Launching:
    case AppState::Running:
    case AppState::Terminating:
      {
        ANT_DBG_VERB("Child was killed [User app pid : %d]", pid);

        // Update state
        app->finishTerminating();
      }
      break;
    case AppState::Initializing:
    case AppState::Initialized:
    case AppState::Installing:
    case AppState::Removing:
    case AppState::Removed:
      ANT_DBG_ERR("unexpected app state: %d", appState);
      break;
    default:
      ANT_DBG_ERR("Invalid app state: %d", appState);
      break;
  }
}

void AppCore::getFileList(BaseMessage* message) {
  // Get arguments
  std::string path;
  AppCoreMessage* payload = (AppCoreMessage*)message->getPayload();
  if(payload->getParamsGetFileList(path) == false) {
    ANT_DBG_ERR("Invalid AppCoreMessage! (commandType: %d)",
        payload->getCommandType());
    return;
  }

  // Check if the directory is available
	DIR *dir;
	if((dir = opendir(path.c_str())) == NULL) {
		ANT_DBG_ERR("cannot open the directory: %s\n", path.c_str());
		return;
	}

  // Add file entries to the file list to be returned
  ParamFileList* paramFileList = ParamFileList::make();
	struct dirent *dirEntry;
	struct stat st;
	
	while((dirEntry = readdir(dir)) != NULL){
    // Get the file entry's stat
    char fullPath[PATH_BUFFER_SIZE];
    snprintf(fullPath, PATH_BUFFER_SIZE, "%s/%s",
        path.c_str(), dirEntry->d_name);

		lstat(fullPath, &st);

    std::string fileName(dirEntry->d_name);
    ParamFileListEntryType::Value fileType;
    if(S_ISDIR(st.st_mode))
      fileType = ParamFileListEntryType::Directory;
    else if(S_ISREG(st.st_mode))
      fileType = ParamFileListEntryType::File;
    else
      fileType = ParamFileListEntryType::Others;
    int fileSizeBytes = st.st_size;

    struct tm* fileTM = localtime(&st.st_atime);
    char fileTimeBuffer[30];
    strftime(fileTimeBuffer, 30, "%Y-%m-%d %H:%M:%S", fileTM);
    std::string fileTime(fileTimeBuffer);

    paramFileList->addEntry(fileName, fileType, fileSizeBytes, fileTime);
	}

  // Make ACK message
  BaseMessage* ackMessage
    = MessageFactory::makeAppCoreAckMessage(this->mLocalChannel->getUri(),
        message); 
  AppCoreAckMessage* ackPayload = (AppCoreAckMessage*)ackMessage->getPayload();
  ackPayload->setParamsGetFileList(path, paramFileList);

  // Send ACK message
  this->mLocalChannel->sendMessage(ackMessage);
  delete paramFileList;
}

void AppCore::getFile(BaseMessage* message) {
  // Get arguments
  std::string path;
  AppCoreMessage* payload = (AppCoreMessage*)message->getPayload();
  if(payload->getParamsGetFile(path) == false) {
    ANT_DBG_ERR("Invalid AppCoreMessage! (commandType: %d)",
        payload->getCommandType());
    return;
  }

  // Make ACK message
  BaseMessage* ackMessage
    = MessageFactory::makeAppCoreAckMessage(this->mLocalChannel->getUri(),
        message); 
  ackMessage->attachFile(path);

  // Send ACK message
  this->mLocalChannel->sendMessage(ackMessage);
}

void AppCore::getRootPath(BaseMessage* message) {
  // No arguments

  // Make ACK message
  BaseMessage* ackMessage
    = MessageFactory::makeAppCoreAckMessage(this->mLocalChannel->getUri(),
        message); 
  AppCoreAckMessage* ackPayload = (AppCoreAckMessage*)ackMessage->getPayload();
  ackPayload->setParamsGetRootPath(this->mDataDir);

  // Send ACK message
  this->mLocalChannel->sendMessage(ackMessage);
}

void AppCore::getAppIcon(BaseMessage* message) {
  // Get arguments
  int appId;
  AppCoreMessage* payload = (AppCoreMessage*)message->getPayload();
  if(payload->getParamsGetAppIcon(appId) == false) {
    ANT_DBG_ERR("Invalid AppCoreMessage! (commandType: %d)",
        payload->getCommandType());
    return;
  }

  // Find app for the appId
  App* app = this->mAppList->getByAppId(appId);
  if(app == NULL) {
    ANT_DBG_ERR("App does not exist in the app list!");
    return;
  }

  BaseMessage* ackMessage
    = MessageFactory::makeAppCoreAckMessage(this->mLocalChannel->getUri(),
        message); 

  // Get app's icon file path
  if(app->getIconFileName().compare(".") != 0) {
    char iconFilePath[PATH_BUFFER_SIZE];
    snprintf(iconFilePath, PATH_BUFFER_SIZE, "%s/%s",
        app->getPackagePath().c_str(), app->getIconFileName().c_str());
    std::string iconFilePathStr(iconFilePath);

    // Make ACK message
    ackMessage->attachFile(iconFilePathStr);
  }

  // Send ACK message
  this->mLocalChannel->sendMessage(ackMessage);
}
