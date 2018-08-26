/* Copyright 2017-2018 All Rights Reserved.
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *  
 * [Contact]
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *
 * Licensed under the Apache License, Version 2.0(the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ServerAdapter.h>

#include <Core.h>
#include <DebugLog.h>

#include <string.h>
#include <arpa/inet.h>
#include <thread>
#include <mutex>

using namespace sc;

bool ServerAdapter::connect(ConnectCallback callback, bool is_send_connect_message) {
  if(this->get_state() != ServerAdapterState::kDisconnected) {
    LOG_ERR("It is already connected or connection/disconnection is in progress.");
    return false;
  }

  if(this->mConnectThread != NULL) {
    LOG_ERR("Connect thread not finished!");
    return false;
  }

  if(is_send_connect_message) {
    Core::get_instance()->send_request_connect(this->get_id());
  }

  this->mConnectCallback = callback;
  this->mConnectThread = new std::thread(std::bind(&ServerAdapter::connect_thread, this)); 
  this->mConnectThread->detach();
  return true;
}

bool ServerAdapter::disconnect(DisconnectCallback callback) {
  ServerAdapterState state = this->get_state();
  if(state == ServerAdapterState::kDisconnected || state == ServerAdapterState::kDisconnecting) {
    LOG_ERR("It is already disconnected or connection/disconnection is in progress.");
    return false;
  }

  if(this->mDisconnectThread != NULL) {
    LOG_ERR("Disconnect thread not finished!");
    return false;
  }

  this->mDisconnectCallback = callback;
  this->mDisconnectThread = new std::thread(std::bind(&ServerAdapter::disconnect_thread, this));
  this->mDisconnectThread->detach();
  return true;
}

void ServerAdapter::connect_thread(void) {
  LOG_DEBUG("%s's Connect thread spawned(tid: %d)",
      this->get_name(), (unsigned int)syscall(224));

  this->set_state(ServerAdapterState::kConnecting);

  bool res = this->__connect_thread();

  if(res) {
    this->set_state(ServerAdapterState::kActive);
    if(this->mConnectCallback != NULL) {
      this->mConnectCallback(true);
      this->mConnectCallback = NULL;
    }
    LOG_DEBUG("%s's Connect thread finished successfully(tid: %d)",
        this->get_name(), (unsigned int)syscall(224));
  } else {
    this->set_state(ServerAdapterState::kDisconnected);
    if(this->mConnectCallback != NULL) {
      this->mConnectCallback(false);
      this->mConnectCallback = NULL;
    }
    LOG_DEBUG("%s's Connect thread failed(tid: %d)",
        this->get_name(), (unsigned int)syscall(224));
  }
  this->mConnectThread = NULL;
}

bool ServerAdapter::__connect_thread(void) {
  if(this->mDevice == NULL || this->mP2PServer == NULL || this->mServerSocket == NULL) {
    return false;
  }

  // Turn on device
  DeviceState deviceState = this->mDevice->get_state();
  bool res = this->mDevice->hold_and_turn_on();

  deviceState = this->mDevice->get_state();
  if(!res || deviceState != DeviceState::kOn) {
    LOG_ERR("Cannot connect the server adapter - turn-on fail: %s", this->get_name());
    return false;
  }

  // Allow client's connection
  P2PServerState p2pServerState = this->mP2PServer->get_state();
  if(p2pServerState != P2PServerState::kAllowed) {
    bool res = this->mP2PServer->hold_and_allow_discover();

    p2pServerState = this->mP2PServer->get_state();
    if(!res || p2pServerState != P2PServerState::kAllowed) {
      LOG_ERR("Cannot connect the server adapter - allow discover fail: %s", this->get_name());
      this->mDevice->release_and_turn_off();
      return false;
    }
  }

  // Open server socket
  ServerSocketState socketState = this->mServerSocket->get_state();
  if(socketState != ServerSocketState::kOpened) {
    bool res = this->mServerSocket->open();

    socketState = this->mServerSocket->get_state();
    if(!res || socketState != ServerSocketState::kOpened) {
      LOG_ERR("Cannot connect the server adapter - socket open fail: %s", this->get_name());
      this->mP2PServer->release_and_disallow_discover();
      this->mDevice->release_and_turn_off();
      return false;
    }
  }

  // Run sender & receiver threads
  if(this->mSenderThreadEnabled && this->mSenderThread == NULL) {
    this->mSenderThread = new std::thread(std::bind(&ServerAdapter::sender_thread, this));
    this->mSenderThread->detach();
  }
  if(this->mReceiverThreadEnabled && this->mReceiverThread == NULL) {
    this->mReceiverThread = new std::thread(std::bind(&ServerAdapter::receiver_thread, this));
    this->mReceiverThread->detach();
  }

  return true;
}

void ServerAdapter::disconnect_thread(void) {
  LOG_DEBUG("%s's Disconnect thread spawned(tid: %d)",
      this->get_name(), (unsigned int)syscall(224));

  ServerAdapterState oldState = this->get_state();
  this->set_state(ServerAdapterState::kDisconnecting);

  bool res = this->__disconnect_thread();

  if(res) {
    this->set_state(ServerAdapterState::kDisconnected);
    if(this->mDisconnectCallback != NULL) {
      this->mDisconnectCallback(true);
      this->mDisconnectCallback = NULL;
    }
    LOG_DEBUG("%s's Disconnect thread finished successfully(tid: %d)",
      this->get_name(), (unsigned int)syscall(224));
  } else {
    this->set_state(oldState);
    LOG_DEBUG("%s's Disconnect thread failed(tid: %d)",
          this->get_name(), (unsigned int)syscall(224));
    if(this->mDisconnectCallback != NULL) {
      this->mDisconnectCallback(false);
      this->mDisconnectCallback = NULL;
    }
  }
  this->mDisconnectThread = NULL;
}

bool ServerAdapter::__disconnect_thread(void) {
  // Finish sender & receiver threads
  if(this->mSenderThread != NULL) {
    this->mSenderLoopOn = false;
  }
  if(this->mReceiverThread != NULL) {
    this->mReceiverLoopOn = false;
  }

  // Close server socket
  if(this->mServerSocket == NULL) {
    LOG_ERR("Cannot find server socket: %s", this->get_name());
    return false;
  }

  ServerSocketState socketState = this->mServerSocket->get_state();
  if(socketState != ServerSocketState::kClosed) {
    bool res = this->mServerSocket->close();

    socketState = this->mServerSocket->get_state();
    if(!res || socketState != ServerSocketState::kClosed) {
      LOG_ERR("Cannot disconnect the server adapter - socket close fail: %s", this->get_name());
      return false;
    }
  }

  // Disallow scan P2P Server
  if(this->mP2PServer == NULL) {
    LOG_ERR("Cannot find P2P server: %s", this->get_name());
    return false;
  }

  P2PServerState p2pServerState = this->mP2PServer->get_state();
  if(p2pServerState != P2PServerState::kAllowed) {
    bool res = this->mP2PServer->release_and_disallow_discover();

    p2pServerState = this->mP2PServer->get_state();
    if(!res || p2pServerState != P2PServerState::kDisallowed) {
      LOG_ERR("Cannot disconnect the server adapter - disallow discover fail: %s", this->get_name());
      return false;
    }
  }

  // Turn off device
  if(this->mDevice == NULL) {
    LOG_ERR("Cannot find device: %s", this->get_name());
    return false;
  }
  DeviceState deviceState = this->mDevice->get_state();
  if(deviceState != DeviceState::kOff) {
    bool res = this->mDevice->release_and_turn_off();

    deviceState = this->mDevice->get_state();
    if(!res || deviceState != DeviceState::kOff) {
      LOG_ERR("Cannot disconnect the server adapter - turn-off fail: %s", this->get_name());
      return false;
    }
  }
  return true;
}

int ServerAdapter::send(const void *buf, size_t len) {
  if(this->get_state() != ServerAdapterState::kActive) {
    LOG_ERR("It is not in active state.");
    return -1;
  }

  if(this->mServerSocket == NULL) {
    return -2;
  }

  int ret = this->mServerSocket->send(buf, len);
  if(ret > 0) {
    this->mSendDataSize.add((int)len);
  }
  return ret;
}

int ServerAdapter::receive(void *buf, size_t len) {
  if(this->get_state() != ServerAdapterState::kActive) {
    LOG_ERR("It is not int active state.");
    return false;
  }

  if(this->mServerSocket == NULL) {
    return false;
  }

  int ret = this->mServerSocket->receive(buf, len);
  if(ret > 0) {
    this->mReceiveDataSize.add((int)len);
  }
  return ret;
}

void ServerAdapter::sender_thread(void) {
  LOG_DEBUG("%s's Sender thread spawned(tid: %d)",
      this->get_name(), (unsigned int)syscall(224));

  this->__sender_thread();

  this->disconnect(NULL);
  LOG_DEBUG("%s's Sender thread ends(tid: %d)",
      this->get_name(), (unsigned int)syscall(224));
  this->mSenderLoopOn = false;
  this->mSenderThread = NULL;
}

void ServerAdapter::__sender_thread(void) {
  this->mSenderLoopOn = true;
  while(this->mSenderLoopOn) {
    SegmentManager *sm = SegmentManager::get_instance();
    Segment *segment_to_send = NULL;

    // At first, dequeue a segment from failed sending queue
    segment_to_send = sm->get_failed_sending();

    // If there is no failed segment, dequeue from send queue
    if (likely(segment_to_send == NULL)){
      segment_to_send = sm->dequeue(kSegSend);
    }

    int len = kSegHeaderSize + kSegSize;
    const void *data = segment_to_send->data;

    int res = this->send(data, len); 
    if (res < 0) {
      LOG_WARN("Sending failed at %s (%d; %s)",
          this->get_name(), errno, strerror(errno));

      sm->failed_sending(segment_to_send);
      break;
    }

    sm->free_segment(segment_to_send);
  }
}

void ServerAdapter::receiver_thread(void) {
  if(this->mReceiveLoop == NULL) {
    LOG_ERR("No receive loop specified!");
    return;
  }

  LOG_DEBUG("%s's Receiver thread spawned(tid: %d)",
      this->get_name(), (unsigned int)syscall(224));

  this->mReceiveLoop(this);

  LOG_DEBUG("%s's Receiver thread ends(tid: %d)",
      this->get_name(), (unsigned int)syscall(224));
  this->mReceiverLoopOn = false;
  this->mReceiverThread = NULL;
}

void ServerAdapter::receive_data_loop(ServerAdapter* adapter) {
  adapter->mReceiverLoopOn = true;
  while(adapter->mReceiverLoopOn) {
    SegmentManager *sm = SegmentManager::get_instance();
    Segment *segment_to_receive = sm->get_free_segment();
    void *buf = reinterpret_cast<void *>(segment_to_receive->data);
    int len = kSegSize + kSegHeaderSize;

    LOG_DEBUG("%s: Receiving...", adapter->get_name());
    int res = adapter->receive(buf, len);
    if (res < len) {
      LOG_WARN("Receiving failed at %s (%s)",
          adapter->get_name(), strerror(errno));
      break;
    }

    uint32_t net_seq_no, net_flag_len;
    memcpy(&net_seq_no, buf, sizeof(uint32_t));
    memcpy(&net_flag_len,
        (reinterpret_cast<uint8_t *>(buf)+4), sizeof(uint32_t));
    segment_to_receive->seq_no = ntohl(net_seq_no);
    segment_to_receive->flag_len = ntohl(net_flag_len);

    sm->enqueue(kSegRecv, segment_to_receive);
    segment_to_receive = sm->get_free_segment();

    LOG_DEBUG("%s: Received: %d",
        adapter->get_name(), res);
  }

  adapter->disconnect(NULL);

  LOG_DEBUG("%s's Receiver thread ends(tid: %d)",
      adapter->get_name(), (unsigned int)syscall(224));
}
