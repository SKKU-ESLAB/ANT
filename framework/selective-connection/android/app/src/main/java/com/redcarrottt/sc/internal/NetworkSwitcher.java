package com.redcarrottt.sc.internal;

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

import com.redcarrottt.testapp.Logger;

public class NetworkSwitcher {
    private static final String kTag = "NetworkSwitcher";

    // Connect adapter command.
    // It is called by peer through Core.
    void connectAdapterByPeer(int adapterId) {
        int state = this.getState();
        if (state == State.kSwitching) {
            Logger.VERB(kTag, "It's now switching. Cannot connect to adapter " + adapterId);
            return;
        }
        this.setState(State.kSwitching);
        runConnectRequestTx(adapterId);
    }

    // Disconnect adapter command.
    // It is called by peer through Core.
    void disconnectAdapterByPeer(int adapterId, int finalSeqNoControl, int finalSeqNoData) {
        int state = this.getState();
        if (state == State.kSwitching) {
            Logger.VERB(kTag, "It's now switching. Cannot disconnect to adapter " + adapterId);
            return;
        }

        Core core = Core.singleton();
        ClientAdapter adapter = core.findAdapterById(adapterId);
        if (adapter == null) {
            Logger.WARN(kTag, "Cannot find adapter " + adapterId);
            return;
        }

        this.setState(State.kSwitching);

        adapter.disconnectOnPeerCommand(onResultDisconnectAdapterByPeer, finalSeqNoControl, finalSeqNoData);
    }

    ClientAdapter.DisconnectResultListener onResultDisconnectAdapterByPeer = new ClientAdapter
            .DisconnectResultListener() {
        @Override
        public void onDisconnectResult(boolean isSuccess) {
            NetworkSwitcher switcher = NetworkSwitcher.singleton();
            switcher.doneSwitch();
        }
    };

    // Sleep adapter command.
    // It is called by peer through Core.
    void sleepAdapterByPeer(int adapterId) {
        int state = this.getState();
        if (state == State.kSwitching) {
            Logger.VERB(kTag, "It's now switching. Cannot sleep to adapter " + adapterId);
            return;
        }
        this.setState(State.kSwitching);
        ClientAdapter adapter = Core.singleton().findAdapterById(adapterId);
        adapter.sleep(false);
        this.setState(State.kReady);
    }

    // Wake up adapter command.
    // It is called by peer through Core.
    void wakeUpAdapterByPeer(int adapterId) {
        int state = this.getState();
        if (state == State.kSwitching) {
            Logger.VERB(kTag, "It's now switching. Cannot sleep to adapter " + adapterId);
            return;
        }
        this.setState(State.kSwitching);
        ClientAdapter adapter = Core.singleton().findAdapterById(adapterId);
        adapter.wakeUp(false);
        this.setState(State.kReady);
    }

    // Reconnect adapter command.
    // It is called by Core.
    void reconnectAdapter(ClientAdapter adapter) {
        // If it is disconnecting on purpose, do not reconnect it.
        if (adapter.isDisconnectingOnPurpose()) {
            return;
        }

        int state = this.getState();
        if (state == State.kSwitching) {
            Logger.VERB(kTag, adapter.getName() + ": It's now switching. Cannot reconnect " +
                    "adapter" + ".");
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            this.reconnectAdapter(adapter);
            return;
        }
        this.setState(State.kSwitching);
        runReconnectAdapterTx(adapter);
    }

    // Notification of switch done event
    private void doneSwitch() {
        int state = this.getState();
        switch (state) {
            case State.kSwitching:
                this.setState(State.kReady);
                break;
            case State.kReady:
                break;
        }
    }

    // State Getter/Setter
    @SuppressWarnings("SynchronizeOnNonFinalField")
    private int getState() {
        int state;
        synchronized (this.mState) {
            state = this.mState;
        }
        return state;
    }

    @SuppressWarnings("SynchronizeOnNonFinalField")
    private void setState(int newState) {
        synchronized (this.mState) {
            this.mState = newState;
        }
    }

    class State {
        public static final int kReady = 0;
        public static final int kSwitching = 1;
    }

    // State
    private Integer mState;

    // Singleton
    private static NetworkSwitcher sSingleton;

    public static NetworkSwitcher singleton() {
        if (sSingleton == null) {
            sSingleton = new NetworkSwitcher();
        }
        return sSingleton;
    }

