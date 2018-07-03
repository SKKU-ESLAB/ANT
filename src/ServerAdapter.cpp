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

#include <Communicator.h>
#include <DebugLog.h>

#include <string.h>
#include <arpa/inet.h>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace cm;

bool ServerAdapter::connect(ConnectCallback callback) {
  if(this->get_state() != ServerAdapterState::kDisconnected) {
    LOG_ERR("It is already connected or connection/disconnection is in progress.");
    return false;
  }

  this->mConnectCallback = callback;
  std::thread(std::bind(&ServerAdapter::connect_thread, this)).detach();
  return true;
}

bool ServerAdapter::disconnect(DisconnectCallback callback) {
  if(this->get_state() != ServerAdapterState::kDisconnected) {
    LOG_ERR("It is already disconnected or connection/disconnection is in progress.");
    return false;
  }

  this->mDisconnectCallback = callback;
  std::thread(std::bind(&ServerAdapter::disconnect_thread, this)).detach();
}

void ServerAdapter::on_fail_connect_thread(void) {
  this->set_state(ServerAdapterState::kDisconnected);
  if(this->mConnectCallback != NULL) {
    this->mConnectCallback(false);
  }
  this->mConnectCallback = NULL;
}

void ServerAdapter::connect_thread(void) {
  LOG_DEBUG("%s's Connect thread spawned(tid: %d)",
      this->get_name(), (unsigned int)syscall(224));

  this->set_state(ServerAdapterState::kConnecting);

  // Turn on device
  if(this->mDevice == NULL) {
    this->on_fail_connect_thread();
    return;
  }
  DeviceState deviceState = this->mDevice->get_state();
  if(deviceState != DeviceState::kOn) {
    bool res = this->mDevice->hold_and_turn_on();

    deviceState = this->mDevice->get_state();
    if(!res || deviceState != DeviceState::kOn) {
      LOG_ERR("Cannot connect the server adapter - turn-on fail: %s", this->get_name());

      this->on_fail_connect_thread();
      return;
    }
  }

  // Allow client's connection
  if(this->mP2PServer == NULL) {
    return;
  }
  P2PServerState p2pServerState = this->mP2PServer->get_state();
  if(p2pServerState != P2PServerState::kAllowed) {
    bool res = this->mP2PServer->allow();

    p2pServerState = this->mP2PServer->get_state();
    if(!res || p2pServerState != P2PServerState::kAllowed) {
      LOG_ERR("Cannot connect the server adapter - allow fail: %s", this->get_name());
      this->mDevice->release_and_turn_off();

      this->on_fail_connect_thread();
      return;
    }
  }

  // Open server socket
  if(this->mServerSocket == NULL) {
    this->mP2PServer->disallow();
    this->mDevice->release_and_turn_off();

    this->on_fail_connect_thread();
    return;
  }
  ServerSocketState socketState = this->mServerSocket->get_state();
  if(socketState != ServerSocketState::kOpened) {
    bool res = this->mServerSocket->open();

    socketState = this->mServerSocket->get_state();
    if(!res || socketState != ServerSocketState::kOpened) {
      LOG_ERR("Cannot connect the server adapter - socket open fail: %s", this->get_name());
      this->mP2PServer->disallow();
      this->mDevice->release_and_turn_off();

      this->on_fail_connect_thread();
      return;
    }
  }

  // Run sender & receiver threads
  if(this->mSenderThreadEnabled && this->mSenderThread == NULL) {
    this->mSenderThreadOn = true;
    this->mSenderThread = new std::thread(std::bind(&ServerAdapter::sender_thread, this));
  }
  if(this->mReceiverThreadEnabled && this->mReceiverThread == NULL) {
    this->mReceiverThreadOn = true;
    this->mReceiverThread = new std::thread(std::bind(&ServerAdapter::receiver_thread, this));
  }

  this->set_state(ServerAdapterState::kConnected);
  if(this->mConnectCallback != NULL) {
    this->mConnectCallback(true);
  }
  this->mConnectCallback = NULL;
  return;
}

