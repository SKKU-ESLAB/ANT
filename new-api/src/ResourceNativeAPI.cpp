/* Copyright (c) 2017-2018 SKKU ESLAB, and contributors. All rights reserved.
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

#include <node.h>
#include <node_object_wrap.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "ANTdbugLog.h"
#include "API.h"
#include "AppBase.h"
#include "ResourceNativeAPI.h"

using namespace v8;

#define MAX_NUMBER_OF_ITEMS 20
#define MESSAGE_BUFFER_SIZE 1024

// Utility Functions
#define getV8String(isolate, cstr)                                             \
  (String::NewFromOneByte(isolate, (const uint8_t *)cstr))

// Static Constructor
Persistent<Function> ResourceNativeAPI::sConstructor;

ResourceCallbackManager *ResourceCallbackManager::sSingleton = NULL;

#define JS_THIS_OBJECT_NAME "ResourceNativeAPI"

/*
 * Prototype Initializer
 */
void ResourceNativeAPI::InitPrototype(Isolate *isolate) {
  // Prepare constructor template
  Local<FunctionTemplate> funcTemplate = FunctionTemplate::New(isolate, New);
  funcTemplate->SetClassName(getV8String(isolate, JS_THIS_OBJECT_NAME));
  funcTemplate->InstanceTemplate()->SetInternalFieldCount(1);

  // Manage local resource
  NODE_SET_PROTOTYPE_METHOD(funcTemplate, "registerResource", registerResource);
  NODE_SET_PROTOTYPE_METHOD(funcTemplate, "findLocalResource",
                            findLocalResource);

  // Basic send operations
  NODE_SET_PROTOTYPE_METHOD(funcTemplate, "post", sendPost);
  NODE_SET_PROTOTYPE_METHOD(funcTemplate, "get", sendGet);
  NODE_SET_PROTOTYPE_METHOD(funcTemplate, "put", sendPut);
  NODE_SET_PROTOTYPE_METHOD(funcTemplate, "delete", sendDelete);

  // Advanced send operations
  NODE_SET_PROTOTYPE_METHOD(funcTemplate, "discover", sendDiscover);
  NODE_SET_PROTOTYPE_METHOD(funcTemplate, "subscribe", sendSubscribe);
  NODE_SET_PROTOTYPE_METHOD(funcTemplate, "unsubscribe", sendUnsubscribe);

  sConstructor.Reset(isolate, funcTemplate->GetFunction());
}

/*
 * Constructor 1
 */
void ResourceNativeAPI::NewInstance(const FunctionCallbackInfo<Value> &args) {
  Isolate *isolate = args.GetIsolate();

  Local<Function> consFunction = Local<Function>::New(isolate, sConstructor);
  Local<Context> context = isolate->GetCurrentContext();
  Local<Object> instance = consFunction->NewInstance(context).ToLocalChecked();

  // No Arguments

  args.GetReturnValue().Set(instance);
}

/*
 * Constructor 2
 */
