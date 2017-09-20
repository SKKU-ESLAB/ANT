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

#ifndef __APP_CORE_H__
#define __APP_CORE_H__

#include "LocalChannel.h"
#include "App.h"
#include "MessageRouter.h"
#include "DbusChannel.h"
#include "CommChannel.h"
#include "BaseMessage.h"
#include "AppList.h"

#include <iostream>
#include <vector>

#define PATH_BUFFER_SIZE 1024

class LocalChannel;

class AppCore
: public CommChannelStateListener,
  public LocalChannelListener,
  public AppStateListener {

  public:
    AppCore() : mAppList(NULL) {
    }

    ~AppCore() {
      delete this->mAppList;
      delete this->mMessageRouter;
      delete this->mDbusChannel;
      delete this->mCommChannel;
      delete this->mLocalChannel;

      std::vector<BaseMessage*>::iterator iter;
      for(iter = this->mListenAppStateMessageList.begin();
          iter != this->mListenAppStateMessageList.end();
          iter++) {
        this->mListenAppStateMessageList.erase(iter);
        delete (*iter);
      }
    }

    // Main loop
    void run();

    // Options
    void setDebugApp(bool isDebugApp) { this->mIsDebugApp = isDebugApp; }

    // Signal handler
    void onSignalSIGCHLD();

    // Appcore Manager's global data
    const char* getUserAppsDir() { return this->mUserAppsDir; }
    const char* getSystemAppsDir() { return this->mSystemAppsDir; }
    const char* getDataDir() { return this->mDataDir; }
    const char* getTempDir() { return this->mTempDir; }

    // LocalChannelListener
    virtual void onReceivedMessage(BaseMessage* message);

    // CommStateListener
    virtual void onCommChannelStateChanged(CommChannelState::Value state);

    // AppStateListener
    virtual void onChangedState(int appId, AppState::Value newState);

  protected:
    // App List
    AppList* mAppList;

    // App State Listen request message list
    std::vector<BaseMessage*> mListenAppStateMessageList;

    bool initializeDirs();

    // Appcore Commands
    void getAppList(BaseMessage* message);
    void listenAppState(BaseMessage* message);
    void initializeApp(BaseMessage* message);
    void installApp(BaseMessage* message);
    void launchApp(BaseMessage* message);
    void completeLaunchingApp(BaseMessage* message);
    void terminateApp(BaseMessage* message);
    void completeTerminatingApp(int pid);
    void removeApp(BaseMessage* message);
    void getFileList(BaseMessage* message);
    void getFile(BaseMessage* message);
    void getRootPath(BaseMessage* message);
    void getAppIcon(BaseMessage* message);

    // Message framework
    MessageRouter* mMessageRouter = NULL;
    DbusChannel* mDbusChannel = NULL;
    CommChannel* mCommChannel = NULL;
    LocalChannel* mLocalChannel = NULL;

    // Options
    bool mIsDebugApp = false;

    // Paths to Directories
    char mUserAppsDir[PATH_BUFFER_SIZE]; // User Apps Dir: ${ANT_APPS_DIR}/user
    char mSystemAppsDir[PATH_BUFFER_SIZE]; // System Apps Dir: ${ANT_APPS_DIR}/system
    char mAppListDBPath[PATH_BUFFER_SIZE]; // App List Path: ${ANT_APPS_DIR}/AppListDB.sqlite
    char mDataDir[PATH_BUFFER_SIZE]; // Data Dir: ${ANT_DATA_DIR}
    char mTempDir[PATH_BUFFER_SIZE]; // Temp Dir: ${ANT_TEMP_DIR}
};

#endif // !defined(__APP_CORE_H__)