    // Constructor
    private NetworkSwitcher() {
        // Since there is no NetworkSwitcher for Android implementation,
        // NetworkSwitcher's initial state goes to Running at beginning.
        this.mState = State.kReady;
    }

    // Transactions
    // ----------------------------------------------------------------
    // Singleton runner
    private static boolean runConnectRequestTx(int adapterId) {
        if (sOngoingConnectRequest == null) {
            sOngoingConnectRequest = new ConnectRequestTransaction(adapterId);
            sOngoingConnectRequest.start();
            return true;
        } else {
            Logger.WARN(kTag, "Already connecting");
            return false;
        }
    }

    private static void doneConnectRequestTx(boolean isSuccess) {
        sOngoingConnectRequest = null;
        if (!isSuccess) {
            Logger.WARN(kTag, "Connection request failed");
        }
        NetworkSwitcher.singleton().doneSwitch();
        sOngoingConnectRequest = null;
    }

    private static void runReconnectAdapterTx(ClientAdapter adapter) {
        if (sOngoingReconnectAdapter == null) {
            sOngoingReconnectAdapter = new ReconnectAdapterTransaction(adapter);
            sOngoingReconnectAdapter.start();
        } else {
            Logger.WARN(kTag, "Already stopping core");
        }
    }

    private static void restartReconnectAdapterTx(ClientAdapter adapter) {
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        sOngoingReconnectAdapter = null;
        runReconnectAdapterTx(adapter);
    }

    private static ConnectRequestTransaction sOngoingConnectRequest = null;
    private static ReconnectAdapterTransaction sOngoingReconnectAdapter = null;

    private static class ConnectRequestTransaction {
        private ConnectRequestTransaction(int adapterId) {
            this.mAdapterId = adapterId;
            this.onConnectAdapter = new OnConnectAdapter();
        }

        void start() {
            ClientAdapter adapter = Core.singleton().findAdapterById(this.mAdapterId);
            if (adapter == null) {
                Logger.ERR(kTag, "Connecting requested adapter is failed");
                doneConnectRequestTx(false);
                return;
            }
            adapter.connect(onConnectAdapter, false);
        }

        private OnConnectAdapter onConnectAdapter;

        class OnConnectAdapter implements ClientAdapter.ConnectResultListener {
            @Override
            public void onConnectResult(boolean isSuccess) {
                if (!isSuccess) {
                    Logger.ERR(kTag, "Connecting requested adapter is failed");
                    doneConnectRequestTx(false);
                    return;
                }
                Logger.VERB(kTag, "Connecting requested adapter is done");
                doneConnectRequestTx(true);
            }
        }

        private int mAdapterId;
    }

    private static class ReconnectAdapterTransaction {
        private ReconnectAdapterTransaction(ClientAdapter targetAdapter) {
            this.onConnectAdapter = new OnConnectAdapter();
            this.onDisconnectAdapter = new OnDisconnectAdapter();
            this.mTargetAdapter = targetAdapter;
        }

        private ClientAdapter mTargetAdapter;

        void start() {
            if (this.mTargetAdapter == null) {
                Logger.ERR(kTag, "Reconnecting adapter is failed: retry");
                restartReconnectAdapterTx(mTargetAdapter);
                return;
            }
            this.mTargetAdapter.disconnectOnFailure(onDisconnectAdapter);
        }

        private OnDisconnectAdapter onDisconnectAdapter;

        class OnDisconnectAdapter implements ClientAdapter.DisconnectResultListener {
            @Override
            public void onDisconnectResult(boolean isSuccess) {
                if (!isSuccess) {
                    Logger.ERR(kTag, "Reconnecting adapter is failed: retry");
                    restartReconnectAdapterTx(mTargetAdapter);
                    return;
                }
                if (mTargetAdapter == null) {
                    Logger.ERR(kTag, "Reconnecting adapter is failed: retry");
                    restartReconnectAdapterTx(mTargetAdapter);
                    return;
                }

                mTargetAdapter.connect(onConnectAdapter, false);
            }
        }

        private OnConnectAdapter onConnectAdapter;

        class OnConnectAdapter implements ClientAdapter.ConnectResultListener {
            @Override
            public void onConnectResult(boolean isSuccess) {
                if (!isSuccess) {
                    Logger.ERR(kTag, "Reconnecting adapter is failed: retry");
                    restartReconnectAdapterTx(mTargetAdapter);
                    return;
                }

                Logger.VERB(kTag, "Reconnecting adapter is done");
                NetworkSwitcher.singleton().doneSwitch();
            }
        }
    }
}