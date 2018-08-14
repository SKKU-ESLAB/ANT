package com.redcarrottt.sc.wfd;

import android.app.Activity;

import com.redcarrottt.sc.ClientAdapter;

public class WfdClientAdapter extends ClientAdapter {
    // Constructor
    public WfdClientAdapter(int id, String name, String targetDevName,
                            String targetIpAddress, int targetPort, Activity ownerActivity) {
        super(id, name);

        // Components
        WfdDevice device = WfdDevice.getSingleton(ownerActivity);
        WfdP2PClient p2pClient = new WfdP2PClient(targetDevName);
        TcpClientSocket clientSocket = new TcpClientSocket(targetIpAddress, targetPort);

        // Initialize
        this.initialize(device, p2pClient, clientSocket);
    }
}
