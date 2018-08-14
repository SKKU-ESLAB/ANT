package com.redcarrottt.sc.wfd;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.net.wifi.WifiManager;

import com.redcarrottt.sc.Device;

class WfdDevice extends Device {
    @Override
    protected boolean turnOnImpl() {
        WifiManager wifiManager = (WifiManager) this.mOwnerActivity.getApplicationContext()
                .getSystemService(Context.WIFI_SERVICE);
        if (wifiManager != null) {
            wifiManager.setWifiEnabled(true);
            return true;
        } else {
            return false;
        }
    }

    @Override
    protected boolean turnOffImpl() {
        WifiManager wifiManager = (WifiManager) this.mOwnerActivity.getApplicationContext()
                .getSystemService(Context.WIFI_SERVICE);
        if (wifiManager != null) {
            wifiManager.setWifiEnabled(false);
            return true;
        } else {
            return false;
        }
    }

    // Singleton
    public static WfdDevice getSingleton(Activity ownerActivity) {
        if (sSingleton == null) {
            sSingleton = new WfdDevice(ownerActivity);
        }
        return sSingleton;
    }

    @SuppressLint("StaticFieldLeak")
    private static WfdDevice sSingleton = null;

    // Constructor
    private WfdDevice(Activity ownerActivity) {
        super("Wi-fi Direct");
        this.mOwnerActivity = ownerActivity;
    }

    // Attributes
    private Activity mOwnerActivity;
}