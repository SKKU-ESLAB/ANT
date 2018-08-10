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
    private final String TAG = "ClientAdapter";
    private final ClientAdapter self = this;

    // Main Functions: connect, disconnect, send, receive
    public boolean connect(ConnectResultListener listener, boolean isSendConnectMessage) {
        if (this.getState() != State.kDisconnected) {
            Logger.print(TAG, "It's already connected or connection/disconnection is in progress");
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
            Logger.print(TAG, "It's already disconnected or connection/disconnection is in " +
                    "progress");
            return false;
        }

        DisconnectThread thread = new DisconnectThread(listener);
        thread.start();
        return true;
    }

    int send(byte[] dataBuffer, int dataLength) {
        if (this.getState() != State.kConnected) {
            Logger.print(TAG, "It's already disconnected or connection/disconnection is in " +
                    "progress");
            return -1;
        }

        if (this.mClientSocket == null) {
            return -2;
        }

        int ret = this.mClientSocket.send(dataBuffer, dataLength);
        // Omit Implementing Statistics: SendDataSize
        return ret;
    }

    int receive(byte[] dataBuffer, int dataLength) {
        if (this.getState() != State.kConnected) {
            Logger.print(TAG, "It's already disconnected or connection/disconnection is in " +
                    "progress");
            return -1;
        }

        if (this.mClientSocket == null) {
            return -2;
        }

        int ret = this.mClientSocket.receive(dataBuffer, dataLength);
        // Omit Implementing Statistics: ReceiveDataSize
        return ret;
    }

    // Connect/Disconnect Threads & Callbacks
    class ConnectThread extends Thread {
        @Override
        public void run() {
            Logger.print(TAG, self.getName() + "'s Connect Thread Spawned! (id:" + this.getId() +
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
                    Logger.print("Cannot connect the server adapter - turn-on fail: " + self
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
                boolean res = self.mP2PClient.discoverAndConnect();

                p2pClientState = self.mP2PClient.getState();
                if (!res || p2pClientState != P2PClient.State.kConnected) {
                    Logger.print("Cannot connect the server adapter - allow fail:" + self.getName
                            ());
                    self.mDevice.releaseAndTurnOff();
                    this.onFail();
                    return;
                }
            }

            // Open client socket
            if (self.mClientSocket == null) {
                self.mP2PClient.disconnect();
                self.mDevice.releaseAndTurnOff();

                this.onFail();
                return;
            }
            int socketState = self.mClientSocket.getState();
            if (socketState != ClientSocket.State.kOpened) {
                boolean res = self.mClientSocket.open();

                socketState = self.mClientSocket.getState();
                if (!res || socketState != ClientSocket.State.kOpened) {
                    Logger.print(TAG, "Cannot connect the server adapter - socket open fail: " +
                            self.getName());
                    self.mP2PClient.disconnect();
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

        public ConnectThread(ConnectResultListener resultListener) {
            this.mResultListener = resultListener;
        }
    }

    class DisconnectThread extends Thread {
        @Override
        public void run() {
            Logger.print(TAG, self.getName() + "'s Disconnect Thread Spawned! (id:" + this.getId
                    () + ")");
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
                    Logger.print(TAG, "Cannot disconnect the server adapter - socket close fail: " +
                            "" + "" + self.getName());
                    this.onFail();
                    return;
                }
            }

            // Disconnect
            int p2pClientState = self.mClientSocket.getState();
            if (p2pClientState != P2PClient.State.kDisconnected) {
                boolean res = self.mP2PClient.disconnect();

                p2pClientState = self.mP2PClient.getState();
                if (!res || p2pClientState != P2PClient.State.kDisconnected) {
                    Logger.print(TAG, "Cannot disconnect the server adapter - disconnect P2P " +
                            "client fail: " + self.getName());
                    this.onFail();
                    return;
                }
            }

            // Turn off device
            int deviceState = self.mDevice.getState();
            if (deviceState != Device.State.kOff) {
                boolean res = self.mDevice.releaseAndTurnOff();

                deviceState = self.mDevice.getState();
                if (!res || deviceState != Device.State.kOff) {
                    Logger.print(TAG, "Cannot disconnect the server adapter - turn-off fail: " +
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

        public DisconnectThread(DisconnectResultListener resultListener) {
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
    boolean enableSenderThread() {
        this.mSenderThread = new SenderThread();
        return false;
    }

    boolean enableReceiverThread() {
        this.mReceiverThread = new ReceiverThread();
        return false;
    }

    // Receiver Loop Interface
    interface ReceiveLoop {
        void receiveLoop(ClientAdapter adapter);
    }

    // Sender/Receiver Thraeds
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

        public SenderThread() {
            synchronized (this.mIsOn) {
                this.mIsOn = false;
            }
        }

        public boolean isOn() {
            return this.mIsOn;
        }

        private Boolean mIsOn;
    }

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

        public ReceiverThread() {
            synchronized (this.mIsOn) {
                this.mIsOn = false;
            }
        }

        public boolean isOn() {
            return this.mIsOn;
        }

        private Boolean mIsOn;
    }

    private SenderThread mSenderThread;
    private ReceiverThread mReceiverThread;

    // Initialize
    public ClientAdapter(int id, String name) {
        this.mId = id;
        this.mName = name;
        this.mState = State.kDisconnected;
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

    protected int getState() {
        int state;
        synchronized (this.mState) {
            state = this.mState;
        }
        return state;
    }

    protected void setState(int newState) {
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
    protected int mId;
    protected String mName;

    // State
    protected Integer mState;

    // State Listener
    protected ArrayList<StateListener> mListeners;

    // Main Components : Device, P2PClient, ClientSocket
    protected Device mDevice;
    protected P2PClient mP2PClient;
    protected ClientSocket mClientSocket;
}