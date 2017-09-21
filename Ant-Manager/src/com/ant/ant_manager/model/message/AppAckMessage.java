package com.ant.ant_manager.model.message;

/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: 
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

import com.fasterxml.jackson.databind.node.ObjectNode;
import com.ant.ant_manager.model.message.params.ParamsUpdateAppConfig;

// AppAckMessage: ack message sent from AppCore Framework
// - Decoding(makeFromJSON): Java
// - Encoding(make, toJSON): C++
public class AppAckMessage extends BaseMessagePayload {
    // JSON field name
    static final String APP_ACK_MESSAGE_KEY_COMMAND_MESSAGE_NUM = "commandMessageId";
    static final String APP_ACK_MESSAGE_KEY_COMMAND_TYPE = "commandType";
    static final String APP_ACK_MESSAGE_KEY_PAYLOAD = "payload";

    // Encoding to JSON: not implemented for Java
    @Override
    public ObjectNode toJSONNode() {
        return null;
    }

    // Get parameters
    public ParamsUpdateAppConfig getParamsUpdateAppConfig() {
        ObjectNode paramsObj = (ObjectNode) this.mAppAckPayloadObj;

        String isSucceedStr = paramsObj.get("isSucceed").asText();
        boolean isSucceed = (isSucceedStr.compareTo("1") == 0);

        return new ParamsUpdateAppConfig(isSucceed);
    }

    public int getCommandMessageId() {
        return this.mCommandMessageId;
    }

    public int getCommandType() {
        return this.mCommandType;
    }

    // Set parameters
    public void setPayload(ObjectNode appCoreAckPayloadObj) {
        this.mAppAckPayloadObj = appCoreAckPayloadObj;
    }

    // Initializer
    public AppAckMessage(int commandMessageId, int commandType) {
        this.mCommandMessageId = commandMessageId;
        this.mCommandType = commandType;
    }

    private int mCommandMessageId;
    private int mCommandType;
    private ObjectNode mAppAckPayloadObj;
}
