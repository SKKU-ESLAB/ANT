package com.redcarrottt.sc.wfd;

import android.app.Activity;
import android.content.Context;
import android.net.wifi.WifiManager;

import com.redcarrottt.sc.Device;

class WfdDevice extends Device {
    @Override
    protected boolean turnOnImpl() {
        WifiManager wifiManager = (WifiManager) this.mOwnerActivity.getApplicationContext()
                .getSystemService(Context.WIFI_SERVICE);
        wifiManager.setWifiEnabled(true);
        return false;
    }

    @Override
    protected boolean turnOffImpl() {
        WifiManager wifiManager = (WifiManager) this.mOwnerActivity.getApplicationContext()
                .getSystemService(Context.WIFI_SERVICE);
        wifiManager.setWifiEnabled(false);
        return false;
    }

    // Singleton
    public static WfdDevice getSingleton(Activity ownerActivity) {
        if (sSingleton == null) {
            sSingleton = new WfdDevice(ownerActivity);
        }
        return sSingleton;
    }

    private static WfdDevice sSingleton = null;

    // Constructor
    private WfdDevice(Activity ownerActivity) {
        super("Wi-fi Direct");
    }

    // Attributes
    Activity mOwnerActivity;
}