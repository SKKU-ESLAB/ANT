package com.redcarrottt.sc.wfd;

import com.redcarrottt.sc.P2PClient;

public class WfdP2PClient extends P2PClient {
    @Override
    protected boolean discoverImpl() {
        return false;
    }

    @Override
    protected boolean connectImpl() {
        return false;
    }

    @Override
    protected boolean disconnectImpl() {
        return false;
    }
}
