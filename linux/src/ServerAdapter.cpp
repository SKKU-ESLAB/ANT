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

#include <ExpConfig.h>
#include <ServerAdapter.h>
#include <NetworkSwitcher.h>

#include <Core.h>
#include <DebugLog.h>

#include <arpa/inet.h>
#include <mutex>
#include <string.h>
#include <thread>

#if EXP_MEASURE_INTERVAL_SENDER != 0
#include <sys/time.h>
#endif

using namespace sc;

void ServerAdapter::connect(ConnectCallback callback, bool is_send_request) {
  if (this->get_state() != ServerAdapterState::kDisconnected) {
    LOG_ERR(
        "It is already connected or connection/disconnection is in progress.");
    callback(false);
    return;
  }

  if (this->mConnectThread != NULL) {
    LOG_ERR("Connect thread not finished!");
    callback(false);
    return;
  }

  // Send request
  if (is_send_request) {
    Core::get_instance()->send_request_connect(this->get_id());
  }

  // Connect
  this->mConnectCallback = callback;
  this->mConnectThread =
      new std::thread(std::bind(&ServerAdapter::connect_thread, this));
  this->mConnectThread->detach();
}

void ServerAdapter::disconnect(DisconnectCallback callback,
                               bool is_send_request) {
  ServerAdapterState state = this->get_state();
  if (state == ServerAdapterState::kDisconnected ||
      state == ServerAdapterState::kDisconnecting) {
    LOG_ERR("It is already disconnected or connection/disconnection is in "
            "progress.");
    callback(false);
    return;
  }

  if (this->mDisconnectThread != NULL) {
    LOG_ERR("Disconnect thread not finished!");
    callback(false);
    return;
  }

  // Send request
  if (is_send_request) {
    this->start_disconnecting_on_purpose();
    Core::get_instance()->send_request_disconnect(this->get_id());
  }

  // Disconnect
  this->mDisconnectCallback = callback;
  this->mDisconnectThread =
      new std::thread(std::bind(&ServerAdapter::disconnect_thread, this));
  this->mDisconnectThread->detach();
}

void ServerAdapter::connect_thread(void) {
  LOG_DEBUG("%s's Connect thread spawned(tid: %d)", this->get_name(),
            (unsigned int)syscall(224));

  this->set_state(ServerAdapterState::kConnecting);

  bool res = this->__connect_thread();

  if (res) {
    this->set_state(ServerAdapterState::kActive);
    if (this->mConnectCallback != NULL) {
      this->mConnectCallback(true);
      this->mConnectCallback = NULL;
    }
    LOG_DEBUG("%s's Connect thread finished successfully(tid: %d)",
              this->get_name(), (unsigned int)syscall(224));
  } else {
    this->set_state(ServerAdapterState::kDisconnected);
    if (this->mConnectCallback != NULL) {
      this->mConnectCallback(false);
      this->mConnectCallback = NULL;
    }
    LOG_DEBUG("%s's Connect thread failed(tid: %d)", this->get_name(),
              (unsigned int)syscall(224));
  }
  this->mConnectThread = NULL;
}

bool ServerAdapter::__connect_thread(void) {
  if (this->mDevice == NULL || this->mP2PServer == NULL ||
      this->mServerSocket == NULL) {
    return false;
  }

  // Turn on device
  {
    DeviceState deviceState = this->mDevice->get_state();
    bool res = this->mDevice->hold_and_turn_on();

    deviceState = this->mDevice->get_state();
    if (!res || deviceState != DeviceState::kOn) {
      LOG_ERR("Cannot connect the server adapter - turn-on fail: %s",
              this->get_name());
      return false;
    }
  }

  // Allow client's connection
  {
    bool res = this->mP2PServer->hold_and_allow_discover();

    P2PServerState p2pServerState = this->mP2PServer->get_state();
    if (!res || p2pServerState != P2PServerState::kAllowed) {
      LOG_ERR("Cannot connect the server adapter - allow discover fail: %s",
              this->get_name());
      this->mDevice->release_and_turn_off();
      return false;
    }
  }

  // Open server socket
  {
    ServerSocketState socketState = this->mServerSocket->get_state();
    if (socketState != ServerSocketState::kOpened) {
      bool res = this->mServerSocket->open();

      socketState = this->mServerSocket->get_state();
      if (!res || socketState != ServerSocketState::kOpened) {
        LOG_ERR("Cannot connect the server adapter - socket open fail: %s",
                this->get_name());
        this->mP2PServer->release_and_disallow_discover();
        this->mDevice->release_and_turn_off();
        return false;
      }
    }
  }

  // Run sender & receiver threads
  if (this->mSenderThreadEnabled && this->mSenderThread == NULL) {
    this->mSenderThread =
        new std::thread(std::bind(&ServerAdapter::sender_thread, this));
    this->mSenderThread->detach();
  }
  if (this->mReceiverThreadEnabled && this->mReceiverThread == NULL) {
    this->mReceiverThread =
        new std::thread(std::bind(&ServerAdapter::receiver_thread, this));
    this->mReceiverThread->detach();
  }

  return true;
}

