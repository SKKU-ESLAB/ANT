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

import java.nio.ByteBuffer;
import java.util.ArrayList;

public class Core {
    private static final String kTag = "Core";

    // APIs: These functions are mapped to ones in API.
    public void start(OnStartSCResult resultListener) {
        if (this.getState() != State.kIdle) {
            Logger.ERR(kTag, "Core has already started");
            doneStart(false, resultListener);
            return;
        } else if (this.mControlAdapter == null) {
            Logger.ERR(kTag, "No control adapter is registered!");
            doneStart(false, resultListener);
            return;
        } else if (this.mDataAdapters.isEmpty()) {
            Logger.ERR(kTag, "No data adapter is registered!");
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
        } else if (this.mControlAdapter == null) {
            Logger.ERR(kTag, "No control adapter is registered!");
            doneStop(false, resultListener);
            return;
        } else if (this.mDataAdapters.isEmpty()) {
            Logger.ERR(kTag, "No data adapter is registered!");
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
        } else {
            Logger.ERR(kTag, "Failed to stop core!");
            this.setState(State.kReady);
        }
        if (resultListener != null) resultListener.onDoneStopSC(isSuccess);
    }

    public void registerControlAdapter(ClientAdapter controlAdapter) {
        if (this.getState() != State.kIdle) {
            Logger.ERR(kTag, "You can register control adapter on only idle state!");
            return;
        }

        if (this.mControlAdapter != null) {
            Logger.ERR(kTag, "Another control adapter has already been registered!");
            return;
        }
        controlAdapter.enableReceiverThread(new ReceiveControlMessageLoop());

        this.mControlAdapter = controlAdapter;
    }

    public void registerDataAdapter(ClientAdapter dataAdapter) {
        if (this.getState() != State.kIdle) {
            Logger.ERR(kTag, "You can register data adapter on only idle state!");
            return;
        }
        synchronized (this.mDataAdapters) {
            dataAdapter.enableReceiverThread();
            dataAdapter.enableSenderThread();

            this.mDataAdapters.add(dataAdapter);
        }
    }

    public int send(byte[] dataBuffer, int dataLength) {
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

        int sentBytes = ProtocolManager.send_packet(packetSize);
        if (sentBytes < 0) {
            Logger.ERR(kTag, "Sending stopped (" + currOffset + "/" + dataLength + ") by " +
                    sentBytes);
            throw new AssertionError();
        }

        return sentBytes;
    }

    public int receive(byte[] dataBuffer) {
        int state = this.getState();
        if (state != State.kReady) {
            Logger.ERR(kTag, "Core is not started yet, so you cannot receive data");
            return -1;
        }

        return ProtocolManager.recv_packet(dataBuffer);
    }

    // Control Message Handling
    private void sendControlMessage(byte[] dataBuffer, int dataLength) {
        int state = this.getState();
        if (state != State.kReady) {
            Logger.ERR(kTag, "Core is not started yet, so you cannot send the data");
            return;
        }
        ClientAdapter controlAdapter = this.mControlAdapter;
        controlAdapter.send(dataBuffer, dataLength);
    }

    private void sendRequest(char requestCode, short adapterId) {
        int state = this.getState();
        if (state != State.kReady) {
            Logger.ERR(kTag, "Core is not started yet, so you cannot send the data");
            return;
        }

        {
            ByteBuffer buffer = ByteBuffer.allocate(3);
            buffer.put((byte)requestCode);
            buffer.putShort(adapterId);
            this.sendControlMessage(buffer.array(), 3);
        }
    }

    public void sendRequestConnect(short adapterId) {
        this.sendRequest(CtrlReq.kConnect, adapterId);
    }

    public void sendRequestSleep(short adapterId) {
        this.sendRequest(CtrlReq.kSleep, adapterId);
    }

    public void sendRequestWakeup(short adapterId) {
        this.sendRequest(CtrlReq.kWakeup, adapterId);
    }

    public void sendNotiPrivateData(short adapterId, byte[] privDataBuffer) {
        // Not implemented yet
    }

