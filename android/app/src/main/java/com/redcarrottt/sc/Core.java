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

import java.nio.ByteBuffer;
import java.util.ArrayList;

public class Core {
    private static final String kTag = "Core";

    // APIs: These functions are mapped to ones in API.
    public boolean start() {
        if (this.getState() == State.kIdle) {
            Logger.ERR(kTag, "Core has already started");
            return false;
        } else if (this.mControlAdapter == null) {
            Logger.ERR(kTag, "No control adapter is registered!");
            return false;
        } else if (this.mDataAdapters.isEmpty()) {
            Logger.ERR(kTag, "No data adapter is registered!");
            return false;
        }
        this.setState(State.kStarting);
        runStartCoreTx(this);

        return false;
    }

    public void doneStart(boolean isSuccess) {
        if (!isSuccess) {
            Logger.ERR(kTag, "Failed to start core!");
            this.setState(State.kIdle);
            return;
        }

        Logger.VERB(kTag, "Succeed to start core!");
        this.setState(State.kReady);
    }

    public boolean stop() {
        int state = this.getState();
        if (state == State.kStarting || state == State.kStopping) {
            Logger.ERR(kTag, "Cannot stop core during starting/stopping!");
            return false;
        } else if (state == State.kIdle) {
            Logger.ERR(kTag, "Core is already idle state!");
            return false;
        } else if (this.mControlAdapter == null) {
            Logger.ERR(kTag, "No control adapter is registered!");
            return false;
        } else if (this.mDataAdapters.isEmpty()) {
            Logger.ERR(kTag, "No data adapter is registered!");
            return false;
        }

        this.setState(State.kStopping);
        runStopCoreTx(this);
        return false;
    }

    public void doneStop(boolean isSuccess) {
        if (!isSuccess) {
            Logger.ERR(kTag, "Failed to stop core!");
            this.setState(State.kReady);
        }

        Logger.VERB(kTag, "Succeed to stop core!");
        this.setState(State.kIdle);
    }

    public void registerControlAdapter(ClientAdapter controlAdapter) {
        if (this.getState() != State.kIdle) {
            Logger.ERR(kTag, "You can register control adapter on only idle state!");
            return;
        }

        synchronized (this.mControlAdapter) {
            if (this.mControlAdapter != null) {
                Logger.ERR(kTag, "Another control adapter has already been registered!");
                return;
            }
            controlAdapter.enableReceiverThread(new ReceiveControlMessageLoop());

            this.mControlAdapter = controlAdapter;
        }
    }

