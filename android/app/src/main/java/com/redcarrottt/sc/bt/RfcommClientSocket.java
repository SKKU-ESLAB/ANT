package com.redcarrottt.sc.bt;

import com.redcarrottt.sc.ClientSocket;

import java.util.UUID;

public class RfcommClientSocket extends ClientSocket {
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
    public RfcommClientSocket(String serviceUuid) {
        this.mServiceUuid = UUID.fromString(serviceUuid);
    }

    UUID mServiceUuid;
}
