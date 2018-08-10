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

import android.os.Handler;

import com.redcarrottt.testapp.Logger;

import java.nio.ByteBuffer;

import static com.redcarrottt.sc.SegmentManager.kSegHeaderSize;
import static com.redcarrottt.sc.SegmentManager.kSegRecv;
import static com.redcarrottt.sc.SegmentManager.kSegSend;
import static com.redcarrottt.sc.SegmentManager.kSegSize;

public abstract class NetworkAdapter {
    static public final int kDevOff = 0;
    static public final int kDevOn = 1;
    static public final int kDevDiscon = 2;
    static public final int kDevDisconnecting = 3;
    static public final int kDevConnecting = 4;
    static public final int kDevCon = 5;

    static public final int kATuninitialized = 0;
    static public final int kATInitialized = 1;
    static public final int kATCtrlable = 2;
    static public final int kATCtrl = 4;

    static public final int kBluetooth = 1;
    static public final int kWifi = 2;
    static public final int kWifiDirect = 3;

    private int sender_semaphore = 0;
    private int recver_semaphore = 0;
    private Object sender_lock;
    private Object recver_lock;

    public abstract boolean device_on();

    public abstract boolean device_off();

    public abstract boolean make_connection();

    public abstract boolean close_connection();

    public abstract int send(byte[] buf, int len);

    public abstract int recv(byte[] buf, int len);

    public abstract void on_control_recv(byte[] buf, int len);

