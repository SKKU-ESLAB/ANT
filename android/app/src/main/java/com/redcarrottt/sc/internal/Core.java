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
        } else if (this.mControlAdapters.isEmpty()) {
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
        } else if (this.mControlAdapters.isEmpty()) {
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

        synchronized (this.mControlAdapters) {
            controlAdapter.enableReceiverThread(new ReceiveControlMessageLoop());

            this.mControlAdapters.add(controlAdapter);
        }
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

        ClientAdapter controlAdapter = this.getActiveControlAdapter();
        if (controlAdapter == null) {
            Logger.ERR(kTag, "Failed to send control message: no active control adapter");
            return;
        }

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
            buffer.put((byte) requestCode);
            buffer.putShort(adapterId);
            this.sendControlMessage(buffer.array(), 3);
        }
    }

    public void sendRequestConnect(short adapterId) {
        this.sendRequest(CMCode.kConnect, adapterId);
        Logger.VERB(kTag, "Send(Control Msg): Request(Connect " + adapterId + ")");
    }

    public void sendRequestDisconnect(short adapterId) {
        this.sendRequest(CMCode.kDisconnect, adapterId);
        Logger.VERB(kTag, "Send(Control Msg): Request(Disconnect " + adapterId + ")");
    }

    public void sendRequestDisconnectAck(short adapterId) {
        this.sendRequest(CMCode.kDisconnectAck, adapterId);
        Logger.VERB(kTag, "Send(Control Msg): Request(DisconnectAck " + adapterId + ")");
    }

    public void sendRequestSleep(short adapterId) {
        this.sendRequest(CMCode.kSleep, adapterId);
        Logger.VERB(kTag, "Send(Control Msg): Request(Sleep " + adapterId + ")");
    }

    public void sendRequestWakeup(short adapterId) {
        this.sendRequest(CMCode.kWakeup, adapterId);
        Logger.VERB(kTag, "Send(Control Msg): Request(WakeUp " + adapterId + ")");
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
                Logger.DEBUG(kTag, "Control Receiver: receiving request code...");
                res = adapter.receive(dataBuffer, 1);
                Logger.DEBUG(kTag, "Control Receiver: RECEIVED request code!");
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
                if (reqCode == CMCode.kConnect || reqCode == CMCode.kSleep || reqCode ==
                        CMCode.kWakeup || reqCode == CMCode.kDisconnect || reqCode == CMCode
                        .kDisconnectAck) {
                    // "Connect Adapter" Request
                    // Receive 2Byte: Adapter ID
                    res = adapter.receive(dataBuffer, 2);
                    if (res <= 0) break;
                    ByteBuffer buffer = ByteBuffer.allocate(2);
                    buffer.put(dataBuffer, 0, 2);
                    short adapterId = buffer.getShort(0);

                    if (reqCode == CMCode.kConnect) {
                        Logger.VERB(kTag, "Receive(Control Msg): Request(Connect " + adapterId +
                                ")");
                        NetworkSwitcher.getInstance().connectAdapterByPeer(adapterId);
                    } else if (reqCode == CMCode.kSleep) {
                        Logger.VERB(kTag, "Receive(Control Msg): Request(Sleep " + adapterId + ")");
                        NetworkSwitcher.getInstance().sleepAdapterByPeer(adapterId);
                    } else if (reqCode == CMCode.kWakeup) {
                        Logger.VERB(kTag, "Receive(Control Msg): Request(WakeUp " + adapterId +
                                ")");
                        NetworkSwitcher.getInstance().wakeUpAdapterByPeer(adapterId);
                    } else if (reqCode == CMCode.kDisconnect) {
                        Logger.VERB(kTag, "Receive(Control Msg): Request(Disconnect " + adapterId
                                + ")");
                        NetworkSwitcher.getInstance().disconnectAdapterByPeer(adapterId);
                    } else if (reqCode == CMCode.kDisconnectAck) {
                        Logger.VERB(kTag, "Receive(Control Msg): Request(DisconnectAck " +
                                adapterId + ")");
                        ClientAdapter disconnect_adapter = findAdapterById(adapterId);
                        if (disconnect_adapter == null) {
                            Logger.WARN(kTag, "Cannot find adapter " + adapterId);
                        } else {
                            disconnect_adapter.peerKnowsDisconnectingOnPurpose();
                        }
                    }
                } else if (reqCode == CMCode.kPriv) {
                    // "Priv" Request
                    int privType;
                    int privDataLength;
                    // Receive 4Byte: Priv Type
                    res = adapter.receive(dataBuffer, 4);
                    if (res <= 0) {
                        break;
                    } else {
                        Logger.VERB(kTag, "Receive(Control Msg): Request(Priv Noti--Start)");
                        ByteBuffer tempBuffer = ByteBuffer.allocate(4);
                        //Logger.DEBUG(kTag, "tempBuffer remaining: " + tempBuffer.remaining());
                        tempBuffer.put(dataBuffer, 0, 4);
                        privType = tempBuffer.getInt(0);
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
                        Logger.VERB(kTag, "Receive(Control Msg): Request(Priv Noti '" +
                                dataBuffer + "'; type=" + privType + ")");
                        for (ControlMessageListener listener : mControlMessageListeners) {
                            listener.onReceiveControlMessage(privType, dataBuffer, privDataLength);
                        }
                    } else {
                        break;
                    }
                }
            }
            Logger.DEBUG(kTag, "Control adapter has been closed");

            NetworkSwitcher.getInstance().reconnectAdapter(adapter);
        }
    }

    // Control Request Code
    class CMCode {
        public static final char kConnect = 1;
        public static final char kSleep = 2;
        public static final char kWakeup = 3;
        public static final char kDisconnect = 4;
        public static final char kPriv = 10;
        public static final char kDisconnectAck = 24;
    }

    // Priv Type Code
    public class PrivType {
        public static final char kWFDInfo = 1;
        public static final char kWFDUnknown = 999;
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

    public ClientAdapter findControlAdapterById(int adapterId) {
        for (ClientAdapter adapter : this.mControlAdapters) {
            if (adapter.getId() == adapterId) {
                return adapter;
            }
        }
        return null;
    }

    public ClientAdapter findAdapterById(int adapterId) {
        ClientAdapter adapter = findDataAdapterById(adapterId);
        if (adapter == null) {
            adapter = findControlAdapterById(adapterId);
        }
        return adapter;
    }

    public ClientAdapter getActiveControlAdapter() {
        synchronized (this.mControlAdapters) {
            for (ClientAdapter controlAdapter : this.mControlAdapters) {
                if (controlAdapter.getState() == ClientAdapter.State.kActive) {
                    return controlAdapter;
                }
            }
        }
        return null;
    }

    // Attributes
    private final ArrayList<ClientAdapter> mDataAdapters;
    private final ArrayList<ClientAdapter> mControlAdapters;
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
        this.mControlAdapters = new ArrayList<>();
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
            // Connect first control adapter
            ClientAdapter controlAdapter = this.mCaller.mControlAdapters.get(0);
            if (controlAdapter == null) {
                Logger.ERR(kTag, "Cannot find first control adapter");
                doneStartCoreTx(mCaller, false, mResultListener);
                return;
            }
            controlAdapter.connect(this.onConnectControlAdapter, false);
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
            this.mControlAdaptersCount = 0;
            this.mDataAdaptersCount = 0;
        }

        @SuppressWarnings("SynchronizeOnNonFinalField")
        void start() {
            synchronized (this.mCaller.mDataAdapters) {
                this.mDataAdaptersCount = this.mCaller.mDataAdapters.size();
            }

            // Get active control adapter count
            synchronized (this.mCaller.mControlAdapters) {
                for (ClientAdapter controlAdapter : this.mCaller.mControlAdapters) {
                    int adapterState = controlAdapter.getState();
                    if (adapterState != ClientAdapter.State.kDisconnected && adapterState !=
                            ClientAdapter.State.kDisconnecting) {
                        this.mControlAdaptersCount++;
                    }
                }
            }

            // Disconnect only active control adapters
            synchronized (this.mCaller.mControlAdapters) {
                for (ClientAdapter controlAdapter : this.mCaller.mControlAdapters) {
                    int adapterState = controlAdapter.getState();
                    if (adapterState != ClientAdapter.State.kDisconnected && adapterState !=
                            ClientAdapter.State.kDisconnecting) {
                        controlAdapter.disconnect(onDisconnectControlAdapter, true, false, true);
                    }
                }
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

                // Check if all the active control adapters are disconnected
                boolean doneDisconnectAll = false;
                synchronized (mControlAdaptersCount) {
                    mControlAdaptersCount--;
                    if (mControlAdaptersCount == 0) {
                        doneDisconnectAll = true;
                    }
                }

                if (doneDisconnectAll) {
                    // Get active data adapter count
                    synchronized (mCaller.mDataAdapters) {
                        for (ClientAdapter dataAdapter : mCaller.mDataAdapters) {
                            int adapterState = dataAdapter.getState();
                            if (adapterState != ClientAdapter.State.kDisconnected && adapterState
                                    != ClientAdapter.State.kDisconnecting) {
                                mDataAdaptersCount++;
                            }
                        }
                    }

                    // Disconnect only active control adapters
                    synchronized (mCaller.mDataAdapters) {
                        for (ClientAdapter dataAdapter : mCaller.mDataAdapters) {
                            dataAdapter.disconnect(onDisconnectDataAdapter, true, false, true);
                        }
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

                // Check if all the data control adapters are disconnected
                boolean doneDisconnectAll = false;
                synchronized (mDataAdaptersCount) {
                    mDataAdaptersCount--;
                    if (mDataAdaptersCount == 0) {
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
        private Integer mDataAdaptersCount;
        private Integer mControlAdaptersCount;
        private OnStopSCResult mResultListener;
    }
}