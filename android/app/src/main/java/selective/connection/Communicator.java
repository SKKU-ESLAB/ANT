package selective.connection;

import android.os.Handler;
import android.os.Message;
import android.util.Log;
import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.ListIterator;

import static selective.connection.NetworkAdapter.kDevCon;
import static selective.connection.NetworkAdapter.kDevDiscon;
import static selective.connection.NetworkAdapter.kDevDisconnecting;

/**
 * Created by eslab on 2017-02-08.
 */

public class Communicator {
    static private Communicator instance = null;
    static String tag = "Comm";
    private Communicator() {
        SegmentManager sm = SegmentManager.get_instance();
        NetworkManager nm = NetworkManager.get_instance();
    }
    static public Communicator get_instance() {
        if (instance == null)
            instance = new Communicator();
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
        //Log.d(tag, "serialized packet size " + packet_size);

        sent_bytes = ProtocolManager.send_packet(packet_size);
        if (sent_bytes < 0) throw new AssertionError();
        //Log.d(tag, "Sent bytes: " + sent_bytes);

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

    static int parse_header(byte[] serialized, ProtocolData ret_pd) {
        if (serialized == null || ret_pd == null) throw new AssertionError();

        int vec_offset = 0;
        ByteBuffer buffer = ByteBuffer.allocate(2);

        buffer.put(serialized, vec_offset, 2);
        ret_pd.id = buffer.getShort(0);
        vec_offset += 2;
        //Log.d(tag, "ret_pd.id is " + ret_pd.id);

        buffer = ByteBuffer.allocate(4);
        buffer.put(serialized, vec_offset, 4);
        ret_pd.len = buffer.getInt(0);
        vec_offset += 4;
        //Log.d(tag, "ret_pd.len is " + ret_pd.len);

        return vec_offset;
    }

    static public int send_packet(int packet_size) {
        SegmentManager sm = SegmentManager.get_instance();

        return sm.send_to_segment_manager(serialized_vector, packet_size);
    }

    static public int recv_packet(byte[] buf) {
        if (buf == null) throw new AssertionError();

        ProtocolData pd = new ProtocolData();
        SegmentManager sm = SegmentManager.get_instance();

        byte[] data = sm.recv_from_segment_manager(pd);

        System.arraycopy(data, 0, buf, 0, (pd.len < buf.length)? pd.len : buf.length);


        return pd.len;
    }

    private static String tag = "protocol manager";
}

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

    static public final int kSegMxQueueSize = 104857600;
    static public final int kSegSize = 512;
    static public final int kSegThreshold = 1024;
    static public final int kSegQueueThreshold = kSegThreshold / 512;
    static public final int kSegFreeThreshold = 256;

    static public final int kSegHeaderSize = 8;

    static public final int kSegLenOffset = 0;
    static public final int kSegFlagOffset = 15;
    static public final int kSegLenMask = 0x00007fff;
    static public final int kSegFlagMask = 0x00008000;

    static public final int kSegSend = 0;
    static public final int kSegRecv = 1;
    static public final int kSegMaxQueueType = 2;

    static public final short kSegFlagMF = 1;

    private int seq_no;
    public int queue_threshold;

    private int[] next_seq_no;

    private LinkedList<Segment>[] queue;
    private LinkedList<Segment> failed_queue;
    private LinkedList<Segment>[] pending_queue;
    private int[] queue_size;
    private String tag = "SegmentManager";

    private LinkedList<Segment> free_list;
    private int free_list_size;

    public int wfd_state = 0;

    // Macro
    static public int mGetSegLenBits(int x) {
        return (x & kSegLenMask) >> kSegLenOffset;
    }

    static public int mGetSegFlagBits(int x) {
        return (x & kSegFlagMask) >> kSegFlagOffset;
    }

    static public int mSetSegLenBits(int x, int dest) {
        return (dest |= (x << kSegLenOffset) & kSegLenMask);
    }

    static public int mSetSegFlagBits(int x, int dest) {
        return (dest |= (x << kSegFlagOffset) & kSegFlagMask);
    }

