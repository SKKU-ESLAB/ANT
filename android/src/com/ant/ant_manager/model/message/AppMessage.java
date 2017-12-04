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

// AppMessage: message sent to AppCore Framework
// - Decoding(makeFromJSON): C++
// - Encoding(make, toJSON): C++, Java
public class AppMessage extends BaseMessagePayload {
    public static final int Type_Terminate = 1; // params: void
    public static final int Type_UpdateAppConfig = 2; // params: String legacyData
    //                                                           (ACK params: boolean isSucceed)

    // JSON field name
    private static final String APP_MESSAGE_KEY_COMMAND_TYPE = "commandType";
    private static final String APP_MESSAGE_KEY_PAYLOAD = "payload";

    // Encoding to JSON
    @Override
    public ObjectNode toJSONNode() {
        ObjectMapper mapper = new ObjectMapper();
        ObjectNode thisObj = mapper.createObjectNode();
        thisObj.put(APP_MESSAGE_KEY_COMMAND_TYPE, "" + this.mCommandType);
        thisObj.set(APP_MESSAGE_KEY_PAYLOAD, this.mAppPayload);
        return thisObj;
    }

    // set parameters
    public void setParamsUpdateAppConfig(String legacyData) {
        ObjectMapper mapper = new ObjectMapper();
        ObjectNode payloadObj = mapper.createObjectNode();
        payloadObj.put("legacyData", "" + legacyData);
        this.mAppPayload = payloadObj;
    }

    // Initializer
    public AppMessage(int commandType) {
        this.mCommandType = commandType;
        this.mAppPayload = null;
    }

    private int mCommandType;
    private ObjectNode mAppPayload;
}
