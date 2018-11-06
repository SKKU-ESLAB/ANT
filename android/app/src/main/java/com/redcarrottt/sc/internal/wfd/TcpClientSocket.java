package com.redcarrottt.sc.internal.wfd;

import com.redcarrottt.sc.internal.ClientSocket;
import com.redcarrottt.testapp.Logger;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;

class TcpClientSocket extends ClientSocket {
    private final String kTag = "TcpClientSocket";

    @Override
    protected boolean openImpl() {
        Logger.DEBUG(kTag, "Connect to server " + this.getTargetIpAddress() + ":" + this
                .mTargetPort);

        // Try to open socket
        final int kMaxTries = 10;
        for (int tries = 0; tries < kMaxTries; tries++) {
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            try {
                this.mSocket = new Socket();
                this.mSocket.setReuseAddress(true);
                this.mSocket.bind(null);
                this.mSocket.connect(new InetSocketAddress(this.getTargetIpAddress(), this
                        .mTargetPort));
                this.mInputStream = new BufferedInputStream(this.mSocket.getInputStream());
                this.mOutputStream = new BufferedOutputStream(this.mSocket.getOutputStream());
                break;
            } catch (IOException e) {
                this.mSocket = null;
                this.mInputStream = null;
                this.mOutputStream = null;
                Logger.WARN(kTag, "Try socket open " + tries);
            }
        }

        if (this.mSocket != null && this.mSocket.isConnected()) {
            Logger.VERB(kTag, "Socket open success");
            return true;
        } else {
            Logger.ERR(kTag, "Socket open failed!");
            this.mSocket = null;
            return false;
        }
    }

    @Override
    protected boolean closeImpl() {
        if (this.mSocket != null && this.mSocket.isConnected()) {
            try {
                this.mInputStream.close();
                this.mOutputStream.close();
                this.mSocket.close();
                Logger.VERB(kTag, "Socket closed");
                return true;
            } catch (IOException e) {
                return false;
            }
        }
        return true;
    }

    @Override
    protected int sendImpl(byte[] dataBuffer, int dataLength) {
        if (this.mSocket == null || !this.mSocket.isConnected() || this.mOutputStream == null) {
            Logger.WARN(kTag, "Send - Socket closed! 1");
            return -1;
        }
        try {
            this.mOutputStream.write(dataBuffer, 0, dataLength);
            this.mOutputStream.flush();
            return dataLength;
        } catch (IOException e) {
            Logger.WARN(kTag, "Send - Socket closed! 2 / " + e.getMessage());
            return -2;
        }
    }

    @Override
    protected int receiveImpl(byte[] dataBuffer, int dataLength) {
        if (this.mSocket == null || !this.mSocket.isConnected() || this.mInputStream == null) {
            Logger.WARN(kTag, "Receive - Socket closed! 1");
            return -1;
        }
        try {
            int receivedBytes = 0;
            while (receivedBytes < dataLength) {
                int onceReceivedBytes = this.mInputStream.read(dataBuffer, receivedBytes,
                        dataLength - receivedBytes);
                if (onceReceivedBytes < 0) {
                    Logger.ERR(kTag, "Receive failed! 1");
                    return -2;
                }
                receivedBytes += onceReceivedBytes;
            }
            return dataLength;
        } catch (IOException e) {
            Logger.WARN(kTag, "Receive - Socket closed! 2 / " + e.getMessage());
            return -3;
        }
    }

    // Constructor
    TcpClientSocket(int targetPort) {
        this.mTargetPort = targetPort;
        this.mIsInfoSet = false;
        this.mInfoSetTrigger = new Object();
    }

    public void setTcpClientInfo(String targetIpAddress) {
        this.mTargetIpAddress = targetIpAddress;

        Logger.DEBUG(kTag, "Notify: WfdP2PInfo");
        this.mIsInfoSet = true;
        synchronized (this.mInfoSetTrigger) {
            this.mInfoSetTrigger.notifyAll();
        }
    }

    private String getTargetIpAddress() {
        // Wait until wps pin is set, then get the information.
        if (this.mIsInfoSet) {
            return this.mTargetIpAddress;
        } else {
            Logger.DEBUG(kTag, "Waiting: mTargetIpAddress");
            while (!this.mIsInfoSet) {
                synchronized (this.mInfoSetTrigger) {
                    try {
                        this.mInfoSetTrigger.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
            return this.mTargetIpAddress;
        }
    }

    // Components
    private Socket mSocket;
    private BufferedInputStream mInputStream;
    private BufferedOutputStream mOutputStream;

    // TODO: adding port to WFD info would be good.
    // Attributes
    private int mTargetPort;

    // TCP Socket Info Attributes
    private boolean mIsInfoSet;
    private final Object mInfoSetTrigger;
    private String mTargetIpAddress;
}
