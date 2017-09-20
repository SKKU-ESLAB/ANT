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
#include <sys/types.h>
#include <unistd.h>

#include "AppBase.h"
#include "BaseMessage.h"
#include "MessageFactory.h"

#define COMPANION_DEVICE_URI "/comp0"
#define APPCORE_URI "/thing/appcore"
#define APPS_URI "/thing/apps"

#define PATH_BUFFER_SIZE 1024

// Static variables
Persistent<Function> AppBase::sOnTerminateCallback;
AppBase* AppBase::sOnTerminateSelf;
Persistent<Function> AppBase::sOnUpdateAppConfigCallback;
AppBase* AppBase::sOnUpdateAppConfigSelf;
uint8_t* AppBase::sOnUpdateAppConfigJsonData;
BaseMessage* AppBase::sOnUpdateAppConfigMessage;

void AppBase::run() {
  // Initialize MessageRouter and Channels
  this->mMessageRouter = new MessageRouter();
  this->mDbusChannel = new DbusChannel(this->mMessageRouter);
  this->mLocalChannel = new LocalChannel(this->mMessageRouter, true);

  // Run DbusChannel: run on child thread
  this->mDbusChannel->run();
  this->mMessageRouter->addRoutingEntry(APPCORE_URI, this->mDbusChannel);
  this->mMessageRouter->addRoutingEntry(COMPANION_DEVICE_URI,
      this->mDbusChannel);

  // LocalChannel: run on child thread
  char appURI[PATH_BUFFER_SIZE];
  snprintf(appURI, PATH_BUFFER_SIZE, "%s/pid%d", APPS_URI, getpid());
  this->mMessageRouter->addRoutingEntry(appURI, this->mLocalChannel);
  this->mLocalChannel->setListener(this);
  this->mLocalChannel->run();
}

// Send appcore commands
void AppBase::completeLaunchingApp() {
  // Make appcore message
  BaseMessage* appcoreMessage 
    = MessageFactory::makeAppCoreMessage(APPCORE_URI,
        AppCoreMessageCommandType::CompleteLaunchingApp); 
  AppCoreMessage* appCorePayload
    = (AppCoreMessage*)appcoreMessage->getPayload();
  appCorePayload->setParamsCompleteLaunchingApp(getpid());

  // Send appcore message
  this->mLocalChannel->sendMessage(appcoreMessage);
}

// Send companion commands
void AppBase::sendEventPageToCompanion(const char* jsonData, bool isNoti) {
  if(this->mAppId == -1) {
    ANT_DBG_ERR("App ID is not initialized!");
    return;
  }

  // Make companion message
  BaseMessage* companionMessage
    = MessageFactory::makeCompanionMessage(COMPANION_DEVICE_URI,
        CompanionMessageCommandType::SendEventPage); 
  CompanionMessage* companionPayload
    = (CompanionMessage*)companionMessage->getPayload();
  companionPayload->setParamsSendEventPage(
      this->mAppId, jsonData, isNoti);

  // Send companion message
  this->mLocalChannel->sendMessage(companionMessage);
}

void AppBase::sendConfigPageToCompanion(const char* jsonData) {
  if(this->mAppId == -1) {
    ANT_DBG_ERR("App ID is not initialized!");
    return;
  }

  // Make companion message
  BaseMessage* companionMessage
    = MessageFactory::makeCompanionMessage(COMPANION_DEVICE_URI,
        CompanionMessageCommandType::SendConfigPage); 
  CompanionMessage* companionPayload
    = (CompanionMessage*)companionMessage->getPayload();
  companionPayload->setParamsSendConfigPage(
      this->mAppId, jsonData);

  // Send companion message
  this->mLocalChannel->sendMessage(companionMessage);
}

void AppBase::updateSensorDataToCompanion(const char* jsonData) {
  // Make companion message
  BaseMessage* companionMessage
    = MessageFactory::makeCompanionMessage(COMPANION_DEVICE_URI,
        CompanionMessageCommandType::UpdateSensorData); 
  CompanionMessage* companionPayload
    = (CompanionMessage*)companionMessage->getPayload();
  companionPayload->setParamsUpdateSensorData(jsonData);

  // Send companion message
  this->mLocalChannel->sendMessage(companionMessage);
}

void AppBase::onReceivedMessage(BaseMessage* message) {
  if(message == NULL) {
    ANT_DBG_ERR("Invalid App Message");
    return;
  }

  switch(message->getType()) {
    case BaseMessageType::App:
      {
        // App Message
        AppMessage* payload = (AppMessage*)message->getPayload();
        if(payload == NULL) {
          ANT_DBG_ERR("AppMessage payload does not exist");
          return;
        }

        switch(payload->getCommandType()) {
          case AppMessageCommandType::Terminate:
            this->terminate(message);
            break;
          case AppMessageCommandType::UpdateAppConfig:
            this->updateAppConfig(message);
            break;
          default:
            // Do not handle it
            break;
        }
        break;
      }
    case BaseMessageType::AppCoreAck:
      {
        // AppCore Ack Message
        AppCoreAckMessage* payload = (AppCoreAckMessage*)message->getPayload();
        if(payload == NULL) {
          ANT_DBG_ERR("AppCoreAckMessage payload does not exist");
          return;
        }

        switch(payload->getCommandType()) {
          case AppCoreMessageCommandType::CompleteLaunchingApp:
            this->onAckCompleteLaunchingApp(message);
            break;
          default:
            // Do not handle it
            break;
        }
        break;
      }
    case BaseMessageType::AppCore:
    case BaseMessageType::AppAck:
    case BaseMessageType::Companion:
    default:
      ANT_DBG_ERR("Invalid Message Type");
      break;
  }
}

