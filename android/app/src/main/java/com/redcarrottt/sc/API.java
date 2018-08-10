package com.redcarrottt.sc;

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

public class API {
    static private API instance = null;
    static String tag = "Comm";
    private API() {
        SegmentManager sm = SegmentManager.getInstance();
        Core nm = Core.getInstance();
    }
    static public API getInstance() {
        if (instance == null)
            instance = new API();
        return instance;
    }

    public int send_data(byte[] buf, int len) {
        int curr_offset = 0;
        int sent_bytes;

        ProtocolData pd = null;
        int packet_size;

        pd = ProtocolManager.data_to_protocol_data(buf, len);
        if (pd == null) throw new AssertionError();

        packet_size = ProtocolManager.serialize(pd, buf, curr_offset, len);
        if (!(packet_size > 0)) throw new AssertionError();
        //Logger.print(tag, "serialized packet size " + packet_size);

        sent_bytes = ProtocolManager.send_packet(packet_size);
        if (sent_bytes < 0) throw new AssertionError();
        //Logger.print(tag, "Sent bytes: " + sent_bytes);

        return sent_bytes;
    }

    public int recv_data(byte[] buf, int len) {
        int ret;
        int packet_size;

        packet_size = ProtocolManager.recv_packet(buf);
        if (buf == null) throw new AssertionError();

        return packet_size;
    }
}