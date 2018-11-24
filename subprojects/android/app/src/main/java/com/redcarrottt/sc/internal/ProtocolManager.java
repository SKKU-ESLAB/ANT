package com.redcarrottt.sc.internal;

/* Copyright (c) 2017-2018. All rights reserved.
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *  Eunsoo Park (esevan.park@gmail.com)
 *  Injung Hwang (sinban04@gmail.com)
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

import java.nio.ByteBuffer;

class ProtocolData {
    short id;
    int len;
    byte[] data;

    ProtocolData() {
        id = 0;
        len = 0;
        data = null;
    }
}

class ProtocolManager {
    static public final int kProtocolHeaderSize = 6;

    static private short sPacketId;
    static private byte[] sSerializedVector = null;

    static private void serialize_header(ProtocolData pd, byte[] vec_ptr) {
        if (vec_ptr == null) throw new AssertionError();

        int vec_offset = 0;

        ByteBuffer buffer = ByteBuffer.allocate(2);

        buffer.putShort(pd.id);
        byte[] net_id = buffer.array();

        buffer = ByteBuffer.allocate(4);
        buffer.putInt(pd.len);
        byte[] net_len = buffer.array();

        System.arraycopy(net_id, 0, vec_ptr, vec_offset, 2);
        vec_offset += 2;

        System.arraycopy(net_len, 0, vec_ptr, vec_offset, 4);
    }

    static public ProtocolData data_to_protocol_data(byte[] buf, int len) {
        ProtocolData pd = new ProtocolData();

        pd.id = sPacketId++;
        pd.len = len;
        pd.data = buf;

        return pd;
    }

    static public int serialize(ProtocolData protocolData, byte[] buf, int offset, int
            payload_size) {
        int vec_size;
        int data_offset;

        data_offset = kProtocolHeaderSize;
        vec_size = data_offset + payload_size;

        sSerializedVector = new byte[vec_size];

        serialize_header(protocolData, sSerializedVector);
        System.arraycopy(buf, offset, sSerializedVector, data_offset, payload_size);

        if (sSerializedVector == null) throw new AssertionError();

        return vec_size;
    }

    static void parse_header(byte[] serialized, ProtocolData protocolData) {
        if (serialized == null || protocolData == null) throw new AssertionError();

        int vec_offset = 0;
        ByteBuffer buffer = ByteBuffer.allocate(2);

        buffer.put(serialized, vec_offset, 2);
        protocolData.id = buffer.getShort(0);
        vec_offset += 2;

        buffer = ByteBuffer.allocate(4);
        buffer.put(serialized, vec_offset, 4);
        protocolData.len = buffer.getInt(0);
    }

    static public int send_packet(int packetSize, boolean isControl) {
        SegmentManager sm = SegmentManager.singleton();

        return sm.send_to_segment_manager(sSerializedVector, packetSize, isControl);
    }

    static public int recv_packet(byte[] buf, boolean isControl) {
        if (buf == null) throw new AssertionError();

        ProtocolData protocolData = new ProtocolData();
        SegmentManager sm = SegmentManager.singleton();

        byte[] data = sm.recv_from_segment_manager(protocolData, isControl);

        System.arraycopy(data, 0, buf, 0, (protocolData.len < buf.length) ? protocolData.len :
                buf.length);


        return protocolData.len;
    }

//    private static String kTag = "protocol manager";
}