package com.ant.cmfw.ports.bluetooth.socket;

import android.bluetooth.BluetoothSocket;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

// RedCarrottt: Fix Bluetooth connection failure bug (Issue #103)
public interface BluetoothSocketWrapper {
    InputStream getInputStream() throws IOException;

    OutputStream getOutputStream() throws IOException;

    String getRemoteDeviceName();

    String getRemoteDeviceAddress();

    BluetoothSocket getUnderlyingSocket();

    void connect() throws IOException;

    void close() throws IOException;
}