// Terminate async callback
// 1. AppBase::setOnTerminate() (callback registration on app main thread)
// 2. (app running)
// 3. AppMessage::terminate comes (on MessageFW listening thread)
// 4. AppBase::terminate() (on MessageFW listening thread)
// 5. AppBase::onTerminateAsyncHandler() (on app main thread)
void AppBase::setOnTerminate(Isolate* isolate,
    Local<Function> onTerminateCallback) {
  uv_loop_t* loop = uv_default_loop();
  uv_async_init(loop, &this->mOnTerminateAsync,
      onTerminateAsyncHandler);
  AppBase::sOnTerminateCallback.Reset(isolate,
      onTerminateCallback);
  AppBase::sOnTerminateSelf = this;

  this->mIsTerminateCallbackEnabled = true;
}

void AppBase::terminate(BaseMessage* message) {
  // If no async is set, quit the application in force.
  if(!this->mIsTerminateCallbackEnabled) {
    exit(1);
  }

  // Call async callback
  uv_async_send(&this->mOnTerminateAsync);
}

void AppBase::onTerminateAsyncHandler(uv_async_t* handle) {
  AppBase* self = sOnTerminateSelf;
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  printf("[NIL] termination Event (app id: %d)\n", self->mAppId);

  // Execute onTerminate callback
  TryCatch try_catch;
  Handle<Value> argv[] = { };
  Local<Function> onTerminateCallback
    = Local<Function>::New(isolate, AppBase::sOnTerminateCallback);
  onTerminateCallback->Call(isolate->GetCurrentContext()->Global(), 0, argv);

  if (try_catch.HasCaught()) {
    Local<Value> exception = try_catch.Exception();
    String::Utf8Value exception_str(exception);
    printf("Exception in onTerminate Callback: %s\n", *exception_str);
  }

  // Terminate this app
  exit(1);
}

// UpdateAppConfig async callback
// 1. AppBase::setOnUpdateAppConfig() (callback registration on app main thread)
// 2. (app running)
// 3. AppMessage::updateAppConfig comes (on MessageFW listening thread)
// 4. AppBase::updateAppConfig() (on MessageFW listening thread)
// 5. AppBase::onUpdateAppConfigAsyncHandler() (on app main thread)
void AppBase::setOnUpdateAppConfig(Isolate* isolate,
    Local<Function> onUpdateAppConfigCallback) {
  uv_loop_t* loop = uv_default_loop();
  uv_async_init(loop, &this->mOnUpdateAppConfigAsync,
      onUpdateAppConfigAsyncHandler);
  AppBase::sOnUpdateAppConfigCallback.Reset(isolate,
      onUpdateAppConfigCallback);
  AppBase::sOnUpdateAppConfigSelf = this;
}

void AppBase::updateAppConfig(BaseMessage* message) {
  // Arguments
  std::string legacyData;

  // Get arguments
  AppMessage* payload = (AppMessage*)message->getPayload();
  payload->getParamsUpdateAppConfig(legacyData);
  AppBase::sOnUpdateAppConfigJsonData = (uint8_t*)legacyData.c_str();

  AppBase::sOnUpdateAppConfigMessage = message;

  // Call async callback
  uv_async_send(&this->mOnUpdateAppConfigAsync);
}

void AppBase::onUpdateAppConfigAsyncHandler(uv_async_t* handle) {
  AppBase* self = sOnUpdateAppConfigSelf;
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  // Get arguments
  unsigned char* jsonData = AppBase::sOnUpdateAppConfigJsonData;

  printf("[NIL] Receive Config value :%s\n", jsonData);

  // Execute onUpdateAppConfig callback
  TryCatch try_catch;
  Handle<Value> argv[] = { String::NewFromOneByte(isolate, jsonData) };
  Local<Function> onUpdateAppConfigCallback
    = Local<Function>::New(isolate, AppBase::sOnUpdateAppConfigCallback);
  onUpdateAppConfigCallback->Call(
      isolate->GetCurrentContext()->Global(), 1, argv);

  // Make ACK message
  BaseMessage* ackMessage =  MessageFactory::makeAppAckMessage(
      COMPANION_DEVICE_URI, AppBase::sOnUpdateAppConfigMessage); 
  AppAckMessage* ackPayload = (AppAckMessage*)ackMessage->getPayload();
  if (try_catch.HasCaught()) {
    Local<Value> exception = try_catch.Exception();
    String::Utf8Value exception_str(exception);
    printf("Exception: %s\n", *exception_str);

    ackPayload->setParamsUpdateAppConfig(false);
  } else {
    ackPayload->setParamsUpdateAppConfig(true);
  }

  // Send ACK message
  self->mLocalChannel->sendMessage(ackMessage);
  return;
}

void AppBase::onAckCompleteLaunchingApp(BaseMessage* message) {
  // Arguments
  int appId = -1;

  // Get arguments
  AppCoreAckMessage* payload = (AppCoreAckMessage*)message->getPayload();
  payload->getParamsCompleteLaunchingApp(appId);

  // Set app id
  this->mAppId = appId;

  // add appId to routing table
  char appURI[PATH_BUFFER_SIZE];
  snprintf(appURI, PATH_BUFFER_SIZE, "%s/pid%d", APPS_URI, getpid());
  this->mMessageRouter->removeRoutingEntry(appURI);
  snprintf(appURI, PATH_BUFFER_SIZE, "%s/%d", APPS_URI, appId);
  this->mMessageRouter->addRoutingEntry(appURI, this->mLocalChannel);
}
