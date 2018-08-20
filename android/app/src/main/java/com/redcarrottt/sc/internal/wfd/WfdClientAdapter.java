package com.redcarrottt.sc.internal.wfd;

import android.app.Activity;

import com.redcarrottt.sc.internal.ClientAdapter;

public class WfdClientAdapter extends ClientAdapter {
    // Constructor
    public WfdClientAdapter(int id, String name, String targetMacAddress,
                            String targetIpAddress, int targetPort, Activity ownerActivity) {
        super(id, name);

        // Components
        WfdDevice device = WfdDevice.getSingleton(ownerActivity);
        WfdP2PClient p2pClient = new WfdP2PClient(ownerActivity, targetMacAddress);
        TcpClientSocket clientSocket = new TcpClientSocket(targetIpAddress, targetPort);

        // Initialize
        this.initialize(device, p2pClient, clientSocket);
    }
}
