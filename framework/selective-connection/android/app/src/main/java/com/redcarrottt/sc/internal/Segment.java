package com.redcarrottt.sc.internal;

/* Copyright (c) 2018, contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong <redcarrottt@gmail.com>
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

import com.redcarrottt.testapp.Logger;

import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.Date;

/*
    Segment is the minimum unit of the sending data through the network.
    Segment header (sequence # + len + flag) is 16bytes (4bytes + 4bytes + 4bytes)
 */
class Segment {
    private final String kTag = "Segment";
    private byte[] mByteArray = null;

    static private final int kSegFlagMoreFragment = 1;
    static private final int kSegFlagControl = 2;
    static private final int kSegFlagAck = 4;

    private final int kOffsetSeqNo = 0;
    private final int kOffsetLength = 4;
    private final int kOffsetFlag = 8;
    private final int kOffsetSendStartTSSec = 12;
    private final int kOffsetSendStartTSUsec = 16;
    private final int kOffsetMediaStartTSSec = 20;
    private final int kOffsetMediaStartTSUsec = 24;
    static public final int kSegHeaderSize = 28;
    static public final int kSegPayloadSize = 512;

    public void initByteArray() {
        this.mByteArray = new byte[kSegHeaderSize + kSegPayloadSize];
    }

    private void setByteArrayForMetadataField(int value, int fieldOffset) {
        if (this.mByteArray == null) {
            Logger.ERR(kTag, "setByteArrayForMetadataField(): Segment byte array not allocated");
            return;
        }

        ByteBuffer fieldBuffer = ByteBuffer.allocate(4);
        fieldBuffer.putInt(value);
        byte[] fieldByteArray = fieldBuffer.array();
        System.arraycopy(fieldByteArray, 0, this.mByteArray, fieldOffset, 4);
    }

    public void setMetadataNormal(int seq_no, int length) {
        // Get timestamp
        Date sendStartTS = new Date();
        long sendStartTSUs = sendStartTS.getTime();
        int sendStartTSSec = (int) (sendStartTSUs / 1000);
        int sendStartTSUsec = (int) (sendStartTSUs % 1000) * 1000;

        // Set metadata fields
        this.setByteArrayForMetadataField(seq_no, kOffsetSeqNo); // seq_no
        this.setByteArrayForMetadataField(length, kOffsetLength); // length
        this.setByteArrayForMetadataField(0, kOffsetFlag); // flag
        // send_start_ts_sec
        this.setByteArrayForMetadataField(sendStartTSSec, kOffsetSendStartTSSec);
        // send_start_ts_usec
        this.setByteArrayForMetadataField(sendStartTSUsec, kOffsetSendStartTSUsec);
        // media_start_ts_sec
        this.setByteArrayForMetadataField(0, kOffsetMediaStartTSSec);
        // media_start_ts_usec
        this.setByteArrayForMetadataField(0, kOffsetMediaStartTSUsec);
    }

    public void setMetadataAck(int seq_no, int length, int flag, int sendStartTSSec, int
            sendStartTSUsec, int mediaStartTSSec, int mediaStartTSUsec) {
        // Set metadata fields
        this.setByteArrayForMetadataField(seq_no, kOffsetSeqNo); // seq_no
        this.setByteArrayForMetadataField(length, kOffsetLength); // length
        this.setByteArrayForMetadataField(flag, kOffsetFlag); // flag
        this.setAckFlag();
        // send_start_ts_sec
        this.setByteArrayForMetadataField(sendStartTSSec, kOffsetSendStartTSSec);
        // send_start_ts_usec
        this.setByteArrayForMetadataField(sendStartTSUsec, kOffsetSendStartTSUsec);
        // media_start_ts_sec
        this.setByteArrayForMetadataField(mediaStartTSSec, kOffsetMediaStartTSSec);
        // media_start_ts_usec
        this.setByteArrayForMetadataField(mediaStartTSUsec, kOffsetMediaStartTSUsec);
    }

    public void setPayloadData(byte[] entire_data_bytes, int offset, int payload_length) {
        System.arraycopy(entire_data_bytes, offset, this.mByteArray, kSegHeaderSize,
                payload_length);
    }

    private void setFlag(int flag) {
        this.setByteArrayForMetadataField(flag, kOffsetFlag);
    }

    public void setMoreFragmentFlag() {
        int flag = this.getFlag();
        this.setFlag(flag | kSegFlagMoreFragment);
    }

    public void setControlFlag() {
        int flag = this.getFlag();
        this.setFlag(flag | kSegFlagControl);
    }

    public void setAckFlag() {
        int flag = this.getFlag();
        this.setFlag(flag | kSegFlagAck);
    }

    public int getSeqNo() {
        return this.getMetadataFieldFromByteArray(kOffsetSeqNo);
    }

    public int getLength() {
        return this.getMetadataFieldFromByteArray(kOffsetLength);
    }

    public int getFlag() {
        return this.getMetadataFieldFromByteArray(kOffsetFlag);
    }

    public boolean isMoreFragment() {
        return ((this.getFlag() & kSegFlagMoreFragment) != 0);
    }

    public boolean isControl() {
        return ((this.getFlag() & kSegFlagControl) != 0);
    }

    public boolean isAck() {
        return ((this.getFlag() & kSegFlagAck) != 0);
    }

    public int getSendStartTSSec() {
        return this.getMetadataFieldFromByteArray(kOffsetSendStartTSSec);
    }

    public int getSendStartTSUsec() {
        return this.getMetadataFieldFromByteArray(kOffsetSendStartTSUsec);
    }

    public int getMediaStartTSSec() {
        return this.getMetadataFieldFromByteArray(kOffsetMediaStartTSSec);
    }

    public int getMediaStartTSUsec() {
        return this.getMetadataFieldFromByteArray(kOffsetMediaStartTSUsec);
    }

    public byte[] getPayloadData() {
        return Arrays.copyOfRange(this.mByteArray, kSegHeaderSize, this.mByteArray.length);
    }

    public int getByteArraySize() {
        return kSegHeaderSize + kSegPayloadSize;
    }

    public byte[] getByteArray() {
        return this.mByteArray;
    }

    private int getMetadataFieldFromByteArray(int offset) {
        ByteBuffer buffer = ByteBuffer.allocate(4);
        buffer.put(this.mByteArray, offset, 4);
        return buffer.getInt(0);
    }

    public void setByteArray(byte[] byteArray) {
        this.mByteArray = byteArray;
    }

    Segment() {
    }
}