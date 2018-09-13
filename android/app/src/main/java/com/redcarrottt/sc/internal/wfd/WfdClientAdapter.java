package com.redcarrottt.sc.internal.wfd;

import android.app.Activity;

import com.redcarrottt.sc.internal.ClientAdapter;
import com.redcarrottt.sc.internal.ControlMessageListener;
import com.redcarrottt.sc.internal.Core;
import com.redcarrottt.testapp.Logger;

import java.util.Arrays;

public class WfdClientAdapter extends ClientAdapter implements ControlMessageListener {
    private static final String kTag = "WfdClientAdapter";

    // Components
    private WfdP2PClient mP2pClient;
    private TcpClientSocket mClientSocket;

    // Constructor
    public WfdClientAdapter(int id, String name,
                            int targetPort, Activity ownerActivity) {
        super(id, name);

        // Components
        WfdDevice device = WfdDevice.getSingleton(ownerActivity);
        this.mP2pClient = WfdP2PClient.getSingleton(ownerActivity);
        this.mClientSocket = new TcpClientSocket(targetPort);

        // Initialize
        this.initialize(device, this.mP2pClient, this.mClientSocket);

        // Add WfdP2PClient as a control message listener
        Core.getInstance().addControlMessageListener(this);
    }

    @Override
    public void onReceiveControlMessage(int privType, byte[] dataBuffer, int dataLength) {
        if (privType == Core.PrivType.kWFDInfo) {
            String message = new String(Arrays.copyOfRange(dataBuffer, 0, dataLength));
            Logger.DEBUG(kTag, "Received WFD info message: " + message);
            String wfdInfoLines[] = message.split("\\r?\\n");
            if (wfdInfoLines.length < 3) {
                Logger.ERR(kTag, "WFD info is insufficient: " + wfdInfoLines.length + " < 3");
            }
            String targetMacAddress = wfdInfoLines[0];
            String wpsPin = wfdInfoLines[1];
            String targetIpAddress = wfdInfoLines[2];

            mP2pClient.setWfdP2PInfo(targetMacAddress, wpsPin);
            mClientSocket.setTcpClientInfo(targetIpAddress);
        }
    }
}
