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

#include <node.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "MLAPI.h"
#include "API.h"
#include "AppBase.h"
#include "ANTdbugLog.h"

#define MAX_NUMBER_OF_ITEMS 20
#define MESSAGE_BUFFER_SIZE 1024

// Utility Functions
#define getV8String(isolate, cstr) (String::NewFromOneByte(isolate, \
      (const uint8_t*)cstr))

// Static Constructor
Persistent<Function> MLAPI::sConstructor;

#define JS_THIS_OBJECT_NAME "MLAPI"

/* 
 * Prototype Initializer
 */
void MLAPI::InitPrototype(Isolate* isolate) {
  // Prepare constructor template
  Local<FunctionTemplate> funcTemplate = FunctionTemplate::New(isolate, New);
  funcTemplate->SetClassName(getV8String(isolate, JS_THIS_OBJECT_NAME));
  funcTemplate->InstanceTemplate()->SetInternalFieldCount(1);

  // Function entries of prototype
  NODE_SET_PROTOTYPE_METHOD(funcTemplate,
      "runModel", RunModel);

  sConstructor.Reset(isolate, funcTemplate->GetFunction());
}

/* 
 * Constructor 1
 */
void MLAPI::NewInstance(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  Local<Function> consFunction = Local<Function>::New(isolate, sConstructor);
  Local<Context> context = isolate->GetCurrentContext();
  Local<Object> instance = consFunction->NewInstance(context).ToLocalChecked();
  
  // No Arguments

  args.GetReturnValue().Set(instance);
}

/* 
 * Constructor 2
 */
void MLAPI::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if(args.IsConstructCall()) {
    // Invoked as constructor: `new **(...)`
    // Get arguments: no arguments

    // Create a native object
    MLAPI* newObject = new MLAPI();
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

/*
 * Machine Learning Framework Prototype API
 *
 * - Argument 0: String modelName
 * - Argument 1: Function callback
 *   - Callback Argument 0: String outputString
 */
void MLAPI::RunModel(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  std::string modelName;
  Local<Function> callback;
  
  // Check arguments
  if (args.Length() != 2) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "Wrong arguments: expected (String modelName, Function callback)")));
    return;
  } else if (!args[0]->IsString()) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "Wrong arguments: expected (String modelName, Function callback)")));
    return;
  } else if (!args[1]->IsFunction()) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "Wrong arguments: expected (String modelName, Function callback)")));
    return;
  }

  // Get argument 0
  v8::String::Utf8Value param1(args[0]->ToString());
  modelName = std::string(*param1);

  // Get argument 1
  callback = Local<Function>::Cast(args[1]);

  gAppBase->runModel(modelName, callback);
}
