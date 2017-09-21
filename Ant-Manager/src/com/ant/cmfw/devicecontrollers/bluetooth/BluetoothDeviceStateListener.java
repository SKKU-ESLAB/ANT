package com.ant.cmfw.devicecontrollers.bluetooth;

public interface BluetoothDeviceStateListener {
    public void onBluetoothDeviceStateChanged(boolean isConnected);
}
