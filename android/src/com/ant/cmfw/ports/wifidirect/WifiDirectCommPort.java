package com.ant.cmfw.ports.wifidirect;

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

import com.ant.cmfw.ports.CommPort;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;

public class WifiDirectCommPort extends CommPort {
    static private String TAG = "WifiDirectCommPort";
    static private int kTimeoutMillisecs = 20000;

    private boolean mIsOpened = false;
    private Socket mSocket = null;

    private String mIPAddress = "";
    private int mTcpPortNum;

    public WifiDirectCommPort(String portName, int tcpPortNum) {
        super(portName);
        this.mTcpPortNum = tcpPortNum;
    }

    public boolean isOpened() {
        return this.mIsOpened;
    }

    public boolean open(String ipAddress) {
        this.mIPAddress = ipAddress;
        this.mSocket = new Socket();

        try {
            this.mSocket.bind(null);
            Log.d(TAG, "Try Socket connectChannel : (ip=" + ipAddress + ", port=" + this
                    .mTcpPortNum + ")");
            this.mSocket.connect((new InetSocketAddress(ipAddress, this.mTcpPortNum)),
                    kTimeoutMillisecs);
        } catch (IOException e) {
            e.printStackTrace();
            this.mSocket = null;
            return false;
        }

        if (this.mSocket.isConnected()) {
            Log.d(TAG, "Connected to device");
            this.mIsOpened = true;
            return true;
        } else {
            return false;
        }
    }

    public void close() {
        if (this.isOpened()) {
            try {
                if (this.mSocket != null) this.mSocket.close();
                Log.d(TAG, "WFD Port closed");
            } catch (IOException e) {
                e.printStackTrace();
            }
            this.mSocket = null;
        }

        this.mIsOpened = false;
    }

    protected OutputStream getOutputStream() {
        if (this.isOpened()) {
            OutputStream outputStream;
            try {
                outputStream = this.mSocket.getOutputStream();
            } catch (IOException e) {
                e.printStackTrace();
                outputStream = null;
            }
            return outputStream;
        } else {
            return null;
        }
    }

    protected InputStream getInputStream() {
        if (this.isOpened()) {
            InputStream inputStream;
            try {
                inputStream = this.mSocket.getInputStream();
            } catch (IOException e) {
                e.printStackTrace();
                inputStream = null;
            }
            return inputStream;
        } else {
            return null;
        }
    }

    public String getIPAddress() {
        return this.mIPAddress;
    }
}