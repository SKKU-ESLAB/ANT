package com.redcarrottt.sc.internal;

import com.redcarrottt.testapp.Logger;

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
    private static final String kTag = "ControlMessageSender";

    public void sendRequestConnect(int adapterId) {
        this.sendRequest(ControlMessageProtocol.CMCode.kConnect, adapterId);
        Logger.VERB(kTag, "Send(Control Msg): Request(Connect " + adapterId + ")");
    }

    public void sendRequestDisconnect(int adapterId, int final_seq_no_control, int
            final_seq_no_data) {
        String message = "" + ControlMessageProtocol.CMCode.kDisconnect + "\n" + adapterId + "\n"
                + final_seq_no_control + "\n" + final_seq_no_data;
        this.sendControlMessage(message);
        Logger.VERB(kTag, "Send(Control Msg): Request(Disconnect " + adapterId + "; " +
                "final_seq_no_control=" + final_seq_no_control + "; final_seq_no_data=" +
                final_seq_no_data + ")");
    }

    public void sendRequestDisconnectAck(int adapterId) {
        this.sendRequest(ControlMessageProtocol.CMCode.kDisconnectAck, adapterId);
        Logger.VERB(kTag, "Send(Control Msg): Request(DisconnectAck " + adapterId + ")");
    }

    public void sendRequestSleep(int adapterId) {
        this.sendRequest(ControlMessageProtocol.CMCode.kSleep, adapterId);
        Logger.VERB(kTag, "Send(Control Msg): Request(Sleep " + adapterId + ")");
    }

    public void sendRequestWakeup(int adapterId) {
        this.sendRequest(ControlMessageProtocol.CMCode.kWakeup, adapterId);
        Logger.VERB(kTag, "Send(Control Msg): Request(WakeUp " + adapterId + ")");
    }

    public void sendNotiPrivateData(int privateType, String privateMessage) {
        String message = "" + ControlMessageProtocol.CMCode.kPriv + "\n" + privateType + "\n" +
                privateMessage;
        this.sendControlMessage(message);
    }

    private void sendRequest(int requestCode, int adapterId) {
        String message = "" + requestCode + "\n" + adapterId;
        this.sendControlMessage(message);
    }

    private void sendControlMessage(String controlMessage) {
        byte[] messageBuffer = controlMessage.getBytes();
        Core.singleton().send(messageBuffer, messageBuffer.length, true);
    }
}
