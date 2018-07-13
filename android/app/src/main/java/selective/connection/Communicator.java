package selective.connection;

import android.os.Handler;
import android.os.Message;

import java.nio.ByteBuffer;
import java.util.LinkedList;
import java.util.ListIterator;

import kr.ac.skku.nyx.selectiveconnection.LogBroadcastSender;

import static selective.connection.NetworkAdapter.kDevCon;
import static selective.connection.NetworkAdapter.kDevDiscon;
import static selective.connection.NetworkAdapter.kDevDisconnecting;

/**
 * Created by eslab on 2017-02-08.
 */

public class Core {
    static private Core instance = null;
    static String tag = "Comm";
    private Core() {
        SegmentManager sm = SegmentManager.get_instance();
        NetworkManager nm = NetworkManager.get_instance();
    }
    static public Core get_instance() {
        if (instance == null)
            instance = new Core();
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
        //LogBroadcastSender.sendLogMessage(tag, "serialized packet size " + packet_size);

        sent_bytes = ProtocolManager.send_packet(packet_size);
        if (sent_bytes < 0) throw new AssertionError();
        //LogBroadcastSender.sendLogMessage(tag, "Sent bytes: " + sent_bytes);

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
                    LogBroadcastSender.sendLogMessage(tag, "Control adapter might be closed");
                    break;
                } else {
                    LogBroadcastSender.sendLogMessage(tag, "Ctrl Msg: " + Byte.toString(data[0]));
                }

                Byte ctrl_req = data[0];

                if (ctrl_req == kCtrlReqIncr) {
                    LogBroadcastSender.sendLogMessage(tag, "DataIncr request arrived");
                    if (na.recv(data, 2) <= 0) {
                        LogBroadcastSender.sendLogMessage(tag, "Control adapter might be closed");
                        break;
                    }

                    buffer = ByteBuffer.allocate(2);
                    buffer.put(data, 0, 2);

                    short dev_id = buffer.getShort(0);
                    LogBroadcastSender.sendLogMessage(tag, "Device ID = " + Short.toString(dev_id));

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
                    LogBroadcastSender.sendLogMessage(tag, "DataDecr request arrived");
                    if (na.recv(data, 2) <= 0) {
                        LogBroadcastSender.sendLogMessage(tag, "Control adapter might be closed");
                        break;
                    }

                    buffer = ByteBuffer.allocate(2);
                    buffer.put(data, 0, 2);

                    short dev_id = buffer.getShort(0);
                    LogBroadcastSender.sendLogMessage(tag, "Device ID = " + Short.toString(dev_id));

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
                        LogBroadcastSender.sendLogMessage(tag, "Control adapter might be closed");
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
            LogBroadcastSender.sendLogMessage(tag, "Control port already added");
            return;
        }

        if (state == kNetStatConnecting) {
            LogBroadcastSender.sendLogMessage(tag, "Already connecting control port");
            return;
        }

        adapter_list[kNetCtrl].offerLast(na);
        LogBroadcastSender.sendLogMessage(tag, "Connecting control adapter");
        connect_control_adapter();
    }

    public void remove_control_adapter(NetworkAdapter na) {
        adapter_list[kNetCtrl].remove(na);
    }

    public void increase_adapter() {
        if (state == kNetStatIncr || state == kNetStatDecr) {
            LogBroadcastSender.sendLogMessage(tag, "Data ports are busy");
            return;
        }

        if (state == kNetStatDiscon) {
            LogBroadcastSender.sendLogMessage(tag, "Control port is not opened yet");
            return;
        }

        LogBroadcastSender.sendLogMessage(tag, "Increasing data adapter");
        prev_state = state;
        state = kNetStatIncr;

        NetworkAdapter na = select_device();
        if (na == null) {
            state = prev_state;
            LogBroadcastSender.sendLogMessage(tag, "All device has been up");
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
            LogBroadcastSender.sendLogMessage(tag, "Data ports are busy");
            return;
        }

        if (state == kNetStatDiscon) {
            LogBroadcastSender.sendLogMessage(tag, "Control port is not opened yet");
            return;
        }
        LogBroadcastSender.sendLogMessage(tag, "decrease_adapter()");
        prev_state = state;
        state = kNetStatDecr;

        NetworkAdapter na = select_device_on();
        if (na == null) {
            state = prev_state;
            LogBroadcastSender.sendLogMessage(tag, "All device has been down");
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
            LogBroadcastSender.sendLogMessage(tag, "connect_control_adapter():Control port already connected");
            return;
        }

        NetworkAdapter na = adapter_list[kNetCtrl].peekFirst();
        state = kNetStatConnecting;
        LogBroadcastSender.sendLogMessage(tag, "Connecting control adapter");

        na.connect(new Handler() {
            public void handleMessage(Message msg) {
                if (msg.what == NetworkAdapter.kDevCon) {
                    LogBroadcastSender.sendLogMessage(tag, "Core successfully connected");
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
            LogBroadcastSender.sendLogMessage(tag, "Core disconnected");
            return;
        }

        if (state != kNetStatIncr && state != kNetStatDecr) {
            LogBroadcastSender.sendLogMessage(tag, "Control data must be sent in increasing or decreasing adapters");
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
