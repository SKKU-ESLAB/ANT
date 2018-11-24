package com.redcarrottt.sc.internal.wfd;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.wifi.WifiManager;

import com.redcarrottt.sc.internal.Device;
import com.redcarrottt.testapp.Logger;

class WfdDevice extends Device {
    private static final String kTag = "WFdDevice";

    // TODO: there is no timeout on turn on/off of WfdDevice
    @Override
    protected void turnOnImpl() {
        WifiManager wifiManager = (WifiManager) this.mOwnerActivity.getApplicationContext()
                .getSystemService(Context.WIFI_SERVICE);
        if (wifiManager == null) {
            doneTurnOn(false);
        } else if (wifiManager.isWifiEnabled()) {
            Logger.DEBUG(kTag, "Wi-fi Direct is already turned on");
            doneTurnOn(true);
        } else {
            // Register wi-fi device on event receiver
            IntentFilter intentFilter = new IntentFilter();
            intentFilter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
            Context context = this.mOwnerActivity.getApplicationContext();
            context.registerReceiver(this.mWfdDeviceOnEventReceiver, intentFilter);

            // turn on wi-fi command
            wifiManager.setWifiEnabled(true);
        }
    }

    @Override
    protected void turnOffImpl() {
        // Register wi-fi device off event receiver
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
        Context context = this.mOwnerActivity.getApplicationContext();
        context.registerReceiver(this.mWfdDeviceOffEventReceiver, intentFilter);

        // turn off wi-fi command
        WifiManager wifiManager = (WifiManager) this.mOwnerActivity.getApplicationContext()
                .getSystemService(Context.WIFI_SERVICE);
        if(wifiManager == null) {
            doneTurnOff(true);
        } else if(!wifiManager.isWifiEnabled()) {
            Logger.DEBUG(kTag, "Wi-fi Direct is already turned off");
            doneTurnOff(true);
        } else {
            wifiManager.setWifiEnabled(false);
        }
    }

    private BroadcastReceiver mWfdDeviceOnEventReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            if (action.equals(WifiManager.WIFI_STATE_CHANGED_ACTION)) {
                final int state = intent.getIntExtra(WifiManager.EXTRA_WIFI_STATE,
                        WifiManager.WIFI_STATE_UNKNOWN);
                Logger.DEBUG(kTag, "Wi-fi Adapter state changed: " + state);
                if (state == WifiManager.WIFI_STATE_ENABLED) {
                    Logger.DEBUG(kTag, "Wi-fi is turned on successfully");
                    doneTurnOn(true);
                    context.unregisterReceiver(mWfdDeviceOnEventReceiver);
                }
            }
        }
    };

    private BroadcastReceiver mWfdDeviceOffEventReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            if (action.equals(WifiManager.WIFI_STATE_CHANGED_ACTION)) {
                final int state = intent.getIntExtra(WifiManager.EXTRA_WIFI_STATE,
                        WifiManager.WIFI_STATE_UNKNOWN);
                Logger.DEBUG(kTag, "Wi-fi Adapter state changed: " + state);
                if (state == WifiManager.WIFI_STATE_DISABLED) {
                    Logger.DEBUG(kTag, "Wi-fi is turned off successfully");
                    doneTurnOff(true);
                    context.unregisterReceiver(mWfdDeviceOffEventReceiver);
                }
            }
        }
    };

    // Constructor
    public WfdDevice(Activity ownerActivity) {
        super("Wi-fi Direct");
        this.mOwnerActivity = ownerActivity;
    }

    // Attributes
    private Activity mOwnerActivity;
}