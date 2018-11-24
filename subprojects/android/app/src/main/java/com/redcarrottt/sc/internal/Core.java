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

import com.redcarrottt.sc.api.OnStartSCResult;
import com.redcarrottt.sc.api.OnStopSCResult;
import com.redcarrottt.testapp.Logger;

import java.util.ArrayList;

public class Core {
    private static final String kTag = "Core";

    // APIs: These functions are mapped to ones in API.
    public void start(OnStartSCResult resultListener) {
        if (this.getState() != State.kIdle) {
            Logger.ERR(kTag, "Core has already started");
            doneStart(false, resultListener);
            return;
        } else if (this.mAdapters.isEmpty()) {
            Logger.ERR(kTag, "No adapter is registered!");
            doneStart(false, resultListener);
            return;
        }
        this.setState(State.kStarting);
        runStartCoreTx(this, resultListener);
    }

    private void doneStart(boolean isSuccess, OnStartSCResult resultListener) {
        if (isSuccess) {
            Logger.VERB(kTag, "Succeed to start core!");
            this.setState(State.kReady);

            // Launch control message receiving thread
            this.mControlMessageReceiver.startReceivingThread();
        } else {
            Logger.ERR(kTag, "Failed to start core!");
            this.setState(State.kIdle);
        }
        if (resultListener != null) resultListener.onDoneStartSC(isSuccess);
    }

    public void stop(OnStopSCResult resultListener) {
        int state = this.getState();
        if (state == State.kStarting || state == State.kStopping) {
            Logger.ERR(kTag, "Cannot stop core during starting/stopping!");
            doneStop(false, resultListener);
            return;
        } else if (state == State.kIdle) {
            Logger.ERR(kTag, "Core is already idle state!");
            doneStop(false, resultListener);
            return;
        } else if (this.mAdapters.isEmpty()) {
            Logger.ERR(kTag, "No adapter is registered!");
            doneStop(false, resultListener);
            return;
        }

        this.setState(State.kStopping);
        runStopCoreTx(this, resultListener);
    }

    private void doneStop(boolean isSuccess, OnStopSCResult resultListener) {
        if (isSuccess) {
            Logger.VERB(kTag, "Succeed to stop core!");
            this.setState(State.kIdle);

            // Finish control message receiving thread
            this.mControlMessageReceiver.stopReceivingThread();
        } else {
            Logger.ERR(kTag, "Failed to stop core!");
            this.setState(State.kReady);
        }
        if (resultListener != null) resultListener.onDoneStopSC(isSuccess);
    }

    public void registerAdapter(ClientAdapter adapter) {
        if (this.getState() != State.kIdle) {
            Logger.ERR(kTag, "You can register adapter on only idle state!");
            return;
        }

        synchronized (this.mAdapters) {
            this.mAdapters.add(adapter);
        }
    }

    public int send(byte[] dataBuffer, int dataLength, boolean isControl) {
        int state = this.getState();
        if (state != State.kReady) {
            Logger.ERR(kTag, "Core is not started yet, so you cannot send the data");
            return -1;
        }

        int currOffset = 0;

        // Attach the protocol header to the payload
        ProtocolData protocolData = ProtocolManager.data_to_protocol_data(dataBuffer, dataLength);
        if (protocolData == null) throw new AssertionError();

        int packetSize = ProtocolManager.serialize(protocolData, dataBuffer, currOffset,
                dataLength);
        if (!(packetSize > 0)) {
            Logger.ERR(kTag, "Failed to make protocol data to send(" + currOffset + "/" +
                    dataLength + ")");
            throw new AssertionError();
        }

        int sentBytes = ProtocolManager.send_packet(packetSize, isControl);
        if (sentBytes < 0) {
            Logger.ERR(kTag, "Sending stopped (" + currOffset + "/" + dataLength + ") by " +
                    sentBytes);
            throw new AssertionError();
        }

        return sentBytes;
    }

    public int receive(byte[] dataBuffer, boolean isControl) {
        int state = this.getState();
        if (state != State.kReady) {
            Logger.ERR(kTag, "Core is not started yet, so you cannot receive data");
            return -1;
        }

        return ProtocolManager.recv_packet(dataBuffer, isControl);
    }

    // State
    class State {
        public static final int kIdle = 0;
        public static final int kStarting = 1;
        public static final int kReady = 2;
        public static final int kStopping = 3;
    }

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


    // Adapters Getter
    public ClientAdapter findAdapterById(int adapterId) {
        for (ClientAdapter adapter : this.mAdapters) {
            if (adapter.getId() == adapterId) {
                return adapter;
            }
        }
        return null;
    }

    public ControlMessageReceiver getControlMessageReceiver() {
        return this.mControlMessageReceiver;
    }

    public ControlMessageSender getControlMessageSender() {
        return this.mControlMessageSender;
    }

    // Components
    private final ArrayList<ClientAdapter> mAdapters;
    private ControlMessageReceiver mControlMessageReceiver;
    private ControlMessageSender mControlMessageSender;

    // State
    private Integer mState;

    // Singleton
    private static Core sSingleton;

    public static Core singleton() {
        if (sSingleton == null) {
            sSingleton = new Core();
        }
        return sSingleton;
    }

