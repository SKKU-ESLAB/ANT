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

public abstract class Device {
    private final String TAG = "Device";

    // Main Functions
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
            return res;
        } else {
            // Already turn on
            return true;
        }
    }

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
            return res;
        } else {
            // Not yet turn off
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

    public Device(String name) {
        this.mState = State.kOff;
        this.mName = name;
    }

    // Attributes
    protected String mName;
    private Integer mRefCount;

    // State
    protected Integer mState;
}