package com.redcarrottt.sc.bt;

import com.redcarrottt.sc.OnConnectResult;
import com.redcarrottt.sc.OnDisconnectResult;
import com.redcarrottt.sc.OnDiscoverResult;
import com.redcarrottt.sc.P2PClient;

class BtP2PClient extends P2PClient {
    @Override
    protected void discoverImpl(OnDiscoverResult onDiscoverResult) {
        // Assume that target device has already been bonded.
        onDiscoverResult.onDoneDiscover(true);
    }

    @Override
    protected void connectImpl(OnConnectResult onConnectResult) {
        // Assume that target device has already been bonded.
        onConnectResult.onDoneConnect(true);
    }

    @Override
    protected void disconnectImpl(OnDisconnectResult onDisconnectResult) {
        // Assume that target device has already been bonded.
        onDisconnectResult.onDoneDisconnect(true);
    }

    // Constructor
    public BtP2PClient() {
    }

    // Attributes
}
