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

import java.util.ArrayList;

public class ClientAdapter {
    private final String kTag = "ClientAdapter";
    private final ClientAdapter self = this;

    // Main Functions: connect, disconnect, send, receive
    public boolean connect(ConnectResultListener listener, boolean isSendConnectMessage) {
        if (this.getState() != State.kDisconnected) {
            Logger.ERR(kTag, "It's already connected or connection/disconnection is in progress");
            return false;
        }

        if (isSendConnectMessage) {
            Core.getInstance().sendRequestConnect((short) this.getId());
        }

        ConnectThread thread = new ConnectThread(listener);
        thread.start();
        return true;
    }

    public boolean disconnect(DisconnectResultListener listener) {
        if (this.getState() != State.kConnected) {
            Logger.ERR(kTag, "It's already disconnected or connection/disconnection is in " +
                    "progress");
            return false;
        }

        DisconnectThread thread = new DisconnectThread(listener);
        thread.start();
        return true;
    }

    int send(byte[] dataBuffer, int dataLength) {
        if (this.getState() != State.kConnected) {
            Logger.ERR(kTag, "It's already disconnected or connection/disconnection is in " +
                    "progress");
            return -1;
        }

        if (this.mClientSocket == null) {
            return -2;
        }

        // Omit Implementing Statistics: SendDataSize
        return this.mClientSocket.send(dataBuffer, dataLength);
    }

    int receive(byte[] dataBuffer, int dataLength) {
        if (this.getState() != State.kConnected) {
            Logger.ERR(kTag, "It's already disconnected or connection/disconnection is in " +
                    "progress");
            return -1;
        }

        if (this.mClientSocket == null) {
            return -2;
        }

        // Omit Implementing Statistics: ReceiveDataSize
        return this.mClientSocket.receive(dataBuffer, dataLength);
    }

    // Connect/Disconnect Threads & Callbacks
    class ConnectThread extends Thread implements DiscoverAndConnectResultListener {
        @Override
        public void run() {
            Logger.VERB(kTag, self.getName() + "'s Connect Thread Spawned! (id:" + this.getId() +
                    ")");
            setState(ClientAdapter.State.kConnecting);

            // Turn on device
            if (self.mDevice == null) {
                this.onFail();
                return;
            }
            int deviceState = self.mDevice.getState();
            if (deviceState != Device.State.kOn) {
                boolean res = self.mDevice.holdAndTurnOn();

                deviceState = self.mDevice.getState();
                if (!res || deviceState != Device.State.kOn) {
                    Logger.ERR(kTag, "Cannot connect the server adapter - turn-on fail: " + self
                            .getName());
                }
                this.onFail();
                return;
            }

            // Discover and connect to server
            if (self.mP2PClient == null) {
                self.mDevice.releaseAndTurnOff();

                this.onFail();
                return;
            }
            int p2pClientState = self.mP2PClient.getState();
            if (p2pClientState != P2PClient.State.kDisconnected) {
                self.mP2PClient.discoverAndConnect(this);
            }
        }

        @Override
        public void onDiscoverAndConnectResult(boolean isSuccess) {
            // Check the result of "Discover and connect"
            int p2pClientState = self.mP2PClient.getState();
            if (!isSuccess || p2pClientState != P2PClient.State.kConnected) {
                Logger.ERR(kTag, "Cannot connect the server adapter - allow fail:" + self.getName
                        ());
                self.mDevice.releaseAndTurnOff();
                this.onFail();
                return;
            }

            // Open client socket
            if (self.mClientSocket == null) {
                self.mP2PClient.disconnect(null);
                self.mDevice.releaseAndTurnOff();

                this.onFail();
                return;
            }
            int socketState = self.mClientSocket.getState();
            if (socketState != ClientSocket.State.kOpened) {
                boolean res = self.mClientSocket.open();

                socketState = self.mClientSocket.getState();
                if (!res || socketState != ClientSocket.State.kOpened) {
                    Logger.ERR(kTag, "Cannot connect the server adapter - socket open fail: " +
                            self.getName());
                    self.mP2PClient.disconnect(null);
                    self.mDevice.releaseAndTurnOff();
                    this.onFail();
                    return;
                }
            }

            // Run sender & receiver threads
            if (self.mSenderThread != null && !self.mSenderThread.isOn()) {
                self.mSenderThread.start();
            }

            if (self.mReceiverThread != null && !self.mReceiverThread.isOn()) {
                self.mReceiverThread.start();
            }

            // Report result success
            self.setState(ClientAdapter.State.kConnected);
            if (this.mResultListener != null) {
                this.mResultListener.onConnectResult(true);
                this.mResultListener = null;
            }
        }

        private void onFail() {
            self.setState(ClientAdapter.State.kDisconnected);

            // Report result fail
            if (this.mResultListener != null) {
                this.mResultListener.onConnectResult(false);
                this.mResultListener = null;
            }
        }

        private ConnectResultListener mResultListener;

        ConnectThread(ConnectResultListener resultListener) {
            this.mResultListener = resultListener;
        }
    }

    class DisconnectThread extends Thread implements com.redcarrottt.sc.DisconnectResultListener {
        @Override
        public void run() {
            Logger.VERB(kTag, self.getName() + "'s Disconnect Thread Spawned! (id:"
                    + this.getId() + ")");
            setState(ClientAdapter.State.kDisconnecting);

            // Finish sender & receiver threads
            if (self.mSenderThread != null) {
                self.mSenderThread.finish();
            }
            if (self.mReceiverThread != null) {
                self.mReceiverThread.finish();
            }

            // Close client socket
            if (self.mClientSocket == null) {
                this.onFail();
                return;
            }
            int socketState = self.mClientSocket.getState();
            if (socketState != ClientSocket.State.kClosed) {
                boolean res = self.mClientSocket.close();

                socketState = self.mClientSocket.getState();
                if (!res || socketState != ClientSocket.State.kClosed) {
                    Logger.ERR(kTag, "Cannot disconnect the server adapter - socket close fail: "
                            + "" + "" + self.getName());
                    this.onFail();
                    return;
                }
            }

            // P2P Disconnect
            int p2pClientState = self.mClientSocket.getState();
            if (p2pClientState != P2PClient.State.kDisconnected) {
                self.mP2PClient.disconnect(this);
            }
        }

