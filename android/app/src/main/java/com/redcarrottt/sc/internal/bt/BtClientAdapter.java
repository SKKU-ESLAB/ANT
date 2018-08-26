package com.redcarrottt.sc.internal.bt;

import android.app.Activity;

import com.redcarrottt.sc.internal.ClientAdapter;

public class BtClientAdapter extends ClientAdapter {
    // Constructor
    public BtClientAdapter(int id, String name, String targetMacAddr, String serviceUuid,
                           Activity ownerActivity) {
        super(id, name);

        // Components
        BtDevice device = BtDevice.getSingleton(ownerActivity);
        BtP2PClient p2pClient = BtP2PClient.getSingleton();
        RfcommClientSocket clientSocket = new RfcommClientSocket(targetMacAddr, serviceUuid);

        // Initialize
        this.initialize(device, p2pClient, clientSocket);
    }
}
