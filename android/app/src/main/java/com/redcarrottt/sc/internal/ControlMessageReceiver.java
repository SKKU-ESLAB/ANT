package com.redcarrottt.sc.internal;

import android.util.Log;

import com.redcarrottt.testapp.Logger;

import java.util.ArrayList;

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
    private static final String kTag = "ControlMessageReceiver";
    private static final String kThreadName = "Control Message Receiving";

    public void startReceivingThread() {
        this.mReceivingThread = new ReceivingThread();
        this.mReceivingThread.start();
    }

    public void stopReceivingThread() {
        this.mReceivingThread.finish();
    }

    // Thread
    private class ReceivingThread extends Thread {
        @Override
        public void run() {
            this.setName("ControlMsgReceiver");
            this.mIsOn = true;
            Logger.THREAD_LAUNCH(kThreadName);

            while (this.mIsOn) {
                this.loopInternal();
            }

            Logger.THREAD_FINISH(kThreadName);
            this.mIsOn = false;
        }

        public boolean loopInternal() {
            byte[] messageBuffer = new byte[10 * 1024];
            int res = Core.singleton().receive(messageBuffer, true);
            if (res <= 0) {
                return false;
            }

            String message = new String(messageBuffer).trim();

            // Find separator location (between first line and other lines)
            Log.d(kTag, "Control message incoming: " + message);
            int separatorPos = message.indexOf('\n');

            // Divide the message into first line & other lines
            String firstLine = message.substring(0, separatorPos);
            String otherLines = message.substring(separatorPos + 1);

            int controlMessageCode = Integer.parseInt(firstLine);

            switch (controlMessageCode) {
                case ControlMessageProtocol.CMCode.kConnect:
                case ControlMessageProtocol.CMCode.kSleep:
                case ControlMessageProtocol.CMCode.kWakeup:
                case ControlMessageProtocol.CMCode.kDisconnectAck: {
                    // Normal type
                    int adapterId = Integer.parseInt(otherLines);
                    onReceiveNormalMessage(controlMessageCode, adapterId);
                    break;
                }
                case ControlMessageProtocol.CMCode.kDisconnect: {
                    // Disconnect type
                    // Divide the message into second line, third line, fourth line
                    separatorPos = otherLines.indexOf('\n');
                    String secondLine = otherLines.substring(0, separatorPos);
                    String thirdFourthLine = otherLines.substring(separatorPos + 1);
                    separatorPos = thirdFourthLine.indexOf('\n');
                    String thirdLine = thirdFourthLine.substring(0, separatorPos);
                    String fourthLine = thirdFourthLine.substring(separatorPos + 1);

                    int adapterId = Integer.parseInt(secondLine);
                    int finalSeqNoControl = Integer.parseInt(thirdLine);
                    int finalSeqNoData = Integer.parseInt(fourthLine);

                    onReceiveDisconnectMessage(adapterId, finalSeqNoControl, finalSeqNoData);
                    break;
                }
                case ControlMessageProtocol.CMCode.kPriv: {
                    // Priv type
                    onReceivePrivateMessage(otherLines);
                    break;
                }
                default: {
                    Logger.ERR(kTag, "Unknown control message code (" + controlMessageCode + ")"
                            + "!\n" + message);
                    break;
                }
            }

            return true;
        }

        public void finish() {
            this.mIsOn = false;
        }

        // Constructor
        public ReceivingThread() {
            this.mIsOn = false;
        }

        // Attributes
        private boolean mIsOn;
    }

    private void onReceiveNormalMessage(int controlMessageCode, int adapterId) {
        switch (controlMessageCode) {
            case ControlMessageProtocol.CMCode.kConnect: {
                Logger.VERB(kTag, "Receive(Control Msg): Request(Connect " + adapterId + ")");
                NetworkSwitcher.singleton().connectAdapterByPeer(adapterId);
                break;
            }
            case ControlMessageProtocol.CMCode.kSleep: {
                Logger.VERB(kTag, "Receive(Control Msg): Request(Sleep " + adapterId + ")");
                NetworkSwitcher.singleton().sleepAdapterByPeer(adapterId);
                break;
            }
            case ControlMessageProtocol.CMCode.kWakeup: {
                Logger.VERB(kTag, "Receive(Control Msg): Request(WakeUp " + adapterId + ")");
                NetworkSwitcher.singleton().wakeUpAdapterByPeer(adapterId);
                break;
            }
            case ControlMessageProtocol.CMCode.kDisconnectAck: {
                Logger.VERB(kTag, "Receive(Control Msg): Request(DisconenctAck " + adapterId + ")");
                ClientAdapter disconnectAdapter = Core.singleton().findAdapterById(adapterId);
                if (disconnectAdapter == null) {
                    Logger.WARN(kTag, "Cannot find adapter " + adapterId);
                } else {
                    disconnectAdapter.peerKnowsDisconnectingOnPurpose();
                }
                break;
            }
        }
    }

    private void onReceiveDisconnectMessage(int adapterId, int finalSeqNoControl, int
            finalSeqNoData) {
        Logger.VERB(kTag, "Receive(Control Msg): Request(Disconnect " + adapterId + " / " +
                "final_seq_no_control=" + finalSeqNoControl + " / final_seq_no_data=" +
                finalSeqNoData + ")");
        NetworkSwitcher.singleton().disconnectAdapterByPeer(adapterId, finalSeqNoControl,
                finalSeqNoData);
    }

    private void onReceivePrivateMessage(String contents) {
        // Find separator location
        int separatorPos = contents.indexOf('\n');

        // Divide the message into second line & other lines
        String secondLine = contents.substring(0, separatorPos);
        String privateMessage = contents.substring(separatorPos + 1);

        int privateType = Integer.parseInt(secondLine);

        // Notify the private message
        for (ControlMessageListener listener : mControlMessageListeners) {
            listener.onReceiveControlMessage(privateType, privateMessage);
        }
    }

    public void addControlMessageListener(ControlMessageListener listener) {
        this.mControlMessageListeners.add(listener);
    }

    public ControlMessageReceiver() {
        this.mReceivingThread = null;
        this.mControlMessageListeners = new ArrayList<>();
    }

    // Attributes
    private ReceivingThread mReceivingThread;
    private ArrayList<ControlMessageListener> mControlMessageListeners;
}
