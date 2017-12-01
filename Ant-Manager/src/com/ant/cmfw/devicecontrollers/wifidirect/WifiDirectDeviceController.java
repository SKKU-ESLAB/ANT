package com.ant.cmfw.devicecontrollers.wifidirect;

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
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.NetworkInfo;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiManager;
import android.net.wifi.WpsInfo;
import android.net.wifi.p2p.WifiP2pConfig;
import android.net.wifi.p2p.WifiP2pDevice;
import android.net.wifi.p2p.WifiP2pDeviceList;
import android.net.wifi.p2p.WifiP2pManager;
import android.util.Log;

import java.util.ArrayList;
import java.util.List;

public class WifiDirectDeviceController {
    static private String TAG = "WFDController";
    private Service mService;
    private String mWifiDirectName;
    private WifiP2pManager mWifiP2pManager = null;
    private WifiP2pManager.Channel mWifiP2pManagerChannel = null;

    private ConnectProcedure mConnectProcedure;
    private State mState;

    public WifiDirectDeviceController(Service service) {
        this.mService = service;

        this.mConnectProcedure = new ConnectProcedure();
        this.mState = new State();

        this.mWifiP2pManager = (WifiP2pManager) this.mService.getSystemService(Activity
                .WIFI_P2P_SERVICE);
        this.mWifiP2pManagerChannel = this.mWifiP2pManager.initialize(this.mService, this
                .mService.getMainLooper(), null);
    }

    public boolean isConnected() {
        return this.mState.isConnected();
    }

    public void connect(WifiDirectConnectingResultListener connectingResultListener, String
            wifiDirectName) {
        this.mWifiDirectName = wifiDirectName;

        this.mConnectProcedure.start(connectingResultListener);
    }

    public void disconnect() {
        // TODO: not implemented yet
    }

    public void addListener(WifiDirectDeviceStateListener deviceStateListener) {
        this.mState.addListener(deviceStateListener);
    }

    public void removeListener(WifiDirectDeviceStateListener deviceStateListener) {
        this.mState.removeListener(deviceStateListener);
    }

    private class ConnectProcedure {
        private WifiDirectPeerChangedEventReceiver mPeerChangedEventReceiver = null;
        private WifiDirectConnectingResultListener mConnectingResultListener;
        private Integer mConnectingResultListenerLock = 0;
        private boolean mIsConnecting = false;

        private boolean mIsWifiDirectMode = true;

        public void start(WifiDirectConnectingResultListener connectingResultListener) {
            this.mConnectingResultListener = connectingResultListener;

            if (mIsWifiDirectMode) {
                this.discoverPeers();
            } else {
                this.connectWiFiAP();
            }
        }

        public void connectWiFiAP() {
            String networkSSID = "DIRECT-CS-ANT";
            String networkPass = "12345670";

            WifiConfiguration wifiConfig = new WifiConfiguration();
            wifiConfig.SSID = "\"" + networkSSID + "\"";
            wifiConfig.preSharedKey = "\"" + networkPass + "\"";

            WifiManager wifiManager = (WifiManager) mService.getApplicationContext()
                    .getSystemService(Context.WIFI_SERVICE);
            wifiManager.addNetwork(wifiConfig);

            List<WifiConfiguration> foundConfigs = wifiManager.getConfiguredNetworks();
            for (WifiConfiguration foundConfig : foundConfigs) {
                if (foundConfig.SSID != null & foundConfig.SSID.equals("\"" + networkSSID + "\"")) {
                    wifiManager.disconnect();
                    wifiManager.enableNetwork(foundConfig.networkId, true);
                    wifiManager.reconnect();
                    this.onSuccess();
                    break;
                }
            }
            this.onFail();
        }

