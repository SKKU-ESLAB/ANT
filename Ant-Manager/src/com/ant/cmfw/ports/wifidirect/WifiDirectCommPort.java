package com.ant.cmfw.ports.wifidirect;

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
            Log.d(TAG, "Try Socket connectChannel");
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