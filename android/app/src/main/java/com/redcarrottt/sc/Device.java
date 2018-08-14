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

public abstract class Device {
    private final String kTag = "Device";

    // Main Functions
    @SuppressWarnings("SynchronizeOnNonFinalField")
    boolean holdAndTurnOn() {
        int refCount;
        synchronized (this.mRefCount) {
            this.mRefCount++;
            refCount = this.mRefCount;
        }
        if (refCount == 1) {
            this.setState(State.kTurningOn);
            boolean res = this.turnOnImpl();
            if (!res) {
                synchronized (this.mRefCount) {
                    this.mRefCount--;
                }
                this.setState(State.kOff);
            } else {
                this.setState(State.kOn);
            }
            Logger.VERB(kTag, this.mName + " is turned on");
            return res;
        } else {
            // Already turn on
            Logger.VERB(kTag, this.mName + " is already turned on");
            return true;
        }
    }

    @SuppressWarnings("SynchronizeOnNonFinalField")
    boolean releaseAndTurnOff() {
        int refCount;
        synchronized (this.mRefCount) {
            this.mRefCount--;
            refCount = this.mRefCount;
        }
        if (refCount == 0) {
            this.setState(State.kTurningOff);
            boolean res = this.turnOffImpl();
            if (!res) {
                synchronized (this.mRefCount) {
                    this.mRefCount++;
                }
                this.setState(State.kOn);
            } else {
                this.setState(State.kOff);
            }
            Logger.VERB(kTag, this.mName + " is turned off");
            return res;
        } else {
            // Not yet turn off
            Logger.VERB(kTag, this.mName + " is used by other components, so not turned off");
            return true;
        }
    }

    // Implemented by child classes
    protected abstract boolean turnOnImpl();

    protected abstract boolean turnOffImpl();

    // State
    class State {
        public static final int kOff = 0;
        public static final int kTurningOn = 1;
        public static final int kOn = 2;
        public static final int kTurningOff = 3;
    }

    @SuppressWarnings("SynchronizeOnNonFinalField")
    protected int getState() {
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

    public Device(String name) {
        this.mState = State.kOff;
        this.mName = name;
    }

    // Attributes
    private String mName;
    private Integer mRefCount;

    // State
    private Integer mState;
}