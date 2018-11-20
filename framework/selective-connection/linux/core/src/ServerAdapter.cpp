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
#include "../inc/ServerAdapterStateListener.h"

#include "../inc/Core.h"
#include "../inc/NetworkSwitcher.h"
#include "../inc/SenderThread.h"

#include "../../common/inc/DebugLog.h"

#include "../../configs/ExpConfig.h"

#include <mutex>
#include <arpa/inet.h>
#include <sys/socket.h>

#ifdef EXP_MEASURE_INTERVAL_SENDER
#include <sys/time.h>
#endif

using namespace sc;

void ServerAdapter::launch_threads(void) {
  // Launch sender/receiver threads (initially, running but disabled state)
  // It is called by StartCoreTransaction.
  if (this->mSenderThread == NULL)
    this->mSenderThread = new SenderThread(this);
  this->mSenderThread->launch_thread();
  if (this->mReceiverThread == NULL)
    this->mReceiverThread = new ReceiverThread(this);
  this->mReceiverThread->launch_thread();
}

void ServerAdapter::finish_thread(void) {
  // Finish sender/receiver threads
  // It is called by StopCoreTransaction.
  this->mSenderThread->finish_thread();
  this->mReceiverThread->finish_thread();
}

void ServerAdapter::connect(ConnectCallback callback, bool is_send_request) {
  // Check adapter's state
  if (this->get_state() != ServerAdapterState::kDisconnected) {
    LOG_ERR(
        "It is already connected or connection/disconnection is in progress.");
    callback(this, false);
    return;
  }

  // Send request
  if (is_send_request) {
    Core::singleton()->get_control_sender()->send_request_connect(
        this->get_id());
  }

  // Connect
  this->connect_internal(callback);
}

void ServerAdapter::disconnect_on_command(DisconnectCallback callback) {
  // Check adapter's state
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
    callback(this, false);
    return;
  }

  // Set this disconnection is on purpose
  this->start_disconnecting_on_purpose();

  // Get my final seq_no
  SegmentManager *sm = SegmentManager::singleton();
  uint32_t my_final_seq_no_control =
      this->mSenderThread->get_last_seq_no_control();
  uint32_t my_final_seq_no_data = this->mSenderThread->get_last_seq_no_data();

  // Send disconnect request
  Core::singleton()->get_control_sender()->send_request_disconnect(
      this->get_id(), my_final_seq_no_control, my_final_seq_no_data);

  this->disconnect_internal(callback);
}

void ServerAdapter::disconnect_on_peer_command(
    DisconnectCallback callback, uint32_t peer_final_seq_no_control,
    uint32_t peer_final_seq_no_data) {
  // Check adapter's state
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
    callback(this, false);
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
  // Check adapter's state
  // Check if the adapter is already disconencted
  ServerAdapterState state = this->get_state();
  if (state == ServerAdapterState::kDisconnected ||
      state == ServerAdapterState::kDisconnecting) {
    LOG_ERR("%s: Disconnect fail - already disconnecting or disconnected",
            this->get_name());
    callback(this, false);
    return;
  }

  this->disconnect_internal(callback);
}

void ServerAdapter::connect_internal(ConnectCallback callback) {
  // Set callback and adapter's state
  this->mConnectCallback = callback;
  this->set_state(ServerAdapterState::kConnecting);

  bool res = this->__connect_internal();

  if (res) {
    this->set_state(ServerAdapterState::kActive);
    if (this->mConnectCallback != NULL) {
      this->mConnectCallback(this, true);
      this->mConnectCallback = NULL;
    }
  } else {
    this->set_state(ServerAdapterState::kDisconnected);
    if (this->mConnectCallback != NULL) {
      this->mConnectCallback(this, false);
      this->mConnectCallback = NULL;
    }
  }
}

bool ServerAdapter::__connect_internal(void) {
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

  // Enable sender thread's sender loop
  this->mSenderThread->enable_loop();

  // Enable receiver thread's receiver loop
  this->mReceiverThread->enable_loop();

  return true;
}

void ServerAdapter::disconnect_internal(DisconnectCallback callback) {
  // Set callback and adapter's state
  this->mDisconnectCallback = callback;
  ServerAdapterState oldState = this->get_state();
  this->set_state(ServerAdapterState::kDisconnecting);

  if (this->is_disconnecting_on_purpose() &&
      !this->is_disconnecting_on_purpose_peer()) {
    this->wait_for_disconnecting_on_purpose_peer();
  }

  bool res = this->__disconnect_internal(oldState);

  this->finish_disconnecting_on_purpose();

  if (res) {
    this->set_state(ServerAdapterState::kDisconnected);
    if (this->mDisconnectCallback != NULL) {
      this->mDisconnectCallback(this, true);
      this->mDisconnectCallback = NULL;
    }
  } else {
    this->set_state(oldState);
    if (this->mDisconnectCallback != NULL) {
      this->mDisconnectCallback(this, false);
      this->mDisconnectCallback = NULL;
    }
  }
}

