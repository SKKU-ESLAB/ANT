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

public class NetworkSwitcher {
    public void start() {
        // TODO:
    }

    public void stop() {
        // TODO:
    }

    // Connet adapter command.
    // It is called by peer through Core.
    void connect_adapter(int adapterId) {
        // TODO:
    }

    // Reconnect control adapter command.
    // It is called by Core.
    void reconnect_control_adapter() {
        // TODO:
    }

    public int getState() {
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

    private Integer mState;

    class State {
        public static final int kInitialized = 0;
        public static final int kRunning = 1;
        public static final int kSwitching = 2;
    }
}