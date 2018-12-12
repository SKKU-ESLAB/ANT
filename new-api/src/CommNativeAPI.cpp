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
#include <node_object_wrap.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "CommNativeAPI.h"
#include "API.h"
#include "AppBase.h"
#include "ANTdbugLog.h"

using namespace v8;

#define MESSAGE_BUFFER_SIZE 1024

// Utility Functions
#define getV8String(isolate, cstr) (String::NewFromOneByte(isolate, \
      (const uint8_t*)cstr))

// Static Constructor
Persistent<Function> CommNativeAPI::sConstructor;

#define JS_THIS_OBJECT_NAME "CommNativeAPI"

/* 
 * Prototype Initializer
 */
void CommNativeAPI::InitPrototype(Isolate* isolate) {
  // Prepare constructor template
  Local<FunctionTemplate> funcTemplate = FunctionTemplate::New(isolate, New);
  funcTemplate->SetClassName(getV8String(isolate, JS_THIS_OBJECT_NAME));
  funcTemplate->InstanceTemplate()->SetInternalFieldCount(1);

  // Sensor Viewer
  NODE_SET_PROTOTYPE_METHOD(funcTemplate,
      "sendToCompanion", sendToCompanion);

  sConstructor.Reset(isolate, funcTemplate->GetFunction());
}

/* 
 * Constructor 1
 */
void CommNativeAPI::NewInstance(const FunctionCallbackInfo<Value>& args) {
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
void CommNativeAPI::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if(args.IsConstructCall()) {
    // Invoked as constructor: `new **(...)`
    // Get arguments: no arguments

    // Create a native object
    CommNativeAPI* newObject = new CommNativeAPI();
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

// send CompanionMessage.SendToCompanion
void CommNativeAPI::sendToCompanion(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  // Arguments
  const char* listenerName;
  const char* data;

  // Check arguments
  if ((args.Length() != 2) ||	!args[0]->IsString() || !args[1]->IsString()) {
    isolate->ThrowException(Exception::TypeError(getV8String(isolate,
            "Invalid Use : 2 arguments expected " \
            "[String listenerName, String data]")));
    return;
  }

  // Get argument 0
  v8::String::Utf8Value param0(args[0]->ToString());
  std::string listenerNameStr = std::string(*param0);
  listenerName = listenerNameStr.c_str();

  // Get argument 1
  v8::String::Utf8Value param1(args[1]->ToString());
  std::string dataStr = std::string(*param1);
  data = dataStr.c_str();

  // Send CompanionMessage.SendToCompanion
  AppBase::singleton()->sendToCompanion(listenerName, data);

  ANT_DBG_VERB("Send data to companion(%s): %s\n", listenerName, data);
  return;
}
