/* Copyright 2017-2018 All Rights Reserved.
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *  Eunsoo Park (esevan.park@gmail.com)
 *  Injung Hwang (sinban04@gmail.com)
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

#include "../inc/ServerAdapter.h"

#include "../inc/Core.h"
#include "../inc/NetworkSwitcher.h"

#include "../../common/inc/DebugLog.h"

#include "../../configs/ExpConfig.h"

#include <arpa/inet.h>
#include <mutex>
#include <string.h>
#include <thread>

#ifdef EXP_MEASURE_INTERVAL_SENDER
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
    Core::singleton()->get_control_sender()->send_request_connect(
        this->get_id());
  }

  // Connect
  this->mConnectCallback = callback;
  this->mConnectThread =
      new std::thread(std::bind(&ServerAdapter::connect_thread, this));
  this->mConnectThread->detach();
}

void ServerAdapter::disconnect_on_command(DisconnectCallback callback) {
  // Check if the adapter is not sleeping
  ServerAdapterState state = this->get_state();
  if (state == ServerAdapterState::kGoingSleeping) {
    LOG_VERB("%s: Disconnect - waiting for sleeping...", this->get_name());
    while (state != ServerAdapterState::kSleeping) {
      ::sleep(1);
      state = this->get_state();
    }
  } else if (state != ServerAdapterState::kSleeping) {
    LOG_ERR("%s: Disconnect fail - not sleeping (state=%d)", this->get_name(),
            state);
    callback(false);
    return;
  }

  // Check if the disconnect thread has already been spawn
  if (this->mDisconnectThread != NULL) {
    LOG_ERR("Disconnect thread not finished!");
    callback(false);
    return;
  }

  // Set this disconnection is on purpose
  this->start_disconnecting_on_purpose();

  // Get my final seq_no
  SegmentManager *sm = SegmentManager::singleton();
  uint32_t my_final_seq_no_control = sm->get_last_seq_no_control();
  uint32_t my_final_seq_no_data = sm->get_last_seq_no_data();

  // Send disconnect request
  Core::singleton()->get_control_sender()->send_request_disconnect(
      this->get_id(), my_final_seq_no_control, my_final_seq_no_data);

  this->disconnect_internal(callback);
}

void ServerAdapter::disconnect_on_peer_command(
    DisconnectCallback callback, uint32_t peer_final_seq_no_control,
    uint32_t peer_final_seq_no_data) {
  // Check if the adapter is not sleeping
  ServerAdapterState state = this->get_state();
  if (state == ServerAdapterState::kGoingSleeping) {
    LOG_VERB("%s: Disconnect - waiting for sleeping...", this->get_name());
    while (state != ServerAdapterState::kSleeping) {
      ::sleep(1);
      state = this->get_state();
    }
  } else if (state != ServerAdapterState::kSleeping) {
    LOG_ERR("%s: Disconnect fail - not sleeping", this->get_name());
    callback(false);
    return;
  }

  // Check if the disconnect thread has already been spawn
  if (this->mDisconnectThread != NULL) {
    LOG_ERR("Disconnect thread not finished!");
    callback(false);
    return;
  }

  // Set this disconnection is on purpose
  this->start_disconnecting_on_purpose();

  // Start wait receiving
  SegmentManager *sm = SegmentManager::singleton();
  sm->wait_receiving(peer_final_seq_no_control, peer_final_seq_no_data);

  // Send disconnect ack
  Core::singleton()->get_control_sender()->send_request_disconnect_ack(
      this->get_id());

  this->disconnect_internal(callback);
}

void ServerAdapter::disconnect_on_failure(DisconnectCallback callback) {
  // Check if the adapter is already disconencted
  ServerAdapterState state = this->get_state();
  if (state == ServerAdapterState::kDisconnected ||
      state == ServerAdapterState::kDisconnecting) {
    LOG_ERR("%s: Disconnect fail - already disconnecting or disconnected",
            this->get_name());
    callback(false);
    return;
  }

  // Check if the disconnect thread has already been spawn
  if (this->mDisconnectThread != NULL) {
    LOG_ERR("Disconnect thread not finished!");
    callback(false);
    return;
  }

  this->disconnect_internal(callback);
}

void ServerAdapter::disconnect_internal(DisconnectCallback callback) {
  // Spawn disconnect thread
  this->mDisconnectCallback = callback;
  this->mDisconnectThread =
      new std::thread(std::bind(&ServerAdapter::disconnect_thread, this));
  this->mDisconnectThread->detach();
}

void ServerAdapter::connect_thread(void) {
  LOG_ADAPTER_THREAD_LAUNCH(this->get_name(), "Connect");

  this->set_state(ServerAdapterState::kConnecting);

  bool res = this->__connect_thread();

  if (res) {
    this->set_state(ServerAdapterState::kActive);
    LOG_ADAPTER_THREAD_FINISH(this->get_name(), "Connect");
    if (this->mConnectCallback != NULL) {
      this->mConnectCallback(true);
      this->mConnectCallback = NULL;
    }
  } else {
    this->set_state(ServerAdapterState::kDisconnected);
    LOG_ADAPTER_THREAD_FAIL(this->get_name(), "Connect");
    if (this->mConnectCallback != NULL) {
      this->mConnectCallback(false);
      this->mConnectCallback = NULL;
    }
  }
  this->mConnectThread = NULL;
}

bool ServerAdapter::__connect_thread(void) {
  if (this->mDevice == NULL || this->mP2PServer == NULL ||
      this->mServerSocket == NULL) {
    return false;
  }

  // Turn on device
  DeviceState deviceState = this->mDevice->get_state();
  if (deviceState != DeviceState::kOn &&
      deviceState != DeviceState::kTurningOn) {
    bool res = this->mDevice->turn_on();
    deviceState = this->mDevice->get_state();
    if (!res || deviceState != DeviceState::kOn) {
      LOG_ERR("%s: Cannot connect the server adapter - turn-on fail",
              this->get_name());
      return false;
    }
  } else {
    LOG_VERB("%s: Already turned on or turning on", this->get_name());
  }

  // Allow client's connection
  P2PServerState p2pServerState = this->mP2PServer->get_state();
  if (p2pServerState != P2PServerState::kAllowed) {
    bool res = this->mP2PServer->allow_discover();

    p2pServerState = this->mP2PServer->get_state();
    if (!res || p2pServerState != P2PServerState::kAllowed) {
      LOG_ERR("%s: Cannot connect the server adapter - allow discover fail",
              this->get_name());
      this->mDevice->turn_off();
      return false;
    }
  } else {
    LOG_VERB("%s: Already allowed discovery", this->get_name());
  }

  // Open server socket
  ServerSocketState socketState = this->mServerSocket->get_state();
  if (socketState != ServerSocketState::kOpened &&
      socketState != ServerSocketState::kOpening) {
    bool res = this->mServerSocket->open();

    socketState = this->mServerSocket->get_state();
    if (!res || socketState != ServerSocketState::kOpened) {
      LOG_ERR("%s: Cannot connect the server adapter - socket open fail",
              this->get_name());
      this->mP2PServer->disallow_discover();
      this->mDevice->turn_off();
      return false;
    }
  } else {
    LOG_VERB("%s: Already socket opened or opening", this->get_name());
  }

  // Run sender/receiver threads
  if (this->mSenderThread == NULL) {
    this->mSenderThread =
        new std::thread(std::bind(&ServerAdapter::sender_thread, this));
    this->mSenderThread->detach();
  }
  if (this->mReceiverThread == NULL) {
    this->mReceiverThread =
        new std::thread(std::bind(&ServerAdapter::receiver_thread, this));
    this->mReceiverThread->detach();
  }

  return true;
}

void ServerAdapter::disconnect_thread(void) {
  LOG_ADAPTER_THREAD_LAUNCH(this->get_name(), "Disconnect");

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
    LOG_ADAPTER_THREAD_FINISH(this->get_name(), "Disconnect");
    if (this->mDisconnectCallback != NULL) {
      this->mDisconnectCallback(true);
      this->mDisconnectCallback = NULL;
    }
  } else {
    this->set_state(oldState);
    LOG_ADAPTER_THREAD_FAIL(this->get_name(), "Disconnect");
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
    // If this adapter is already sleeping, wake up and finish the sender
    // thread
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

  ServerSocketState socketState = this->mServerSocket->get_state();
  if (socketState != ServerSocketState::kClosed &&
      socketState != ServerSocketState::kClosing) {
    socketState = this->mServerSocket->get_state();
    if (socketState != ServerSocketState::kClosed) {
      bool res = this->mServerSocket->close();

      socketState = this->mServerSocket->get_state();
      if (!res || socketState != ServerSocketState::kClosed) {
        LOG_ERR("Cannot disconnect the server adapter - socket close fail: %s",
                this->get_name());
        return false;
      }
    }
  } else {
    LOG_VERB("%s: Already socket closed or closing", this->get_name());
  }

  // Disallow scan P2P Server
  if (this->mP2PServer == NULL) {
    LOG_ERR("Cannot find P2P server: %s", this->get_name());
    return false;
  }

  P2PServerState p2pServerState = this->mP2PServer->get_state();
  if (p2pServerState != P2PServerState::kDisallowed) {
    bool res = this->mP2PServer->disallow_discover();

    p2pServerState = this->mP2PServer->get_state();
    if (!res) {
      LOG_ERR(
          "Cannot disconnect the server adapter - disallow discover fail: %s",
          this->get_name());
      return false;
    }
  } else {
    LOG_VERB("%s: Already disallowed discovery", this->get_name());
  }

  // Turn off device
  if (this->mDevice == NULL) {
    LOG_ERR("Cannot find device: %s", this->get_name());
    return false;
  }

  DeviceState deviceState = this->mDevice->get_state();
  if (deviceState != DeviceState::kOff && deviceState != DeviceState::kOn) {
    bool res = this->mDevice->turn_off();

    deviceState = this->mDevice->get_state();
    if (!res) {
      LOG_ERR("Cannot disconnect the server adapter - turn-off fail: %s",
              this->get_name());
      return false;
    }
  } else {
    LOG_VERB("%s: Already turned off or turning off", this->get_name());
  }

  // Wait for sender/receiver thread
  if (this->mSenderThread != NULL) {
    LOG_DEBUG("Waiting for sender thread: %s", this->get_name());
    std::unique_lock<std::mutex> senderLock(this->mWaitSenderThreadMutex);
    this->mWaitSenderThreadCond.wait(senderLock);
  }
  if (this->mReceiverThread != NULL) {
    LOG_DEBUG("Waiting for receiver thread: %s", this->get_name());
    std::unique_lock<std::mutex> receiverLock(this->mWaitReceiverThreadMutex);
    this->mWaitReceiverThreadCond.wait(receiverLock);
  }

  return true;
}

int ServerAdapter::send(const void *buf, size_t len) {
  if (this->get_state() != ServerAdapterState::kActive) {
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
  LOG_ADAPTER_THREAD_LAUNCH(this->get_name(), "Sender");

  this->mSenderLoopOn = true;
  this->sender_thread_loop();

  this->mSenderLoopOn = false;
  {
    std::unique_lock<std::mutex> lck(this->mSenderSuspendedMutex);
    this->mSenderSuspended = false;
  }
  this->mSenderThread = NULL;

  // If it is disconnecting on purpose, do not reconnect it.
  if (this->is_disconnecting_on_purpose()) {
    LOG_ADAPTER_THREAD_FINISH(this->get_name(), "Sender");
  } else {
    // Reconnect the adapter
    LOG_ADAPTER_THREAD_FAIL(this->get_name(), "Sender");
    NetworkSwitcher::singleton()->reconnect_adapter(this, true);
  }

  this->mWaitSenderThreadCond.notify_all();
}

void ServerAdapter::sender_thread_loop(void) {
  {
    std::unique_lock<std::mutex> lck(this->mSenderSuspendedMutex);
    this->mSenderSuspended = false;
  }
  while (this->mSenderLoopOn) {
#ifdef EXP_MEASURE_INTERVAL_SENDER
    struct timeval times[5];
    gettimeofday(&times[0], NULL);
#endif

    SegmentManager *sm = SegmentManager::singleton();
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

        // Suspended due to sleeping
        {
          std::unique_lock<std::mutex> lck2b(this->mSenderSuspendedMutex);
          this->mSenderSuspendedCond.wait(lck2b);
        }

        this->set_state(ServerAdapterState::kActive);
      } while (true);
    }

#ifdef EXP_MEASURE_INTERVAL_SENDER
    gettimeofday(&times[1], NULL);
#endif

    // Dequeue from a queue (one of the three queues)
    // Priority 1. Failed sending queue
    segment_to_send = sm->get_failed_sending();
#if defined(VERBOSE_DEQUEUE_SEND_CONTROL) || defined(VERBOSE_DEQUEUE_SEND_DATA)
    bool is_get_failed_segment = (segment_to_send != NULL);
#endif

    // Priority 2. Send control queue
    // Priority 3. Send data queue
    if (likely(segment_to_send == NULL)) {
      segment_to_send = sm->dequeue(kDeqSendControlData);
    }

    if (unlikely(segment_to_send == NULL)) {
      // Nothing to send.
      // SegmentManager::wake_up_dequeue_waiting() function may make this case
      continue;
    }

#if defined(VERBOSE_DEQUEUE_SEND_CONTROL) || defined(VERBOSE_DEQUEUE_SEND_DATA)
    bool is_control = ((segment_to_send->flag & kSegFlagControl) != 0);
#endif
#ifdef VERBOSE_DEQUEUE_SEND_CONTROL
    if (is_control) {
      LOG_DEBUG("%s: Send %s Segment (type=Ctrl, seqno=%d, len=%d)",
                this->get_name(), (is_get_failed_segment ? "Failed" : "Normal"),
                segment_to_send->seq_no, (int)segment_to_send->len);
      LOG_DEBUG("SEND C: %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
                segment_to_send->data[0], segment_to_send->data[1],
                segment_to_send->data[2], segment_to_send->data[3],
                segment_to_send->data[4], segment_to_send->data[5],
                segment_to_send->data[6], segment_to_send->data[7],
                segment_to_send->data[8], segment_to_send->data[9],
                segment_to_send->data[10], segment_to_send->data[11]);
    }
#endif
#ifdef VERBOSE_DEQUEUE_SEND_DATA
    if (!is_control) {
      LOG_DEBUG("%s: Send %s Segment (type=Data, seqno=%d)", this->get_name(),
                (is_get_failed_segment ? "Failed" : "Normal"),
                segment_to_send->seq_no);
    }
#endif

#ifdef EXP_MEASURE_INTERVAL_SENDER
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

#ifdef EXP_MEASURE_INTERVAL_SENDER
    gettimeofday(&times[3], NULL);
#endif

    int len = kSegHeaderSize + kSegSize;
    const void *data = segment_to_send->data;

    // Send data
    int res = this->send(data, len);

#ifdef EXP_MEASURE_INTERVAL_SENDER
    gettimeofday(&times[4], NULL);
#endif

    // Error handling
    if (res <= 0) {
      if (is_disconnecting_on_purpose()) {
        // Now disconnecting on purpose... Send this segment by another
        // adapter.
        LOG_DEBUG("%s: Disconnecting on purpose (type=%d, seq_no=%lu)",
                  this->get_name(), (int)is_control, segment_to_send->seq_no);
        sm->failed_sending(segment_to_send);
        break;
      } else if (errno == 0) {
        LOG_DEBUG("%s: Unknown error occured (type=%d, seq_no=%lu)",
                  this->get_name(), (int)is_control, segment_to_send->seq_no);
        sm->failed_sending(segment_to_send);
        continue;
      } else if (errno == EINTR) {
        // Handling interrupted system call
        LOG_DEBUG("%s: Send interrupted (type=%d, seq_no=%lu)",
                  this->get_name(), (int)is_control, segment_to_send->seq_no);
        sm->failed_sending(segment_to_send);
        continue;
      } else if (errno == EAGAIN) {
        // Kernel I/O buffer is full
        LOG_WARN(
            "%s: Send fail - Kernel I/O buffer is full (type=%d, seq_no=%lu)",
            this->get_name(), (int)is_control, segment_to_send->seq_no);
        sm->failed_sending(segment_to_send);
        continue;
      } else {
        // Other errors
        LOG_WARN("%s: Send fail - res=%d errno=%d(%s) (type=%d, seq_no=%lu)",
                 this->get_name(), res, errno, strerror(errno), (int)is_control,
                 segment_to_send->seq_no);
        sm->failed_sending(segment_to_send);
        break;
      }
    } else if (res != len) {
      LOG_WARN("%s: Send fail - sent_bytes are not accurate (type=%d, "
               "seq_no=%lu, expected=%d, sent=%d) - %d %s",
               this->get_name(), (int)is_control, segment_to_send->seq_no, len,
               res, errno, strerror(errno));
      sm->failed_sending(segment_to_send);
      break;
    }

    sm->free_segment(segment_to_send);

#ifdef EXP_MEASURE_INTERVAL_SENDER
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
  LOG_ADAPTER_THREAD_LAUNCH(this->get_name(), "Receiver");

  this->mReceiverLoopOn = true;
  this->receiver_thread_loop();

  LOG_DEBUG("%s: Receiver thread ends(tid: %d)", this->get_name(),
            (unsigned int)syscall(224));
  this->mReceiverLoopOn = false;
  this->mReceiverThread = NULL;

  // If it is disconnecting on purpose, do not reconnect it.
  if (this->is_disconnecting_on_purpose()) {
    LOG_ADAPTER_THREAD_FINISH(this->get_name(), "Receiver");
  } else {
    // Reconnect the adapter
    LOG_ADAPTER_THREAD_FAIL(this->get_name(), "Receiver");
    NetworkSwitcher::singleton()->reconnect_adapter(this, true);
  }

  this->mWaitReceiverThreadCond.notify_all();
}

void ServerAdapter::receiver_thread_loop(void) {
  while (this->mReceiverLoopOn) {
    SegmentManager *sm = SegmentManager::singleton();
    Segment *segment_to_receive = sm->get_free_segment();
    void *buf = reinterpret_cast<void *>(segment_to_receive->data);
    int len = kSegSize + kSegHeaderSize;

#ifdef VERBOSE_SERVER_ADAPTER_RECEIVING
    LOG_DEBUG("%s: Receiving...", this->get_name());
#endif
    int res = this->receive(buf, len);
    if (errno == EINTR) {
      continue;
    } else if (errno == EAGAIN) {
      LOG_WARN("Kernel I/O buffer is full at %s", this->get_name());
      continue;
    } else if (res < len && errno != 0) {
      if (!this->is_disconnecting_on_purpose()) {
        LOG_WARN("Receiving failed at %s (%d / %d; %s)", this->get_name(),
                 errno, res, strerror(errno));
      } else {
        LOG_DEBUG("Receiving broken at %s (%d / %d; %s)", this->get_name(),
                  errno, res, strerror(errno));
      }
      break;
    }

    uint32_t net_seq_no, net_len, net_flag;
    memcpy(&net_seq_no, buf, sizeof(uint32_t));
    memcpy(&net_len, (reinterpret_cast<uint8_t *>(buf) + 4), sizeof(uint32_t));
    memcpy(&net_flag, (reinterpret_cast<uint8_t *>(buf) + 8), sizeof(uint32_t));
    segment_to_receive->seq_no = ntohl(net_seq_no);
    segment_to_receive->len = ntohl(net_len);
    segment_to_receive->flag = ntohl(net_flag);

    bool is_control = ((segment_to_receive->flag & kSegFlagControl) != 0);

    if (is_control) {
#ifdef VERBOSE_ENQUEUE_RECV
      LOG_DEBUG("%s: Receive Segment (type=Ctrl, seqno=%d)", this->get_name(),
                segment_to_receive->seq_no);
#endif
      sm->enqueue(kSQRecvControl, segment_to_receive);
    } else {
#ifdef VERBOSE_ENQUEUE_RECV
      LOG_DEBUG("%s: Receive Segment (type=Data, seqno=%d)", this->get_name(),
                segment_to_receive->seq_no);
#endif
      sm->enqueue(kSQRecvData, segment_to_receive);
    }
    segment_to_receive = sm->get_free_segment();

#ifdef VERBOSE_SERVER_ADAPTER_RECEIVING
    LOG_DEBUG("%s: Received: %d", this->get_name(), res);
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

  bool sender_suspended = false;
  {
    std::unique_lock<std::mutex> lck(this->mSenderSuspendedMutex);
    if (this->mSenderSuspended) {
      LOG_ERR("Sender has already been suspended!: %s", this->get_name());
      if (callback != NULL)
        callback(false);
    } else {
      // Send Request
      if (is_send_request) {
        Core::singleton()->get_control_sender()->send_request_sleep(
            this->get_id());
      }

      // Sleep
      this->set_state(ServerAdapterState::kGoingSleeping);
      this->mSenderSuspended = true;

      // Wake up sender thread waiting segment queue
      SegmentManager *sm = SegmentManager::singleton();
      sm->wake_up_dequeue_waiting(kDeqSendControlData);
    }
  }

  // Execute callback
  if (callback != NULL)
    callback(true);
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
      Core::singleton()->get_control_sender()->send_request_wake_up(
          this->get_id());
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

void ServerAdapter::set_state(ServerAdapterState new_state) {
  ServerAdapterState old_state;

  {
    std::unique_lock<std::mutex> lck(this->mStateLock);
    old_state = this->mState;
    this->mState = new_state;
  }

  std::string old_state_str(server_adapter_state_to_string(old_state));
  std::string new_state_str(server_adapter_state_to_string(new_state));

  LOG_DEBUG("%s: State(%s->%s)", this->get_name(), old_state_str.c_str(),
            new_state_str.c_str());

  for (std::vector<ServerAdapterStateListener *>::iterator it =
           this->mStateListeners.begin();
       it != this->mStateListeners.end(); it++) {
    ServerAdapterStateListener *listener = (*it);
    listener->onUpdateServerAdapterState(this, old_state, new_state);
  }
}