package com.redcarrottt.sc.internal;

import com.redcarrottt.testapp.Logger;

import java.nio.ByteBuffer;

/* Copyright (c) 2018, contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong <redcarrottt@gmail.com>
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
public class ControlMessageSender {
    // TODO: convert it to ControlMessageSender
    // Control Message Handling
    private void sendControlMessage(byte[] dataBuffer, int dataLength) {
        int state = this.getState();
        if (state != State.kReady) {
            Logger.ERR(kTag, "Core is not started yet, so you cannot send the data");
            return;
        }
        // TODO: use Core API
//        ClientAdapter controlAdapter = this.getActiveControlAdapter();
//        if (controlAdapter == null) {
//            Logger.ERR(kTag, "Failed to send control message: no active control adapter");
//            return;
//        }
//
//        controlAdapter.send(dataBuffer, dataLength);
    }

    // TODO: convert it to ControlMessageSender
    private void sendRequest(char requestCode, short adapterId) {
        int state = this.getState();
        if (state != State.kReady) {
            Logger.ERR(kTag, "Core is not started yet, so you cannot send the data");
            return;
        }

        {
            ByteBuffer buffer = ByteBuffer.allocate(3);
            buffer.put((byte) requestCode);
            buffer.putShort(adapterId);
            this.sendControlMessage(buffer.array(), 3);
        }
    }

    // TODO: convert it to ControlMessageSender
    public void sendRequestConnect(short adapterId) {
        this.sendRequest(CMCode.kConnect, adapterId);
        Logger.VERB(kTag, "Send(Control Msg): Request(Connect " + adapterId + ")");
    }

    // TODO: convert it to ControlMessageSender
    public void sendRequestDisconnect(short adapterId) {
        this.sendRequest(CMCode.kDisconnect, adapterId);
        Logger.VERB(kTag, "Send(Control Msg): Request(Disconnect " + adapterId + ")");
    }

    // TODO: convert it to ControlMessageSender
    public void sendRequestDisconnectAck(short adapterId) {
        this.sendRequest(CMCode.kDisconnectAck, adapterId);
        Logger.VERB(kTag, "Send(Control Msg): Request(DisconnectAck " + adapterId + ")");
    }

    // TODO: convert it to ControlMessageSender
    public void sendRequestSleep(short adapterId) {
        this.sendRequest(CMCode.kSleep, adapterId);
        Logger.VERB(kTag, "Send(Control Msg): Request(Sleep " + adapterId + ")");
    }

    // TODO: convert it to ControlMessageSender
    public void sendRequestWakeup(short adapterId) {
        this.sendRequest(CMCode.kWakeup, adapterId);
        Logger.VERB(kTag, "Send(Control Msg): Request(WakeUp " + adapterId + ")");
    }

    // TODO: convert it to ControlMessageSender
    public void sendNotiPrivateData(short adapterId, byte[] privDataBuffer) {
        // Not implemented yet
    }
}
