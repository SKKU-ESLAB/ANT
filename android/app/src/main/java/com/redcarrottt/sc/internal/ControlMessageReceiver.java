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
public class ControlMessageReceiver {
    // TODO: convert it to ControlMessageReceiver
    public void receiveLoop(ClientAdapter adapter) {
        byte[] dataBuffer = new byte[512];
        int res;
        while (true) {
            // Receive 1Byte: Control Request Code
            Logger.DEBUG(kTag, "Control Receiver: receiving request code...");
            res = adapter.receive(dataBuffer, 1);
            Logger.DEBUG(kTag, "Control Receiver: RECEIVED request code!");
            if (res <= 0) {
                Logger.VERB(kTag, "Control adapter could be closed");
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                break;
            }

            Byte reqCode = dataBuffer[0];
            if (reqCode == CMCode.kConnect || reqCode == CMCode.kSleep || reqCode == CMCode
                    .kWakeup || reqCode == CMCode.kDisconnect || reqCode == CMCode.kDisconnectAck) {
                // "Connect Adapter" Request
                // Receive 2Byte: Adapter ID
                res = adapter.receive(dataBuffer, 2);
                if (res <= 0) break;
                ByteBuffer buffer = ByteBuffer.allocate(2);
                buffer.put(dataBuffer, 0, 2);
                short adapterId = buffer.getShort(0);

                if (reqCode == CMCode.kConnect) {
                    Logger.VERB(kTag, "Receive(Control Msg): Request(Connect " + adapterId + ")");
                    NetworkSwitcher.singleton().connectAdapterByPeer(adapterId);
                } else if (reqCode == CMCode.kSleep) {
                    Logger.VERB(kTag, "Receive(Control Msg): Request(Sleep " + adapterId + ")");
                    NetworkSwitcher.singleton().sleepAdapterByPeer(adapterId);
                } else if (reqCode == CMCode.kWakeup) {
                    Logger.VERB(kTag, "Receive(Control Msg): Request(WakeUp " + adapterId + ")");
                    NetworkSwitcher.singleton().wakeUpAdapterByPeer(adapterId);
                } else if (reqCode == CMCode.kDisconnect) {
                    Logger.VERB(kTag, "Receive(Control Msg): Request(Disconnect " + adapterId +
                            ")");
                    NetworkSwitcher.singleton().disconnectAdapterByPeer(adapterId);
                } else if (reqCode == CMCode.kDisconnectAck) {
                    Logger.VERB(kTag, "Receive(Control Msg): Request(DisconnectAck " + adapterId
                            + ")");
                    ClientAdapter disconnect_adapter = findAdapterById(adapterId);
                    if (disconnect_adapter == null) {
                        Logger.WARN(kTag, "Cannot find adapter " + adapterId);
                    } else {
                        disconnect_adapter.peerKnowsDisconnectingOnPurpose();
                    }
                }
            } else if (reqCode == CMCode.kPriv) {
                // "Priv" Request
                int privType;
                int privDataLength;
                // Receive 4Byte: Priv Type
                res = adapter.receive(dataBuffer, 4);
                if (res <= 0) {
                    break;
                } else {
                    Logger.VERB(kTag, "Receive(Control Msg): Request(Priv Noti--Start)");
                    ByteBuffer tempBuffer = ByteBuffer.allocate(4);
                    //Logger.DEBUG(kTag, "tempBuffer remaining: " + tempBuffer.remaining());
                    tempBuffer.put(dataBuffer, 0, 4);
                    privType = tempBuffer.getInt(0);
                }

                // Receive 4Byte: Private Data Length
                res = adapter.receive(dataBuffer, 4);
                if (res <= 0) {
                    break;
                } else {
                    ByteBuffer tempBuffer2 = ByteBuffer.allocate(4);
                    //Logger.DEBUG(kTag, "tempBuffer2 remaining: " + tempBuffer2.remaining());
                    tempBuffer2.put(dataBuffer, 0, 4);
                    privDataLength = tempBuffer2.getInt(0);
                }
                if (privDataLength > 512) throw new AssertionError();

                // Receive nByte: Private Data
                res = adapter.receive(dataBuffer, privDataLength);
                if (res > 0) {
                    Logger.VERB(kTag, "Receive(Control Msg): Request(Priv Noti '" + dataBuffer +
                            "'; type=" + privType + ")");
                    for (ControlMessageListener listener : mControlMessageListeners) {
                        listener.onReceiveControlMessage(privType, dataBuffer, privDataLength);
                    }
                } else {
                    break;
                }
            }
        }
        Logger.DEBUG(kTag, "Control adapter has been closed");

        NetworkSwitcher.singleton().reconnectAdapter(adapter);
    }
}
