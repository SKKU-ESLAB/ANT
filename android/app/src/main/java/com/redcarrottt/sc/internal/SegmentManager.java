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

import com.redcarrottt.testapp.Logger;

import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.ListIterator;

/*
    Segment is the minimum unit of the sending data through the network.
    Segment header (sequence # + flag_len) is 8bytes (4bytes + 4bytes)
 */
class Segment {
    int seq_no;
    int flag_len;
    byte[] data;

    Segment() {
        seq_no = -1;
        flag_len = 0;
        data = new byte[SegmentManager.kSegSize + SegmentManager.kSegHeaderSize];
    }
}

class SegmentManager {
    static private SegmentManager instance = null;
    static public final int kSegSize = 512;
    private static final int kSegFreeThreshold = 256;

    static public final int kSegHeaderSize = 8;

    // kSegLenOffset: not used
    // private static final int kSegLenOffset = 0;
    private static final int kSegFlagOffset = 14;
    private static final int kSegLenMask = 0x00003fff;
    private static final int kSegFlagMask = 0x0000C000;

    static final int kSegSendData = 0;
    static final int kSegRecvData = 1;
    static final int kSegSendControl = 2;
    static final int kSegRecvControl = 3;
    private static final int kSegMaxQueueType = 4;

    static final short kSegFlagMF = 1;
    static final short kSegFlagControl = 2;

    private int seq_no;
    private int[] next_seq_no;

    private LinkedList[] queue;
    private final LinkedList<Segment> failed_queue;
    private LinkedList[] pending_queue;
    private int[] queue_size;
    private static String kTag = "SegmentManager";

    private final LinkedList<Segment> free_list;
    private int free_list_size;

    // Macro
    private static int mGetSegLenBits(int x) {
        return (x & kSegLenMask);
    }

    public static int mGetSegFlagBits(int x) {
        return (x & kSegFlagMask) >> kSegFlagOffset;
    }

    private static int mSetSegLenBits(int x, int dest) {
        return (dest | (x & kSegLenMask));
    }

    private static int mSetSegFlagBits(int x, int dest) {
        return (dest | ((x << kSegFlagOffset) & kSegFlagMask));
    }

    private SegmentManager() {
        queue = new LinkedList[kSegMaxQueueType];
        for (int i = 0; i < kSegMaxQueueType; i++) {
            queue[i] = new LinkedList<Segment>();
        }

        failed_queue = new LinkedList<Segment>();
        pending_queue = new LinkedList[kSegMaxQueueType];
        for (int i = 0; i < kSegMaxQueueType; i++) {
            pending_queue[i] = new LinkedList<Segment>();
        }

        queue_size = new int[kSegMaxQueueType];
        for (int i = 0; i < kSegMaxQueueType; i++) {
            queue_size[i] = 0;
        }

        next_seq_no = new int[kSegMaxQueueType];
        for (int i = 0; i < kSegMaxQueueType; i++) {
            next_seq_no[i] = 0;
        }

        free_list = new LinkedList<Segment>();
        seq_no = 0;
    }


    static public SegmentManager singleton() {
        if (instance == null) instance = new SegmentManager();

        return instance;
    }

    private int get_seq_no(int length) {
        int ret = seq_no;
        seq_no += length;

        return ret;
    }

    public int send_to_segment_manager(byte[] data, int length, boolean isControl) {
        if (data == null || length <= 0) throw new AssertionError();

        int offset = 0;
        int num_of_segments = (length + kSegSize - 1) / kSegSize;
        int allocated_seq_no = get_seq_no(num_of_segments);
        int seg_idx;
        for (seg_idx = 0; seg_idx < num_of_segments; seg_idx++) {
            int seg_len = (length - offset < kSegSize) ? (length - offset) : kSegSize;
            Segment seg = get_free_segment();

            seg.flag_len = mSetSegLenBits(seg_len, seg.flag_len);
            seg.seq_no = allocated_seq_no++;

            System.arraycopy(data, offset, seg.data, kSegHeaderSize, seg_len);
            offset += seg_len;

            int flag = 0;
            if (offset < length) flag |= kSegFlagMF;
            if (isControl) flag |= kSegFlagControl;
            seg.flag_len = mSetSegFlagBits(flag, seg.flag_len);

            serialize_segment_header(seg);

            if (isControl) {
                enqueue(kSegSendControl, seg);
            } else {
                enqueue(kSegSendData, seg);
            }
        }

        return 0;
    }