void ResourceNativeAPI::New(const FunctionCallbackInfo<Value> &args) {
  Isolate *isolate = args.GetIsolate();

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new **(...)`
    // Get arguments: no arguments

    // Create a native object
    ResourceNativeAPI *newObject = new ResourceNativeAPI();
    newObject->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function: `**(...)`, turn into constructor call
    // Get arguments: no arguments

    // Call constructor
    Local<Context> context = isolate->GetCurrentContext();
    Local<Function> consFunction = Local<Function>::New(isolate, sConstructor);
    Local<Object> result = consFunction->NewInstance(context).ToLocalChecked();
    args.GetReturnValue().Set(result);
  }
}

void ResourceNativeAPI::registerResource(
    const FunctionCallbackInfo<Value> &args) {
  // TODO: Resource hosting feature
}
void ResourceNativeAPI::findLocalResource(
    const FunctionCallbackInfo<Value> &args) {
  // TODO: Resource hosting feature
}

// sendPost(function)
void ResourceNativeAPI::sendPost(const FunctionCallbackInfo<Value> &args) {
  ResourceNativeAPI::sendRequest(ResourceOperationType::POST, args);
}

// sendGet(function)
void ResourceNativeAPI::sendGet(const FunctionCallbackInfo<Value> &args) {
  ResourceNativeAPI::sendRequest(ResourceOperationType::GET, args);
}

// sendPut(function)
void ResourceNativeAPI::sendPut(const FunctionCallbackInfo<Value> &args) {
  ResourceNativeAPI::sendRequest(ResourceOperationType::PUT, args);
}

// sendDelete(function)
void ResourceNativeAPI::sendDelete(const FunctionCallbackInfo<Value> &args) {
  ResourceNativeAPI::sendRequest(ResourceOperationType::DELETE, args);
}

// sendDiscover(function)
void ResourceNativeAPI::sendDiscover(const FunctionCallbackInfo<Value> &args) {
  ResourceNativeAPI::sendRequest(ResourceOperationType::DISCOVER, args);
}

// sendSubscribe(function)
void ResourceNativeAPI::sendSubscribe(const FunctionCallbackInfo<Value> &args) {
  ResourceNativeAPI::sendRequest(ResourceOperationType::SUBSCRIBE, args);
}

// sendSubscribe(function)
void ResourceNativeAPI::sendUnsubscribe(
    const FunctionCallbackInfo<Value> &args) {
  ResourceNativeAPI::sendRequest(ResourceOperationType::UNSUBSCRIBE, args);
}

inline void printSendRequestHelp(Isolate *isolate) {
  isolate->ThrowException(Exception::TypeError(
      getV8String(isolate, "Invalid Use : 3 or 4 arguments expected\n"
                           "[String senderUri, String targetUri , String "
                           "bodyString]\n"
                           "[String senderUri, String targetUri , String "
                           "bodyString, Function responseCallback]")));
}

void ResourceNativeAPI::sendRequest(ResourceOperationType::Value opType,
                                    const FunctionCallbackInfo<Value> &args) {
  Isolate *isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  // Arguments
  std::string senderUri;
  std::string targetUri;
  std::string body;
  Local<Function> responseJSCallback;

  // Check arguments
  if (args.Length() >= 3) {
    // args.length >= 3
    if (!args[0]->IsString() || !args[1]->IsString() || !args[2]->IsString()) {
      // args type error in args.length >= 3
      printSendRequestHelp(isolate);
      return;
    }
    if (args.Length() == 4) {
      // args.length == 4
      if (!args[3]->IsFunction()) {
        // args[3] type error in args.length == 4
        printSendRequestHelp(isolate);
        return;
      }
    } else {
      // args.length > 4
      printSendRequestHelp(isolate);
      return;
    }
  } else {
    // args.length < 3
    printSendRequestHelp(isolate);
    return;
  }

  // Get argument 0~3
  v8::String::Utf8Value senderUriV8(args[0]->ToString());
  senderUri = std::string(*senderUriV8);
  v8::String::Utf8Value targetUriV8(args[1]->ToString());
  targetUri = std::string(*targetUriV8);
  v8::String::Utf8Value bodyV8(args[2]->ToString());
  body = std::string(*bodyV8);

  if (args.Length() == 4) {
    responseJSCallback = Local<Function>::Cast(args[3]);
  }

  // Send request
  int requestId = NativeResourceAPI::sendRequest(
      opType, senderUri, targetUri, body,
      ResourceCallbackManager::onResourceResponse);

  // Register response callback
  switch (opType) {
  case ResourceOperationType::POST:
  case ResourceOperationType::GET:
  case ResourceOperationType::PUT:
  case ResourceOperationType::DELETE:
  case ResourceOperationType::DISCOVER: {
    ResourceCallbackManager::singleton()->addJSCallback(requestId,
                                                        responseJSCallback);
    break;
  }
  case ResourceOperationType::SUBSCRIBE: {
    ResourceCallbackManager::singleton()->addJSCallback(requestId,
                                                        responseJSCallback);
    ResourceCallbackManager::singleton()->addSubscription(targetUri, requestId);
    break;
  }
  case ResourceOperationType::UNSUBSCRIBE: {
    int requestMessageId =
        ResourceCallbackManager::singleton()->removeSubscription(targetUri);
    if (requestMessageId >= 0) {
      ResourceCallbackManager::singleton()->removeJSCallback(requestMessageId);
    }
    break;
  }
  case ResourceOperationType::NotDetermined:
  default: {
    ANT_DBG_ERR("Invalid opType: %d", opType);
    break;
  }
  }
  return;
}

void ResourceCallbackManager::addSubscription(std::string targetUri,
                                              int requestMessageId) {
  this->mSubscriptionList.insert(
      std::pair<std::string, int>(targetUri, requestMessageId));
}

int ResourceCallbackManager::removeSubscription(std::string targetUri) {
  std::map<std::string, int>::iterator found =
      this->mSubscriptionList.find(targetUri);
  if (found == this->mSubscriptionList.end()) {
    return -1;
  }

  int requestMessageId = found->second;
  this->mSubscriptionList.erase(found);

  return requestMessageId;
}

void ResourceCallbackManager::addJSCallback(
    int requestId, Local<Function> responseJSCallback) {
  // Register JS callback to be called on response message
  Isolate *isolate = Isolate::GetCurrent();
  CopyablePersistentFunction persistentCallback(isolate, responseJSCallback);

  this->mResponseJSCallbacks.insert(
      std::pair<int, CopyablePersistentFunction>(requestId, persistentCallback));
}

void ResourceCallbackManager::removeJSCallback(int requestId) {
  std::map<int, CopyablePersistentFunction>::iterator found =
      this->mResponseJSCallbacks.find(requestId);
  if (found != this->mResponseJSCallbacks.end()) {
    found->second.Reset();
    this->mResponseJSCallbacks.erase(found);
  }
}

void ResourceCallbackManager::onResourceResponse(
    BaseMessage *responseMessage) {
  ResourceCallbackManager *self = ResourceCallbackManager::singleton();
  ResourceResponse *response =
      (ResourceResponse *)responseMessage->getPayload();
  int requestMessageId = response->getRequestMessageId();
  std::string &messageBody = response->getBody();
  ResourceOperationType::Value opType = response->getOpType();

  // Find JS callback
  std::map<int, CopyablePersistentFunction>::iterator found =
      self->mResponseJSCallbacks.find(requestMessageId);
  if (found == self->mResponseJSCallbacks.end()) {
    ANT_DBG_ERR("ResourceResponse: no JS callback is found: %s",
                messageBody.c_str());
    return;
  }

  // Initialize V8 context
  Isolate *isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
  TryCatch try_catch;

  // Prepare the callback
  CopyablePersistentFunction persistentCallback = found->second;
  Handle<Value> args[1] = {};
  args[0] = v8::String::NewFromUtf8(isolate, messageBody.c_str());
  Local<Function> jsCallback =
      Local<Function>::New(isolate, persistentCallback);

  // Execute the callback
  jsCallback->Call(isolate->GetCurrentContext()->Global(), 1, args);
  if (try_catch.HasCaught()) {
    Local<Value> exception = try_catch.Exception();
    String::Utf8Value exception_str(exception);
    ANT_DBG_ERR("Exception in ResourceResponse JS Callback: %s\n%s\n",
                messageBody.c_str(), *exception_str);
  }

  // Remove the callback
  switch (opType) {
  case ResourceOperationType::POST:
  case ResourceOperationType::GET:
  case ResourceOperationType::PUT:
  case ResourceOperationType::DELETE:
  case ResourceOperationType::DISCOVER: {
    self->removeJSCallback(requestMessageId);
    break;
  }
  case ResourceOperationType::SUBSCRIBE:
  case ResourceOperationType::UNSUBSCRIBE:
  case ResourceOperationType::NotDetermined:
  default: {
    // Do nothing
    break;
  }
  }
}