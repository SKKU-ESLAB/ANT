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

public class Core {
    private static final String kTag = "Core";

    // APIs: These functions are mapped to ones in API.
    boolean start() {
        if (this.getState() == State.kIdle) {
            Logger.print(kTag, "Core has already started");
            return false;
        } else if (this.mControlAdapter == null) {
            Logger.print("No control adapter is registered!");
            return false;
        } else if (this.mDataAdapters.isEmpty()) {
            Logger.print("No data adapter is registered!");
            return false;
        }
        this.setState(State.kStarting);
        StartCoreTransaction.start();

        return false;
    }

    boolean stop() {
        // TODO
        return false;
    }

    public void registerControlAdapter(ClientAdapter adapter) {
        // TODO
    }

    public void registerDataAdapter(ClientAdapter adapter) {
        // TODO
    }

    public int send(byte[] dataBuffer, int dataLength) {
        // TODO
        return -1;
    }

    public int receive(byte[] dataBuffer, int dataLength) {
        // TODO
        return -1;
    }

    // Control Message Handling
    protected void sendControlMessage(byte[] dataBuffer, int dataLength) {
        // TODO
    }

    public void sendRequestConnect(short adapterId) {
        // TODO
    }

    class ReceiveControlMessageLoop implements ClientAdapter.ReceiveLoop {
        @Override
        public void receiveLoop(ClientAdapter adapter) {
            // TODO
        }
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

    protected int getState() {
        int state;
        synchronized (this.mState) {
            state = this.mState;
        }
        return state;
    }

    protected void setState(int newState) {
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

}

class StartCoreTransaction {
    public static StartCoreTransaction start() {
        if (sSingleton == null) {
            sSingleton = new StartCoreTransaction();
            sSingleton.connectControlAdapter();
            return sSingleton;
        } else {
            return null;
        }
    }

    private void connectControlAdapter() {

    }

    private void onConnectControlAdapter() {

    }

    private void onConnectFirstDataAdapter() {
    }

    private StartCoreTransaction() {
        // Private Constructor
    }

    private static StartCoreTransaction sSingleton = null;
}
