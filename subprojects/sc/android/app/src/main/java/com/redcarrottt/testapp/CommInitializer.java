package com.redcarrottt.testapp;

import android.bluetooth.BluetoothAdapter;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.wifi.WifiManager;

/* Copyright (c) 2018, contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong <redcarrottt@gmail.com>
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
public class CommInitializer {
    private static final String kTag = "CommInit";

    public void start() {
        new Thread() {
            @Override
            public void run() {
                turnOffBt();
            }
        }.start();
    }

    private void turnOffBt() {
        BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
        if (adapter.isEnabled()) {
            // Register bluetooth device on event receiver
            IntentFilter intentFilter = new IntentFilter();
            intentFilter.addAction(BluetoothAdapter.ACTION_STATE_CHANGED);
            Context context = this.mMainActivity.getApplicationContext();
            context.registerReceiver(this.TurnOffBtResult, intentFilter);

            // turn on adapter command
            Logger.DEBUG(kTag, "Turn off BT Start");
            adapter.disable();
        } else {
            Logger.DEBUG(kTag, "BT already turned off");
            this.turnOnBt();
        }
    }

    private BroadcastReceiver TurnOffBtResult = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            if (action.equals(BluetoothAdapter.ACTION_STATE_CHANGED)) {
                final int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE,
                        BluetoothAdapter.ERROR);
                if (state == BluetoothAdapter.STATE_OFF) {
                    Logger.DEBUG(kTag, "Turn off BT Success");
                    context.unregisterReceiver(TurnOffBtResult);
                    turnOnBt();
                }
            }
        }
    };

    private void turnOnBt() {
        BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
        if (!adapter.isEnabled()) {
            // Register bluetooth device on event receiver
            IntentFilter intentFilter = new IntentFilter();
            intentFilter.addAction(BluetoothAdapter.ACTION_STATE_CHANGED);
            Context context = this.mMainActivity.getApplicationContext();
            context.registerReceiver(this.TurnOnBtResult, intentFilter);

            // turn on adapter command
            Logger.DEBUG(kTag, "Turn on BT Start");
            adapter.enable();
        } else {
            Logger.WARN(kTag, "BT not turned on");
            this.mOnResult.onCommInitializerResult(false);
        }
    }

    private BroadcastReceiver TurnOnBtResult = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            if (action.equals(BluetoothAdapter.ACTION_STATE_CHANGED)) {
                final int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE,
                        BluetoothAdapter.ERROR);
                if (state == BluetoothAdapter.STATE_ON) {
                    Logger.DEBUG(kTag, "Turn on BT Success");
                    context.unregisterReceiver(TurnOnBtResult);
                    turnOffWifi();
                }
            }
        }
    };

    private void turnOffWifi() {
        // Register wi-fi device off event receiver
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
        Context context = this.mMainActivity.getApplicationContext();
        context.registerReceiver(this.TurnOffWifiResult, intentFilter);

        // turn off wi-fi command
        WifiManager wifiManager = (WifiManager) this.mMainActivity.getApplicationContext()
                .getSystemService(Context.WIFI_SERVICE);
        if (wifiManager == null) {
            Logger.ERR(kTag, "Failed to get Wi-fi Manager");
            return;
        }

        if (wifiManager.isWifiEnabled()) {
            Logger.DEBUG(kTag, "Turn off WIFI Start");
            wifiManager.setWifiEnabled(false);
        } else {
            Logger.DEBUG(kTag, "WIFI already turned off");
            this.turnOnWifi();
        }
    }

    private BroadcastReceiver TurnOffWifiResult = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            if (action.equals(WifiManager.WIFI_STATE_CHANGED_ACTION)) {
                final int state = intent.getIntExtra(WifiManager.EXTRA_WIFI_STATE, WifiManager
                        .WIFI_STATE_UNKNOWN);
                if (state == WifiManager.WIFI_STATE_DISABLED) {
                    Logger.DEBUG(kTag, "Turn off WFD success");
                    context.unregisterReceiver(TurnOffWifiResult);
                    turnOnWifi();
                }
            }
        }
    };

    private void turnOnWifi() {
        // Register wi-fi device off event receiver
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
        Context context = this.mMainActivity.getApplicationContext();
        context.registerReceiver(this.TurnOnWifiResult, intentFilter);

        // turn off wi-fi command
        WifiManager wifiManager = (WifiManager) this.mMainActivity.getApplicationContext()
                .getSystemService(Context.WIFI_SERVICE);
        if (wifiManager == null) {
            Logger.ERR(kTag, "Failed to get Wi-fi Manager");
            return;
        }

        if (!wifiManager.isWifiEnabled()) {
            Logger.DEBUG(kTag, "Turn on WIFI Start");
            wifiManager.setWifiEnabled(true);
        } else {
            Logger.DEBUG(kTag, "WIFI already turned on");
            this.mOnResult.onCommInitializerResult(true);
        }
    }

    private BroadcastReceiver TurnOnWifiResult = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            if (action.equals(WifiManager.WIFI_STATE_CHANGED_ACTION)) {
                final int state = intent.getIntExtra(WifiManager.EXTRA_WIFI_STATE, WifiManager
                        .WIFI_STATE_UNKNOWN);
                if (state == WifiManager.WIFI_STATE_ENABLED) {
                    Logger.DEBUG(kTag, "Turn on WFD success");
                    context.unregisterReceiver(TurnOnWifiResult);
                    mOnResult.onCommInitializerResult(true);
                }
            }
        }
    };

    public CommInitializer(MainActivity mainActivity, CommInitializerResult onResult) {
        this.mMainActivity = mainActivity;
        this.mOnResult = onResult;
    }

    private MainActivity mMainActivity;
    private CommInitializerResult mOnResult;
}

interface CommInitializerResult {
    public void onCommInitializerResult(boolean isSuccess);
}