    class ReceiveControlMessageLoop implements ClientAdapter.ReceiveLoop {
        @Override
        public void receiveLoop(ClientAdapter adapter) {
            byte[] dataBuffer = new byte[512];
            int res;
            while (true) {
                // Receive 1Byte: Control Request Code
                res = adapter.receive(dataBuffer, 1);
                if (res <= 0) {
                    Logger.VERB(kTag, "Control adapter could be closed");
                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    break;
                }

                Byte reqCode = dataBuffer[0];
                if (reqCode == CtrlReq.kConnect || reqCode == CtrlReq.kSleep || reqCode ==
                        CtrlReq.kWakeup) {
                    // "Connect Adapter" Request
                    // Receive 2Byte: Adapter ID
                    res = adapter.receive(dataBuffer, 2);
                    if (res <= 0) break;
                    ByteBuffer buffer = ByteBuffer.allocate(2);
                    buffer.put(dataBuffer, 0, 2);
                    short adapterId = buffer.getShort(0);

                    if (reqCode == CtrlReq.kConnect) {
                        Logger.DEBUG(kTag, "Control Request: 'Connect Adapter Request' (" +
                                adapterId + ")");
                        NetworkSwitcher.getInstance().connectAdapter(adapterId);
                    } else if (reqCode == CtrlReq.kSleep) {
                        Logger.DEBUG(kTag, "Sleep Request: 'Connect Adapter Request' (" +
                                adapterId + ")");
                        NetworkSwitcher.getInstance().sleepAdapter(adapterId);
                    } else if (reqCode == CtrlReq.kWakeup) {
                        Logger.DEBUG(kTag, "Wake Up Request: 'Connect Adapter Request' (" +
                                adapterId + ")");
                        NetworkSwitcher.getInstance().wakeUpAdapter(adapterId);
                    }
                } else if (reqCode == CtrlReq.kPriv) {
                    // "Priv" Request
                    short adapterId;
                    int privDataLength;
                    // Receive 2Byte: Adapter ID
                    res = adapter.receive(dataBuffer, 2);
                    if (res <= 0) {
                        break;
                    } else {
                        ByteBuffer tempBuffer = ByteBuffer.allocate(2);
                        //Logger.DEBUG(kTag, "tempBuffer remaining: " + tempBuffer.remaining());
                        tempBuffer.put(dataBuffer, 0, 2);
                        adapterId = tempBuffer.getShort(0);
                    }

                    // Receive 4Byte: Private Data Length
                    res = adapter.receive(dataBuffer, 4);
                    if (res <= 0) {
                        break;
                    } else {
                        ByteBuffer tempBuffer2 = ByteBuffer.allocate(4);
                        //Logger.DEBUG(kTag, "tempBuffer2 remaining: " + tempBuffer2.remaining());
                        tempBuffer2.put(dataBuffer, 0, 4);
                        privDataLength = tempBuffer2.getInt(0);
                    }
                    if (privDataLength > 512) throw new AssertionError();

                    // Receive nByte: Private Data
                    res = adapter.receive(dataBuffer, privDataLength);
                    if (res > 0) {
                        Logger.DEBUG(kTag, "Control Request: 'Priv Data Noti' (" + adapterId + ")");
                        for (ControlMessageListener listener : mControlMessageListeners) {
                            listener.onReceiveControlMessage(adapterId, dataBuffer, privDataLength);
                        }
                    } else {
                        break;
                    }
                }
            }
            Logger.DEBUG(kTag, "Control adapter has been closed");

            if(!ExpConfig.EXP_NO_CONTROL_ADAPTER_AFTER_SWITCHING) {
                NetworkSwitcher.getInstance().reconnectControlAdapter();
            }
        }
    }

    // Control Request Code
    class CtrlReq {
        public static final char kConnect = 1;
        public static final char kSleep = 2;
        public static final char kWakeup = 3;
        public static final char kPriv = 10;
    }

    // State
    class State {
        public static final int kIdle = 0;
        public static final int kStarting = 1;
        public static final int kReady = 2;
        public static final int kConnecting = 3; // Deprecated: this state is managed by network
        // switcher.
        public static final int kDisconnecting = 4; // Deprecated: this state is managed by
        // network switcher.
        public static final int kStopping = 5;
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

    public void addControlMessageListener(ControlMessageListener listener) {
        this.mControlMessageListeners.add(listener);
    }

    // Adapters Getter
    public ClientAdapter findDataAdapterById(int adapterId) {
        for (ClientAdapter adapter : this.mDataAdapters) {
            if (adapter.getId() == adapterId) {
                return adapter;
            }
        }
        return null;
    }

    public ClientAdapter getControlAdapter() {
        return this.mControlAdapter;
    }

    // Attributes
    private final ArrayList<ClientAdapter> mDataAdapters;
    private ClientAdapter mControlAdapter;
    private ArrayList<ControlMessageListener> mControlMessageListeners;

    // State
    private Integer mState;

    // Singleton
    private static Core sSingleton;

