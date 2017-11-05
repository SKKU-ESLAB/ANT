package com.ant.cmfw.service;

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

import android.app.Service;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.IBinder;
import android.util.Log;

import com.ant.cmfw.devicecontrollers.bluetooth.BluetoothConnectingResultListener;
import com.ant.cmfw.devicecontrollers.bluetooth.BluetoothDeviceController;
import com.ant.cmfw.devicecontrollers.bluetooth.BluetoothDeviceStateListener;
import com.ant.cmfw.devicecontrollers.wifidirect.WifiDirectConnectingResultListener;
import com.ant.cmfw.devicecontrollers.wifidirect.WifiDirectDeviceController;
import com.ant.cmfw.devicecontrollers.wifidirect.WifiDirectDeviceStateListener;
import com.ant.cmfw.glue.CommBroadcastReceiver;
import com.ant.cmfw.glue.CommBroadcastSender;
import com.ant.cmfw.ports.CommPort;
import com.ant.cmfw.ports.CommPortListener;
import com.ant.cmfw.ports.bluetooth.BluetoothCommPort;
import com.ant.cmfw.ports.wifidirect.WifiDirectCommPort;

import java.io.File;
import java.io.UnsupportedEncodingException;
import java.util.Date;
import java.util.UUID;

/* Threads of CommChannelService
    - main thread: send messages that are coming from ANT Manager internal to CommPort
    - listening thread: poll messages from CommPort and pass them to ANT Manager internal
*/

/* Roles of CommChannelService
    - Initialize/disconnectChannel Bluetooth, Wi-fi Direct device
    (BluetoothDeviceController, WifiDirectDeviceController)

    - State Management of Bluetooth, Wi-fi Direct device
    (BluetoothDeviceController.State, WifiDirectDeviceController.State)

    - Transfer data via Bluetooth or Wi-fi Direct
    (BlutoothCommPort, WifiDirectCommPort)
 */

public class CommChannelService extends Service implements CommPortListener {
    private static final String TAG = "CommChannelService";
    private final CommChannelService self = this;
    private final IBinder mBinder;
    private int mBindersCount = 0;

    private State mState;
    private ConnectChannelProcedure mConnectChannelProcedure;
    private EnableLargeDataModeProcedure mEnableLargeDataModeProcedure;

    // Device Controller
    private BluetoothDeviceController mBluetoothDeviceController = null;
    private WifiDirectDeviceController mWifiDirectDeviceController = null;

    // Ports
    private BluetoothCommPort mDefaultPort;
    private BluetoothCommPort mControlPort;
    private WifiDirectCommPort mLargeDataPort;
    private LargeDataPortWatcher mLargeDataPortWatcher;

    // Download path
    private String mDownloadFilePath;

    static public final int STATE_DISCONNECTED = 0;
    static public final int STATE_CONNECTING_DEFAULT = 1;
    static public final int STATE_CONNECTED_DEFAULT = 2;
    static public final int STATE_CONNECTING_LARGE_DATA = 3;
    static public final int STATE_CONNECTED_LARGE_DATA = 4;

    static private String DEFAULT_PORT_BLUETOOTH_UUID = "0a1b2c3d-4e5f-6a1c-2d0e-1f2a3b4c5d6e";
    static private String CONTROL_PORT_BLUETOOTH_UUID = "0a1b2c3d-4e5f-6a1c-2d0e-1f2a3b4c5d6d";
    static private int LARGEDATA_PORT_TCP_PORT = 10001;

    static private final String kWifiDirectOnMessage = "on";
    static private final String kWifiDirectOffMessage = "off";

    public CommChannelService() {
        this.mBinder = new CommBinder();
        this.mState = new State();
        this.mConnectChannelProcedure = new ConnectChannelProcedure();
        this.mEnableLargeDataModeProcedure = new EnableLargeDataModeProcedure();
    }

    public boolean isDefaultPortAvailable() {
        return this.mState.get() == STATE_CONNECTED_DEFAULT || this.mState.get() ==
                STATE_CONNECTING_LARGE_DATA || this.mState.get() == STATE_CONNECTED_LARGE_DATA;
    }

    public boolean isLargeDataPortAvailable() {
        return this.mState.get() == STATE_CONNECTED_LARGE_DATA;
    }

