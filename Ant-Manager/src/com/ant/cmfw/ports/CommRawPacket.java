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

import java.io.File;

public class CommRawPacket {
    private final String TAG = "CommRawPacket";
    private CommRawPacketHeader mHeader;
    private CommRawPacketPayload mPayload;

    private CommRawPacket(CommRawPacketHeader header, CommRawPacketPayload payload) {
        this.mHeader = header;
        this.mPayload = payload;
    }

    static public CommRawPacket makeMessageMetadataPacket(byte headerId, int totalDataSize,
                                                          boolean isFileAttached) {
        CommPayloadMessageMetadata payload = new CommPayloadMessageMetadata(totalDataSize,
                isFileAttached);
        CommRawPacketHeader header = new CommRawPacketHeader(headerId, payload.getBytesSize(), 0,
                false, false, false, true);

        return new CommRawPacket(header, payload);
    }

    static public CommRawPacket makeFileMetadataPacket(byte headerId, File srcFile) {
        String fileName = srcFile.getName();
        CommPayloadFileMetadata payload = new CommPayloadFileMetadata((int) srcFile.length(),
                fileName);
        CommRawPacketHeader header = new CommRawPacketHeader(headerId, payload.getBytesSize(), 0,
                false, false, false, true);
        return new CommRawPacket(header, payload);
    }

    static public CommRawPacket makeDataPacket(byte headerId, byte[] data, int currOffset, short
            size, boolean isEnd, boolean isFile) {
        CommPayloadData payload = new CommPayloadData(data, size);
        CommRawPacketHeader header = new CommRawPacketHeader(headerId, payload.getBytesSize(),
                currOffset, isFile, !isFile, isEnd, false);

        return new CommRawPacket(header, payload);
    }

    public byte[] toByteArray() {
        int resLength = this.getBytesSize();
        byte resBytes[] = new byte[resLength];
        byte headerBytes[] = null;
        byte payloadBytes[] = null;

        // Payload header
        headerBytes = this.mHeader.toByteArray();
        if (headerBytes == null) return null;
        Log.d(TAG, "CommRawPacket");
        Log.d(TAG, "Header: start=0 ~ end=" + (this.mHeader.getBytesSize() - 1) + " / size=" +
                headerBytes.length);
        System.arraycopy(headerBytes, 0, resBytes, 0, this.mHeader.getBytesSize());

        // Payload
        payloadBytes = this.mPayload.toByteArray();
        if (payloadBytes == null) return null;
        Log.d(TAG, "Payload: start=" + this.mHeader.getBytesSize() + " ~ end=" + (this.mHeader
                .getBytesSize() + this.mPayload.getBytesSize()) + " / size=" + payloadBytes.length);
        System.arraycopy(payloadBytes, 0, resBytes, this.mHeader.getBytesSize(), this.mPayload
                .getBytesSize());

        return resBytes;
    }

    public int getBytesSize() {
        return this.mHeader.getBytesSize() + this.mPayload.getBytesSize();
    }
}