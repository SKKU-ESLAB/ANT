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
  // TODO:
}
void ResourceNativeAPI::findLocalResource(
    const FunctionCallbackInfo<Value> &args) {
  // TODO:
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
  if ((args.Length() != 4) || !args[0]->IsString() || !args[1]->IsString() ||
      !args[2]->IsString() || !args[3]->IsFunction()) {
    isolate->ThrowException(Exception::TypeError(
        getV8String(isolate, "Invalid Use : 4 arguments expected")));
    return;
  }

  // Get argument 0~3
  v8::String::Utf8Value senderUriV8(args[0]->ToString());
  senderUri = std::string(*senderUriV8);
  v8::String::Utf8Value targetUriV8(args[1]->ToString());
  targetUri = std::string(*targetUriV8);
  v8::String::Utf8Value bodyV8(args[2]->ToString());
  body = std::string(*bodyV8);
  responseJSCallback = Local<Function>::Cast(args[3]);

  // Send request
  NativeResourceAPI::sendRequest(
      opType, senderUri, targetUri, body,
      ResourceCallbackManager::resourceResponseCallback);
  // Register response callback

  return;
}

void ResourceCallbackManager::addJSCallback(
    int requestId, Local<Function> responseJSCallback) {
  // TODO:
}

void ResourceCallbackManager::resourceResponseCallback(
    BaseMessage *responseMessage) {
  // TODO: call JS callback
}