        // Step 1. Discover peers
        private void discoverPeers() {
            IntentFilter wifiP2PIntentFilter;
            wifiP2PIntentFilter = new IntentFilter();
            wifiP2PIntentFilter.addAction(WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION);
            synchronized (this.mConnectingResultListenerLock) {
                this.mPeerChangedEventReceiver = new WifiDirectPeerChangedEventReceiver();
                mService.registerReceiver(this.mPeerChangedEventReceiver, wifiP2PIntentFilter);
            }

            // Start Wi-fi direct discovery
            mWifiP2pManager.discoverPeers(mWifiP2pManagerChannel, null);
        }

        class WifiDirectPeerChangedEventReceiver extends BroadcastReceiver {
            @Override
            public void onReceive(Context context, Intent intent) {
                String action = intent.getAction();
                if (WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION.compareTo(action) == 0) {
                    // Unregister receiver
                    synchronized (mConnectingResultListenerLock) {
                        if (mPeerChangedEventReceiver != null) {
                            mService.unregisterReceiver(mPeerChangedEventReceiver);
                            mPeerChangedEventReceiver = null;
                        }
                    }

                    onPeerChanged();
                }
            }
        }

        // Step 2. Request peer list
        private void onPeerChanged() {
            Log.d(TAG, "Request peer list");
            mWifiP2pManager.requestPeers(mWifiP2pManagerChannel, new WifiDirectPeerListListener());
        }

        class WifiDirectPeerListListener implements WifiP2pManager.PeerListListener {
            @Override
            public void onPeersAvailable(WifiP2pDeviceList peerDeviceList) {
                onPeerListReceived(peerDeviceList);
            }
        }

        // Step 3. Check the given peer list
        private void onPeerListReceived(WifiP2pDeviceList peerDeviceList) {
            // Step 3-1. Drop duplicated connection tries
            if (this.mIsConnecting) {
                return;
            } else {
                Log.d(TAG, "Peer List Received");
                this.mIsConnecting = true;
            }

            if (mWifiDirectName.isEmpty()) {
                Log.d(TAG, "Failed to connectChannel! No Wi-fi Direct name given!");
                this.mIsConnecting = false;
                this.onFail();
                return;
            }

            // Step 3-2. Check if there is peer device that we want
            for (WifiP2pDevice peerDevice : peerDeviceList.getDeviceList()) {
                if (peerDevice.deviceName.compareTo(mWifiDirectName) == 0) {
                    Log.d(TAG, "Found Wi-fi peer device: name=" + mWifiDirectName + " / address="
                            + peerDevice.deviceAddress + " / status=" + peerDevice.status);
                    if (peerDevice.status == WifiP2pDevice.AVAILABLE || peerDevice.status ==
                            WifiP2pDevice.CONNECTED || peerDevice.status == WifiP2pDevice.INVITED) {
                        Log.d(TAG, "Connecting Wi-fi Direct device: " + mWifiDirectName);
                        requestConnection(peerDevice);
                    } else {
                        Log.d(TAG, "Not yet initiated: " + mWifiDirectName + " / " + peerDevice
                                .status);
                        this.onFail();
                    }
                    this.mIsConnecting = false;
                    return;
                }
            }
            this.mIsConnecting = false;
        }

        // Step 4. Request for connecting to the peer device
        private void requestConnection(WifiP2pDevice peerDevice) {
            WifiP2pConfig wifiP2pConfig = new WifiP2pConfig();
            wifiP2pConfig.deviceAddress = peerDevice.deviceAddress;
            if (peerDevice.wpsPbcSupported()) {
                wifiP2pConfig.wps.setup = WpsInfo.PBC;
                Log.d(TAG, "WPS: PBC");
            } else if (peerDevice.wpsKeypadSupported()) {
                wifiP2pConfig.wps.setup = WpsInfo.KEYPAD;
                wifiP2pConfig.wps.pin = "12345670";
                Log.d(TAG, "WPS:KeyPad");
            } else {
                wifiP2pConfig.wps.setup = WpsInfo.DISPLAY;
                Log.d(TAG, "WPS:Display");
            }

            // Start to watch Bluetooth device's status
            mState.startToWatchDeviceState();

            Log.d(TAG, "Request to connect Wi-fi Direct connection");
            mWifiP2pManager.connect(mWifiP2pManagerChannel, wifiP2pConfig, new WifiP2pManager
                    .ActionListener() {
                @Override
                public void onSuccess() {
                    new Thread() {
                        @Override
                        public void run() {
                            Log.d(TAG, "Wi-fi Direct Connection Success");
                            ConnectProcedure.this.onSuccess();
                        }
                    }.start();
                }

                @Override
                public void onFailure(int reason) {
                    Log.d(TAG, "Wi-fi Direct Connection Fail");
                }
            });
        }

