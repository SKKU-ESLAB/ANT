package com.redcarrottt.sc.wfd;

import com.redcarrottt.sc.ClientSocket;

public class TcpClientSocket extends ClientSocket {
    @Override
    protected boolean openImpl() {
        // TODO:
        return false;
    }

    @Override
    protected boolean closeImpl() {
        // TODO:
        return false;
    }

    @Override
    protected int sendImpl(byte[] dataBuffer, int dataLength) {
        // TODO:
        return 0;
    }

    @Override
    protected int receiveImpl(byte[] dataBuffer, int dataLength) {
        // TODO:
        return 0;
    }

    // Constructor
    public TcpClientSocket(String targetIpAddress, int targetPort) {
        this.mTargetIpAddress = targetIpAddress;
        this.mTargetPort = targetPort;
    }

    // Attributes
    private String mTargetIpAddress;
    private int mTargetPort;
}
