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
    static public final int kProtHeaderSize = 6;

    static private short packet_id;
    static private byte[] serialized_vector = null;
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

        pd.id = packet_id++;
        pd.len = len;
        pd.data = buf;

        return pd;
    }

    static public int serialize(ProtocolData pd, byte[] buf, int offset, int payload_size) {
        //if (serialized_vector != null) throw new AssertionError();

        int vec_size;
        int data_offset;

        data_offset = kProtHeaderSize;
        vec_size = data_offset + payload_size;

        serialized_vector = new byte[vec_size];

        serialize_header(pd, serialized_vector);
        System.arraycopy(buf, offset, serialized_vector, data_offset, payload_size);

        if (serialized_vector == null) throw new AssertionError();

        return vec_size;
    }

    static void parse_header(byte[] serialized, ProtocolData ret_pd) {
        if (serialized == null || ret_pd == null) throw new AssertionError();

        int vec_offset = 0;
        ByteBuffer buffer = ByteBuffer.allocate(2);

        buffer.put(serialized, vec_offset, 2);
        ret_pd.id = buffer.getShort(0);
        vec_offset += 2;
        //Logger.DEBUG(kTag, "ret_pd.id is " + ret_pd.id);

        buffer = ByteBuffer.allocate(4);
        buffer.put(serialized, vec_offset, 4);
        ret_pd.len = buffer.getInt(0);
        //vec_offset += 4;
        //Logger.DEBUG(kTag, "ret_pd.len is " + ret_pd.len);

    }

    static public int send_packet(int packet_size) {
        SegmentManager sm = SegmentManager.getInstance();

        return sm.send_to_segment_manager(serialized_vector, packet_size);
    }

    static public int recv_packet(byte[] buf) {
        if (buf == null) throw new AssertionError();

        ProtocolData pd = new ProtocolData();
        SegmentManager sm = SegmentManager.getInstance();

        byte[] data = sm.recv_from_segment_manager(pd);

        System.arraycopy(data, 0, buf, 0, (pd.len < buf.length)? pd.len : buf.length);


        return pd.len;
    }

//    private static String kTag = "protocol manager";
}