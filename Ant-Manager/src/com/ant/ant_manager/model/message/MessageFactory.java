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

import android.util.Log;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ObjectNode;

import java.io.IOException;

public class MessageFactory {
    private static final String TAG = "MessageFactory";

    // string -> JSON -> BaseMessage
    public static BaseMessage makeMessageFromJSONString(String rawString) {
        ObjectMapper mapper = new ObjectMapper();
        JsonNode rootNode = null;
        try {
            rootNode = mapper.readTree(rawString);
        } catch (IOException e) {
            e.printStackTrace();
            Log.e(TAG, "Failed to parse JSON string");
        }
        return makeBaseMessageFromJSON(rootNode);
    }

    // Make AppCoreMessage from bottom
    public static BaseMessage makeAppCoreMessage(String senderUri, String uri, int commandType) {
        AppCoreMessage payload = new AppCoreMessage(commandType);
        BaseMessage message = new BaseMessage(currentMessageId++, senderUri, uri, BaseMessage
                .Type_AppCore);
        message.setPayload(payload);
        return message;
    }

    // Make AppMessage from bottom
    public static BaseMessage makeAppMessage(String senderUri, String uri, int commandType) {
        AppMessage payload = new AppMessage(commandType);
        BaseMessage message = new BaseMessage(currentMessageId++, senderUri, uri, BaseMessage
                .Type_App);
        message.setPayload(payload);
        return message;
    }

    // JSON -> BaseMessage / AppCoreAckMessage / AppAckMessage / CompanionMessage
    private static BaseMessage makeBaseMessageFromJSON(JsonNode messageObj) {
        ObjectNode thisObj = (ObjectNode) messageObj;

        // messageId
        String messageIdStr = thisObj.get(BaseMessage.ANT_MESSAGE_KEY_MESSAGE_NUM).asText();
        int messageId = Integer.parseInt(messageIdStr);

        // sender URI
        String senderUri = thisObj.get(BaseMessage.ANT_MESSAGE_KEY_SENDER_URI).asText();

        // URI
        String uri = thisObj.get(BaseMessage.ANT_MESSAGE_KEY_URI).asText();

        // Type
        String typeStr = thisObj.get(BaseMessage.ANT_MESSAGE_KEY_TYPE).asText();
        int type = Integer.parseInt(typeStr);

        // isFileAttached
        String isFileAttachedStr = thisObj.get(BaseMessage.ANT_MESSAGE_KEY_IS_FILE_ATTACHED)
                .asText();
        boolean isFileAttached = (Integer.parseInt(isFileAttachedStr) == 1);

        // mFileName
        String fileName = "";
        JsonNode fileNameObject = thisObj.get(BaseMessage.ANT_MESSAGE_KEY_FILE_NAME);
        if (fileNameObject != null) fileName = fileNameObject.asText();

        // payload
        JsonNode payloadObj = thisObj.get(BaseMessage.ANT_MESSAGE_KEY_PAYLOAD);

        // Allocate and open a new BaseMessage
        BaseMessage newMessage = new BaseMessage(messageId, senderUri, uri, type, isFileAttached,
                fileName);
        switch (type) {
            case BaseMessage.Type_AppCoreAck: {
                AppCoreAckMessage messagePayload = makeAppCoreAckMessageFromJSON(payloadObj);
                newMessage.setPayload(messagePayload);
                break;
            }
            case BaseMessage.Type_AppAck: {
                AppAckMessage messagePayload = makeAppAckMessageFromJSON(payloadObj);
                newMessage.setPayload(messagePayload);
                break;
            }
            case BaseMessage.Type_Companion: {
                CompanionMessage messagePayload = makeCompanionMessageFromJSON(payloadObj);
                newMessage.setPayload(messagePayload);
                break;
            }
            case BaseMessage.Type_AppCore:
            case BaseMessage.Type_App:
            case BaseMessage.Type_NotDetermined:
            default:
                // These types cannot be handled.
                break;
        }

        return newMessage;
    }

    private static AppCoreAckMessage makeAppCoreAckMessageFromJSON(JsonNode messagePayloadObj) {
        ObjectNode thisObj = (ObjectNode) messagePayloadObj;

        // commandMessageId
        String commandMessageIdStr = thisObj.get(AppCoreAckMessage
                .APPCORE_ACK_MESSAGE_KEY_COMMAND_MESSAGE_NUM).asText();
        int commandMessageId = Integer.parseInt(commandMessageIdStr);

        // commandType
        String commandTypeStr = thisObj.get(AppCoreAckMessage
                .APPCORE_ACK_MESSAGE_KEY_COMMAND_TYPE).asText();
        int commandType = Integer.parseInt(commandTypeStr);

        // payload (AppCoreAckMessage's)
        ObjectNode payloadObj = (ObjectNode) thisObj.get(AppCoreAckMessage
                .APPCORE_ACK_MESSAGE_KEY_PAYLOAD);

        // Allocate and open a new AppCoreAckMessage
        AppCoreAckMessage newMessage = new AppCoreAckMessage(commandMessageId, commandType);
        if (payloadObj != null) {
            newMessage.setPayload(payloadObj);
        }
        return newMessage;
    }

    private static AppAckMessage makeAppAckMessageFromJSON(JsonNode messagePayloadObj) {
        ObjectNode thisObj = (ObjectNode) messagePayloadObj;

        // commandMessageId
        String commandMessageIdStr = thisObj.get(AppAckMessage
                .APP_ACK_MESSAGE_KEY_COMMAND_MESSAGE_NUM).asText();
        int commandMessageId = Integer.parseInt(commandMessageIdStr);

        // commandType
        String commandTypeStr = thisObj.get(AppAckMessage.APP_ACK_MESSAGE_KEY_COMMAND_TYPE)
                .asText();
        int commandType = Integer.parseInt(commandTypeStr);

        // payload (AppAckMessage's)
        ObjectNode payloadObj = (ObjectNode) thisObj.get(AppAckMessage.APP_ACK_MESSAGE_KEY_PAYLOAD);

        // Allocate and open a new AppAckMessage
        AppAckMessage newMessage = new AppAckMessage(commandMessageId, commandType);
        if (payloadObj != null) {
            newMessage.setPayload(payloadObj);
        }
        return newMessage;
    }

    private static CompanionMessage makeCompanionMessageFromJSON(JsonNode messagePayloadObj) {
        ObjectNode thisObj = (ObjectNode) messagePayloadObj;

        // commandType
        String commandTypeStr = thisObj.get(CompanionMessage.COMPANION_MESSAGE_KEY_COMMAND_TYPE)
                .asText();
        int commandType = Integer.parseInt(commandTypeStr);

        // payload (CompanionMessage's)
        ObjectNode payloadObj = (ObjectNode) thisObj.get(CompanionMessage
                .COMPANION_MESSAGE_KEY_PAYLOAD);

        // Allocate and open a new CompanionMessage
        CompanionMessage newMessage = new CompanionMessage(commandType);
        if (payloadObj != null) {
            newMessage.setPayload(payloadObj);
        }
        return newMessage;
    }

    private static int currentMessageId = 0;
}