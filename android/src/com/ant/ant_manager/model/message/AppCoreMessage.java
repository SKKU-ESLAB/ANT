package com.ant.ant_manager.model.message;

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

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ObjectNode;

// AppCoreMessage: message sent to AppCore Framework
// - Decoding(makeFromJSON): C++
// - Encoding(make, toJSON): Java
public class AppCoreMessage extends BaseMessagePayload {
    // AppCoreMessageCommandType
    public static final int Type_NotDetermined = 0;
    public static final int Type_GetAppList = 1; // params: void (ACK params: AppList)
    public static final int Type_ListenAppState = 2; // params: int appId (ACK params: int appId,
    //                                                                                 int appState)
    public static final int Type_InitializeApp = 3; // params: void (ACK params: int appId)
    public static final int Type_InstallApp = 4; // params: int appId, String packageFileName
    public static final int Type_LaunchApp = 5; // params: int appId
    // public static final int Type_CompleteLaunchingApp = 6; // params: int appId, int pid
    public static final int Type_TerminateApp = 7; // params: int appId
    public static final int Type_RemoveApp = 8; // params: int appId
    public static final int Type_GetFileList = 9; // params: String path (ACK params: FileList)
    public static final int Type_GetFile = 10; // params: String path (ACK params: void)
    public static final int Type_GetRootPath = 11; // params: void (ACK params: String rootPath)
    public static final int Type_GetAppIcon = 12; // params: int appId (ACK params: void)

    // JSON field name
    private static final String APPCORE_MESSAGE_KEY_COMMAND_TYPE = "commandType";
    private static final String APPCORE_MESSAGE_KEY_PAYLOAD = "payload";

    // Encoding to JSON
    @Override
    public ObjectNode toJSONNode() {
        ObjectMapper mapper = new ObjectMapper();
        ObjectNode thisObj = mapper.createObjectNode();
        thisObj.put(APPCORE_MESSAGE_KEY_COMMAND_TYPE, "" + this.mCommandType);
        thisObj.set(APPCORE_MESSAGE_KEY_PAYLOAD, this.mAppCorePayload);
        return thisObj;
    }

    // set parameters
    public void setParamsListenAppState(int appId) {
        ObjectMapper mapper = new ObjectMapper();
        ObjectNode payloadObj = mapper.createObjectNode();
        payloadObj.put("appId", "" + appId);
        this.mAppCorePayload = payloadObj;
    }

    public void setParamsInstallApp(int appId, String packageFileName) {
        ObjectMapper mapper = new ObjectMapper();
        ObjectNode payloadObj = mapper.createObjectNode();
        payloadObj.put("appId", "" + appId);
        payloadObj.put("packageFileName", "" + packageFileName);
        this.mAppCorePayload = payloadObj;
    }

    public void setParamsLaunchApp(int appId) {
        ObjectMapper mapper = new ObjectMapper();
        ObjectNode payloadObj = mapper.createObjectNode();
        payloadObj.put("appId", "" + appId);
        this.mAppCorePayload = payloadObj;
    }

    public void setParamsTerminateApp(int appId) {
        ObjectMapper mapper = new ObjectMapper();
        ObjectNode payloadObj = mapper.createObjectNode();
        payloadObj.put("appId", "" + appId);
        this.mAppCorePayload = payloadObj;
    }

    public void setParamsRemoveApp(int appId) {
        ObjectMapper mapper = new ObjectMapper();
        ObjectNode payloadObj = mapper.createObjectNode();
        payloadObj.put("appId", "" + appId);
        this.mAppCorePayload = payloadObj;
    }

    public void setParamsGetFileList(String path) {
        ObjectMapper mapper = new ObjectMapper();
        ObjectNode payloadObj = mapper.createObjectNode();
        payloadObj.put("path", "" + path);
        this.mAppCorePayload = payloadObj;
    }

    public void setParamsGetFile(String path) {
        ObjectMapper mapper = new ObjectMapper();
        ObjectNode payloadObj = mapper.createObjectNode();
        payloadObj.put("path", "" + path);
        this.mAppCorePayload = payloadObj;
    }

    public void setParamsGetAppIcon(int appId) {
        ObjectMapper mapper = new ObjectMapper();
        ObjectNode payloadObj = mapper.createObjectNode();
        payloadObj.put("appId", "" + appId);
        this.mAppCorePayload = payloadObj;
    }

    // Initializer
    public AppCoreMessage(int commandType) {
        this.mCommandType = commandType;
        this.mAppCorePayload = null;
    }

    private int mCommandType;
    private ObjectNode mAppCorePayload;
}