    public static Core getInstance() {
        if (sSingleton == null) {
            sSingleton = new Core();
        }
        return sSingleton;
    }

    // Constructor
    private Core() {
        this.mState = State.kIdle;
        this.mDataAdapters = new ArrayList<>();
        this.mControlAdapter = null;
        this.mControlMessageListeners = new ArrayList<>();
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
            this.onConnectControlAdapter = new OnConnectControlAdapter();
            this.onConnectFirstDataAdapter = new OnConnectFirstDataAdapter();
        }

        @SuppressWarnings("SynchronizeOnNonFinalField")
        private void start() {
            synchronized (this.mCaller.mControlAdapter) {
                ClientAdapter controlAdapter = this.mCaller.mControlAdapter;
                controlAdapter.connect(this.onConnectControlAdapter, false);
            }
        }

        private OnConnectControlAdapter onConnectControlAdapter;

        class OnConnectControlAdapter implements ClientAdapter.ConnectResultListener {
            @SuppressWarnings("SimplifiableIfStatement")
            @Override
            public void onConnectResult(boolean isSuccess) {
                if (!isSuccess) {
                    Logger.ERR(kTag, "Connecting control adapter is failed");
                    doneStartCoreTx(mCaller, false, mResultListener);
                    return;
                }

                // Connect first data adapter
                synchronized (mCaller.mDataAdapters) {
                    if (mCaller.mDataAdapters.isEmpty()) {
                        Logger.ERR(kTag, "No data adapter: failed to connect first data adapter");
                        doneStartCoreTx(mCaller, false, mResultListener);
                        return;
                    }

                    ClientAdapter firstDataAdapter = mCaller.mDataAdapters.get(0);
                    firstDataAdapter.connect(onConnectFirstDataAdapter, false);
                }
            }
        }

        private OnConnectFirstDataAdapter onConnectFirstDataAdapter;

        class OnConnectFirstDataAdapter implements ClientAdapter.ConnectResultListener {
            @Override
            public void onConnectResult(boolean isSuccess) {
                if (isSuccess) {
                    // Done transaction
                    doneStartCoreTx(mCaller, true, mResultListener);
                } else {
                    Logger.ERR(kTag, "Connecting first data adapter is failed");
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
            this.onDisconnectControlAdapter = new OnDisconnectControlAdapter();
            this.onDisconnectDataAdapter = new OnDisconnectDataAdapter();
            this.mDataAdaptersCount = 0;
        }

        @SuppressWarnings("SynchronizeOnNonFinalField")
        void start() {
            synchronized (this.mCaller.mDataAdapters) {
                this.mDataAdaptersCount = this.mCaller.mDataAdapters.size();
            }

            // Disconnect control adapter
            synchronized (this.mCaller.mControlAdapter) {
                this.mCaller.mControlAdapter.disconnect(onDisconnectControlAdapter);
            }
        }

        private OnDisconnectControlAdapter onDisconnectControlAdapter;

        class OnDisconnectControlAdapter implements ClientAdapter.DisconnectResultListener {
            @Override
            public void onDisconnectResult(boolean isSuccess) {
                if (!isSuccess) {
                    Logger.ERR(kTag, "Disconnecting control adapter is failed");
                    doneStopCoreTx(mCaller, false, mResultListener);
                    return;
                }

                synchronized (mCaller.mDataAdapters) {
                    for (ClientAdapter dataAdapter : mCaller.mDataAdapters) {
                        dataAdapter.disconnect(onDisconnectDataAdapter);
                    }
                }
            }
        }

        private OnDisconnectDataAdapter onDisconnectDataAdapter;

        class OnDisconnectDataAdapter implements ClientAdapter.DisconnectResultListener {
            @SuppressWarnings("SynchronizeOnNonFinalField")
            @Override
            public void onDisconnectResult(boolean isSuccess) {
                if (!isSuccess) {
                    Logger.ERR(kTag, "Connecting data adapter is failed");
                    doneStopCoreTx(mCaller, false, mResultListener);
                    return;
                }

                boolean doneDisconnectAll = false;
                synchronized (mDataAdaptersCount) {
                    mDataAdaptersCount--;
                    if (mDataAdaptersCount == 0) {
                        doneDisconnectAll = true;
                    }
                }

                if (doneDisconnectAll) {
                    doneStopCoreTx(mCaller, true, mResultListener);
                } else {
                    doneStopCoreTx(mCaller, false, mResultListener);
                }
            }
        }

        // Attributes
        private Core mCaller;
        private Integer mDataAdaptersCount;
        private OnStopSCResult mResultListener;
    }
}