    public boolean isBluetoothDeviceConnected() {
        return this.mBluetoothDeviceController.isConnected();
    }

    public boolean isWifiDirectDeviceConnected() {
        return this.mWifiDirectDeviceController.isConnected();
    }

    public void setDownloadFilePath(String downloadFilePath) {
        this.mDownloadFilePath = downloadFilePath;
    }

    public void connectChannel() {
        // CommChannel State: transit to ConnectingDefault
        this.mState.transitToConnectingDefault();

        // Start to connect channel
        this.mConnectChannelProcedure.start();
    }

    public void disconnectChannel() {
        // Finish listening threads
        if (this.mDefaultPort != null) this.mDefaultPort.stopListeningThread();
        if (this.mControlPort != null) this.mControlPort.stopListeningThread();
        if (this.mLargeDataPort != null) this.mLargeDataPort.stopListeningThread();

        // Finish watcher thread
        if (this.mLargeDataPortWatcher != null) mLargeDataPortWatcher.stopToWatch();

        // Close ports
        if (this.mDefaultPort != null) this.mDefaultPort.close();
        if (this.mControlPort != null) this.mControlPort.close();
        if (this.mLargeDataPort != null) this.mLargeDataPort.close();

        // Disconnect devices
        if (this.mBluetoothDeviceController != null) this.mBluetoothDeviceController.disconnect();
        if (this.mWifiDirectDeviceController != null) this.mWifiDirectDeviceController.disconnect();

        // CommChannel State: transit to Disconnected
        this.mState.transitToDisconnected();
    }

    public void enableLargeDataMode() {
        // CommChannel State: transit to ConnectingLargeData
        this.mState.transitToConnectingLargeData();

        this.mEnableLargeDataModeProcedure.start();
    }

    private void disableLargeDataMode() {
        Log.d(TAG, "Disable large data mode");

        // Send turning on Wi-fi direct message
        sendRawMessageOnControl(kWifiDirectOffMessage, null);

        // Finish listening threads
        if (this.mControlPort != null) this.mControlPort.stopListeningThread();
        if (this.mLargeDataPort != null) this.mLargeDataPort.stopListeningThread();

        // Finish watcher thread
        if (this.mLargeDataPortWatcher != null) mLargeDataPortWatcher.stopToWatch();

        // Close ports
        if (this.mControlPort != null) this.mControlPort.close();
        if (this.mLargeDataPort != null) this.mLargeDataPort.close();

        // Disconnect device
        if (this.mWifiDirectDeviceController != null) this.mWifiDirectDeviceController.disconnect();

        // CommChannel State: transit to ConnectedDefault
        this.mState.transitToConnectedDefault();
    }

    public void lockLargeDataMode() {
        if (this.mLargeDataPortWatcher == null || !this.mLargeDataPortWatcher.isAlive()) {
            Log.e(TAG, "Since large data port is not being watched, cannot lock large data mode");
            return;
        } else if (this.mLargeDataPort == null) {
            Log.e(TAG, "Since large data port is not initialized, cannot lock large data mode");
            return;
        }
        this.mLargeDataPortWatcher.startToUse();
    }

    public void unlockLargeDataMode() {
        if (this.mLargeDataPortWatcher == null || !this.mLargeDataPortWatcher.isAlive()) {
            Log.e(TAG, "Since large data port is not being watched, cannot unlock large data mode");
            return;
        } else if (this.mLargeDataPort == null) {
            Log.e(TAG, "Since large data port is not initialized, cannot unlock large data mode");
            return;
        }
        this.mLargeDataPortWatcher.endToUse();
    }

    public String getLargeDataIPAddress() {
        if (this.mLargeDataPort == null) return "";

        return this.mLargeDataPort.getIPAddress();
    }