void ServerAdapter::disconnect_thread(void) {
  LOG_DEBUG("%s's Disconnect thread spawned(tid: %d)", this->get_name(),
            (unsigned int)syscall(224));

  ServerAdapterState oldState = this->get_state();
  this->set_state(ServerAdapterState::kDisconnecting);

  if (this->is_disconnecting_on_purpose() &&
      !this->is_disconnecting_on_purpose_peer()) {
    this->wait_for_disconnecting_on_purpose_peer();
  }

  bool res = this->__disconnect_thread();

  this->finish_disconnecting_on_purpose();
  
  if (res) {
    this->set_state(ServerAdapterState::kDisconnected);
    if (this->mDisconnectCallback != NULL) {
      this->mDisconnectCallback(true);
      this->mDisconnectCallback = NULL;
    }
    LOG_DEBUG("%s's Disconnect thread finished successfully(tid: %d)",
              this->get_name(), (unsigned int)syscall(224));
  } else {
    this->set_state(oldState);
    LOG_DEBUG("%s's Disconnect thread failed(tid: %d)", this->get_name(),
              (unsigned int)syscall(224));
    if (this->mDisconnectCallback != NULL) {
      this->mDisconnectCallback(false);
      this->mDisconnectCallback = NULL;
    }
  }
  this->mDisconnectThread = NULL;
}

bool ServerAdapter::__disconnect_thread(void) {
  // Finish sender & receiver threads
  if (this->mSenderThread != NULL) {
    // If this adapter is already sleeping, wake up and finish the sender thread
    this->mSenderLoopOn = false;
    if (this->get_state() == ServerAdapterState::kSleeping) {
      this->wake_up(NULL, false);
    }
  }
  if (this->mReceiverThread != NULL) {
    this->mReceiverLoopOn = false;
  }

  // Close server socket
  if (this->mServerSocket == NULL) {
    LOG_ERR("Cannot find server socket: %s", this->get_name());
    return false;
  }

  {
    ServerSocketState socketState = this->mServerSocket->get_state();
    if (socketState != ServerSocketState::kClosed) {
      bool res = this->mServerSocket->close();

      socketState = this->mServerSocket->get_state();
      if (!res || socketState != ServerSocketState::kClosed) {
        LOG_ERR("Cannot disconnect the server adapter - socket close fail: %s",
                this->get_name());
        return false;
      }
    }
  }

  // Disallow scan P2P Server
  if (this->mP2PServer == NULL) {
    LOG_ERR("Cannot find P2P server: %s", this->get_name());
    return false;
  }

  {
    bool res = this->mP2PServer->release_and_disallow_discover();

    P2PServerState p2pServerState = this->mP2PServer->get_state();
    if (!res) {
      LOG_ERR(
          "Cannot disconnect the server adapter - disallow discover fail: %s",
          this->get_name());
      return false;
    }
  }

  // Turn off device
  if (this->mDevice == NULL) {
    LOG_ERR("Cannot find device: %s", this->get_name());
    return false;
  }

  {
    bool res = this->mDevice->release_and_turn_off();

    DeviceState deviceState = this->mDevice->get_state();
    if (!res) {
      LOG_ERR("Cannot disconnect the server adapter - turn-off fail: %s",
              this->get_name());
      return false;
    }
  }
  return true;
}

int ServerAdapter::send(const void *buf, size_t len) {
  if (this->get_state() != ServerAdapterState::kActive) {
    LOG_ERR("It is not in active state.");
    return -1;
  }

  if (this->mServerSocket == NULL) {
    return -2;
  }

  // Send
  int ret = this->mServerSocket->send(buf, len);

  // Update statistics
  if (ret > 0) {
    this->mSendDataSize.add((int)len);
  }
  return ret;
}

int ServerAdapter::receive(void *buf, size_t len) {
  ServerAdapterState state = this->get_state();
  if (state != ServerAdapterState::kActive &&
      state != ServerAdapterState::kGoingSleeping &&
      state != ServerAdapterState::kSleeping &&
      state != ServerAdapterState::kWakingUp) {
    LOG_ERR("It is not in active state.");
    return false;
  }

  if (this->mServerSocket == NULL) {
    return false;
  }

  // Receive
  int ret = this->mServerSocket->receive(buf, len);

  // Update statistics
  if (ret > 0) {
    this->mReceiveDataSize.add((int)len);
  }
  return ret;
}

