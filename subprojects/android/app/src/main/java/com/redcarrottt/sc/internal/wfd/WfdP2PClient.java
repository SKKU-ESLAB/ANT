package com.redcarrottt.sc.internal.wfd;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.NetworkInfo;
import android.net.wifi.WpsInfo;
import android.net.wifi.p2p.WifiP2pConfig;
import android.net.wifi.p2p.WifiP2pDevice;
import android.net.wifi.p2p.WifiP2pDeviceList;
import android.net.wifi.p2p.WifiP2pGroup;
import android.net.wifi.p2p.WifiP2pManager;
import android.os.Looper;

import com.redcarrottt.sc.internal.OnConnectResult;
import com.redcarrottt.sc.internal.OnDisconnectResult;
import com.redcarrottt.sc.internal.OnDiscoverResult;
import com.redcarrottt.sc.internal.P2PClient;
import com.redcarrottt.testapp.Logger;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

class WfdP2PClient extends P2PClient {
    private static final String kTag = "WfdP2PClient";

    @Override
    protected void discoverImpl(OnDiscoverResult onDiscoverResult) {
        // Run Discover Transaction
        runDiscoverTx(onDiscoverResult);
    }

    private void runDiscoverTx(OnDiscoverResult onDiscoverResult) {
        if (sOngoingDiscover == null) {
            sOngoingDiscover = new DiscoverTransaction();
            sOngoingDiscover.run(onDiscoverResult);
        } else {
            Logger.WARN(kTag, "Already stopping core");
            onDiscoverResult.onDoneDiscover(false);
        }
    }

    private static void doneDiscoverTx(OnDiscoverResult onDiscoverResult, boolean isSuccess) {
        onDiscoverResult.onDoneDiscover(isSuccess);
        sOngoingDiscover = null;
    }

    private static DiscoverTransaction sOngoingDiscover = null;

    private class DiscoverTransaction {
        // TODO: Adding timeout on this transaction would be good.
        private OnDiscoverResult mOnDiscoverResult;
        private int mTries = 0;
        private final int kMaxTries = 100;

        void run(OnDiscoverResult onDiscoverResult) {
            this.mTries = 0;
            this.mOnDiscoverResult = onDiscoverResult;
            stopP2PDiscovery();
        }

        void retry() {
            stopP2PDiscovery();
        }

        // Step 1
        private void stopP2PDiscovery() {
            Logger.DEBUG(kTag, "Stop discovery");
            mWFDManager.stopPeerDiscovery(mWFDChannel, new WifiP2pManager.ActionListener() {
                @Override
                public void onSuccess() {
                    startP2PDiscovery();
                }

                @Override
                public void onFailure(int i) {
                    Logger.ERR(kTag, "Stopping discovery failed / reason=" + i);
                    mTries++;
                    if (mTries < kMaxTries) {
                        retry();
                    } else {
                        doneDiscoverTx(mOnDiscoverResult, false);
                    }
                }
            });
        }

        // Step 2
        private void startP2PDiscovery() {
            Logger.DEBUG(kTag, "Start discovery");

            // Start to listen peer change
            IntentFilter intentFilter = new IntentFilter();
            intentFilter.addAction(WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION);
            Context context = mOwnerActivity.getApplicationContext();
            this.mPeersChangedReceiver = new PeersChangedReceiver();
            context.registerReceiver(mPeersChangedReceiver, intentFilter);
            this.mPeerListListener = new PeerListListener();

            // Discover peers
            mWFDManager.discoverPeers(mWFDChannel, null);
        }

        // Step 4
        private PeersChangedReceiver mPeersChangedReceiver;

        private class PeersChangedReceiver extends BroadcastReceiver {
            @Override
            public void onReceive(Context context, Intent intent) {
                String action = intent.getAction();
                assert action != null;
                if (action.equals(WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION)) {
                    Logger.DEBUG(kTag, "Peers changed event");

                    // Request peer list
                    Logger.DEBUG(kTag, "Request WFD peers");
                    mWFDManager.requestPeers(mWFDChannel, mPeerListListener);
                }
            }
        }

        ;

        // Step 5
        private PeerListListener mPeerListListener;

        class PeerListListener implements WifiP2pManager.PeerListListener {
            private Lock mCallbackLock;
            private Boolean mIsPeerFound;

            PeerListListener() {
                this.mCallbackLock = new ReentrantLock();
                this.mIsPeerFound = false;
            }

