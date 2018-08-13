package com.redcarrottt.sc.wfd;

import com.redcarrottt.sc.ClientAdapter;

public class WfdClientAdapter extends ClientAdapter {
    // Constructor
    public WfdClientAdapter(int id, String name, String targetDevName, String targetIpAddress, int targetPort) {
        super(id, name);

        // Components
        WfdDevice device = WfdDevice.getSingleton();
        WfdP2PClient p2PClient = new WfdP2PClient(targetDevName);
        TcpClientSocket clientSocket = new TcpClientSocket(targetIpAddress, targetPort);
    }
}