void ServerAdapter::sender_thread(void) {
  LOG_DEBUG("%s's Sender thread spawned(tid: %d)", this->get_name(),
            (unsigned int)syscall(224));

  this->mSenderLoopOn = true;
  this->data_adapter_send_loop();

  LOG_DEBUG("%s's Sender thread ends(tid: %d)", this->get_name(),
            (unsigned int)syscall(224));
  this->mSenderLoopOn = false;
  {
    std::unique_lock<std::mutex> lck(this->mSenderSuspendedMutex);
    this->mSenderSuspended = false;
  }
  this->mSenderThread = NULL;

  // Reconnect the adapter
  NetworkSwitcher::get_instance()->reconnect_adapter(this, true);
}

void ServerAdapter::data_adapter_send_loop(void) {
  {
    std::unique_lock<std::mutex> lck(this->mSenderSuspendedMutex);
    this->mSenderSuspended = false;
  }
  while (this->mSenderLoopOn) {
#if EXP_MEASURE_INTERVAL_SENDER != 0
    struct timeval times[5];
    gettimeofday(&times[0], NULL);
#endif

    SegmentManager *sm = SegmentManager::get_instance();
    Segment *segment_to_send = NULL;

    // If this sender is set to be suspended, wait until it wakes up
    {
      bool sender_suspended;
      do {
        {
          std::unique_lock<std::mutex> lck2a(this->mSenderSuspendedMutex);
          sender_suspended = this->mSenderSuspended;
          if (!sender_suspended) {
            break;
          }
        }
        LOG_VERB("Sender thread suspended: %s", this->get_name());
        this->set_state(ServerAdapterState::kSleeping);
        {
          std::unique_lock<std::mutex> lck2b(this->mSenderSuspendedMutex);
          this->mSenderSuspendedCond.wait(lck2b);
        }
        this->set_state(ServerAdapterState::kActive);
      } while (true);
    }

    // At first, dequeue a segment from failed sending queue
    segment_to_send = sm->get_failed_sending();

#if EXP_MEASURE_INTERVAL_SENDER != 0
    gettimeofday(&times[1], NULL);
#endif

    // If there is no failed segment, dequeue from send queue
    if (likely(segment_to_send == NULL)) {
      segment_to_send = sm->dequeue(kSegSend);
    }

#if EXP_MEASURE_INTERVAL_SENDER != 0
    gettimeofday(&times[2], NULL);
#endif

    // If it is suspended, push the segment to the send-fail queue
    {
      bool sender_suspended;
      {
        std::unique_lock<std::mutex> lck3(this->mSenderSuspendedMutex);
        sender_suspended = this->mSenderSuspended;
      }
      if (sender_suspended) {
        LOG_VERB("Sending segment is pushed to failed queue at %s (suspended)",
                 this->get_name());

        sm->failed_sending(segment_to_send);
        continue;
      }
    }

#if EXP_MEASURE_INTERVAL_SENDER != 0
    gettimeofday(&times[3], NULL);
#endif

    int len = kSegHeaderSize + kSegSize;
    const void *data = segment_to_send->data;

    int res = this->send(data, len);

#if EXP_MEASURE_INTERVAL_SENDER != 0
    gettimeofday(&times[4], NULL);
#endif

    if (errno == EAGAIN) {
      LOG_VERB("Sending %dB: Kernel I/O buffer is full at %s", len,
               this->get_name());
      sm->failed_sending(segment_to_send);
      continue;
    } else if (res < 0) {
      LOG_WARN("Sending %dB failed at %s (%d; %s)", len, this->get_name(),
               errno, strerror(errno));
      sm->failed_sending(segment_to_send);
      break;
    }

    sm->free_segment(segment_to_send);

#if EXP_MEASURE_INTERVAL_SENDER != 0
    gettimeofday(&times[5], NULL);

    for (int i = 0; i < 4; i++) {
      this->mIntervals[i] += (times[i + 1].tv_sec - times[i].tv_sec) * 1000 +
                             (times[i + 1].tv_usec - times[i].tv_usec) / 1000;
    }
    this->mSendCount++;
    if (this->mSendCount % 500 == 0) {
      LOG_DEBUG("Send Time: %d / %d / %d / %d", this->mIntervals[0],
                this->mIntervals[1], this->mIntervals[2], this->mIntervals[3]);
      for (int i = 0; i < 4; i++) {
        this->mIntervals[i] = 0;
      }
    }
#endif
  }
}

