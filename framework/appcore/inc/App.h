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

#ifndef __APP_H__
#define __APP_H__

#include <iostream>

#define PATH_BUFFER_SIZE 1024

// Use namespace + enum for readability
namespace AppState {
  enum Value {
    Initializing = 1,
    Initialized = 2, // set id
    Installing = 3, // set packageFilePath
    Ready = 4, // set isDefaultApp, name, mainJSFilePath
               // store to DB
               // reset pid
    Launching = 5, // set pid
    Running = 6,
    Terminating = 7,
    Removing = 8,
    Removed = 9 // reset id, packageFilePath, isDefaultApp, name, mainJSFilePath
  };
}

class AppStateListener {
  public:
    virtual void onChangedState(int appId, AppState::Value newState) = 0;
};

class App {
  public:
    App()
      : mState(AppState::Initializing), mStateListener(NULL),
      mId(-1),
      mIsDefaultApp(false),
      mName(""),
      mPackagePath(""),
      mMainJSFileName(""), mIconFileName(""),
      mPid(-1) {
    }

    App(int id, bool isDefaultApp, std::string name,
        std::string packagePath,
        std::string mainJSFileName, std::string iconFileName,
        AppState::Value state)
      : mState(state), mStateListener(NULL),
      mId(id),
      mIsDefaultApp(isDefaultApp),
      mName(name),
      mPackagePath(packagePath),
      mMainJSFileName(mainJSFileName), mIconFileName(iconFileName),
      mPid(-1) {
    }

    // Getters
    AppState::Value getState() { return this->mState; }
    int getId() { return this->mId; }
    bool isDefaultApp() { return this->mIsDefaultApp; }
    std::string getName() { return this->mName; }
    std::string getPackagePath() { return this->mPackagePath; }
    std::string getMainJSFileName() { return this->mMainJSFileName; }
    std::string getIconFileName() { return this->mIconFileName; }
    int getPid() { return this->mPid; }

    // Setters
    bool setFromManifest(std::string manifestFilePath);
    
    // Commands
    void finishInitializing(int appId); // Initializing -> Initialized

    void startInstalling(); // Initialized -> Installing
    void successInstalling(std::string packagePath); // Installing -> Ready
    void failInstalling(); // Installing -> Removed

    void startLaunching(int pid); // Ready -> Launching
    void successLaunching(); // Launching -> Running
    // TODO: how to know if launching is failed? --> timer? (no response watcher)
    void failLaunching(); // Launching -> Ready

    void startTerminating(); // Running -> Terminating
    void finishTerminating(); // Terminating -> Ready

    void startRemoving(); // Ready -> Removing
    void finishRemoving(); // Removing -> Removed

    // State listener
    void setStateListener(AppStateListener* stateListener) {
      this->mStateListener = stateListener;
    }

  protected:
    // Change app state
    bool changeState(AppState::Value newState);

    // State listener
    AppStateListener* mStateListener;

    // App information
    AppState::Value mState;
    int mId; // determined at Initialized, reset at Removed (stored to DB)
    bool mIsDefaultApp; // determined at Ready, reset at Removed (stored to DB)
    std::string mName; // determined at Ready, reset at Removed (stored to DB)
    std::string mPackagePath; // determined at Ready, reset at Removed (stored to DB)
    std::string mMainJSFileName; // determined at Ready, reset at Removed (stored to DB)
    std::string mIconFileName; // determined at Ready, reset at Removed (stored to DB)
    int mPid; // determined at Launching, reset at Ready
};

#endif // !defined(__APP_H__)