bool ServerAdapter::__disconnect_internal(ServerAdapterState oldState) {
  // Disable sender thread's sender loop
  if (this->mSenderThread != NULL) {
    this->mSenderThread->disable_loop();

    // If this adapter is already sleeping(sender thread is paused),
    //   resume the sender thread
    if (oldState == ServerAdapterState::kSleeping) {
      // Since it does not need state checking, it directly wakes up.
      this->wake_up_internal();
    }
  }
  // Disable receiver thread's receiver loop
  if (this->mReceiverThread != NULL) {
    this->mReceiverThread->disable_loop();
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
  if (deviceState != DeviceState::kOff &&
      deviceState != DeviceState::kTurningOff) {
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
    LOG_DEBUG("Wait sender loop(%s)", this->get_name());
    this->mSenderThread->wait_until_disable_loop_done();
    LOG_DEBUG("End sender loop(%s)", this->get_name());
  }
  if (this->mReceiverThread != NULL) {
    LOG_DEBUG("Wait receiver loop(%s)", this->get_name());
    this->mReceiverThread->wait_until_disable_loop_done();
    LOG_DEBUG("End receiver loop(%s)", this->get_name());
  }

  return true;
}

int ServerAdapter::send(void *buf, size_t len) {
  ServerAdapterState state = this->get_state();
  if (state == ServerAdapterState::kConnecting) {
    LOG_VERB("Send blocked: waiting for connection... (%d)", state);
    do {
      state = this->get_state();
      ::sleep(1);
    } while (state != ServerAdapterState::kActive);
  } else if (state != ServerAdapterState::kActive) {
    return -1;
  }

  if (this->mServerSocket == NULL) {
    return -2;
  }

  // TODO: hard-coded for Cap-Dynamic policy
  struct timeval media_start_ts;
  gettimeofday(&media_start_ts, NULL);
  int32_t net_media_start_ts_sec = htonl(media_start_ts.tv_sec);
  int32_t net_media_start_ts_usec = htonl(media_start_ts.tv_usec);
  memcpy((void*)((uint8_t*)buf + 20), &net_media_start_ts_sec, sizeof(int32_t));
  memcpy((void*)((uint8_t*)buf + 24), &net_media_start_ts_usec, sizeof(int32_t));

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
  if (state == ServerAdapterState::kConnecting) {
    LOG_VERB("Receive blocked: waiting for connection... (%d)", state);
    do {
      state = this->get_state();
      ::sleep(1);
    } while (state != ServerAdapterState::kActive);
  } else if (state != ServerAdapterState::kActive &&
             state != ServerAdapterState::kGoingSleeping &&
             state != ServerAdapterState::kSleeping &&
             state != ServerAdapterState::kWakingUp) {
    return -1;
  }

  if (this->mServerSocket == NULL) {
    return -2;
  }

  // Receive
  int ret = this->mServerSocket->receive(buf, len);

  // Update statistics
  if (ret > 0) {
    this->mReceiveDataSize.add((int)len);
  }
  return ret;
}

void ServerAdapter::sleep(DisconnectCallback callback, bool is_send_request) {
  ServerAdapterState state = this->get_state();
  if (state != ServerAdapterState::kActive) {
    LOG_ERR("Failed to sleep: %s (state: %d)", this->get_name(), state);
    if (callback != NULL)
      callback(this, false);
    return;
  }

  // Set sender thread paused
  bool sender_paused = false;
  {
    if (this->mSenderThread->is_loop_paused()) {
      LOG_ERR("Sender has already been paused!: %s", this->get_name());
      if (callback != NULL)
        callback(this, false);
    } else {
      // Send Request
      if (is_send_request) {
        Core::singleton()->get_control_sender()->send_request_sleep(
            this->get_id());
      }

      // Pause sender thread's sender loop
      this->set_state(ServerAdapterState::kGoingSleeping);
      this->mSenderThread->pause_loop();

      // Wake up sender thread waiting segment queue
      SegmentManager *sm = SegmentManager::singleton();
      sm->wake_up_dequeue_waiting(kDeqSendControlData);
    }
  }

  // Execute callback
  if (callback != NULL)
    callback(this, true);
}

void ServerAdapter::wake_up(DisconnectCallback callback, bool is_send_request) {
  ServerAdapterState state = this->get_state();
  if (state != ServerAdapterState::kSleeping) {
    LOG_ERR("Failed to wake up: %s (state: %d)", this->get_name(), state);
    if (callback != NULL)
      callback(this, false);
    return;
  }

  if (this->mSenderThread->is_loop_paused()) {
    // Send Wakeup Request
    if (is_send_request) {
      Core::singleton()->get_control_sender()->send_request_wake_up(
          this->get_id());
    }

    this->set_state(ServerAdapterState::kWakingUp);

    // Resume sender thread's sender loop
    this->wake_up_internal();

    if (callback != NULL)
      callback(this, true);
  } else {
    LOG_ERR("Sender has not been suspended!: %s", this->get_name());
    if (callback != NULL)
      callback(this, false);
  }
}

void ServerAdapter::wake_up_internal(void) {
  // Resume sender thread's sender loop
  this->mSenderThread->resume_loop();
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