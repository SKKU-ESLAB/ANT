package com.redcarrottt.sc.bt;

import com.redcarrottt.sc.ClientAdapter;

public class BtClientAdapter extends ClientAdapter {
    // Constructor
    public BtClientAdapter(int id, String name, String targetMacAddr, String serviceUuid) {
        super(id, name);

        // Components
        BtDevice device = BtDevice.getSingleton();
        BtP2PClient p2pClient = new BtP2PClient(targetMacAddr);
        RfcommClientSocket clientSocket = new RfcommClientSocket(serviceUuid);

        // Initialize
        this.initialize(device, p2pClient, clientSocket);
    }
}