    // On received raw message (via default or largedata port)
    @Override
    public void onReceivingRawMessage(byte[] messageData, int messageDataLength, String filePath) {
        String listenedMessage = null;
        try {
            listenedMessage = new String(messageData, "UTF-8");
            CommBroadcastSender.onReceivedRawMessage(self, listenedMessage, filePath);
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onSuddenlyClosed(CommPort port) {
        if (port == this.mLargeDataPort) {
            this.disableLargeDataMode();
        } else if (port == this.mDefaultPort) {
            this.disconnectChannel();
        }
    }

    public byte[] transformToNullTerminatedString(String messageData) {
        // Transform to null-terminated string since Java does not produce null-terminated string
        byte[] messageBytes = new byte[messageData.length() + 1];
        System.arraycopy(messageData.getBytes(), 0, messageBytes, 0, messageData.length());
        messageBytes[messageBytes.length - 1] = '\0';
        return messageBytes;
    }

    public void sendRawMessage(String messageData) {
        this.sendRawMessage(messageData, null);
    }

    // Send raw message (via default or largedata port)
    public void sendRawMessage(String messageData, File file) {
        if (!this.isBluetoothDeviceConnected() || !this.isDefaultPortAvailable()) {
            return;
        }

        // Transform to null-terminated string since Java does not produce null-terminated string
        byte[] messageBytes = transformToNullTerminatedString(messageData);

        if (this.isLargeDataPortAvailable()) {
            int res;
            if (this.mLargeDataPort != null && this.mLargeDataPortWatcher.isAlive()) {
                this.mLargeDataPortWatcher.startToUse();
            }

            res = this.mLargeDataPort.sendRawMessage(messageBytes, messageBytes.length, file);

            if (this.mLargeDataPort != null && this.mLargeDataPortWatcher.isAlive()) {
                this.mLargeDataPortWatcher.endToUse();
            }

            if (res < 0) {
                this.disableLargeDataMode();
            }
        } else {
            int res;
            res = this.mDefaultPort.sendRawMessage(messageBytes, messageBytes.length, file);
            if (res < 0) {
                this.disconnectChannel();
            }
        }
    }

    private boolean sendRawMessageOnControl(String messageData, File file) {
        if (!isBluetoothDeviceConnected() || !isDefaultPortAvailable()) {
            return false;
        }

        // Transform to null-terminated string since Java does not produce null-terminated string
        byte[] messageBytes = transformToNullTerminatedString(messageData);

        int res = mControlPort.sendRawMessage(messageBytes, messageBytes.length, file);
        if (res < 0) {
            return false;
        }
        return true;
    }

    // Connect CommChannelService = Connect Bluetooth Device + Open Default Port
    private class ConnectChannelProcedure {
        private BluetoothConnectingResultListener mConnectingResultListener = null;

        public void start() {
            // TODO: storing connection information and auto-connection
            // (Connect) Step 1. Connect Bluetooth Device
            this.mConnectingResultListener = new ConnectingResultListener();
            mBluetoothDeviceController.connect(this.mConnectingResultListener);
        }

        private class ConnectingResultListener implements BluetoothConnectingResultListener {
            @Override
            public void onConnectingBluetoothDeviceSuccess(BluetoothDevice bluetoothDevice) {
                Log.d(TAG, "Connecting Bluetooth Device success");
                mDefaultPort.setBluetoothDevice(bluetoothDevice);
                mControlPort.setBluetoothDevice(bluetoothDevice);
                openDefaultPort();
            }

            @Override
            public void onConnectingBluetoothDeviceFail() {
                Log.d(TAG, "Connecting Bluetooth Device fail");
                onFail();
            }
        }

        private void openDefaultPort() {
            // (Connect) Step 2. Open Port
            boolean isOpeningSuccess = mDefaultPort.open();

            // Connect the Bluetooth device
            if (isOpeningSuccess) {
                onSuccess();
            } else {
                onFail();
            }
        }

        private void onSuccess() {
            // Start listening thread
            if (mDefaultPort != null) mDefaultPort.runListeningThread(self, mDownloadFilePath);

            // CommChannel State: transit to ConnectedDefault
            Log.d(TAG, "Succeed to connect CommChannel");
            mState.transitToConnectedDefault();
        }

        private void onFail() {
            disconnectChannel();
        }
    }

    // Enable Largedata = Notify Wi-fi direct ON Command + Connect WFD Device + Open
    // Largedata Port
    private class EnableLargeDataModeProcedure {
        private ControlPortListener mControlPortListener = null;
        private ConnectingResultListener mConnectingResultListener = null;
        private String mIpAddress = null;

        public void start() {
            // (Enable Largedata) Step 1. Notify Wi-fi direct ON Command
            boolean isOpenSuccess = mControlPort.open();
            if (!isOpenSuccess) return;

            // Start listening thread
            this.mControlPortListener = new ControlPortListener();
            mControlPort.runListeningThread(this.mControlPortListener, mDownloadFilePath);

            // Send turning on Wi-fi direct message
            boolean res = sendRawMessageOnControl(kWifiDirectOnMessage, null);
            if (!res) {
                onFail();
            }
        }

        class ControlPortListener implements CommPortListener {
            @Override
            public void onReceivingRawMessage(byte[] messageData, int messageDataLength, String
                    filePath) {
                String rawMessage = null;
                try {
                    rawMessage = new String(messageData, "UTF-8");
                    String[] words = rawMessage.split("\\r?\\n");
                    mIpAddress = words[0];
                    String wifiDirectName = words[1];
                    discoverAndConnectWifiDirectDevice(wifiDirectName);
                } catch (UnsupportedEncodingException e) {
                    e.printStackTrace();
                    onFail();
                }
            }

            @Override
            public void onSuddenlyClosed(CommPort port) {
                Log.e(TAG, "Control port is suddenly closed");
                onFail();
            }
        }

        private void discoverAndConnectWifiDirectDevice(String wifiDirectName) {
            // (Enable Largedata) Step 2. Connect WFD Device
            this.mConnectingResultListener = new ConnectingResultListener();
            mWifiDirectDeviceController.connect(this.mConnectingResultListener, wifiDirectName);
        }

        class ConnectingResultListener implements WifiDirectConnectingResultListener {
            @Override
            public void onConnectingWifiDirectDeviceSuccess() {
                openLargeDataPort();
            }

            @Override
            public void onConnectingWifiDirectDeviceFail() {
                onFail();
            }
        }

        private void openLargeDataPort() {
            // (Enable Largedata) Step 3. Open largedata port
            int kSleepMillisecs = 1000;
            int kMaxTries = 10;
            boolean isOpeningSuccess = false;
            int tries = 0;
            while (!isOpeningSuccess && tries++ < kMaxTries) {
                isOpeningSuccess = mLargeDataPort.open(this.mIpAddress);

                // Connect the Bluetooth device
                if (isOpeningSuccess) {
                    Log.d(TAG, "Succeed to connect large data (" + tries + "/" + kMaxTries + ")");
                    this.onSuccess();
                } else {
                    Log.d(TAG, "Fail to connect large data (" + tries + "/" + kMaxTries + ")");
                }

                try {
                    Thread.sleep(kSleepMillisecs);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            if(!isOpeningSuccess) {
                this.onFail();
            }
        }

        private void onSuccess() {
            // Start listening thread
            if (mLargeDataPort != null) mLargeDataPort.runListeningThread(self, mDownloadFilePath);

            // Start watcher thread
            mLargeDataPortWatcher = new LargeDataPortWatcher();
            mLargeDataPortWatcher.startToWatch();

            // CommChannel State: transit to ConnectedLargeData
            mState.transitToConnectedLargeData();
        }

        private void onFail() {
            //disableLargeDataMode();
        }
    }

    private class LargeDataPortWatcher extends Thread {
        static private final String TAG = "LargeDataPortWatcher";
        static private final long kSleepMillisecs = 1000;
        static private final long kThresholdMillisecs = 5000;

        private Integer mNumInUse = 0;
        private Date mLastAccess;
        private boolean mIsEnabled = false;

        public void startToWatch() {
            this.start();
        }

        public void stopToWatch() {
            this.mIsEnabled = false;
        }

        public void startToUse() {
            synchronized (this.mNumInUse) {
                this.mLastAccess = new Date();
                this.mNumInUse++;
            }
        }

        public void endToUse() {
            synchronized (this.mNumInUse) {
                this.mLastAccess = new Date();
                this.mNumInUse--;
            }
        }

        @Override
        public void run() {
            this.mIsEnabled = true;
            this.mLastAccess = new Date();

            while (this.mIsEnabled) {
                synchronized (this.mNumInUse) {
                    Date now = new Date();
                    Log.d(TAG, "LargeDataPort users = " + mNumInUse + " recent access = " + this
                            .mLastAccess);
                    if ((mNumInUse == 0) && ((now.getTime() - this.mLastAccess.getTime()) >
                            kThresholdMillisecs)) {

                        Log.d(TAG, "No users detected for large data port over " +
                                kThresholdMillisecs + " ms.");
                        //disableLargeDataMode();
                    }
                }

                try {
                    Thread.sleep(kSleepMillisecs);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    // State
    public int getCommChannelState() {
        return this.mState.get();
    }

    private class State {
        private int mState = STATE_DISCONNECTED;
        private DeviceStateListener mDeviceStateListener;

        public State() {
            this.mDeviceStateListener = new DeviceStateListener();
        }

        private void transitTo(int newState) {
            int prevState = this.mState;
            this.mState = newState;

            // Broadcast CommChannel disconnection event via CommBroadcastSender
            if (prevState != newState)
                CommBroadcastSender.onCommChannelStateChanged(self, prevState, newState);
        }

        public void transitToDisconnected() {
            // Transit to new state
            this.transitTo(STATE_DISCONNECTED);

            // Stop to watch Bluetooth & Wi-fi Direct device's status
            this.stopToWatchDeviceState();
        }

        public void transitToConnectingDefault() {
            // Transit to new state
            this.transitTo(STATE_CONNECTING_DEFAULT);
        }

        public void transitToConnectedDefault() {
            // Transit to new state
            this.transitTo(STATE_CONNECTED_DEFAULT);

            // Start to watch Bluetooth & Wi-fi Direct device's status
            this.startToWatchDeviceState();
        }

        public void transitToConnectingLargeData() {
            // Transit to new state
            this.transitTo(STATE_CONNECTING_LARGE_DATA);
        }

        public void transitToConnectedLargeData() {
            // Transit to new state
            this.transitTo(STATE_CONNECTED_LARGE_DATA);
        }

        private void startToWatchDeviceState() {
            IntentFilter broadcastIntentFilter = new IntentFilter();
            broadcastIntentFilter.addAction(CommBroadcastReceiver.ACTION);
            mBluetoothDeviceController.addListener(this.mDeviceStateListener);
            mWifiDirectDeviceController.addListener(this.mDeviceStateListener);
        }

        private void stopToWatchDeviceState() {
            mBluetoothDeviceController.removeListener(this.mDeviceStateListener);
            mWifiDirectDeviceController.removeListener(this.mDeviceStateListener);
        }

        class DeviceStateListener implements BluetoothDeviceStateListener,
                WifiDirectDeviceStateListener {
            @Override
            public void onWifiDirectDeviceStateChanged(boolean isConnected) {
                if (!isConnected) {
                    Log.d(TAG, "Wi-fi Direct is disconnected");
                    if (mState == STATE_CONNECTED_LARGE_DATA || mState ==
                            STATE_CONNECTING_LARGE_DATA)
                        transitToConnectedDefault();
                }
            }

            @Override
            public void onBluetoothDeviceStateChanged(boolean isConnected) {
                if (!isConnected) {
                    if (mState == STATE_CONNECTED_DEFAULT || mState == STATE_CONNECTED_LARGE_DATA)
                        transitToDisconnected();
                }
            }
        }

        public int get() {
            return this.mState;
        }
    }

    // Android Service
    @Override
    public void onCreate() {
        super.onCreate();

        // Initialize device controllers
        this.mBluetoothDeviceController = new BluetoothDeviceController(this);
        this.mWifiDirectDeviceController = new WifiDirectDeviceController(this);

        // Initialize ports
        this.mDefaultPort = new BluetoothCommPort("DefaultPort", UUID.fromString
                (DEFAULT_PORT_BLUETOOTH_UUID));
        this.mControlPort = new BluetoothCommPort("ControlPort", UUID.fromString
                (CONTROL_PORT_BLUETOOTH_UUID));
        this.mLargeDataPort = new WifiDirectCommPort("LargeDataPort", LARGEDATA_PORT_TCP_PORT);
    }

    @Override
    public IBinder onBind(Intent intent) {
        this.mBindersCount++;

        return this.mBinder;
    }

    @Override
    public boolean onUnbind(Intent intent) {
        this.mBindersCount--;
        if (this.mBindersCount <= 0) {
            // No one uses CommChannelService
            this.disconnectChannel();
        }
        return false;
    }

    // Binder (for the caller of CommChannelService)
    public class CommBinder extends android.os.Binder {
        public CommChannelService getService() {
            return CommChannelService.this;
        }
    }
}