    // Constructor
    private Core() {
        this.mState = State.kIdle;
        this.mAdapters = new ArrayList<>();
        this.mControlMessageReceiver = new ControlMessageReceiver();
        this.mControlMessageSender = new ControlMessageSender();
    }

    // Transactions
    // ----------------------------------------------------------------
    // Singleton runner
    private static void runStartCoreTx(Core caller, OnStartSCResult resultListener) {
        if (sOngoingStartCore == null) {
            sOngoingStartCore = new StartCoreTransaction(caller, resultListener);
            sOngoingStartCore.start();
        } else {
            Logger.WARN(kTag, "Already starting core");
            doneStartCoreTx(caller, false, resultListener);
        }
    }

    private static void doneStartCoreTx(Core caller, boolean isSuccess, OnStartSCResult
            resultListener) {
        sOngoingStartCore = null;
        caller.doneStart(isSuccess, resultListener);
    }

    private static void runStopCoreTx(Core caller, OnStopSCResult resultListener) {
        if (sOngoingStopCore == null) {
            sOngoingStopCore = new StopCoreTransaction(caller, resultListener);
            sOngoingStopCore.start();
        } else {
            Logger.WARN(kTag, "Already stopping core");
            doneStopCoreTx(caller, false, resultListener);
        }
    }

    private static void doneStopCoreTx(Core caller, boolean isSuccess, OnStopSCResult
            resultListener) {
        sOngoingStopCore = null;
        caller.doneStop(isSuccess, resultListener);
    }

    private static StartCoreTransaction sOngoingStartCore = null;
    private static StopCoreTransaction sOngoingStopCore = null;

    // Start Core
    private static class StartCoreTransaction {
        // Private Constructor
        private StartCoreTransaction(Core caller, OnStartSCResult resultListener) {
            this.mCaller = caller;
            this.mResultListener = resultListener;
            this.onConnectFirstAdapter = new OnConnectFirstAdapter();
        }

        @SuppressWarnings("SynchronizeOnNonFinalField")
        private void start() {
            // Connect first adapter
            ClientAdapter firstAdapter = this.mCaller.mAdapters.get(0);
            if (firstAdapter == null) {
                Logger.ERR(kTag, "Cannot find first adapter");
                doneStartCoreTx(mCaller, false, mResultListener);
                return;
            }
            firstAdapter.connect(this.onConnectFirstAdapter, false);
        }

        private OnConnectFirstAdapter onConnectFirstAdapter;

        class OnConnectFirstAdapter implements ClientAdapter.ConnectResultListener {
            @SuppressWarnings("SimplifiableIfStatement")
            @Override
            public void onConnectResult(boolean isSuccess) {
                if (isSuccess) {
                    // Done transaction
                    doneStartCoreTx(mCaller, true, mResultListener);
                } else {
                    Logger.ERR(kTag, "Connecting first adapter is failed");
                    doneStartCoreTx(mCaller, false, mResultListener);
                }
            }
        }

        // Attributes
        private Core mCaller;
        private OnStartSCResult mResultListener;
    }

    // Stop Core
    private static class StopCoreTransaction {
        private StopCoreTransaction(Core caller, OnStopSCResult resultListener) {
            this.mCaller = caller;
            this.mResultListener = resultListener;
            this.onDisconnectAdapter = new OnDisconnectAdapter();
            this.mAdaptersCount = 0;
        }

        @SuppressWarnings("SynchronizeOnNonFinalField")
        void start() {
            // Get active adapter count
            synchronized (this.mCaller.mAdapters) {
                for (ClientAdapter adapter : this.mCaller.mAdapters) {
                    int adapterState = adapter.getState();
                    if (adapterState != ClientAdapter.State.kDisconnected && adapterState !=
                            ClientAdapter.State.kDisconnecting) {
                        this.mAdaptersCount++;
                    }
                }
            }

            // Disconnect only active adapters
            synchronized (this.mCaller.mAdapters) {
                for (ClientAdapter adapter : this.mCaller.mAdapters) {
                    int adapterState = adapter.getState();
                    if (adapterState != ClientAdapter.State.kDisconnected && adapterState !=
                            ClientAdapter.State.kDisconnecting) {
                        adapter.disconnectOnCommand(onDisconnectAdapter);
                    }
                }
            }
        }

        private OnDisconnectAdapter onDisconnectAdapter;

        class OnDisconnectAdapter implements ClientAdapter.DisconnectResultListener {
            @Override
            public void onDisconnectResult(boolean isSuccess) {
                if (!isSuccess) {
                    Logger.ERR(kTag, "Disconnecting adapter is failed");
                    doneStopCoreTx(mCaller, false, mResultListener);
                    return;
                }

                // Check if all the active adapters are disconnected
                boolean doneDisconnectAll = false;
                synchronized (mAdaptersCount) {
                    mAdaptersCount--;
                    if (mAdaptersCount == 0) {
                        doneDisconnectAll = true;
                    }
                }

                if (doneDisconnectAll) {
                    doneStopCoreTx(mCaller, true, mResultListener);
                }
            }
        }

        // Attributes
        private Core mCaller;
        private Integer mAdaptersCount;
        private OnStopSCResult mResultListener;
    }
}