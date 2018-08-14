package com.redcarrottt.sc;

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
    boolean connectAdapter(int adapterId) {
        int state = this.getState();
        if (state != State.kSwitching) {
            Logger.VERB(kTag, "It's now switching. Cannot connect to adapter " + adapterId);
            return false;
        }
        this.setState(State.kSwitching);
        return runConnectRequestTx(adapterId);
    }

    // Reconnect control adapter command.
    // It is called by Core.
    boolean reconnectControlAdapter() {
        int state = this.getState();
        if (state != State.kSwitching) {
            Logger.VERB(kTag, "It's now switching. Cannot reconnect control adapter.");
            return false;
        }
        this.setState(State.kSwitching);
        return runReconnectControlAdapterTx();
    }

    // Notification of switch done event
    private void doneSwitch() {
        int state = this.getState();
        switch (state) {
            case State.kSwitching:
                this.setState(State.kRunning);
                break;
            case State.kInitialized:
            case State.kRunning:
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
        public static final int kInitialized = 0;
        public static final int kRunning = 1;
        public static final int kSwitching = 2;
    }

    // State
    private Integer mState;

    // Singleton
    private static NetworkSwitcher sSingleton;

    public static NetworkSwitcher getInstance() {
        if (sSingleton == null) {
            sSingleton = new NetworkSwitcher();
        }
        return sSingleton;
    }

    // Constructor
    private NetworkSwitcher() {
        // Since there is no NetworkSwitcher for Android implementation,
        // NetworkSwitcher's initial state goes to Running at beginning.
        this.mState = State.kRunning;
    }

    // Transactions
    // ----------------------------------------------------------------
    // Singleton runner
    private static boolean runConnectRequestTx(int adapterId) {
        if (sOngoingConnectRequest == null) {
            sOngoingConnectRequest = new ConnectRequestTransaction(adapterId);
            return sOngoingConnectRequest.start();
        } else {
            Logger.WARN(kTag, "Already connecting");
            return false;
        }
    }

    private static void doneConnectRequestTx() {
        NetworkSwitcher.getInstance().doneSwitch();
        sOngoingConnectRequest = null;
    }

    private static boolean runReconnectControlAdapterTx() {
        if (sOngoingReconnectControlAdapter == null) {
            sOngoingReconnectControlAdapter = new ReconnectControlAdapterTransaction();
            return sOngoingReconnectControlAdapter.start();
        } else {
            Logger.WARN(kTag, "Already stopping core");
            return false;
        }
    }

    private static void restartReconnectControlAdapterTx() {
        sOngoingReconnectControlAdapter = null;
        runReconnectControlAdapterTx();
    }

    private static ConnectRequestTransaction sOngoingConnectRequest = null;
    private static ReconnectControlAdapterTransaction sOngoingReconnectControlAdapter = null;

    private static class ConnectRequestTransaction {
        private ConnectRequestTransaction(int adapterId) {
            this.mAdapterId = adapterId;
            this.onConnectAdapter = new OnConnectAdapter();
        }

        boolean start() {
            ClientAdapter adapter = Core.getInstance().findDataAdapterById(this.mAdapterId);
            if (adapter == null) {
                Logger.ERR(kTag, "Connecting requested data adapter is failed");
                doneConnectRequestTx();
                return false;
            }
            boolean res = adapter.connect(onConnectAdapter, false);
            if (!res) {
                Logger.ERR(kTag, "Connecting requested data adapter is failed");
                doneConnectRequestTx();
                return false;
            }
            return true;
        }

        private OnConnectAdapter onConnectAdapter;

        class OnConnectAdapter implements ClientAdapter.ConnectResultListener {
            @Override
            public void onConnectResult(boolean isSuccess) {
                if (!isSuccess) {
                    Logger.ERR(kTag, "Connecting requested data adapter is failed");
                    doneConnectRequestTx();
                    return;
                }
                Logger.VERB(kTag, "Connecting requested data adapter is done");
                doneConnectRequestTx();
            }
        }

        private int mAdapterId;
    }

    private static class ReconnectControlAdapterTransaction {
        private ReconnectControlAdapterTransaction() {
            this.onConnectControlAdapter = new OnConnectControlAdapter();
            this.onDisconnectControlAdapter = new OnDisconnectControlAdapter();
        }

        boolean start() {
            ClientAdapter controlAdapter = Core.getInstance().getControlAdapter();
            if (controlAdapter == null) {
                Logger.ERR(kTag, "Reconnecting control adapter is failed: retry");
                restartReconnectControlAdapterTx();
                return false;
            }
            boolean res = controlAdapter.disconnect(onDisconnectControlAdapter);
            if (!res) {
                Logger.ERR(kTag, "Reconnecting control adapter is failed: retry");
                restartReconnectControlAdapterTx();
                return false;
            }
            return true;
        }

        private OnDisconnectControlAdapter onDisconnectControlAdapter;

        class OnDisconnectControlAdapter implements ClientAdapter.DisconnectResultListener {
            @Override
            public void onDisconnectResult(boolean isSuccess) {
                if (!isSuccess) {
                    Logger.ERR(kTag, "Reconnecting control adapter is failed: retry");
                    restartReconnectControlAdapterTx();
                    return;
                }
                ClientAdapter controlAdapter = Core.getInstance().getControlAdapter();
                if (controlAdapter == null) {
                    Logger.ERR(kTag, "Reconnecting control adapter is failed: retry");
                    restartReconnectControlAdapterTx();
                    return;
                }

                boolean res = controlAdapter.connect(onConnectControlAdapter, false);
                if (!res) {
                    Logger.ERR(kTag, "Reconnecting control adapter is failed: retry");
                    restartReconnectControlAdapterTx();
                }
            }
        }

        private OnConnectControlAdapter onConnectControlAdapter;

        class OnConnectControlAdapter implements ClientAdapter.ConnectResultListener {
            @Override
            public void onConnectResult(boolean isSuccess) {
                if (!isSuccess) {
                    Logger.ERR(kTag, "Reconnecting control adapter is failed: retry");
                    restartReconnectControlAdapterTx();
                    return;
                }

                Logger.VERB(kTag, "Reconnecting control adapter is done");
                NetworkSwitcher.getInstance().doneSwitch();
            }
        }
    }
}