package com.redcarrottt.sc.internal.bt;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

import com.redcarrottt.sc.internal.Device;
import com.redcarrottt.testapp.Logger;

/* Copyright (c) 2017-2018. All rights reserved.
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
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

class BtDevice extends Device {
    private static final String kTag = "BtDevice";

    // TODO: there is no timeout on turn on/off of BtDevice
    @Override
    protected void turnOnImpl() {
        BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
        if (adapter.isEnabled()) {
            Logger.DEBUG(kTag, "Bluetooth is already turned on");
            this.doneTurnOn(true);
        } else {
            // Register bluetooth device on event receiver
            IntentFilter intentFilter = new IntentFilter();
            intentFilter.addAction(BluetoothAdapter.ACTION_STATE_CHANGED);
            Context context = this.mOwnerActivity.getApplicationContext();
            context.registerReceiver(this.mBtDeviceOnEventReceiver, intentFilter);

            // turn on adapter command
            adapter.enable();
        }
    }

    @Override
    protected void turnOffImpl() {
        // Register bluetooth device off event receiver
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(BluetoothAdapter.ACTION_STATE_CHANGED);
        Context context = this.mOwnerActivity.getApplicationContext();
        context.registerReceiver(this.mBtDeviceOffEventReceiver, intentFilter);

        // turn off adapter command
        BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
        if(!adapter.isEnabled()) {
            Logger.DEBUG(kTag, "Bluetooth is already turned off");
            this.doneTurnOff(true);
        } else {
            adapter.disable();
        }
    }

    private BroadcastReceiver mBtDeviceOnEventReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            if (action.equals(BluetoothAdapter.ACTION_STATE_CHANGED)) {
                final int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE,
                        BluetoothAdapter.ERROR);
                Logger.DEBUG(kTag, "Bluetooth Adapter state changed: " + state);
                if (state == BluetoothAdapter.STATE_ON) {
                    Logger.DEBUG(kTag, "Bluetooth is turned on successfully");
                    doneTurnOn(true);
                    context.unregisterReceiver(mBtDeviceOnEventReceiver);
                }
            }
        }
    };

    private BroadcastReceiver mBtDeviceOffEventReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            if (action.equals(BluetoothAdapter.ACTION_STATE_CHANGED)) {
                final int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE,
                        BluetoothAdapter.ERROR);
                Logger.DEBUG(kTag, "Bluetooth Adapter state changed: " + state);
                if (state == BluetoothAdapter.STATE_OFF) {
                    Logger.DEBUG(kTag, "Bluetooth is turned off successfully");
                    doneTurnOff(true);
                    context.unregisterReceiver(mBtDeviceOffEventReceiver);
                }
            }
        }
    };

    private Activity mOwnerActivity;

    // Constructor
    public BtDevice(Activity ownerActivity) {
        super("Bluetooth");
        this.mOwnerActivity = ownerActivity;
    }
}
