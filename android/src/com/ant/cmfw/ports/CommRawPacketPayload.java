package com.ant.cmfw.ports;

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

import java.io.DataInputStream;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;

public abstract class CommRawPacketPayload {
    abstract public byte[] toByteArray();

    abstract public short getBytesSize();
}

class CommPayloadMessageMetadata extends CommRawPacketPayload {
    private final String TAG = "PayloadMsgMetadata";
    private static final short kMessageHeaderSize = 8;
    private int mMessageDataLength;
    private int mIsFileAttached;

    public CommPayloadMessageMetadata(int messageDataLength, boolean isFileAttached) {
        this.mMessageDataLength = messageDataLength;
        this.mIsFileAttached = (isFileAttached) ? 1 : 0;
    }

    public byte[] toByteArray() {
        if (mMessageDataLength == 0) return null;
        ByteBuffer bb = ByteBuffer.allocate(this.getBytesSize());
        Log.d(TAG, "MessageMetadata: " + mMessageDataLength + " / " + mIsFileAttached);
        bb.putInt(mMessageDataLength);
        bb.putInt(mIsFileAttached);
        return bb.array();
    }

    public static CommPayloadMessageMetadata read(DataInputStream dataInputStream) throws
            IOException {
        int messageDataLength = dataInputStream.readInt();
        int isFileAttached = dataInputStream.readInt();
        CommPayloadMessageMetadata messageMetadata = new CommPayloadMessageMetadata
                (messageDataLength, (isFileAttached != 0));
        return messageMetadata;
    }

    public short getBytesSize() {
        return kMessageHeaderSize;
    }

    public int getMessageDataLength() {
        return this.mMessageDataLength;
    }

    public int getIsFileAttached() {
        return this.mIsFileAttached;
    }
}

class CommPayloadFileMetadata extends CommRawPacketPayload {
    private final String TAG = "PayloadFileMetadata";
    private int mFileSize;
    private String mFileName;

    public CommPayloadFileMetadata(int fileSize, String fileName) {
        mFileSize = fileSize;
        mFileName = fileName;
    }

    public byte[] toByteArray() {
        try {
            // Transform to null-terminated name
            int fileNameLength = this.mFileName.length() + 1;
            byte[] nameBytes = new byte[fileNameLength];
            System.arraycopy(this.mFileName.getBytes("UTF-8"), 0, nameBytes, 0, this.mFileName
                    .length());
            nameBytes[this.mFileName.length()] = '\0';

            ByteBuffer bb = ByteBuffer.allocate(this.getBytesSize());
            Log.d(TAG, "FileMetadata: " + mFileSize + " / " + fileNameLength + " / " + mFileName);
            bb.putInt(this.mFileSize);
            bb.putInt(fileNameLength);
            bb.put(nameBytes, 0, fileNameLength);
            return bb.array();
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
            return null;
        }
    }

    public static CommPayloadFileMetadata read(DataInputStream dataInputStream) throws IOException {
        int fileSize = dataInputStream.readInt();
        int fileNameLength = dataInputStream.readInt();
        byte[] fileNameBytes = new byte[fileNameLength];
        dataInputStream.readFully(fileNameBytes, 0, fileNameLength);

        String fileNameStr = new String(fileNameBytes, "UTF-8").trim();

        CommPayloadFileMetadata fileMetadata = new CommPayloadFileMetadata(fileSize,
                fileNameStr);
        return fileMetadata;
    }

    public short getBytesSize() {
        return (short) (4 + 4 + this.getSrcFileNameLength());
    }

    public int getFileSize() {
        return this.mFileSize;
    }

    public int getSrcFileNameLength() {
        return this.mFileName.length() + 1;
    }

    public String getSrcFileName() {
        return this.mFileName;
    }
}

class CommPayloadData extends CommRawPacketPayload {
    private final String TAG = "PayloadData";
    private short mSize;
    private byte mData[];

    public CommPayloadData(byte[] data, short size) {
        this.mData = data;
        this.mSize = size;
    }

    public byte[] toByteArray() {
        return this.mData;
    }

    public static CommPayloadData read(DataInputStream dataInputStream, short dataSize) throws
            IOException {
        byte[] data = new byte[dataSize];
        dataInputStream.readFully(data, 0, dataSize);
        CommPayloadData dataPayload = new CommPayloadData(data, dataSize);
        return dataPayload;
    }

    public short getBytesSize() {
        return this.mSize;
    }
}