            @Override
            public void onPeersAvailable(WifiP2pDeviceList peers) {
                final WifiP2pDeviceList kPeers = peers;
                // Since getting peer device address requires locks, it should run on child thread.
                Thread childThread = new Thread() {
                    @Override
                    public void run() {
                        if (mCallbackLock.tryLock()) {
                            try {
                                synchronized (mIsPeerFound) {
                                    if (!mIsPeerFound) {
                                        Logger.DEBUG(kTag, "Got peer list (Target=" +
                                                getTargetMacAddress() + ")");
                                        for (WifiP2pDevice p2pDevice : kPeers.getDeviceList()) {
                                            Logger.DEBUG(kTag, "Peer: " + p2pDevice.deviceAddress
                                                    + " / " + p2pDevice.deviceName + " / " +
                                                    p2pDevice.status);

                                            if (p2pDevice.deviceAddress.equals
                                                    (getTargetMacAddress()) && p2pDevice.status
                                                    == WifiP2pDevice.AVAILABLE) {
                                                mFoundWFDDevice = p2pDevice;
                                                Logger.DEBUG(kTag, "Peer found");
                                                mIsPeerFound = true;
                                                break;
                                            }
                                        }
                                    }
                                }

                                // Unregister PeersChangedReceiver
                                if (mPeerListListener != null) {
                                    mOwnerActivity.getApplicationContext().unregisterReceiver
                                            (mPeersChangedReceiver);
                                    mPeerListListener = null;
                                }

                                synchronized (mIsPeerFound) {
                                    if (mIsPeerFound) {
                                        doneDiscoverTx(mOnDiscoverResult, true);
                                    } else {
                                        mTries++;
                                        if (mTries < kMaxTries) {
                                            retry();
                                        } else {
                                            doneDiscoverTx(mOnDiscoverResult, false);
                                        }
                                    }
                                }
                            } finally {
                                mCallbackLock.unlock();
                            }
                        }
                    }
                };
                childThread.start();
            }
        }
    }

    private WifiP2pDevice mFoundWFDDevice = null;

    // TODO: Adding timeout on this transaction would be good.
    @Override
    protected void connectImpl(OnConnectResult onConnectResult) {
        if (this.mFoundWFDDevice == null) {
            Logger.ERR(kTag, "Not found target WFD device");
            onConnectResult.onDoneConnect(false);
            return;
        }

        // Start to listen P2P connection event
        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(WifiP2pManager.WIFI_P2P_CONNECTION_CHANGED_ACTION);
        Context context = mOwnerActivity.getApplicationContext();
        mWFDConnectionReceiver.setOnConnectResult(onConnectResult);
        context.registerReceiver(mWFDConnectionReceiver, intentFilter);

        // Setting P2P config
        Logger.DEBUG(kTag, "Connect WFD Peer");
        WifiP2pDevice p2pDevice = this.mFoundWFDDevice;
        WifiP2pConfig p2pConfig = new WifiP2pConfig();
        p2pConfig.deviceAddress = p2pDevice.deviceAddress;
        if (p2pDevice.wpsKeypadSupported()) {
            Logger.DEBUG(kTag, "WPS Method: Keypad");
            p2pConfig.wps.setup = WpsInfo.KEYPAD;
            p2pConfig.wps.pin = this.getWpsPin();
        } else if (p2pDevice.wpsPbcSupported()) {
            Logger.DEBUG(kTag, "WPS Method: PBC - Unsupported");
            p2pConfig.wps.setup = WpsInfo.PBC;
            onConnectResult.onDoneConnect(false);
            return;
        } else if (p2pDevice.wpsDisplaySupported()) {
            Logger.DEBUG(kTag, "WPS Method: Display - Unsupported");
            onConnectResult.onDoneConnect(false);
            return;
        }
        p2pConfig.groupOwnerIntent = 0;

        // P2P connection request
        final OnConnectResult fOnConnectResult = onConnectResult;
        Logger.DEBUG(kTag, "Request to connect: " + p2pDevice.deviceName + " " + p2pDevice
                .deviceAddress + " (" + p2pConfig.wps.pin + ")");
        mWFDManager.connect(mWFDChannel, p2pConfig, new WifiP2pManager.ActionListener() {
            @Override
            public void onSuccess() {
                Logger.DEBUG(kTag, "WFD Connection Success");
            }

            @Override
            public void onFailure(int i) {
                Logger.WARN(kTag, "WFD Connection Failure / reason=" + i + " - retry to connect...");
                connectImpl(fOnConnectResult);
            }
        });
    }

    private WFDConnectionReceiver mWFDConnectionReceiver = new WFDConnectionReceiver();

