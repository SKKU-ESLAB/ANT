package com.redcarrottt.sc.bt;

import com.redcarrottt.sc.P2PClient;

class BtP2PClient extends P2PClient {
    @Override
    protected boolean discoverImpl() {
        // Assume that target device has already been bonded.
        return true;
    }

    @Override
    protected boolean connectImpl() {
        // Assume that target device has already been bonded.
        return true;
    }

    @Override
    protected boolean disconnectImpl() {
        // Assume that target device has already been bonded.
        return true;
    }

    // Constructor
    public BtP2PClient(String targetMacAddr) {
        this.mTargetMacAddr = targetMacAddr;
    }

    // Attributes
    private String mTargetMacAddr;
}
