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

public abstract class P2PClient {
    private final String kTag = "P2PClient";

    // Main Functions
    public boolean discoverAndConnect() {
        if (this.getState() != State.kDisconnected) {
            Logger.print(kTag, "It's already connected or discovering is in progress");
            return false;
        }

        this.setState(State.kDiscovering);
        boolean res = this.discoverImpl();
        if (!res) {
            this.setState(State.kDisconnected);
            return res;
        }

        this.setState(State.kConnecting);
        res = this.connectImpl();
        if (!res) {
            this.setState(State.kDisconnected);
            return res;
        }
        this.setState(State.kConnected);

        return res;
    }

    public boolean disconnect() {
        if (this.getState() != State.kConnected) {
            Logger.print(kTag, "It's already disconnected or disconnecting is in progress");
            return false;
        }

        this.setState(State.kDisconnecting);
        boolean res = this.disconnectImpl();
        this.setState(State.kDisconnected);

        return res;
    }

    // Implemented by child classes
    protected abstract boolean discoverImpl();

    protected abstract boolean connectImpl();

    protected abstract boolean disconnectImpl();

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

    public P2PClient() {
        this.mState = State.kDisconnected;
    }

    // Attributes

    // State
    protected Integer mState;
}