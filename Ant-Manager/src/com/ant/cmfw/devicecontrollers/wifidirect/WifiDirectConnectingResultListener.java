package com.ant.cmfw.devicecontrollers.wifidirect;

public interface WifiDirectConnectingResultListener {
    public void onConnectingWifiDirectDeviceSuccess();

    public void onConnectingWifiDirectDeviceFail();
}