void ServerAdapter::receiver_thread(void) {
  if (this->mReceiveLoop == NULL) {
    LOG_ERR("No receive loop specified!");
    return;
  }

  LOG_DEBUG("%s: Receiver thread spawned(tid: %d)", this->get_name(),
            (unsigned int)syscall(224));

  this->mReceiverLoopOn = true;
  this->mReceiveLoop(this);

  LOG_DEBUG("%s: Receiver thread ends(tid: %d)", this->get_name(),
            (unsigned int)syscall(224));
  this->mReceiverLoopOn = false;
  this->mReceiverThread = NULL;

  // Reconnect the adapter
  NetworkSwitcher::get_instance()->reconnect_adapter(this, true);
}

void ServerAdapter::data_adapter_receive_loop(ServerAdapter *adapter) {
  while (adapter->is_receiver_loop_on()) {
    SegmentManager *sm = SegmentManager::get_instance();
    Segment *segment_to_receive = sm->get_free_segment();
    void *buf = reinterpret_cast<void *>(segment_to_receive->data);
    int len = kSegSize + kSegHeaderSize;

#if VERBOSE_SERVER_ADAPTER_RECEIVING != 0
    LOG_DEBUG("%s: Receiving...", adapter->get_name());
#endif
    int res = adapter->receive(buf, len);
    if (errno == EAGAIN) {
      LOG_WARN("Kernel I/O buffer is full at %s", adapter->get_name());
      continue;
    } else if (res < len) {
      LOG_WARN("Receiving failed at %s (%s)", adapter->get_name(),
               strerror(errno));
      break;
    }

    uint32_t net_seq_no, net_flag_len;
    memcpy(&net_seq_no, buf, sizeof(uint32_t));
    memcpy(&net_flag_len, (reinterpret_cast<uint8_t *>(buf) + 4),
           sizeof(uint32_t));
    segment_to_receive->seq_no = ntohl(net_seq_no);
    segment_to_receive->flag_len = ntohl(net_flag_len);

    sm->enqueue(kSegRecv, segment_to_receive);
    segment_to_receive = sm->get_free_segment();

#if VERBOSE_SERVER_ADAPTER_RECEIVING != 0
    LOG_DEBUG("%s: Received: %d", adapter->get_name(), res);
#endif
  }
}

void ServerAdapter::sleep(DisconnectCallback callback, bool is_send_request) {
  ServerAdapterState state = this->get_state();
  if (state != ServerAdapterState::kActive) {
    LOG_ERR("Failed to sleep: %s (state: %d)", this->get_name(), state);
    if (callback != NULL)
      callback(false);
    return;
  }

  {
    std::unique_lock<std::mutex> lck(this->mSenderSuspendedMutex);
    if (this->mSenderSuspended) {
      LOG_ERR("Sender has already been suspended!: %s", this->get_name());
      if (callback != NULL)
        callback(false);
    } else {
      // Send Request
      if (is_send_request) {
        Core::get_instance()->send_request_sleep(this->get_id());
      }

      // Sleep
      this->set_state(ServerAdapterState::kGoingSleeping);
      this->mSenderSuspended = true;
      if (callback != NULL)
        callback(true);
    }
  }
}

void ServerAdapter::wake_up(DisconnectCallback callback, bool is_send_request) {
  ServerAdapterState state = this->get_state();
  if (state != ServerAdapterState::kSleeping) {
    LOG_ERR("Failed to wake up: %s (state: %d)", this->get_name(), state);
    if (callback != NULL)
      callback(false);
    return;
  }

  bool sender_suspended;
  {
    std::unique_lock<std::mutex> lck(this->mSenderSuspendedMutex);
    sender_suspended = this->mSenderSuspended;
  }
  if (sender_suspended) {
    // Send Request
    if (is_send_request) {
      Core::get_instance()->send_request_wake_up(this->get_id());
    }

    // Wake up
    this->set_state(ServerAdapterState::kWakingUp);
    {
      std::unique_lock<std::mutex> lck2(this->mSenderSuspendedMutex);
      this->mSenderSuspended = false;
    }
    this->mSenderSuspendedCond.notify_all();
    if (callback != NULL)
      callback(true);
  } else {
    LOG_ERR("Sender has not been suspended!: %s", this->get_name());
    if (callback != NULL)
      callback(false);
  }
}

void ServerAdapter::connect_or_wake_up(ConnectCallback callback,
                                       bool is_send_request) {
  ServerAdapterState state = this->get_state();
  if (state == ServerAdapterState::kDisconnected) {
    this->connect(callback, is_send_request);
  } else if (state == ServerAdapterState::kSleeping) {
    this->wake_up(callback, is_send_request);
  }
}
void ServerAdapter::disconnect_or_sleep(DisconnectCallback callback,
                                        bool is_send_request) {
  if (this->is_sleeping_allowed()) {
    this->sleep(callback, is_send_request);
  } else {
    this->disconnect(callback, is_send_request);
  }
}