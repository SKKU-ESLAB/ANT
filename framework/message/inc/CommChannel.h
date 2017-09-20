/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
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

#ifndef __COMM_CHANNEL_H__
#define __COMM_CHANNEL_H__

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#include "BaseMessage.h"
#include "MessageRouter.h"
#include "CommPort.h"
#include "DeviceController.h"

class CommChannel;
class CommChannelState;
class CommChannelStateListener;

class CommChannelState {
  // State: IDLE -> LISTENING_DEFAULT -> CONNECTED_DEFAULT
  //             -> LISTENING_LARGEDATA -> CONNECTED_LARGEDATA
  public:
    enum Value {
      IDLE,
      LISTENING_DEFAULT,
      CONNECTED_DEFAULT,
      LISTENING_LARGEDATA,
      CONNECTED_LARGEDATA
    };

  public:
    CommChannelState() : mValue(IDLE), mStateListener(NULL) { }

    void set(Value newValue);

    Value get() { return this->mValue; }

    void setStateListener(CommChannelStateListener* stateListener) {
      this->mStateListener = stateListener;
    }

  protected:
    Value mValue;
    CommChannelStateListener* mStateListener;
};

class CommChannelStateListener {
  public:
    virtual void onCommChannelStateChanged(CommChannelState::Value state) = 0;
};

class DataPortsListener : public CommPortListener {
  public:
    DataPortsListener(CommChannel* owner) : mOwner(owner) { }

    // Receive raw message
    virtual void onReceivedRawMessage(std::string messageData,
        std::string filePath);

  private:
    CommChannel* mOwner;
};

class ControlPortsListener : public CommPortListener {
  public:
    ControlPortsListener(CommChannel* owner) : mOwner(owner) { }

    // Receive raw message
    virtual void onReceivedRawMessage(std::string messageData,
        std::string filePath);

  private:
    CommChannel* mOwner;
};

class CommChannel : public Channel, CommPortStateListener {
  public:
    CommChannel(MessageRouter* messageRouter, const char* fileTempDir)
      : Channel(messageRouter, "CommChannel"), mFileTempDir(fileTempDir) {
        // Initialize ports
        char defaultPortUUID[]
          = {0x0a,0x1b,0x2c,0x3d,0x4e,0x5f,0x6a,0x1c,0x2d,0x0e,0x1f,0x2a,
            0x3b,0x4c,0x5d,0x6e};
        char controlPortUUID[]
          = {0x0a,0x1b,0x2c,0x3d,0x4e,0x5f,0x6a,0x1c,0x2d,0x0e,0x1f,0x2a,
            0x3b,0x4c,0x5d,0x6d};
        const int kLargeDataPortNum = 10001;

        this->mDefaultPort = new BluetoothCommPort(
            "Default Port", this, defaultPortUUID);
        this->mControlPort = new BluetoothCommPort(
            "Control Port", this, controlPortUUID);
        this->mLargeDataPort = new WifiDirectCommPort(
            "LargeData Port", this, kLargeDataPortNum);

        // Initialize ports listeners
        this->mDataPortsListener = new DataPortsListener(this);
        this->mControlPortsListener = new ControlPortsListener(this);
    }

    ~CommChannel() {
      this->stop();

      delete this->mDefaultPort;
      delete this->mControlPort;
      delete this->mLargeDataPort;
    }

    // Channel function
    virtual void run();

    // State managememnt
    void setStateListener(CommChannelStateListener* stateListener) {
      this->mState.setStateListener(stateListener);
    }
    CommChannelState::Value getState() {
      this->mState.get();
    }

    // CommPortListener
    virtual void onCommPortStateChanged(CommPort *port,
        CommPortState::Value state);

    void onReceivedControlMessage(std::string messageData);

    // CommPortListener - Receive message
    void onReceivedMessage(BaseMessage* message);

  protected:
    // Start/stop CommChannel
    bool start();
    void stop();

    // Enable/disable large data mode
    bool enableLargeDataMode();
    void disableLargeDataMode();

    // Channel function (RoutedThread) - Send message
    virtual void onRoutedMessage(BaseMessage* message);

    // Get IP Address
    int getIpAddress(const char* interfaceName, char* ipAddr);

  protected:
    // State
    CommChannelState mState;

    // Directory path to store received files temporarily
    const char* mFileTempDir;

    // DeviceControllers
    BluetoothDeviceController mBluetoothDeviceController;
    WifiDirectDeviceController mWifiDirectDeviceController;

    // CommPorts
    BluetoothCommPort* mDefaultPort;
    BluetoothCommPort* mControlPort;
    WifiDirectCommPort* mLargeDataPort;

    // CommPortListeners
    DataPortsListener* mDataPortsListener;
    ControlPortsListener* mControlPortsListener;
};
#endif // !defined(__COMM_CHANNEL_H__)
