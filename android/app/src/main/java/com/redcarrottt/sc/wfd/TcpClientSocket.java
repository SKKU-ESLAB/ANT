package com.redcarrottt.sc.wfd;

import com.redcarrottt.sc.ClientSocket;

public class TcpClientSocket extends ClientSocket {
    @Override
    protected boolean openImpl() {
        return false;
    }

    @Override
    protected boolean closeImpl() {
        return false;
    }

    @Override
    protected int sendImpl(byte[] dataBuffer, int dataLength) {
        return 0;
    }

    @Override
    protected int receiveImpl(byte[] dataBuffer, int dataLength) {
        return 0;
    }
}
