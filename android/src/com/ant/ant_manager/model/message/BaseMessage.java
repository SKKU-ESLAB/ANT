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

import android.os.Parcel;
import android.os.Parcelable;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ObjectNode;

import java.io.File;

// BaseMessage: the root base message
// - Decoding(makeFromJSON): C++, Java
// - Encoding(make, toJSON): C++, Java
public class BaseMessage implements Parcelable {
    // BaseMessageType
    public static final int Type_NotDetermined = 0;
    public static final int Type_AppCore = 10;
    public static final int Type_AppCoreAck = 11;
    public static final int Type_App = 20;
    public static final int Type_AppAck = 21;
    public static final int Type_Companion = 30;
    public static final int Type_ML = 40;
    public static final int Type_MLAck = 41;

    // JSON field name
    static final String ANT_MESSAGE_KEY_MESSAGE_NUM = "messageId";
    static final String ANT_MESSAGE_KEY_SENDER_URI = "senderUri";
    static final String ANT_MESSAGE_KEY_URI = "uri";
    static final String ANT_MESSAGE_KEY_TYPE = "type";
    static final String ANT_MESSAGE_KEY_IS_FILE_ATTACHED = "isFileAttached";
    static final String ANT_MESSAGE_KEY_FILE_NAME = "fileName";
    static final String ANT_MESSAGE_KEY_PAYLOAD = "payload";

    // Encoding to JSON
    public String toJSONString() {
        return this.toJSONNode().toString();
    }

    // Encoding to JSON
    public JsonNode toJSONNode() {
        ObjectMapper mapper = new ObjectMapper();
        ObjectNode thisObj = mapper.createObjectNode();
        thisObj.put(ANT_MESSAGE_KEY_MESSAGE_NUM, "" + this.mMessageId);
        thisObj.put(ANT_MESSAGE_KEY_SENDER_URI, "" + this.mSenderUri);
        thisObj.put(ANT_MESSAGE_KEY_URI, "" + this.mUri);
        thisObj.put(ANT_MESSAGE_KEY_TYPE, "" + this.mType);
        thisObj.put(ANT_MESSAGE_KEY_IS_FILE_ATTACHED, (this.isFileAttached()) ? "1" : "0");
        if (this.mIsFileAttached) thisObj.put(ANT_MESSAGE_KEY_FILE_NAME, "" + this.mFileName);
        thisObj.set(ANT_MESSAGE_KEY_PAYLOAD, this.mPayload.toJSONNode());
        return thisObj;
    }

    // Get parameters
    public int getMessageId() {
        return this.mMessageId;
    }

    public String getSenderUri() {
        return this.mSenderUri;
    }

    public String getUri() {
        return this.mUri;
    }

    public int getType() {
        return this.mType;
    }

    public boolean isFileAttached() {
        return this.mIsFileAttached;
    }

    public String getFileName() {
        return this.mFileName;
    }

    // Payload
    public void setPayload(BaseMessagePayload payload) {
        this.mPayload = payload;
    }

    public BaseMessagePayload getPayload() {
        return this.mPayload;
    }


    // Attach file on message to be sent
    public void attachFile(String filePath) {
        this.mIsFileAttached = true;
        File file = new File(filePath);
        this.mFileName = file.getName();
        this.setStoredFilePath(filePath);
    }

    // Set local file path when attached file has come
    public void setStoredFilePath(String storedFilePath) {
        this.mStoredFilePath = storedFilePath;
    }

    public String getStoredFilePath() {
        return this.mStoredFilePath;
    }

    // Android Parcelable
    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel out, int flags) {
        // BaseMessage -> Parcel
        // JSON
        out.writeString(this.toJSONString());

        // Private parameter
        out.writeString(this.mStoredFilePath);
    }

    public static final Parcelable.Creator<BaseMessage> CREATOR = new Parcelable
            .Creator<BaseMessage>() {
        public BaseMessage createFromParcel(Parcel in) {
            // Parcel -> BaseMessage
            String jsonString = in.readString();
            String storedFileName = in.readString();
            BaseMessage message = MessageFactory.makeMessageFromJSONString(jsonString);
            message.setStoredFilePath(storedFileName);
            return message;
        }

        public BaseMessage[] newArray(int size) {
            return new BaseMessage[size];
        }
    };

    // Initializer
    public BaseMessage(int messageId, String senderUri, String uri, int type, boolean
            isFileAttached, String fileName) {
        this.mMessageId = messageId;
        this.mSenderUri = senderUri;
        this.mUri = uri;
        this.mType = type;
        this.mIsFileAttached = isFileAttached;
        this.mFileName = fileName;
        this.mStoredFilePath = "";
        this.mPayload = null;
    }

    public BaseMessage(int messageId, String senderUri, String uri, int type) {
        this(messageId, senderUri, uri, type, false, "");
    }

    // Exported to JSON
    private int mMessageId;
    private String mSenderUri;
    private String mUri;
    private int mType;
    private boolean mIsFileAttached;
    private String mFileName;
    private BaseMessagePayload mPayload;

    // Private
    protected String mStoredFilePath;
}

abstract class BaseMessagePayload {
    // Encoding to JSON
    abstract public ObjectNode toJSONNode();
}