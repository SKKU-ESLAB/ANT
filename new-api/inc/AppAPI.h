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

#ifndef __APP_API_H__
#define __APP_API_H__
#include <node.h>
#include <node_object_wrap.h>

using namespace v8;

void CreateAppAPIObject(const FunctionCallbackInfo<Value>& args);

// How to create this object: antAPI.app();
class AppAPI
: public node::ObjectWrap {
  public:
    // Prototype Intializer
    static void InitPrototype(Isolate* isolate);

    // Constructor 1
    static void NewInstance(const FunctionCallbackInfo<Value>& args);

  protected:
    AppAPI() {
    }
    ~AppAPI() {
    }

    // Static Constructor
    static Persistent<Function> sConstructor;

    // Constructor 2
    static void New(const FunctionCallbackInfo<Value>& args);

    // Member functions
    static void makeEventPage(const FunctionCallbackInfo<Value>& args);
    static void addEventText(const FunctionCallbackInfo<Value>& args);
    static void addEventImg(const FunctionCallbackInfo<Value>& args);
    static void sendEventPage(const FunctionCallbackInfo<Value>& args);
    static void sendEventPageWithNoti(const FunctionCallbackInfo<Value>& args);
    static void makeConfigPage(const FunctionCallbackInfo<Value>& args);
    static void addStrTextbox(const FunctionCallbackInfo<Value>& args);
    static void addNumberTextbox(const FunctionCallbackInfo<Value>& args);
    static void addSingleDialog(const FunctionCallbackInfo<Value>& args);
    static void addMultipleDialog(const FunctionCallbackInfo<Value>& args);
    static void addDateDialog(const FunctionCallbackInfo<Value>& args);
    static void addTimeDialog(const FunctionCallbackInfo<Value>& args);
    static void sendConfigPage(const FunctionCallbackInfo<Value>& args);
    static void getData(const FunctionCallbackInfo<Value>& args);
    static void onTermination(const FunctionCallbackInfo<Value>& args);
    static void sendMsgToSensorViewer(const FunctionCallbackInfo<Value>& args);
};

#endif // !defined(__APP_API_H__)