    private SegmentManager() {
        queue = new LinkedList[kSegMaxQueueType];
        for (int i=0; i<kSegMaxQueueType; i++) {
            queue[i] = new LinkedList<Segment>();
        }

        failed_queue = new LinkedList<Segment>();
        pending_queue = new LinkedList[kSegMaxQueueType];
        for (int i=0; i<kSegMaxQueueType; i++) {
            pending_queue[i] = new LinkedList<Segment>();
        }

        queue_size = new int[kSegMaxQueueType];
        for (int i=0; i<kSegMaxQueueType; i++) {
            queue_size[i] = 0;
        }

        next_seq_no = new int[kSegMaxQueueType];
        for (int i=0; i<kSegMaxQueueType; i++) {
            next_seq_no[i] = 0;
        }

        free_list = new LinkedList<Segment>();
        seq_no = 0;
        queue_threshold = 0;
    }


    static public SegmentManager get_instance() {
        if (instance == null)
            instance = new SegmentManager();

        return instance;
    }

    int get_seq_no(int len) {
        int ret = seq_no;
        seq_no += len;

        return ret;
    }

    public int send_to_segment_manager(byte[] data, int len) {
        if (data == null || len <= 0) throw new AssertionError();

        int offset = 0;
        int num_of_segments = (int)((len + kSegSize - 1) / kSegSize);
        int allocated_seq_no = get_seq_no(num_of_segments);
        int seg_idx;
        for (seg_idx = 0; seg_idx < num_of_segments; seg_idx++) {
            int seg_len = (len - offset < kSegSize)? (len - offset) : kSegSize;
            Segment seg = get_free_segment();

            seg.flag_len = mSetSegLenBits(seg_len, seg.flag_len);
            seg.seq_no = allocated_seq_no++;

            System.arraycopy(data, offset, seg.data, kSegHeaderSize, seg_len);
            offset += seg_len;

            if (offset < len) seg.flag_len = mSetSegFlagBits(kSegFlagMF, seg.flag_len);

            serialize_segment_header(seg);

            enqueue(kSegSend, seg);
        }

        return 0;
    }

    private void serialize_segment_header(Segment seg) {
        ByteBuffer buffer = ByteBuffer.allocate(4);
        buffer.putInt(seg.seq_no);
        byte[] net_seq_no = buffer.array();

        buffer = ByteBuffer.allocate(4);
        buffer.putInt(seg.flag_len);
        byte[] net_flag_len = buffer.array();

        System.arraycopy(net_seq_no, 0, seg.data, 0, 4);
        System.arraycopy(net_flag_len, 0, seg.data, 4, 4);
    }

    public byte[] recv_from_segment_manager(ProtocolData pd) {
        if (pd == null) throw new AssertionError();

        byte[] serialized = null;
        int offset = 0;
        int data_size = 0;
        boolean cont = false;

        Segment seg = dequeue(kSegRecv);
        ProtocolManager.parse_header(Arrays.copyOfRange(seg.data, kSegHeaderSize, seg.data.length), pd);
        if (pd.len == 0) return null;

        //Log.d(tag, "pd.len is " + pd.len);
        serialized = new byte[pd.len];

        // Handle the first segment of the data bulk, because it contains protocol data
        data_size = mGetSegLenBits(seg.flag_len) - ProtocolManager.kProtHeaderSize;
        System.arraycopy(seg.data, kSegHeaderSize + ProtocolManager.kProtHeaderSize, serialized, offset, data_size);
        offset += data_size;

        cont = (mGetSegFlagBits(seg.flag_len) == kSegFlagMF);
        free_segment(seg);

        while (cont) {
            seg = dequeue(kSegRecv);
            //Log.d(tag, "Dequeing recved data : " + Integer.toString(seg.seq_no));
            data_size = mGetSegLenBits(seg.flag_len);
            System.arraycopy(seg.data, kSegHeaderSize, serialized, offset, data_size);
            cont = (mGetSegFlagBits(seg.flag_len) == kSegFlagMF);
            offset += data_size;
            free_segment(seg);
        }

        if (serialized == null) throw new AssertionError();

        return serialized;
    }