    private void serialize_segment_header(Segment segment) {
        ByteBuffer buffer = ByteBuffer.allocate(4);
        buffer.putInt(segment.seq_no);
        byte[] net_seq_no = buffer.array();

        buffer = ByteBuffer.allocate(4);
        buffer.putInt(segment.flag_len);
        byte[] net_flag_len = buffer.array();

        System.arraycopy(net_seq_no, 0, segment.data, 0, 4);
        System.arraycopy(net_flag_len, 0, segment.data, 4, 4);
    }

    public byte[] recv_from_segment_manager(ProtocolData protocolData, boolean isControl) {
        if (protocolData == null) throw new AssertionError();

        byte[] serialized;
        int offset = 0;
        int data_size;
        boolean cont;

        Segment seg;
        if (isControl) {
            seg = dequeue(kSegRecvControl);
        } else {
            seg = dequeue(kSegRecvData);
        }
        ProtocolManager.parse_header(Arrays.copyOfRange(seg.data, kSegHeaderSize, seg.data
                .length), protocolData);
        if (protocolData.len == 0) return null;

        //Logger.DEBUG(kTag, "pd.len is " + pd.len);
        serialized = new byte[protocolData.len];

        // Handle the first segment of the data bulk, because it contains protocol data
        data_size = mGetSegLenBits(seg.flag_len) - ProtocolManager.kProtocolHeaderSize;
        System.arraycopy(seg.data, kSegHeaderSize + ProtocolManager.kProtocolHeaderSize, serialized,
                offset, data_size);
        offset += data_size;

        cont = ((mGetSegFlagBits(seg.flag_len) & kSegFlagMF) != 0);
        free_segment(seg);

        while (cont) {
            if (isControl) {
                seg = dequeue(kSegRecvControl);
            } else {
                seg = dequeue(kSegRecvData);
            }
            data_size = mGetSegLenBits(seg.flag_len);
            System.arraycopy(seg.data, kSegHeaderSize, serialized, offset, data_size);
            cont = ((mGetSegFlagBits(seg.flag_len) & kSegFlagMF) != 0);
            offset += data_size;
            free_segment(seg);
        }

        return serialized;
    }

    public void enqueue(int type, Segment segment) {
        if (type >= kSegMaxQueueType) throw new AssertionError();

        synchronized (queue[type]) {
            boolean segment_enqueued = false;

            if (segment.seq_no == next_seq_no[type]) {
                next_seq_no[type]++;
                queue[type].offerLast(segment);
                queue_size[type]++;
                segment_enqueued = true;
            } else {
                if (segment.seq_no < next_seq_no[type]) {
                    // If duplicated data comes, ignore it.
                    Logger.DEBUG(kTag, "Sequence No Error: (" + type + ") incoming=" + segment
                            .seq_no + " / expected_next=" + next_seq_no[type]);
                    return;
                }

                ListIterator it = pending_queue[type].listIterator();
                while (it.hasNext()) {
                    Segment walker = (Segment) it.next();

                    if (walker.seq_no > segment.seq_no) break;
                }

                it.add(segment);
            }

            ListIterator it = pending_queue[type].listIterator();
            while (it.hasNext()) {
                Segment walker = (Segment) it.next();

                if (walker.seq_no != next_seq_no[type]) break;

                queue[type].offerLast(walker);
                queue_size[type]++;
                next_seq_no[type]++;
                segment_enqueued = true;

                it.remove();
            }

            if (segment_enqueued) {
                //Logger.DEBUG(kTag, "WakeUP!");
                queue[type].notifyAll();
            }
        }
    }

    public Segment dequeue(int type) {
        synchronized (queue[type]) {
            if (queue_size[type] == 0) {
                try {
                    queue[type].wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            Segment ret = (Segment) queue[type].pollFirst();
            queue_size[type]--;

            return ret;
        }
    }

    public Segment get_free_segment() {
        Segment ret;
        synchronized (free_list) {
            if (free_list_size == 0) {
                ret = new Segment();
            } else {
                ret = free_list.pop();
                free_list_size--;
            }

            if (ret == null) throw new AssertionError();

            ret.seq_no = -1;
            ret.flag_len = 0;
        }
        return ret;
    }

    public void free_segment(Segment seg) {
        synchronized (free_list) {
            free_list.push(seg);
            free_list_size++;

            if (free_list_size > kSegFreeThreshold) {
                release_segment_from_free_list(kSegFreeThreshold / 2);
            }
        }
    }

    private void release_segment_from_free_list(int threshold) {
        while (free_list_size > threshold) {
            free_list.pop();
            free_list_size--;
        }
    }

    public void failed_sending(Segment seg) {
        synchronized (failed_queue) {
            failed_queue.offerLast(seg);
        }
    }

    public Segment get_failed_sending() {
        Segment ret;
        synchronized (failed_queue) {
            ret = failed_queue.pollFirst();
        }

        return ret;
    }
}