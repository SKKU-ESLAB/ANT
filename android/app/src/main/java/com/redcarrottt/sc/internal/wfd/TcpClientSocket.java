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
        Logger.DEBUG(kTag, "Connect to server " + this.mTargetIpAddress + ":" + this.mTargetPort);

        // Try to open socket
        final int kMaxTries = 2;
        for (int tries = 0; tries < kMaxTries; tries++) {
            try {
                this.mSocket = new Socket();
                this.mSocket.setReuseAddress(true);
                this.mSocket.bind(null);
                this.mSocket.connect(new InetSocketAddress(this.mTargetIpAddress, this
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

        if (this.mSocket.isConnected()) {
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
            Logger.ERR(kTag, "Socket closed! Send failed!");
            return -1;
        }
        try {
            this.mOutputStream.write(dataBuffer, 0, dataLength);
            this.mOutputStream.flush();
            return dataLength;
        } catch (IOException e) {
            Logger.ERR(kTag, "Send failed!");
            return -2;
        }
    }

    @Override
    protected int receiveImpl(byte[] dataBuffer, int dataLength) {
        if (this.mSocket == null || !this.mSocket.isConnected() || this.mInputStream == null) {
            Logger.ERR(kTag, "Socket closed! Receive failed!");
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
            Logger.ERR(kTag, "Receive failed! 2");
            return -3;
        }
    }

    // TODO: targetIpAddress and targetPort is hard-coded.
    // TODO: It can be transferred through "priv noti request".

    // Constructor
    TcpClientSocket(String targetIpAddress, int targetPort) {
        this.mTargetIpAddress = targetIpAddress;
        this.mTargetPort = targetPort;
    }

    // Components
    private Socket mSocket;
    private BufferedInputStream mInputStream;
    private BufferedOutputStream mOutputStream;

    // Attributes
    private String mTargetIpAddress;
    private int mTargetPort;
}