    public void enqueue(int type, Segment seg) {
        if (type >= kSegMaxQueueType) throw new AssertionError();

        synchronized (queue[type]) {
            boolean segment_enqueued = false;

            if (seg.seq_no == next_seq_no[type]) {
                next_seq_no[type]++;
                queue[type].offerLast(seg);
                queue_size[type]++;
                segment_enqueued = true;
            } else {
                if (seg.seq_no < next_seq_no[type]) {
                    Log.d(tag, ((type==kSegSend)? "Sending Queue" : "Recving Queue") + Integer.toString(seg.seq_no) + ":"+ Integer.toString(next_seq_no[type]));
                    throw new AssertionError();
                }

                ListIterator<Segment> it = pending_queue[type].listIterator();
                while (it.hasNext()) {
                    Segment walker = it.next();

                    if (walker.seq_no > seg.seq_no) break;
                }

                it.add(seg);
            }

            ListIterator<Segment> it = pending_queue[type].listIterator();
            while (it.hasNext()) {
                Segment walker = it.next();

                if (walker.seq_no != next_seq_no[type]) break;

                queue[type].offerLast(walker);
                queue_size[type]++;
                next_seq_no[type]++;
                segment_enqueued = true;

                it.remove();
            }

            if (segment_enqueued) {
                //Log.d(tag, "WakeUP!");
                queue[type].notifyAll();
            }
        }

        if (type == kSegSend) {
            /*
            if (queue_size[type] > queue_threshold) {
                NetworkManager.get_instance().increase_adapter();
            } else if (queue_size[type] == 0) {
                NetworkManager.get_instance().decrease_adapter();
            }
            */
        }
    }