    private class SenderThread extends Thread {
        public void run() {
            SegmentManager sm = SegmentManager.getInstance();
            if (dev_type == kWifiDirect) {
                sm.wfd_state = 1;
            }

            if (dev_type == kBluetooth) {
                Logger.print(tag, "start bluetooth sender thread");
            } else if (dev_type == kWifiDirect) {
                Logger.print(tag, "start WiFi direct sender thread");
            }

            while (true) {
                if (dev_type == kBluetooth) {
                    while (sm.wfd_state == 1) {
                        try {
                            //Logger.print(tag, "WFD is on. stop bluetooth
                            // sender thread");
                            sleep(1000);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }

                if (sender_semaphore == 1) {
                    Logger.print(tag, "sender semaphore is 1. stop sender " +
                            "thread");
                    if (dev_type == kWifiDirect) {
                        sm.wfd_state = 0;
                    }
                    break;
                }

                Segment to_send = sm.dequeue(kSegSend);
                if (to_send != null) {
                    int len = kSegSize + kSegHeaderSize;
                    byte[] data = to_send.data;
                    int res = send(data, len);
                    if (res < len) {
                        sm.enqueue(kSegSend, to_send);
                        if (res <= 0) break;
                    }
                    sm.free_segment(to_send);
                }
            }
            synchronized (sender_lock) {
                sender_lock.notify();
            }
        }
    }

    private class RecverThread extends Thread {
        public void run() {
            SegmentManager sm = SegmentManager.getInstance();
            Segment free_seg = sm.get_free_segment();

            if (dev_type == kBluetooth) {
                Logger.print(tag, "start bluetooth recver thread");
            } else if (dev_type == kWifiDirect) {
                Logger.print(tag, "start WiFi direct recver thread");
            }

            while (true) {
                /*
                if(dev_type == kBluetooth){

                    while(sm.wfd_state == 1){
                        try {
                            Logger.print(tag, "WFD is on. stop bluetooth
                            recver thread");
                            sleep(1000);
                        } catch (Exception e){
                            e.printStackTrace();
                        }
                    }
                }*/

                if (recver_semaphore == 1) {
                    Logger.print(tag, "recver semaphore is 1. stop the " +
                            "recver thread");
                    break;
                }

                int len = kSegSize + kSegHeaderSize;
                //Logger.print(tag, "Recving thread start:"+Integer.toString
                // (dev_id));
                int res = recv(free_seg.data, len);
                if (res < len) {
                    Logger.print(tag, "Recving failed");
                    sm.free_segment(free_seg);

                    if (res <= 0) {
                        break;
                    }
                } else {
                    //Logger.print(tag, "Recved : " + Short.toString(dev_id));
                }

                ByteBuffer buffer = ByteBuffer.allocate(4);
                buffer.put(free_seg.data, 0, 4);
                free_seg.seq_no = buffer.getInt(0);
                //Logger.print(tag, "Recved Seq No : " + Integer.toString
                // (free_seg.seq_no));

                buffer = ByteBuffer.allocate(4);
                buffer.put(free_seg.data, 4, 4);
                free_seg.flag_len = buffer.getInt(0);
                //Logger.print(tag, "Recved flag_len: " + Integer.toString
                // (free_seg.flag_len));

                sm.enqueue(kSegRecv, free_seg);
                free_seg = sm.get_free_segment();
            }
            try {
                sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            synchronized (recver_lock) {
                recver_lock.notify();
            }
        }
    }

    private class ConnectingThread extends Thread {
        Handler mHandler;

        ConnectingThread(Handler handler) {
            mHandler = handler;
        }

        public void run() {
            if (stat != kDevConnecting) return;

            boolean res = make_connection();

            if (res) {
                if ((at & kATCtrl) == 0) {
                    if (send_thread != null) send_thread.interrupt();

                    send_thread = new SenderThread();
                    send_thread.start();

                    if (recv_thread != null) recv_thread.interrupt();

                    recv_thread = new RecverThread();
                    recv_thread.start();
                } else {
                    if (send_thread != null) send_thread.interrupt();

                    if (recv_thread != null) recv_thread.interrupt();

                    send_thread = null;
                    recv_thread = null;
                }

                stat = kDevCon;
            } else {
                stat = kDevDiscon;
            }

            mHandler.sendEmptyMessage(stat);
        }
    }

    private class ClosingThread extends Thread {
        Handler mHandler;

        ClosingThread(Handler handler) {
            mHandler = handler;
        }

        public void run() {
            if (stat != kDevDisconnecting) return;

            if (send_thread != null) {
                sender_semaphore = 1;
                try {
                    synchronized (sender_lock) {
                        sender_lock.wait();
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
                send_thread.interrupt();
                send_thread = null;
                Logger.print(tag, "sender thread stopped ");
                sender_semaphore = 0;
            }
            if (recv_thread != null) {
                recver_semaphore = 1;
                try {
                    synchronized (recver_lock) {
                        recver_lock.wait();
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }

                recv_thread.interrupt();
                recv_thread = null;
                Logger.print(tag, "recver thread stopped ");
                recver_semaphore = 0;
            }

            boolean res = close_connection();

            if (res) stat = kDevDiscon;
            else stat = kDevCon;

            if (mHandler != null) mHandler.sendEmptyMessage(stat);
        }
    }

    private String tag = "NetworkAdapter";

    private SenderThread send_thread;
    private RecverThread recv_thread;
    private ConnectingThread connect_thread;
    private ClosingThread close_thread;

    public int dev_type;
    public short dev_id = 0;
    public int stat;
    private int at;

    NetworkAdapter() {
        at = kATuninitialized;
        stat = kDevDiscon;
        dev_id = 0;

        send_thread = null;
        recv_thread = null;
        connect_thread = null;
        close_thread = null;

        sender_lock = new Object();
        recver_lock = new Object();
        dev_type = 0;
    }

    public final void set_controllable() {
        at |= kATCtrlable;
    }

    public final void set_data_adapter() {
        if ((at & kATInitialized) == kATInitialized) {
            Logger.print(tag, "Already initialized");
            return;
        }

        Core.getInstance().install_data_adapter(this);
        at |= kATInitialized;
    }

    public final void set_control_adapter() {
        if ((at & kATInitialized) == kATInitialized) {
            Logger.print(tag, "Already initialized");
            return;
        }

        if ((at & kATCtrlable) == 0) {
            Logger.print(tag, "Not controllable adapter");
            return;
        }
        at |= kATCtrl | kATInitialized;

        Core.getInstance().install_control_adapter(this);
    }

    public void connect(Handler handler) {
        if (at == kATuninitialized) {
            Logger.print(tag, "The adapter has not been installed to " +
                    "core");
            return;
        }

        if (stat != kDevDiscon) {
            Logger.print(tag, "The adapter is not disconnected");
            return;
        }

        stat = kDevConnecting;
        if (connect_thread != null) {
            connect_thread.interrupt();
        }

        connect_thread = new ConnectingThread(handler);
        connect_thread.start();
    }

    public void close(Handler handler) {
        if (stat != kDevCon) {
            Logger.print(tag, "The adapter is not connected");
            return;
        }

        Logger.print(tag, "close adapter");
        stat = kDevDisconnecting;
        if (close_thread != null) {
            close_thread.interrupt();
        }

        close_thread = new ClosingThread(handler);
        close_thread.start();
    }

    public int get_stat() {
        return stat;
    }

    public void send_ctrl_msg(byte[] data, int len) {
        if ((at & kATCtrl) == kATCtrl) {
            Logger.print(tag, "Cannot transfer private data in control " +
                    "adapter");
            return;
        }

        Core core = Core.getInstance();
        ByteBuffer buffer = ByteBuffer.allocate(1);
        final int kCtrlReqPriv = 2;
        buffer.put((byte) kCtrlReqPriv);

        core.sendControlMessage(buffer.array(), 1);

        buffer = ByteBuffer.allocate(2);
        buffer.putShort(dev_id);
        core.sendControlMessage(buffer.array(), 2);

        buffer = ByteBuffer.allocate(4);
        buffer.putInt(len);
        core.sendControlMessage(buffer.array(), 4);

        core.sendControlMessage(data, len);
    }
}
