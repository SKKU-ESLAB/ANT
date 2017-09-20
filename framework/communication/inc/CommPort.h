/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
 *              Eunsoo Park<esevan.park@gmail.com>
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

#ifndef __COMM_PORT_H__
#define __COMM_PORT_H__

#include <string>

#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

class CommPort;
class CommPortState;
class CommPortStateListener;

class CommPortState {
  // State: CLOSED -> OPENED -> CONNECTED
  public:
    enum Value {
      CLOSED,
      OPENED,
      CONNECTED
    };

  public:
    CommPortState(CommPort* owner)
      : mOwner(owner), mValue(CLOSED), mStateListener(NULL) { }

    void set(Value newValue);

    Value get() { return this->mValue; }

    void setStateListener(CommPortStateListener* stateListener) {
      this->mStateListener = stateListener;
    }

  protected:
    CommPort* mOwner;
    Value mValue;
    CommPortStateListener* mStateListener;
};

class CommPortStateListener {
  public:
    virtual void onCommPortStateChanged(CommPort *port,
        CommPortState::Value state) = 0;
};

class CommPortListener {
  public:
    // Receive raw message
    virtual void onReceivedRawMessage(std::string messageData,
        std::string filePath) = 0;
    void onReceivedRawMessage(std::string messageData) {
      this->onReceivedRawMessage(messageData, "");
    }
};

#define COMM_PORT_SOCKET_UNINITIALIZED -1

class CommPort {
  public:
    CommPort(std::string portName, CommPortStateListener* stateListener)
      : mPortName(portName),
      mSocket(COMM_PORT_SOCKET_UNINITIALIZED),
      mListenedSocket(COMM_PORT_SOCKET_UNINITIALIZED),
      mPresentHeaderId(0),
      mIsListeningThreadOn(false), mListener(NULL) {
      this->mState = new CommPortState(this);
      this->mState->setStateListener(stateListener);
    }

    ~CommPort() {
      delete this->mState;
    }

    // State management
    bool isOpened() {
      CommPortState::Value state = this->mState->get();
      return (state == CommPortState::OPENED
          || state == CommPortState::CONNECTED);
    }
    bool isConnected() {
      CommPortState::Value state = this->mState->get();
      return (state == CommPortState::CONNECTED);
    }

    // Primitive functions
    virtual bool openConnection() {
      this->mState->set(CommPortState::OPENED);
      return true;
    }
    virtual void closeConnection() {
      this->mState->set(CommPortState::CLOSED);
    }

    // Listen
    void runListeningThread(CommPortListener* listener,
        std::string downloadFilePath);
    void stopListeningThread();

    // Send
    bool sendRawMessage(std::string messageData);
    bool sendRawMessage(std::string messageData, std::string filePath);

  protected:
    virtual bool acceptConnection() {
      this->mState->set(CommPortState::CONNECTED);
      return true;
    }

    static void* listeningLoop(void* data);

    int getListenedSocket() { return this->mListenedSocket; }
    int getSocket() { return this->mSocket; }
    void setListenedSocket(int listenedSocket) { this->mListenedSocket = listenedSocket; }
    void setSocket(int socket) { this->mSocket = socket; }
    std::string getPortName() { return this->mPortName; }

  protected:
    std::string mPortName;
    CommPortState* mState;
    int mListenedSocket;
    int mSocket;
    int mPresentHeaderId;
    std::string mDownloadFilePath;

    // Listening thread
    pthread_t mListeningThread;
    bool mIsListeningThreadOn;
    CommPortListener* mListener;
};

#define BLUETOOTH_UUID_LENGTH 16
class BluetoothCommPort : public CommPort {
  public:
    BluetoothCommPort(std::string portName,
        CommPortStateListener* stateListener,
        char* uuid)
      : CommPort(portName, stateListener), mSdpSession(NULL) {
      memcpy(this->mUUID, uuid, BLUETOOTH_UUID_LENGTH);
    }
    virtual bool openConnection();
    virtual void closeConnection();

  protected:
    virtual bool acceptConnection();
    
  private:
    sdp_session_t* registerBluetoothService(int bluetoothPort);
    int bindDynamically(int socket);

  protected:
    sdp_session_t* mSdpSession;
    char mUUID[BLUETOOTH_UUID_LENGTH];
};

class WifiDirectCommPort : public CommPort {
  public:
    WifiDirectCommPort(std::string portName,
        CommPortStateListener* stateListener,
        short tcpPortNum)
      : CommPort(portName, stateListener), mTcpPortNum(tcpPortNum) { }
    virtual bool openConnection();
    virtual void closeConnection();

  protected:
    virtual bool acceptConnection();
    
  protected:
    short mTcpPortNum;
};

#endif /* !defined(__COMM_PORT_H__) */
