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

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.node.ArrayNode;
import com.fasterxml.jackson.databind.node.ObjectNode;
import com.ant.ant_manager.model.message.params.ParamAppListEntry;
import com.ant.ant_manager.model.message.params.ParamFileListEntry;
import com.ant.ant_manager.model.message.params.ParamsGetAppList;
import com.ant.ant_manager.model.message.params.ParamsGetFileList;
import com.ant.ant_manager.model.message.params.ParamsGetRootPath;
import com.ant.ant_manager.model.message.params.ParamsInitializeApp;
import com.ant.ant_manager.model.message.params.ParamsListenAppState;

import java.util.ArrayList;

// AppCoreAckMessage: ack message sent from AppCore Framework
// - Decoding(makeFromJSON): Java
// - Encoding(make, toJSON): C++
public class AppCoreAckMessage extends BaseMessagePayload {
    // JSON field name
    static final String APPCORE_ACK_MESSAGE_KEY_COMMAND_MESSAGE_NUM = "commandMessageId";
    static final String APPCORE_ACK_MESSAGE_KEY_COMMAND_TYPE = "commandType";
    static final String APPCORE_ACK_MESSAGE_KEY_PAYLOAD = "payload";

    // Encoding to JSON: not implemented for Java
    @Override
    public ObjectNode toJSONNode() {
        return null;
    }

    // Get parameters
    public ParamsGetAppList getParamsGetAppList() {
        ObjectNode paramsObj = (ObjectNode) this.mAppCoreAckPayloadObj;
        ArrayList<ParamAppListEntry> appList = new ArrayList<>();

        ArrayNode appListObj = (ArrayNode) paramsObj.get("appList");

        for (JsonNode appListEntryObj : appListObj) {
            // appId
            String appIdStr = appListEntryObj.get("appId").asText();
            int appId = Integer.parseInt(appIdStr);

            // appName
            String appName = appListEntryObj.get("appName").asText();

            // isDefaultApp
            String isDefaultAppStr = appListEntryObj.get("isDefaultApp").asText();
            boolean isDefaultApp = (isDefaultAppStr.compareTo("1") == 0);

            ParamAppListEntry appListEntry = new ParamAppListEntry(appId, appName, isDefaultApp);
            appList.add(appListEntry);
        }

        return new ParamsGetAppList(appList);
    }

    public ParamsListenAppState getParamsListenAppState() {
        ObjectNode paramsObj = (ObjectNode) this.mAppCoreAckPayloadObj;

        String appIdStr = paramsObj.get("appId").asText();
        int appId = Integer.parseInt(appIdStr);

        String appStateStr = paramsObj.get("appState").asText();
        int appState = Integer.parseInt(appStateStr);

        return new ParamsListenAppState(appId, appState);
    }

    public ParamsInitializeApp getParamsInitializeApp() {
        ObjectNode paramsObj = (ObjectNode) this.mAppCoreAckPayloadObj;

        String appIdStr = paramsObj.get("appId").asText();
        int appId = Integer.parseInt(appIdStr);

        return new ParamsInitializeApp(appId);
    }

    public ParamsGetFileList getParamsGetFileList() {
        ObjectNode paramsObj = (ObjectNode) this.mAppCoreAckPayloadObj;
        ArrayList<ParamFileListEntry> fileList = new ArrayList<>();

        String path = paramsObj.get("path").asText();

        ArrayNode fileListObj = (ArrayNode) paramsObj.get("fileList");

        for (JsonNode fileListEntryObj : fileListObj) {
            // fileName
            String fileName = fileListEntryObj.get("fileName").asText();

            // fileType
            String fileTypeStr = fileListEntryObj.get("fileType").asText();
            int fileType = Integer.parseInt(fileTypeStr);

            // fileSizeBytes
            String fileSizeBytesStr = fileListEntryObj.get("fileSizeBytes").asText();
            int fileSizeBytes = Integer.parseInt(fileSizeBytesStr);

            // fileTime
            String fileTime = fileListEntryObj.get("fileTime").asText();

            ParamFileListEntry fileListEntry = new ParamFileListEntry(fileName, fileType,
                    fileSizeBytes, fileTime);
            fileList.add(fileListEntry);
        }

        return new ParamsGetFileList(path, fileList);
    }

    public ParamsGetRootPath getParamsGetRootPath() {
        ObjectNode paramsObj = (ObjectNode) this.mAppCoreAckPayloadObj;

        String rootPath = paramsObj.get("rootPath").asText();

        return new ParamsGetRootPath(rootPath);
    }

    public int getCommandMessageId() {
        return this.mCommandMessageId;
    }

    public int getCommandType() {
        return this.mCommandType;
    }

    // Set parameters
    public void setPayload(ObjectNode appCoreAckPayloadObj) {
        this.mAppCoreAckPayloadObj = appCoreAckPayloadObj;
    }

    // Initializer
    public AppCoreAckMessage(int commandMessageId, int commandType) {
        this.mCommandMessageId = commandMessageId;
        this.mCommandType = commandType;
    }

    private int mCommandMessageId;
    private int mCommandType;
    private ObjectNode mAppCoreAckPayloadObj;
}