    private class WFDConnectionReceiver extends BroadcastReceiver {
        private OnConnectResult mOnConnectResult = null;

        public void setOnConnectResult(OnConnectResult onConnectResult) {
            this.mOnConnectResult = onConnectResult;
        }

        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            assert (action != null);

            if (action.equals(WifiP2pManager.WIFI_P2P_CONNECTION_CHANGED_ACTION)) {
                NetworkInfo netInfo = (NetworkInfo) intent.getParcelableExtra(WifiP2pManager
                        .EXTRA_NETWORK_INFO);
                Logger.DEBUG(kTag, "Wi-fi direct connection change detected! " + netInfo.toString
                        ());
                if (netInfo.isConnected()) {
                    Logger.DEBUG(kTag, "Wi-fi direct Connected!");
                    WifiP2pGroup p2pGroup = intent.getParcelableExtra(WifiP2pManager
                            .EXTRA_WIFI_P2P_GROUP);
                    String goMacAddress = p2pGroup.getOwner().deviceAddress;
                    Logger.DEBUG(kTag, "Group owner address: " + goMacAddress);
                    if (goMacAddress != null && goMacAddress.equals(getTargetMacAddress())) {
                        context.unregisterReceiver(mWFDConnectionReceiver);
                        mOnConnectResult.onDoneConnect(true);
                    }
                }
            }
        }
    }

    @Override
    protected void disconnectImpl(OnDisconnectResult onDisconnectResult) {
        final OnDisconnectResult kOnDisconnectResult = onDisconnectResult;
        mWFDManager.removeGroup(mWFDChannel, new WifiP2pManager.ActionListener() {
            @Override
            public void onSuccess() {
                kOnDisconnectResult.onDoneDisconnect(true);
            }

            @Override
            public void onFailure(int i) {
                kOnDisconnectResult.onDoneDisconnect(false);
            }
        });
    }

    public void setWfdP2PInfo(String targetMacAddress, String wpsPin) {
        this.mTargetMacAddress = targetMacAddress;
        this.mWpsPin = wpsPin;

        Logger.DEBUG(kTag, "Notify: WfdP2PInfo");
        this.mIsInfoSet = true;
        synchronized (this.mInfoSetTrigger) {
            this.mInfoSetTrigger.notifyAll();
        }
    }

    private String getTargetMacAddress() {
        // Wait until target mac address is set, then get the information.
        if (this.mIsInfoSet) {
            return this.mTargetMacAddress;
        } else {
            Logger.DEBUG(kTag, "Waiting: mTargetMacAddress");
            while (!this.mIsInfoSet) {
                synchronized (this.mInfoSetTrigger) {
                    try {
                        this.mInfoSetTrigger.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
            Logger.DEBUG(kTag, "Got: mTargetMacAddress");
            return this.mTargetMacAddress;
        }
    }

    private String getWpsPin() {
        // Wait until wps pin is set, then get the information.
        if (this.mIsInfoSet) {
            return this.mWpsPin;
        } else {
            Logger.DEBUG(kTag, "Waiting: mWpsPin");
            while (!this.mIsInfoSet) {
                synchronized (this.mInfoSetTrigger) {
                    try {
                        this.mInfoSetTrigger.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
            Logger.DEBUG(kTag, "Got: mWpsPin");
            return this.mWpsPin;
        }
    }

    // Constructor
    public WfdP2PClient(Activity ownerActivity) {
        this.mOwnerActivity = ownerActivity;
        this.mIsInfoSet = false;
        this.mInfoSetTrigger = new Object();

        // Initialize Android WFD Manager Channel
        this.mWFDManager = (WifiP2pManager) this.mOwnerActivity.getApplicationContext()
                .getSystemService(Context.WIFI_P2P_SERVICE);
        if (this.mWFDManager == null) {
            Logger.ERR(kTag, "Failed to initialize Wi-fi Direct Manager");
            return;
        }


        this.mWFDChannel = this.mWFDManager.initialize(this.mOwnerActivity, Looper.getMainLooper
                (), null);
        if (this.mWFDChannel == null) {
            Logger.ERR(kTag, "Failed to initialize Wi-fi Direct Manager");
        }
    }

    // Attributes
    private Activity mOwnerActivity;

    // P2P Info Attributes
    private boolean mIsInfoSet;
    private final Object mInfoSetTrigger;
    private String mTargetMacAddress;
    private String mWpsPin;

    // Components
    private WifiP2pManager mWFDManager;
    private WifiP2pManager.Channel mWFDChannel;
}