    public void registerDataAdapter(ClientAdapter dataAdapter) {
        if (this.getState() != State.kIdle) {
            Logger.ERR(kTag, "You can register data adapter on only idle state!");
            return;
        }
        synchronized (this.mDataAdapters) {
            dataAdapter.enableReceiverThread(null);
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

        int packetSize = ProtocolManager.serialize(protocolData, dataBuffer, currOffset, dataLength);
        if (!(packetSize > 0)) {
            Logger.ERR(kTag, "Failed to make protocol data to send(" + currOffset + "/" + dataLength + ")");
            throw new AssertionError();
        }

        int sentBytes = ProtocolManager.send_packet(packetSize);
        if (sentBytes < 0) {
            Logger.ERR(kTag, "Sending stopped (" + currOffset + "/" + dataLength + ") by " + sentBytes);
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

        int packetSize = ProtocolManager.recv_packet(dataBuffer);
        return packetSize;
    }

    // Control Message Handling
    protected void sendControlMessage(byte[] dataBuffer, int dataLength) {
        int state = this.getState();
        if (state != State.kReady) {
            Logger.ERR(kTag, "Core is not started yet, so you cannot send the data");
            return;
        }
        ClientAdapter controlAdapter = this.mControlAdapter;
        controlAdapter.send(dataBuffer, dataLength);
    }

    public void sendRequestConnect(short adapterId) {
        int state = this.getState();
        if (state != State.kReady) {
            Logger.ERR(kTag, "Core is not started yet, so you cannot send the data");
            return;
        }

        char requestCode = CtrlReq.kConnect;

        {
            ByteBuffer buffer = ByteBuffer.allocate(1);
            buffer.putChar(requestCode);
            this.sendControlMessage(buffer.array(), 1);
        }
        {
            ByteBuffer buffer = ByteBuffer.allocate(2);
            buffer.putShort(adapterId);
            this.sendControlMessage(buffer.array(), 1);
        }
    }

    public void sendNotiPrivateData(short adapterId, byte[] privDataBuffer) {
        // Not implemented yet
    }

    class ReceiveControlMessageLoop implements ClientAdapter.ReceiveLoop {
        @Override
        public void receiveLoop(ClientAdapter adapter) {
            byte[] dataBuffer = new byte[512];
            int res = 0;
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
                    continue;
                }

                Byte reqCode = dataBuffer[0];
                if (reqCode == CtrlReq.kConnect) {
                    // "Connect Adapter" Request
                    // Receive 2Byte: Adapter ID
                    res = adapter.receive(dataBuffer, 2);
                    if (res <= 0)
                        break;
                    ByteBuffer buffer = ByteBuffer.allocate(2);
                    buffer.put(dataBuffer, 0, 2);
                    short adapterId = buffer.getShort();

                    Logger.DEBUG(kTag, "Control Request: 'Connect Adapter Request' (" + adapterId + ")");
                    NetworkSwitcher.getInstance().connectAdapter(adapterId);
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
                        tempBuffer.put(dataBuffer, 0, 2);
                        adapterId = tempBuffer.getShort();
                    }

                    // Receive 4Byte: Private Data Length
                    res = adapter.receive(dataBuffer, 4);
                    if (res <= 0) {
                        break;
                    } else {
                        ByteBuffer tempBuffer2 = ByteBuffer.allocate(4);
                        tempBuffer2.put(dataBuffer, 0, 4);
                        privDataLength = tempBuffer2.getInt();
                    }
                    if (privDataLength > 512)
                        throw new AssertionError();

                    // Receive nByte: Private Data
                    res = adapter.receive(dataBuffer, privDataLength);
                    if (res > 0) {
                        for (ControlMessageListener listener : mControlMessageListeners) {
                            listener.onReceiveControlMessage(adapterId, dataBuffer, privDataLength);
                        }
                    } else {
                        break;
                    }
                }
            }
            Logger.DEBUG(kTag, "Control adapter has been closed");

            NetworkSwitcher.getInstance().reconnectControlAdapter();
        }
    }

    // Control Request Code
    class CtrlReq {
        public static final char kConnect = 1;
        public static final char kPriv = 2;
    }

    // State
    class State {
        public static final int kIdle = 0;
        public static final int kStarting = 1;
        public static final int kReady = 2;
        public static final int kConnecting = 3;
        public static final int kDisconnecting = 4;
        public static final int kStopping = 5;
    }

    private int getState() {
        int state;
        synchronized (this.mState) {
            state = this.mState;
        }
        return state;
    }

    private void setState(int newState) {
        synchronized (this.mState) {
            this.mState = newState;
        }
    }

    // Control Message Listener
    interface ControlMessageListener {
        void onReceiveControlMessage(int adapterId, byte[] dataBuffer, int dataLength);
    }

    public void addControlMessageListener(ControlMessageListener listener) {
        this.mControlMessageListeners.add(listener);
    }

    // Adapters Getter
    public ClientAdapter findDataAdapterById(int adapterId) {
        for(ClientAdapter adapter : this.mDataAdapters) {
            if(adapter.getId() == adapterId) {
                return adapter;
            }
        }
        return null;
    }

    public ClientAdapter getControlAdapter() {
        return this.mControlAdapter;
    }

    // Attributes
    ArrayList<ClientAdapter> mDataAdapters;
    ClientAdapter mControlAdapter;
    ArrayList<ControlMessageListener> mControlMessageListeners;

    // State
    protected Integer mState;

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
    private static boolean runStartCoreTx(Core caller) {
        if (sOngoingStartCore == null) {
            sOngoingStartCore = new StartCoreTransaction(caller);
            sOngoingStartCore.start();
            return true;
        } else {
            Logger.WARN(kTag, "Already starting core");
            return false;
        }
    }

    private static void doneStartCoreTx(Core caller, boolean isSuccess) {
        caller.doneStart(isSuccess);
        sOngoingStartCore = null;
    }

    private static boolean runStopCoreTx(Core caller) {
        if (sOngoingStopCore == null) {
            sOngoingStopCore = new StopCoreTransaction(caller);
            return true;
        } else {
            Logger.WARN(kTag, "Already stopping core");
            return false;
        }
    }

    private static void doneStopCoreTx(Core caller, boolean isSuccess) {
        caller.doneStop(isSuccess);
        sOngoingStopCore = null;
    }

    private static StartCoreTransaction sOngoingStartCore = null;
    private static StopCoreTransaction sOngoingStopCore = null;

    // Start Core
    private static class StartCoreTransaction {
        // Private Constructor
        private StartCoreTransaction(Core caller) {
            this.mCaller = caller;
            this.onConnectControlAdapter = new OnConnectControlAdapter();
            this.onConnectFirstDataAdapter = new OnConnectFirstDataAdapter();
        }

        private boolean start() {
            boolean res;

            synchronized (this.mCaller.mControlAdapter) {
                res = this.mCaller.mControlAdapter.connect(this.onConnectControlAdapter, false);
            }

            if (!res) {
                Logger.ERR(kTag, "Connecting control adapter is failed");
                doneStartCoreTx(this.mCaller, false);
            }
            return res;
        }

        private OnConnectControlAdapter onConnectControlAdapter;

        class OnConnectControlAdapter implements ClientAdapter.ConnectResultListener {
            @Override
        public void onConnectResult(boolean isSuccess) {
                if (!isSuccess) {
                    Logger.ERR(kTag, "Connecting control adapter is failed");
                    doneStartCoreTx(mCaller, false);
                    return;
                }

                boolean res = true;

                // Connect first data adapter
                synchronized (mCaller.mDataAdapters) {
                    if (mCaller.mDataAdapters.isEmpty()) {
                        res = false;
                    } else {
                        res = mCaller.mDataAdapters.get(0).connect(onConnectFirstDataAdapter, true);
                    }
                }

                if (!res) {
                    Logger.ERR(kTag, "Connecting first data adapter is failed");
                    doneStartCoreTx(mCaller, false);
                }
            }
        }

        private OnConnectFirstDataAdapter onConnectFirstDataAdapter;

        class OnConnectFirstDataAdapter implements ClientAdapter.ConnectResultListener {
            @Override
            public void onConnectResult(boolean isSuccess) {
                if (isSuccess) {
                    // Done transaction
                    doneStartCoreTx(mCaller, true);
                } else {
                    Logger.ERR(kTag, "Connecting first data adapter is failed");
                    doneStartCoreTx(mCaller, false);
                }
            }
        }

        // Attributes
        private Core mCaller;
    }

    // Stop Core
    private static class StopCoreTransaction {
        private StopCoreTransaction(Core caller) {
            this.mCaller = caller;
            this.onDisconnectControlAdapter = new OnDisconnectControlAdapter();
            this.onDisconnectDataAdapter = new OnDisconnectDataAdapter();
        }

        public boolean start() {
            synchronized (this.mCaller.mDataAdapters) {
                this.mDataAdaptersCount = this.mCaller.mDataAdapters.size();
            }

            boolean res;

            // Disconnect control adapter
            synchronized (this.mCaller.mControlAdapter) {
                res = this.mCaller.mControlAdapter.disconnect(onDisconnectControlAdapter);
            }

            if (!res) {
                Logger.ERR(kTag, "Connecting control adapter is failed");
                doneStopCoreTx(this.mCaller, false);
            }
            return res;
        }

        private OnDisconnectControlAdapter onDisconnectControlAdapter;

        class OnDisconnectControlAdapter implements ClientAdapter.DisconnectResultListener {
            @Override
            public void onDisconnectResult(boolean isSuccess) {
                if (!isSuccess) {
                    Logger.ERR(kTag, "Connecting control adapter is failed");
                    doneStopCoreTx(mCaller, false);
                    return;
                }

                synchronized (mCaller.mDataAdapters) {
                    for (ClientAdapter dataAdapter : mCaller.mDataAdapters) {
                        boolean res = dataAdapter.disconnect(onDisconnectDataAdapter);
                        if (!res) {
                            Logger.ERR(kTag, "Disconnecting data adapter is failed");
                        }
                    }
                }
            }
        }

        private OnDisconnectDataAdapter onDisconnectDataAdapter;

        class OnDisconnectDataAdapter implements ClientAdapter.DisconnectResultListener {
            @Override
            public void onDisconnectResult(boolean isSuccess) {
                if (!isSuccess) {
                    Logger.ERR(kTag, "Connecting data adapter is failed");
                    doneStopCoreTx(mCaller, false);
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
                    doneStopCoreTx(mCaller, true);
                } else {
                    doneStopCoreTx(mCaller, false);
                }
            }
        }

        // Attributes
        private Core mCaller;
        private Integer mDataAdaptersCount;
    }
}