void ServerAdapter::on_fail_disconnect_thread(void) {
  this->set_state(ServerAdapterState::kConnected);
  if(this->mDisconnectCallback != NULL) {
    this->mDisconnectCallback(false);
  }
  this->mDisconnectCallback = NULL;
  return;
}

void ServerAdapter::disconnect_thread(void) {
  LOG_DEBUG("%s's Disconnect thread spawned(tid: %d)",
      this->get_name(), (unsigned int)syscall(224));

  this->set_state(ServerAdapterState::kDisconnecting);

  // Finish sender & receiver threads
  if(this->mSenderThread != NULL) {
    this->mSenderThreadOn = false;
  }
  if(this->mReceiverThread != NULL) {
    this->mReceiverThreadOn = false;
  }

  // Close server socket
  if(this->mServerSocket == NULL) {
    this->on_fail_disconnect_thread();
    return;
  }
  ServerSocketState socketState = this->mServerSocket->get_state();
  if(socketState != ServerSocketState::kClosed) {
    bool res = this->mServerSocket->close();

    socketState = this->mServerSocket->get_state();
    if(!res || socketState != ServerSocketState::kClosed) {
      LOG_ERR("Cannot disconnect the server adapter - socket close fail: %s", this->get_name());

      this->on_fail_disconnect_thread();
      return;
    }
  }

  // Turn off device
  if(this->mDevice == NULL) {
    this->on_fail_disconnect_thread();
    return;
  }
  DeviceState deviceState = this->mDevice->get_state();
  if(deviceState != DeviceState::kOff) {
    bool res = this->mDevice->release_and_turn_off();

    deviceState = this->mDevice->get_state();
    if(!res || deviceState != DeviceState::kOff) {
      LOG_ERR("Cannot disconnect the server adapter - turn-off fail: %s", this->get_name());

      this->on_fail_disconnect_thread();
      return;
    }
  }

  // Send control message to turn off the working data adapter
//  Communicator *communicator = Communicator::get_instance();
//  unsigned char buf[512];
//  buf[0] = kCtrlReqDecr;
//  uint16_t ndev_id = htons(communicator->decreasing_adapter_id);
//  memcpy(buf+1, &ndev_id, 2);
//  communicator->send_control_data((const void *)buf, 3);

  this->set_state(ServerAdapterState::kDisconnected);
  if(this->mDisconnectCallback != NULL) {
    this->mDisconnectCallback(true);
  }
  this->mDisconnectCallback = NULL;
  return;
}

int ServerAdapter::send(const void *buf, size_t len) {
  if(this->get_state() != ServerAdapterState::kDisconnected) {
    LOG_ERR("It is already disconnected or connection/disconnection is in progress.");
    return false;
  }

  if(this->mServerSocket == NULL) {
    return false;
  }

  int ret = this->mServerSocket->send(buf, len);
  if(ret > 0) {
    this->mSendDataSize.add((int)len);
  }
  return ret;
}

int ServerAdapter::receive(void *buf, size_t len) {
  if(this->get_state() != ServerAdapterState::kDisconnected) {
    LOG_ERR("It is already disconnected or connection/disconnection is in progress.");
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

  while(this->mSenderThreadOn) {
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

  LOG_DEBUG("%s's Sender thread ends(tid: %d)",
      this->get_name(), (unsigned int)syscall(224));
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
}

void ServerAdapter::receive_data_loop(ServerAdapter* adapter) {
  while(adapter->mReceiverThreadOn) {
    SegmentManager *sm = SegmentManager::get_instance();
    Segment *segment_to_receive = sm->get_free_segment();
    void *buf = reinterpret_cast<void *>(segment_to_receive->data);
    int len = kSegSize + kSegHeaderSize;

    LOG_DEBUG("%s: Receiving...", adapter->get_name());
    int res = adapter->receive(buf, len);
    if (res < len) {
      LOG_WARN("Recving failed at %s (%s)",
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
}
