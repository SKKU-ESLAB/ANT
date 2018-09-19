package com.redcarrottt.sc.internal;

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

interface TurnOnResultListener {
    void onTurnOnResult(boolean isSuccess);
}

interface TurnOffResultListener {
    void onTurnOffResult(boolean isSuccess);
}

public abstract class Device {
    private final String kTag = "Device";

    // Main Functions
    @SuppressWarnings("SynchronizeOnNonFinalField")
    void turnOn(TurnOnResultListener resultListener) {
        this.mTurnOnResultListener = resultListener;
            this.setState(State.kTurningOn);
            this.turnOnImpl();
        } else {
            // Already turn on
            Logger.VERB(kTag, this.mName + " is already turned on");
            if (this.mTurnOnResultListener != null)
                this.mTurnOnResultListener.onTurnOnResult(true);
        }
    }

    private TurnOnResultListener mTurnOnResultListener;

    protected void doneTurnOn(boolean isSuccess) {
        if (isSuccess) {
            // Proceed state
            this.setState(State.kOn);
        } else {
            // Recover original state
            synchronized (this.mRefCount) {
                this.mRefCount--;
            }
            this.setState(State.kOff);
        }
        if (this.mTurnOnResultListener != null)
            this.mTurnOnResultListener.onTurnOnResult(isSuccess);
    }

    @SuppressWarnings("SynchronizeOnNonFinalField")
    void turnOff(TurnOffResultListener resultListener) {
        int refCount;
        synchronized (this.mRefCount) {
            this.mRefCount--;
            refCount = this.mRefCount;
        }
        this.mTurnOffResultListener = resultListener;
        if (refCount == 0) {
            this.setState(State.kTurningOff);
            this.turnOffImpl();
        } else {
            // Not yet turn off
            Logger.VERB(kTag, this.mName + " is used by other components, so not turned off");
            if (this.mTurnOffResultListener != null)
                this.mTurnOffResultListener.onTurnOffResult(true);
        }
    }

    private TurnOffResultListener mTurnOffResultListener;

    protected void doneTurnOff(boolean isSuccess) {
        if (isSuccess) {
            // Proceed state
            this.setState(State.kOff);
        } else {
            // Recover original state
            synchronized (this.mRefCount) {
                this.mRefCount++;
            }
            this.setState(State.kOn);
        }
        if (this.mTurnOffResultListener != null)
            this.mTurnOffResultListener.onTurnOffResult(isSuccess);
    }

    // Implemented by child classes
    protected abstract void turnOnImpl();

    protected abstract void turnOffImpl();

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

    // State
    private Integer mState;
}