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

#ifndef __MESSAGE_ADAPTER_H__
#define __MESSAGE_ADAPTER_H__

#include <node.h>
#include <uv.h>

#include "ANTdbugLog.h"
#include "BaseMessage.h"
#include "DbusChannel.h"
#include "LocalChannel.h"
#include "MessageRouter.h"

using namespace v8;

class OnLaunchJSAsync;
class OnTerminateJSAsync;
class OnUpdateAppConfigJSAsync;

class AppBase : public LocalChannelListener {
public:
  friend class OnLaunchJSAsync;
  friend class OnTerminateJSAsync;
  friend class OnUpdateAppConfigJSAsync;

  AppBase() : mAppId(-1) {}
  ~AppBase() {
    if (this->mMessageRouter != NULL)
      delete this->mMessageRouter;
    if (this->mDbusChannel != NULL)
      delete this->mDbusChannel;
    if (this->mLocalChannel != NULL)
      delete this->mLocalChannel;
  }

  // Run message framework
  void run();

  // LocalChannelListener
  virtual void onReceivedMessage(BaseMessage *message);

  // App Ready
  void appReady();

  // Set JS Async callbacks
  void setOnLaunch(Local<Function> callback);
  void setOnTerminate(Local<Function> callback);
  void setOnUpdateAppConfig(Local<Function> callback);

  // Send appcore comamnds
  void completeLaunchingApp();

  // Send companion commands
  void sendToCompanion(const char *listenerName, const char *data);
  void sendToCompanion(const char *listenerName, const char *data,
                       const char *attachedFilePath);

  // Send machine learning commands
  void runModel(std::string modelName, Local<Function> callback);

  // Native async callbacks
  void onRunModelAsyncHandler(uv_async_t *handle);

protected:
  // App ID
  int mAppId;

  // App Commands
  void terminate(BaseMessage *message);
  void updateAppConfig(BaseMessage *message);

  // Appcore Ack
  void onAckCompleteLaunchingApp(BaseMessage *message);

  // ML Ack
  void onAckRunModel(BaseMessage *message);

  // Wait until appId is assigned
  pthread_mutex_t mWaitMutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t mWaitCond = PTHREAD_COND_INITIALIZER;

  // Message framework
  MessageRouter *mMessageRouter = NULL;
  DbusChannel *mDbusChannel = NULL;
  LocalChannel *mLocalChannel = NULL;
};

// JS async callback
// 1. JSAsync::setCallback() (callback registration on app main thread)
// 2. (app running)
// 3. Message comes and executes message handler (on message listening thread)
// 4. JSAsync::onAsyncEvent() (on message listening thread)
// 5. uv_async_send() (message listening thread -> app main thread)
// 6. nativeCallback() (on app main thread)
// 7. call JSCallback (on app main thread)
class JSAsync {
public:
  void setCallback(AppBase *appBase, Local<Function> jsCallback) {
    this->setJSCallback(jsCallback);
    this->setNativeCallback();
    this->setAppBase(appBase);

    this->mIsCallbacksEnabled = true;
  }

  void onAsyncEvent() { uv_async_send(&this->mUVAsync); }

  bool isCallbacksEnabled() { return this->mIsCallbacksEnabled; }

protected:
  JSAsync(uv_async_cb nativeCb) { this->mNativeCallback = nativeCb; }

  void setJSCallback(Local<Function> jsCallback) {
    // Set JS callback
    Isolate *isolate = Isolate::GetCurrent();
    this->mJSCallback.Reset(isolate, jsCallback);
  }

  void setNativeCallback() {
    // Set native callback as libuv async callee
    uv_loop_t *loop = uv_default_loop();
    uv_async_init(loop, &this->mUVAsync, this->mNativeCallback);
  }

  void setAppBase(AppBase *appBase) { this->mAppBase = appBase; }

  uv_async_t mUVAsync;
  uv_async_cb mNativeCallback;
  Persistent<Function> mJSCallback;
  bool mIsCallbacksEnabled = false;
  AppBase *mAppBase;
};

class OnLaunchJSAsync : public JSAsync {
public:
  static OnLaunchJSAsync *get() {
    if (sSingleton == NULL) {
      sSingleton = new OnLaunchJSAsync();
    }
    return sSingleton;
  }

protected:
  OnLaunchJSAsync() : JSAsync(OnLaunchJSAsync::nativeCallback) {}

  static void nativeCallback(uv_async_t *handle);

  static OnLaunchJSAsync *sSingleton;
};

class OnTerminateJSAsync : public JSAsync {
public:
  static OnTerminateJSAsync *get() {
    if (sSingleton == NULL) {
      sSingleton = new OnTerminateJSAsync();
    }
    return sSingleton;
  }

protected:
  OnTerminateJSAsync() : JSAsync(OnTerminateJSAsync::nativeCallback) {}

  static void nativeCallback(uv_async_t *handle);

  static OnTerminateJSAsync *sSingleton;
};

class OnUpdateAppConfigJSAsync : public JSAsync {
public:
  static OnUpdateAppConfigJSAsync *get() {
    if (sSingleton == NULL) {
      sSingleton = new OnUpdateAppConfigJSAsync();
    }
    return sSingleton;
  }

  void setMessage(BaseMessage *message) { this->mMessage = message; }
  void setLegacyData(uint8_t *legacyData) { this->mLegacyData = legacyData; }

protected:
  OnUpdateAppConfigJSAsync()
      : JSAsync(OnUpdateAppConfigJSAsync::nativeCallback) {}

  static void nativeCallback(uv_async_t *handle);

  static OnUpdateAppConfigJSAsync *sSingleton;

  // Parameter to native callback
  BaseMessage *mMessage;
  uint8_t *mLegacyData;
};

class OnRunModelJSAsync : public JSAsync {
public:
  static OnRunModelJSAsync *get() {
    if (sSingleton == NULL) {
      sSingleton = new OnRunModelJSAsync();
    }
    return sSingleton;
  }

  void setOutputData(uint8_t *outputData) { this->mOutputData = outputData; }

protected:
  OnRunModelJSAsync() : JSAsync(OnRunModelJSAsync::nativeCallback) {}

  static void nativeCallback(uv_async_t *handle);

  static OnRunModelJSAsync *sSingleton;

  // Parameter to native callback
  uint8_t *mOutputData;
};

#endif // !defined(__MESSAGE_ADAPTER_H__)
