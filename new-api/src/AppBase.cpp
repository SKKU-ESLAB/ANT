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
#define ML_URI "/thing/ml"
#define APPS_URI "/thing/apps"

#define PATH_BUFFER_SIZE 1024

// Static variables
OnTerminateJSAsync* OnTerminateJSAsync::sSingleton;
OnUpdateAppConfigJSAsync* OnUpdateAppConfigJSAsync::sSingleton;

void AppBase::run() {
  // URI
  char appURI[PATH_BUFFER_SIZE];
  snprintf(appURI, PATH_BUFFER_SIZE, "%s/pid%d", APPS_URI, getpid());

  // Initialize MessageRouter and Channels
  this->mMessageRouter = new MessageRouter();
  this->mDbusChannel = new DbusChannel(this->mMessageRouter);
  this->mLocalChannel = new LocalChannel(this->mMessageRouter, appURI, true);

  // Run DbusChannel: run on child thread
  this->mDbusChannel->run();
  this->mMessageRouter->addRoutingEntry(APPCORE_URI, this->mDbusChannel);
  this->mMessageRouter->addRoutingEntry(COMPANION_DEVICE_URI,
      this->mDbusChannel);

  // LocalChannel: run on child thread
  this->mMessageRouter->addRoutingEntry(appURI, this->mLocalChannel);
  this->mLocalChannel->setListener(this);
  this->mLocalChannel->run();
}

// Send appcore commands
void AppBase::completeLaunchingApp() {
  // Make appcore message
  BaseMessage* appcoreMessage 
    = MessageFactory::makeAppCoreMessage(this->mLocalChannel->getUri(),
        APPCORE_URI,
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
    = MessageFactory::makeCompanionMessage(this->mLocalChannel->getUri(),
        COMPANION_DEVICE_URI,
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
    = MessageFactory::makeCompanionMessage(this->mLocalChannel->getUri(),
        COMPANION_DEVICE_URI,
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
    = MessageFactory::makeCompanionMessage(this->mLocalChannel->getUri(),
        COMPANION_DEVICE_URI,
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
    case BaseMessageType::MLAck:
      {
        // ML Ack Message
        MLAckMessage* payload = (MLAckMessage*)message->getPayload();
        if(payload == NULL) {
          ANT_DBG_ERR("MLAckMessage payload does not exist");
          return;
        }

        switch(payload->getCommandType()) {
          case MLMessageCommandType::RunModel:
            this->onAckRunModel(message);
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
    case BaseMessageType::ML:
    default:
      ANT_DBG_ERR("Invalid Message Type");
      break;
  }
}

// Terminate async callback
void AppBase::setOnTerminate(Local<Function> jsCallback) {
  OnTerminateJSAsync* jsAsync = OnTerminateJSAsync::get();
  jsAsync->setCallback(this, jsCallback);
}

void AppBase::terminate(BaseMessage* message) {
  OnTerminateJSAsync* jsAsync = OnTerminateJSAsync::get();

  // If no async is set, quit the application in force.
  if(!jsAsync->isCallbacksEnabled()) {
    exit(1);
  }

  // Call async native callback
  jsAsync->onAsyncEvent();
}

void OnTerminateJSAsync::nativeCallback(uv_async_t* handle) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  OnTerminateJSAsync* self = OnTerminateJSAsync::get();

  printf("[NIL] termination Event (app id: %d)\n", self->mAppBase->mAppId);

  // Execute JS callback: no arguments
  TryCatch try_catch;
  Handle<Value> argv[] = { };
  Local<Function> jsCallback
    = Local<Function>::New(isolate, self->mJSCallback);
  jsCallback->Call(isolate->GetCurrentContext()->Global(), 0, argv);

  if (try_catch.HasCaught()) {
    Local<Value> exception = try_catch.Exception();
    String::Utf8Value exception_str(exception);
    printf("Exception in onTerminate Callback: %s\n", *exception_str);
  }

  // Terminate this app
  exit(1);
}

// UpdateAppConfig async callback
void AppBase::setOnUpdateAppConfig(Local<Function> jsCallback) {
  OnUpdateAppConfigJSAsync* jsAsync = OnUpdateAppConfigJSAsync::get();
  jsAsync->setCallback(this, jsCallback);
}

void AppBase::updateAppConfig(BaseMessage* message) {
  // Arguments
  std::string legacyData;

  // Get arguments
  AppMessage* payload = (AppMessage*)message->getPayload();
  payload->getParamsUpdateAppConfig(legacyData);

  // Set parameters to native callback
  OnUpdateAppConfigJSAsync* jsAsync = OnUpdateAppConfigJSAsync::get();
  jsAsync->setMessage(message);
  jsAsync->setLegacyData((uint8_t*)legacyData.c_str());

  // Call async native callback
  jsAsync->onAsyncEvent();
}

void OnUpdateAppConfigJSAsync::nativeCallback(uv_async_t* handle) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  OnUpdateAppConfigJSAsync* self = OnUpdateAppConfigJSAsync::get();

  // Get arguments
  unsigned char* jsonData = self->mLegacyData;

  printf("[NIL] Receive Config value :%s\n", jsonData);

  // Execute onUpdateAppConfig callback
  TryCatch try_catch;
  Handle<Value> argv[] = { String::NewFromOneByte(isolate, jsonData) };
  Local<Function> onUpdateAppConfigCallback
    = Local<Function>::New(isolate, self->mJSCallback);
  onUpdateAppConfigCallback->Call(
      isolate->GetCurrentContext()->Global(), 1, argv);

  // Make ACK message
  BaseMessage* originalMessage = self->mMessage;
  BaseMessage* ackMessage =  MessageFactory::makeAppAckMessage(
      self->mAppBase->mLocalChannel->getUri(), originalMessage); 
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
  self->mAppBase->mLocalChannel->sendMessage(ackMessage);
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

  this->mLocalChannel->setUri(appURI);
}

// Send machine learning commands
void AppBase::runModel(std::string modelName, Local<Function> callback) {
  // Set async callback
  OnRunModelJSAsync* jsAsync = OnRunModelJSAsync::get();
  jsAsync->setCallback(this, callback);

  // Make ML message
  BaseMessage* mlMessage = MessageFactory::makeMLMessage(
      this->mLocalChannel->getUri(), ML_URI, MLMessageCommandType::RunModel);
  MLMessage* mlPayload = (MLMessage*)mlMessage->getPayload();
  mlPayload->setParamsRunModel(modelName);

  // Send ML message
  this->mLocalChannel->sendMessage(mlMessage);
}

// Native async callbacks
void AppBase::onAckRunModel(BaseMessage* message) {
  // Arguments
  std::string outputData;

  // Get arguments
  MLAckMessage* payload = (MLAckMessage*)message->getPayload();
  payload->getParamsRunModel(outputData);

  // Execute JS callback
  OnRunModelJSAsync* jsAsync = OnRunModelJSAsync::get();
  jsAsync->setOutputData((uint8_t*)outputData.c_str());
  jsAsync->onAsyncEvent();
}
