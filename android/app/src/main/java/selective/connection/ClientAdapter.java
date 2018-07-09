package selective.connection;

/* Copyright (c) 2018, contributors. All rights reserved.
 *
 * Contributor: 
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

class Device {
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
        synchronized (this.mState) {
            this.mState = newState;
        }
    }

    // Attributes

    // State
    protected Integer mState;
}

class P2PClient {
    // State
    class State {
        public static final int kDisconnected = 0;
        public static final int kDiscovering = 1;
        public static final int kConnecting = 2;
        public static final int kConnected = 3;
        public static final int kDisconnecting = 4;
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

    // Attributes

    // State
    protected Integer mState;
}

class ClientSocket {
    // State
    class State {
        public static final int kOff = 0;
        public static final int kTurningOff = 1;
        public static final int kOn = 2;
        public static final int k
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

    // Attributes

    // State
    protected Integer mState;
}

public class ClientAdapter {
    // Main Functions: connect, disconnect, send, receive
    public boolean connect() {
        return false;
    }

    public boolean disconnect() {
        return false;
    }

    int send(byte[] buf, int len) {
        return -1;
    }

    int receive(byte[] buf, int len) {
        return -1;
    }

    // Thread Control Functions: enableSenderThread, enableReceiverThread
    boolean enableSenderThread() {
        return false;
    }

    boolean enableReceiverThread() {
        return false;
    }

    // Initialize
    public ClientAdapter(int id, String name) {

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
        synchronized (this.mState) {
            this.mState = newState;
        }
    }

    // Attributes
    protected int mId;
    protected String mName;

    // State
    protected Integer mState;

    // Main Components : Device, P2PClient, ClientSocket
    protected Device mDevice;
    protected P2PClient mP2PClient;
    protected ClientSocket mClientSocket;
}