    public Segment dequeue(int type) {
        synchronized (queue[type]) {
            if (queue_size[type] == 0) {
                try {
                    //Log.d(tag, "Wating for queue is filled");
                    queue[type].wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            Segment ret = queue[type].pollFirst();
            queue_size[type]--;

            return ret;
        }
    }

    public Segment get_free_segment() {
        Segment ret = null;
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

    public void free_segment_all() {
        synchronized (free_list) {
            release_segment_from_free_list(0);
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
        Segment ret = null;
        synchronized (failed_queue) {
            ret = failed_queue.pollFirst();
        }

        return ret;
    }


}

class NetworkManager {
    static private NetworkManager instance = null;

    static public final int kNetCtrl = 0;
    static public final int kNetData = 1;
    static public final int kNetMaxPort = 2;

    static public final int kNetStatDiscon = 0;
    static public final int kNetStatConnecting = 1;
    static public final int kNetStatControl = 2;
    static public final int kNetStatIncr = 3;
    static public final int kNetStatDecr = 4;
    static public final int kNetStatData = 5;

    static public final int kCtrlReqOk = 0;
    static public final int kCtrlReqFail = 1;
    static public final int kCtrlReqIncr = 2;
    static public final int kCtrlReqDecr = 3;
    static public final int kCtrlReqPriv = 4;

    private String tag = "NetworkManager";

    private class CtrlRecvThread extends Thread {
        private final String tag = "CtrlRecvThread";
        private Handler mHandler = null;

        public CtrlRecvThread(Handler handler) {
            mHandler = handler;
        }
        public void run() {
            byte[] data = new byte[SegmentManager.kSegSize];
            ByteBuffer buffer;

            while (true) {
                NetworkAdapter na = adapter_list[kNetCtrl].peekFirst();
                if (na == null) throw new AssertionError();

                if (na.recv(data, 1) <= 0) {
                    Log.d(tag, "Control adapter might be closed");
                    break;
                } else {
                    Log.d(tag, "Ctrl Msg: " + Byte.toString(data[0]));
                }

                Byte ctrl_req = data[0];

                if (ctrl_req == kCtrlReqIncr) {
                    Log.d(tag, "DataIncr request arrived");
                    if (na.recv(data, 2) <= 0) {
                        Log.d(tag, "Control adapter might be closed");
                        break;
                    }

                    buffer = ByteBuffer.allocate(2);
                    buffer.put(data, 0, 2);

                    short dev_id = buffer.getShort(0);
                    Log.d(tag, "Device ID = " + Short.toString(dev_id));

                    NetworkAdapter data_na = null;

                    ListIterator<NetworkAdapter> it = adapter_list[kNetData].listIterator();
                    while (it.hasNext()) {
                        NetworkAdapter walker = it.next();
                        if (walker.dev_id == dev_id) {
                            data_na = walker;
                            break;
                        }
                    }
                    if (data_na == null) throw new AssertionError();

                    NetworkManager.get_instance().prev_state = NetworkManager.get_instance().state;
                    NetworkManager.get_instance().state = kNetStatIncr;
                    data_na.connect(mDataConnectingHandler);
                } else if (ctrl_req == kCtrlReqDecr) {
                    Log.d(tag, "DataDecr request arrived");
                    if (na.recv(data, 2) <= 0) {
                        Log.d(tag, "Control adapter might be closed");
                        break;
                    }

                    buffer = ByteBuffer.allocate(2);
                    buffer.put(data, 0, 2);

                    short dev_id = buffer.getShort(0);
                    Log.d(tag, "Device ID = " + Short.toString(dev_id));

                    NetworkAdapter data_na = null;

                    ListIterator<NetworkAdapter> it = adapter_list[kNetData].listIterator();
                    while (it.hasNext()) {
                        NetworkAdapter walker = it.next();
                        if (walker.dev_id == dev_id) {
                            data_na = walker;
                            break;
                        }
                    }
                    if (data_na == null) throw new AssertionError();

                    NetworkManager.get_instance().prev_state = NetworkManager.get_instance().state;
                    NetworkManager.get_instance().state = kNetStatDecr;
                    data_na.close(mDataDisconnectingHandler);

                } else if (ctrl_req == kCtrlReqPriv) {
                    short dev_id;
                    int len;
                    int res = -1;
                    do {
                        res = na.recv(data, 2);
                        if (res <= 0) break;
                        buffer = ByteBuffer.allocate(2);

                        buffer.put(data, 0, 2);
                        dev_id = buffer.getShort(0);

                        res = na.recv(data, 4);
                        if (res <= 0) break;

                        buffer = ByteBuffer.allocate(4);
                        buffer.put(data, 0, 4);
                        len = buffer.getInt(0);

                        res = na.recv(data, len);
                        if (res <= 0) break;

                        ListIterator<NetworkAdapter> it = adapter_list[kNetData].listIterator();
                        while (it.hasNext()) {
                            NetworkAdapter walker = it.next();
                            if (walker.dev_id == dev_id) {
                                walker.on_control_recv(data, len);
                                break;
                            }
                        }
                    } while (false);

                    if (res <= 0) {
                        Log.d(tag, "Control adapter might be closed");
                        break;
                    }
                }
            }

            network_closed();
            mHandler.sendEmptyMessage(kNetStatDiscon);
        }
    }

    CtrlRecvThread recv_thread = null;

    private short connecting_dev_id;

    private int state;
    private int prev_state;
    private Handler mDataConnectingHandler;
    private Handler mDataDisconnectingHandler;
    private Handler mCtrlHandler;

    LinkedList<NetworkAdapter> adapter_list[];
    NetworkAdapter connecting_adapter;

    private NetworkManager() {
        connecting_dev_id = 0;
        connecting_adapter = null;
        adapter_list = new LinkedList[kNetMaxPort];

        for (int i=0; i<kNetMaxPort; i++) {
            adapter_list[i] = new LinkedList<NetworkAdapter>();
        }

        state = kNetStatDiscon;
        prev_state = kNetStatDiscon;

        mDataConnectingHandler = new Handler() {
            public void handleMessage(Message msg) {
                NetworkManager nm = NetworkManager.get_instance();

                if (nm.state != kNetStatIncr) throw new AssertionError();

                if (msg.what == NetworkAdapter.kDevCon) {
                    nm.state = kNetStatData;
                    connecting_adapter = null;
                    SegmentManager.get_instance().queue_threshold += SegmentManager.kSegThreshold;
                } else {
                    nm.state = nm.prev_state;
                }
            }
        };

        mDataDisconnectingHandler = new Handler() {
            public void handleMessage(Message msg) {
                NetworkManager nm = NetworkManager.get_instance();

                if(nm.state != kNetStatDecr) throw new AssertionError();

                if(msg.what == NetworkAdapter.kDevDiscon) {
                    if(is_data_adapter_on()){
                        nm.state = kNetStatData;
                    } else {
                        nm.state = kNetStatControl;
                    }
                    connecting_adapter = null;
                    SegmentManager.get_instance().queue_threshold -= SegmentManager.kSegThreshold;
                } else {
                    nm.state = nm.prev_state;
                }
            }
        };

        mCtrlHandler = new Handler() {
            public void handleMessage(Message msg) {
                if (msg.what == kNetStatDiscon) {
                    NetworkManager.get_instance().connect_control_adapter();
                }
            }
        };
    }

    static public NetworkManager get_instance() {
        if (instance == null)
            instance = new NetworkManager();

        return instance;
    }

    public void install_data_adapter(NetworkAdapter na) {
        synchronized (adapter_list[kNetData]) {
            adapter_list[kNetData].offerLast(na);
        }
    }

    public void remove_data_adapter(NetworkAdapter na) {
        synchronized (adapter_list[kNetData]) {
            adapter_list[kNetData].remove(na);
        }
    }

    public void install_control_adapter(NetworkAdapter na) {
        if (state > kNetStatDiscon || adapter_list[kNetCtrl].size() > 0) {
            Log.d(tag, "Control port already added");
            return;
        }

        if (state == kNetStatConnecting) {
            Log.d(tag, "Already connecting control port");
            return;
        }

        adapter_list[kNetCtrl].offerLast(na);
        Log.d(tag, "Connecting control adapter");
        connect_control_adapter();
    }

    public void remove_control_adapter(NetworkAdapter na) {
        adapter_list[kNetCtrl].remove(na);
    }

    public void increase_adapter() {
        if (state == kNetStatIncr || state == kNetStatDecr) {
            Log.d(tag, "Data ports are busy");
            return;
        }

        if (state == kNetStatDiscon) {
            Log.d(tag, "Control port is not opened yet");
            return;
        }

        Log.d(tag, "Increasing data adapter");
        prev_state = state;
        state = kNetStatIncr;

        NetworkAdapter na = select_device();
        if (na == null) {
            state = prev_state;
            Log.d(tag, "All device has been up");
            return;
        }

        byte[] buf = new byte[SegmentManager.kSegSize];
        ByteBuffer buffer = ByteBuffer.allocate(2);

        buffer.putShort(na.dev_id);
        buf[0] = kCtrlReqIncr;
        System.arraycopy(buffer.array(), 0, buf, 1, 2);

        send_control_data(buf, 3);

        connecting_adapter = na;
        na.connect(new Handler() {
            public void handleMessage(Message msg) {
                NetworkManager nm = NetworkManager.get_instance();

                if (nm.state != kNetStatIncr) throw new AssertionError();

                if (msg.what == NetworkAdapter.kDevCon) {
                    nm.state = kNetStatData;
                    connecting_adapter = null;
                    SegmentManager.get_instance().queue_threshold += SegmentManager.kSegThreshold;
                } else {
                    nm.state = nm.prev_state;
                }
            }
        });
    }

    public void decrease_adapter() {
        if (state == kNetStatIncr || state == kNetStatDecr) {
            Log.d(tag, "Data ports are busy");
            return;
        }

        if (state == kNetStatDiscon) {
            Log.d(tag, "Control port is not opened yet");
            return;
        }
        Log.d(tag, "decrease_adapter()");
        prev_state = state;
        state = kNetStatDecr;

        NetworkAdapter na = select_device_on();
        if (na == null) {
            state = prev_state;
            Log.d(tag, "All device has been down");
            return;
        }

        byte[] buf = new byte[SegmentManager.kSegSize];
        ByteBuffer buffer = ByteBuffer.allocate(2);

        buffer.putShort(na.dev_id);
        buf[0] = kCtrlReqDecr;
        System.arraycopy(buffer.array(), 0, buf, 1, 2);

        send_control_data(buf, 3);

        connecting_adapter = na;
        na.close(new Handler() {
            public void handleMessage(Message msg) {
                NetworkManager nm = NetworkManager.get_instance();

                if (nm.state != kNetStatDecr) throw new AssertionError();

                if (msg.what == NetworkAdapter.kDevDiscon) {
                    if(is_data_adapter_on()){
                        nm.state = kNetStatData;
                    } else {
                        nm.state = kNetStatControl;
                    }
                    connecting_adapter = null;
                    SegmentManager.get_instance().queue_threshold -= SegmentManager.kSegThreshold;
                } else {
                    nm.state = nm.prev_state;
                }
            }
        });





    }

    private boolean is_data_adapter_on() {
        ListIterator<NetworkAdapter> iterator  = adapter_list[kNetData].listIterator();
        while (iterator.hasNext()) {
            NetworkAdapter walker = iterator.next();
            if (walker.get_stat() == kDevCon) {
                return true;
            }
        }
        return false;
    }

    private void connect_control_adapter() {
        if (state > kNetStatDiscon) {
            Log.d(tag, "connect_control_adapter():Control port already connected");
            return;
        }

        NetworkAdapter na = adapter_list[kNetCtrl].peekFirst();
        state = kNetStatConnecting;
        Log.d(tag, "Connecting control adapter");

        na.connect(new Handler() {
            public void handleMessage(Message msg) {
                if (msg.what == NetworkAdapter.kDevCon) {
                    Log.d(tag, "Communicator successfully connected");
                    NetworkManager.get_instance().state = kNetStatControl;
                    NetworkManager.get_instance().recv_thread = new CtrlRecvThread(mCtrlHandler);
                    NetworkManager.get_instance().recv_thread.start();
                } else {
                    NetworkManager.get_instance().state = kNetStatDiscon;
                    try {
                        Thread.sleep(500);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                    NetworkManager.get_instance().connect_control_adapter();
                }
            }
        });
    }

    public void send_control_data(byte[] data, int len) {
        if (state < kNetStatControl) {
            Log.d(tag, "Communicator disconnected");
            return;
        }

        if (state != kNetStatIncr && state != kNetStatDecr) {
            Log.d(tag, "Control data must be sent in increasing or decreasing adapters");
            return;
        }

        NetworkAdapter na = adapter_list[kNetCtrl].peekFirst();
        na.send(data, len);
    }

    private void network_closed() {
        NetworkAdapter ca = adapter_list[kNetCtrl].peekFirst();
        ca.stat = kDevDisconnecting;

        ListIterator<NetworkAdapter> it = adapter_list[kNetData].listIterator();
        while (it.hasNext()) {
            NetworkAdapter walker = it.next();

            walker.close(null);
        }

        ca.close_connection();
        ca.stat = kDevDiscon;

        state = kNetStatDiscon;
        prev_state = kNetStatDiscon;
    }

    private NetworkAdapter select_device() {
        NetworkAdapter res = null;
        ListIterator<NetworkAdapter> iterator  = adapter_list[kNetData].listIterator();
        while (iterator.hasNext()) {
            NetworkAdapter walker = iterator.next();

            if (walker.get_stat() == kDevDiscon) {
                res = walker;
                break;
            }
        }
        return res;
    }

    private NetworkAdapter select_device_on() {
        NetworkAdapter res = null;
        ListIterator<NetworkAdapter> iterator  = adapter_list[kNetData].listIterator();
        while (iterator.hasNext()) {
            NetworkAdapter walker = iterator.next();

            if (walker.get_stat() == kDevCon) {
                res = walker;
                break;
            }
        }
        return res;
    }
}
