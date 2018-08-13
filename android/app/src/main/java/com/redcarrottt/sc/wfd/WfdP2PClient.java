package com.redcarrottt.sc.wfd;

import com.redcarrottt.sc.P2PClient;

public class WfdP2PClient extends P2PClient {
    @Override
    protected boolean discoverImpl() {
        // TODO:
        return false;
    }

    @Override
    protected boolean connectImpl() {
        // TODO:
        return false;
    }

    @Override
    protected boolean disconnectImpl() {
        // TODO:
        return false;
    }

    // Constructor
    public WfdP2PClient(String targetDevName) {
        this.mTargetDevName = targetDevName;
    }

    private String mTargetDevName;
}
