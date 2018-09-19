package com.redcarrottt.sc.internal.bt;

import android.app.Activity;

import com.redcarrottt.sc.internal.ClientAdapter;

public class BtClientAdapter extends ClientAdapter {
    // Singleton
    static public BtClientAdapter singleton(int id, String name, String targetMacAddr, String
            serviceUuid, Activity ownerActivity) {
        if (sSingleton == null) {
            sSingleton = new BtClientAdapter(id, name, targetMacAddr, serviceUuid, ownerActivity);
        }
        return sSingleton;
    }

    static private BtClientAdapter sSingleton = null;

    // Constructor
    private BtClientAdapter(int id, String name, String targetMacAddr, String serviceUuid,
                            Activity ownerActivity) {
        super(id, name);

        // Components
        BtDevice device = new BtDevice(ownerActivity);
        BtP2PClient p2pClient = new BtP2PClient();
        RfcommClientSocket clientSocket = new RfcommClientSocket(targetMacAddr, serviceUuid);

        // Initialize
        this.initialize(device, p2pClient, clientSocket);
    }
}
