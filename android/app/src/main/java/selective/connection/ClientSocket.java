package selective.connection;

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

public abstract class ClientSocket {
    private final String TAG = "ClientSocket";

    // Main Functions
    public boolean open() {
        if (this.getState() != State.kClosed) {
            Logger.print(TAG, "It's already opened or opening/closing is in progress");
            return false;
        }

        this.setState(State.kOpening);
        boolean res = this.openImpl();
        if (!res) {
            this.setState(State.kClosed);
        } else {
            this.setState(State.kOpened);
        }
        return res;
    }

    public boolean close() {
        if (this.getState() != State.kOpened) {
            Logger.print(TAG, "It's already closed or opening/closing is in progress");
            return false;
        }

        this.setState(State.kClosing);
        boolean res = this.closeImpl();
        this.setState(State.kClosed);

        return res;
    }

    public int send(byte[] dataBuffer, int dataLength) {
        if (this.getState() != State.kOpened) {
            Logger.print(TAG, "Socket is not opened");
            return -1;
        }

        int res = this.sendImpl(dataBuffer, dataLength);
        if (res < 0) {
            this.setState(State.kClosed);
        }
        return res;
    }

    public int receive(byte[] dataBuffer, int dataLength) {
        if (this.getState() != State.kOpened) {
            Logger.print(TAG, "Socket is not opened");
            return -1;
        }

        int res = this.receiveImpl(dataBuffer, dataLength);
        if (res < 0) {
            this.setState(State.kClosed);
        }
        return res;
    }

    // Implemented by child classes
    protected abstract boolean openImpl();

    protected abstract boolean closeImpl();

    protected abstract int sendImpl(byte[] dataBuffer, int dataLength);

    protected abstract int receiveImpl(byte[] dataBuffer, int dataLength);

    // State
    class State {
        public static final int kClosed = 0;
        public static final int kOpening = 1;
        public static final int kOpened = 2;
        public static final int kClosing = 3;
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

    public ClientSocket() {
        this.mState = State.kClosed;
    }

    // Attributes

    // State
    protected Integer mState;
}