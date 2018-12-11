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

#include <node.h>
#include <node_object_wrap.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "ANTdbugLog.h"
#include "AppBase.h"
#include "AppNativeAPI.h"

using namespace v8;

#define MAX_NUMBER_OF_ITEMS 20
#define MESSAGE_BUFFER_SIZE 1024

// Utility Functions
#define getV8String(isolate, cstr)                                             \
  (String::NewFromOneByte(isolate, (const uint8_t *)cstr))

// Static Constructor
Persistent<Function> AppNativeAPI::sConstructor;

#define JS_THIS_OBJECT_NAME "AppNativeAPI"

/*
 * Prototype Initializer
 */
void AppNativeAPI::InitPrototype(Isolate *isolate) {
  // Prepare constructor template
  Local<FunctionTemplate> funcTemplate = FunctionTemplate::New(isolate, New);
  funcTemplate->SetClassName(getV8String(isolate, JS_THIS_OBJECT_NAME));
  funcTemplate->InstanceTemplate()->SetInternalFieldCount(1);

  // App Ready
  NODE_SET_PROTOTYPE_METHOD(funcTemplate, "appReady", appReady);

  // Lifecycle callbacks
  NODE_SET_PROTOTYPE_METHOD(funcTemplate, "onLaunch", onLaunch);
  NODE_SET_PROTOTYPE_METHOD(funcTemplate, "onTermination", onTermination);

  sConstructor.Reset(isolate, funcTemplate->GetFunction());
}

/*
 * Constructor 1
 */
void AppNativeAPI::NewInstance(const FunctionCallbackInfo<Value> &args) {
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
void AppNativeAPI::New(const FunctionCallbackInfo<Value> &args) {
  Isolate *isolate = args.GetIsolate();

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new **(...)`
    // Get arguments: no arguments

    // Create a native object
    AppNativeAPI *newObject = new AppNativeAPI();
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

// appReady(function)
void AppNativeAPI::appReady(const FunctionCallbackInfo<Value> &args) {
  Isolate *isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  // Check arguments
  if ((args.Length() != 0)) {
    isolate->ThrowException(Exception::TypeError(
        getV8String(isolate, "Invalid Use : 0 arguments expected")));
    return;
  }

  // App ready
  gAppBase->appReady();

  return;
}

// onLaunch(function)
void AppNativeAPI::onLaunch(const FunctionCallbackInfo<Value> &args) {
  Isolate *isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  // Arguments
  Local<Function> onLaunchCallback;

  // Check arguments
  if ((args.Length() != 1) || !args[0]->IsFunction()) {
    isolate->ThrowException(Exception::TypeError(getV8String(
        isolate, "Invalid Use : 1 arguments expected [Callback Function]")));
    return;
  }

  // Get argument 0
  onLaunchCallback = Local<Function>::Cast(args[0]);

  // Register onTerminate callback
  gAppBase->setOnLaunch(onLaunchCallback);

  return;
}

// onTermination(function)
void AppNativeAPI::onTermination(const FunctionCallbackInfo<Value> &args) {
  Isolate *isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  // Arguments
  Local<Function> onTerminateCallback;

  // Check arguments
  if ((args.Length() != 1) || !args[0]->IsFunction()) {
    isolate->ThrowException(Exception::TypeError(getV8String(
        isolate, "Invalid Use : 1 arguments expected [Callback Function]")));
    return;
  }

  // Get argument 0
  onTerminateCallback = Local<Function>::Cast(args[0]);

  // Register onTerminate callback
  gAppBase->setOnTerminate(onTerminateCallback);

  return;
}