        @Override
        public void onDisconnectResult(boolean isSuccess) {
            // Check the result of "P2P Disconnect"
            int p2pClientState = self.mP2PClient.getState();
            if (!isSuccess || p2pClientState != P2PClient.State.kDisconnected) {
                Logger.ERR(kTag, "Cannot disconnect the server adapter - disconnect P2P " +
                        "client fail: " + self.getName());
                this.onFail();
                return;
            }


            // Turn off device
            int deviceState = self.mDevice.getState();
            if (deviceState != Device.State.kOff) {
                boolean res = self.mDevice.releaseAndTurnOff();

                deviceState = self.mDevice.getState();
                if (!res || deviceState != Device.State.kOff) {
                    Logger.ERR(kTag, "Cannot disconnect the server adapter - turn-off fail: " +
                            self.getName());
                    this.onFail();
                    return;
                }
            }

            // Report result success
            self.setState(ClientAdapter.State.kDisconnected);
            if (this.mResultListener != null) {
                this.mResultListener.onDisconnectResult(true);
                this.mResultListener = null;
            }
        }

        private void onFail() {
            self.setState(ClientAdapter.State.kConnected);

            // Report result fail
            if (this.mResultListener != null) {
                this.mResultListener.onDisconnectResult(false);
                this.mResultListener = null;
            }
        }

        private DisconnectResultListener mResultListener;

        DisconnectThread(DisconnectResultListener resultListener) {
            this.mResultListener = resultListener;
        }
    }

    interface ConnectResultListener {
        void onConnectResult(boolean isSuccess);
    }

    interface DisconnectResultListener {
        void onDisconnectResult(boolean isSuccess);
    }

    // Thread Control Functions: enableSenderThread, enableReceiverThread
    void enableSenderThread() {
        this.mSenderThread = new SenderThread();
    }

    void enableReceiverThread(ReceiveLoop receiveLoop) {
        if (receiveLoop == null) {
            this.mReceiveLoop = new ReceiveDataLoop();
        } else {
            this.mReceiveLoop = receiveLoop;
        }

        this.mReceiverThread = new ReceiverThread();
    }

    // Receiver Loop Interface
    interface ReceiveLoop {
        void receiveLoop(ClientAdapter adapter);
    }

    // Sender/Receiver Threads
    @SuppressWarnings("SynchronizeOnNonFinalField")
    class SenderThread extends Thread {
        @Override
        public void run() {
            synchronized (this.mIsOn) {
                this.mIsOn = true;
            }
        }

        public void finish() {
            synchronized (this.mIsOn) {
                this.mIsOn = false;
            }
        }

        SenderThread() {
            this.mIsOn = false;
        }

        public boolean isOn() {
            return this.mIsOn;
        }

        private Boolean mIsOn;
    }

    @SuppressWarnings("SynchronizeOnNonFinalField")
    class ReceiverThread extends Thread {
        @Override
        public void run() {
            synchronized (this.mIsOn) {
                this.mIsOn = true;
            }
        }

        public void finish() {
            synchronized (this.mIsOn) {
                this.mIsOn = false;
            }
        }

        ReceiverThread() {
            this.mIsOn = false;
        }

        public boolean isOn() {
            return this.mIsOn;
        }

        private Boolean mIsOn;
    }

    class ReceiveDataLoop implements ReceiveLoop {
        @Override
        public void receiveLoop(ClientAdapter adapter) {
            // TODO: implement it (same as ServerAdapter::receive_data_loop())

        }
    }

    private SenderThread mSenderThread;
    private ReceiverThread mReceiverThread;
    private ReceiveLoop mReceiveLoop;

    // Initialize
    public ClientAdapter(int id, String name) {
        this.mId = id;
        this.mName = name;
        this.mState = State.kDisconnected;
        this.mListeners = new ArrayList<>();
    }

    protected void initialize(Device device, P2PClient p2pClient, ClientSocket clientSocket) {
        this.mDevice = device;
        this.mP2PClient = p2pClient;
        this.mClientSocket = clientSocket;
    }

    // Attribute Getters
    public int getId() {
        return this.mId;
    }

    public String getName() {
        return this.mName;
    }

    // State
    class State {
        public static final int kDisconnected = 0;
        public static final int kConnecting = 1;
        public static final int kConnected = 2;
        public static final int kDisconnecting = 3;
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
        int oldState;
        synchronized (this.mState) {
            oldState = this.mState;
            this.mState = newState;
        }

        for (StateListener listener : this.mListeners) {
            listener.onUpdateClientAdapterState(this, oldState, newState);
        }
    }

    // State Listener
    interface StateListener {
        void onUpdateClientAdapterState(ClientAdapter adapter, int oldState, int newState);
    }

    public void listenState(StateListener listener) {
        synchronized (this.mListeners) {
            this.mListeners.add(listener);
        }
    }

    // Attributes
    private int mId;
    private String mName;

    // State
    private Integer mState;

    // State Listener
    private final ArrayList<StateListener> mListeners;

    // Main Components : Device, P2PClient, ClientSocket
    private Device mDevice;
    private P2PClient mP2PClient;

    private ClientSocket mClientSocket;
}