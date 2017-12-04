package com.ant.cmfw.devicecontrollers.bluetooth;

/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import android.app.Activity;
import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.ResultReceiver;
import android.util.Log;

import java.util.ArrayList;
import java.util.Set;

import static android.content.ContentValues.TAG;

public class BluetoothDeviceController {
    private Service mService;

    private ConnectProcedure mConnectProcedureProcedure = new ConnectProcedure();
    private State mState = new State();

    public BluetoothDeviceController(Service service) {
        this.mService = service;
    }

    public boolean isConnected() {
        return this.mState.isConnected();
    }

    public void connect(BluetoothConnectingResultListener connectingResultListener) {
        this.mConnectProcedureProcedure.start(connectingResultListener);
    }

    public void disconnect() {
        // not yet implemented
        this.mState.transitToDisconnected();
    }

    public void addListener(BluetoothDeviceStateListener deviceStateListener) {
        this.mState.addListener(deviceStateListener);
    }

    public void removeListener(BluetoothDeviceStateListener deviceStateListener) {
        this.mState.removeListener(deviceStateListener);
    }

    class ConnectProcedure {
        private BluetoothConnectingResultListener mConnectingResultListener = null;

        // ConnectProcedure to Bluetooth device
        public void start(BluetoothConnectingResultListener connectingResultListener) {
            this.mConnectingResultListener = connectingResultListener;

            Intent bluetoothConnectorIntent = new Intent(mService, BluetoothConnectorActivity
                    .class);
            bluetoothConnectorIntent.putExtra(BluetoothConnectorActivity.INTENT_KEY_RECEIVER, new
                    BluetoothConnectorResultReceiver());
            bluetoothConnectorIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            mService.startActivity(bluetoothConnectorIntent);
        }

        class BluetoothConnectorResultReceiver extends ResultReceiver {
            static final String RECEIVER_KEY_FAIL_MESSAGE = "FailMessage";
            static final String RECEIVER_KEY_BT_ADDRESS = "BluetooothDeviceAddress";

            // Receiver from BluetoothConnectorActivity
            private BluetoothConnectorResultReceiver() {
                super(null);
            }

            @Override
            protected void onReceiveResult(int resultCode, Bundle resultData) {
                if (resultCode == Activity.RESULT_OK) {
                    String bluetoothAddress = resultData.getString(RECEIVER_KEY_BT_ADDRESS);
                    if (bluetoothAddress != null && !bluetoothAddress.isEmpty()) {
                        // Success
                        Log.d(TAG, "Connecting success");
                        onSuccess(bluetoothAddress);
                    } else {
                        // Fail
                        Log.d(TAG, "Connecting fail");
                        onFail();
                    }
                } else {
                    // Fail
                    Log.d(TAG, "Connecting fail");
                    onFail();
                }
            }
        }

        private void onSuccess(String bluetoothAddress) {
            BluetoothDevice bluetoothDevice = findBondedBluetoothDevice(bluetoothAddress);

            // State transition
            mState.transitToConnected(bluetoothDevice);

            // Notify result
            this.mConnectingResultListener.onConnectingBluetoothDeviceSuccess(bluetoothDevice);
        }

        private void onFail() {
            // State transition
            mState.transitToDisconnected();

            // Notify result
            this.mConnectingResultListener.onConnectingBluetoothDeviceFail();
        }

        // Utility functions
        private BluetoothDevice findBondedBluetoothDevice(String bluetoothAddress) {
            if (bluetoothAddress == null || bluetoothAddress.isEmpty()) {
                return null;
            }

            Set<BluetoothDevice> bluetoothDevices = BluetoothAdapter.getDefaultAdapter()
                    .getBondedDevices();
            if (bluetoothDevices.size() > 0) {
                for (BluetoothDevice bluetoothDevice : bluetoothDevices) {
                    String deviceName = bluetoothDevice.getName();
                    String deviceAddress = bluetoothDevice.getAddress();
                    if (deviceAddress.compareTo(bluetoothAddress) == 0) {
                        return bluetoothDevice;
                    }
                }
            }
            return null;
        }
    }

    private class State {
        private boolean mIsConnected = false;
        private ArrayList<BluetoothDeviceStateListener> mDeviceStateListeners = new ArrayList<>();

        private BluetoothDeviceStatusReceiver mBluetoothDeviceStatusReceiver = null;

        public boolean isConnected() {
            return this.mIsConnected;
        }

        public void addListener(BluetoothDeviceStateListener deviceStateListener) {
            this.mDeviceStateListeners.add(deviceStateListener);
        }

        public void removeListener(BluetoothDeviceStateListener deviceStateListener) {
            this.mDeviceStateListeners.remove(deviceStateListener);
        }

        public void transitToConnected(BluetoothDevice bluetoothDevice) {
            this.mIsConnected = true;

            // Start to watch Bluetooth device's status
            this.startToWatchDeviceState(bluetoothDevice);

            // Notify Bluetooth device connection event to listeners
            for(BluetoothDeviceStateListener listener : this.mDeviceStateListeners) {
                listener.onBluetoothDeviceStateChanged(this.mIsConnected);
            }
        }

        public void transitToDisconnected() {
            this.mIsConnected = false;

            // Stop to watch Bluetooth device's status
            this.stopToWatchDeviceState();

            // Notify Bluetooth device disconnection event to listeners
            for(BluetoothDeviceStateListener listener : this.mDeviceStateListeners) {
                listener.onBluetoothDeviceStateChanged(this.mIsConnected);
            }
        }

        private void startToWatchDeviceState(BluetoothDevice bluetoothDevice) {
            IntentFilter filter = new IntentFilter();
            filter.addAction(BluetoothDevice.ACTION_BOND_STATE_CHANGED);
            this.mBluetoothDeviceStatusReceiver = new BluetoothDeviceStatusReceiver
                    (bluetoothDevice);
            mService.registerReceiver(this.mBluetoothDeviceStatusReceiver, filter);
        }

        private void stopToWatchDeviceState() {
            if (this.mBluetoothDeviceStatusReceiver != null) {
                mService.unregisterReceiver(this.mBluetoothDeviceStatusReceiver);
                this.mBluetoothDeviceStatusReceiver = null;
            }
        }

        // Receive Bluetooth device disconnection event from Android Bluetooth framework
        class BluetoothDeviceStatusReceiver extends BroadcastReceiver {
            private BluetoothDevice mBindingBluetoothDevice;

            public BluetoothDeviceStatusReceiver(BluetoothDevice bindingBluetoothDevice) {
                this.mBindingBluetoothDevice = bindingBluetoothDevice;
            }

            @Override
            public void onReceive(Context context, Intent intent) {
                String action = intent.getAction();
                if (BluetoothDevice.ACTION_BOND_STATE_CHANGED.compareTo(action) == 0) {
                    BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice
                            .EXTRA_DEVICE);
                    if (this.mBindingBluetoothDevice != null) {
                        if (device.getBondState() != BluetoothDevice.BOND_BONDED) {
                            transitToDisconnected();
                        }
                    }
                }
            }
        }
    }
}