        private void onSuccess() {
            // State transition
            mState.transitToConnected();

            // Notify result
            this.mConnectingResultListener.onConnectingWifiDirectDeviceSuccess();
        }

        private void onFail() {
            // State transition
            mState.transitToDisconnected();

            // Notify result
            this.mConnectingResultListener.onConnectingWifiDirectDeviceFail();
        }
    }

    private class State {
        private WifiDirectDeviceStatusReceiver mWifiDirectDeviceStatusReceiver;

        private boolean mIsConnected = false;
        private ArrayList<WifiDirectDeviceStateListener> mDeviceStateListeners = new ArrayList<>();

        public boolean isConnected() {
            return this.mIsConnected;
        }

        public void addListener(WifiDirectDeviceStateListener deviceStateListener) {
            this.mDeviceStateListeners.add(deviceStateListener);
        }

        public void removeListener(WifiDirectDeviceStateListener deviceStateListener) {
            this.mDeviceStateListeners.remove(deviceStateListener);
        }

        public void transitToConnected() {
            this.mIsConnected = true;

            // Start to watch Bluetooth device's status
            //this.startToWatchDeviceState();

            // Notify Wi-fi direct device connection event to listeners
            for (WifiDirectDeviceStateListener listener : this.mDeviceStateListeners) {
                listener.onWifiDirectDeviceStateChanged(this.mIsConnected);
            }
        }

        public void transitToDisconnected() {
            this.mIsConnected = false;

            // Stop to watch Bluetooth device's status
            this.stopToWatchDeviceState();

            // Notify Wi-fi direct device disconnection event to listeners
            for (WifiDirectDeviceStateListener listener : this.mDeviceStateListeners) {
                listener.onWifiDirectDeviceStateChanged(this.mIsConnected);
            }
        }

        public void startToWatchDeviceState() {
            IntentFilter filter = new IntentFilter();
            filter.addAction(WifiP2pManager.WIFI_P2P_CONNECTION_CHANGED_ACTION);
            this.mWifiDirectDeviceStatusReceiver = new WifiDirectDeviceStatusReceiver();
            mService.registerReceiver(this.mWifiDirectDeviceStatusReceiver, filter);
        }

        private void stopToWatchDeviceState() {
            if (this.mWifiDirectDeviceStatusReceiver != null) {
                mService.unregisterReceiver(this.mWifiDirectDeviceStatusReceiver);
                this.mWifiDirectDeviceStatusReceiver = null;
            }
        }

        // Receive Wi-fi direct device disconnection event from Android Bluetooth framework
        class WifiDirectDeviceStatusReceiver extends BroadcastReceiver {
            @Override
            public void onReceive(Context context, Intent intent) {
                String action = intent.getAction();
                if (WifiP2pManager.WIFI_P2P_CONNECTION_CHANGED_ACTION.compareTo(action) == 0) {
                    NetworkInfo networkInfo = (NetworkInfo) intent.getParcelableExtra
                            (WifiP2pManager.EXTRA_NETWORK_INFO);
                    Log.d(TAG, "Wi-fi Direct state change listener: Android-side state=" +
                            networkInfo.getState());
//                    if (!networkInfo.isConnectedOrConnecting() || !networkInfo.isAvailable()) {
//                        Log.d(TAG, "Transit wi-fi direct state to disconnected: Android-side " +
//                                "state=" + networkInfo.getState());
//                        transitToDisconnected();
//                    }
                }
            }
        }
    }
}