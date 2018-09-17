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

#include <Core.h>

#include <ExpConfig.h>

#include <DebugLog.h>
#include <NetworkSwitcher.h>
#include <ProtocolManager.h>
#include <SegmentManager.h>

#include <arpa/inet.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>

/**
 * < When to Free Buffer >
 * [*] Sending
 * When ProtocolManager::serialize() method is invoked, memory will be
 * allocated. ProtocolManager is in charge of allocating this memory.
 * SegmentManager is in charge of freeing this memory.
 * [*] Receiving
 * SegmentManager invokes ProtocolManager::parse() with allocated serialized
 * vector. SegmentManager is in charge of allocating this memory. After copying
 * to application memory, serialized vector should be freed. ProtocolManager is
 * in charge of freeing this memory.
 */
namespace sc {

Core *Core::singleton = NULL;

StartCoreTransaction *StartCoreTransaction::sOngoing = NULL;
StopCoreTransaction *StopCoreTransaction::sOngoing = NULL;

// Start core: connect initial adapters
void Core::start() {
  setlocale(LC_ALL, "");

  if (this->get_state() != CMState::kCMStateIdle) {
    LOG_ERR("Core has already started.");
    this->done_start(false);
    return;
  } else if (this->mControlAdapters.empty()) {
    LOG_ERR("No control adapter is registered!");
    this->done_start(false);
    return;
  } else if (this->mDataAdapters.empty()) {
    LOG_ERR("No data adapter is registered!");
    this->done_start(false);
    return;
  }

  // Connect control adapter & first data adapter
  this->set_state(CMState::kCMStateStarting);
  StartCoreTransaction::run(this);
}

void Core::done_start(bool is_success) {
  if (is_success) {
    LOG_VERB("Succeed to start core!");
    this->set_state(CMState::kCMStateReady);
  } else {
    LOG_ERR("Failed to start core!");
    this->set_state(CMState::kCMStateIdle);
    return;
  }

  start_sc_done(is_success);
}

// Stop core: disconnect all the adapters
void Core::stop() {
  CMState state = this->get_state();
  if (state == CMState::kCMStateStarting ||
      state == CMState::kCMStateStopping) {
    LOG_ERR("Cannot stop core during starting/stopping!");
    this->done_stop(false);
    return;
  } else if (state == CMState::kCMStateIdle) {
    LOG_ERR("Core is already idle state!");
    this->done_stop(false);
    return;
  } else if (this->mControlAdapters.empty()) {
    LOG_ERR("No control adapter is registered!");
    this->done_stop(false);
    return;
  } else if (this->mDataAdapters.empty()) {
    LOG_ERR("No data adapter is registered!");
    this->done_stop(false);
    return;
  }

  // Disconnect all the adapters
  this->set_state(CMState::kCMStateStopping);
  StopCoreTransaction::run(this);
}

void Core::done_stop(bool is_success) {
  if (is_success) {
    LOG_VERB("Succeed to stop core!");
    this->set_state(CMState::kCMStateIdle);
  } else {
    LOG_ERR("Failed to stop core!");
    this->set_state(CMState::kCMStateReady);
  }

  stop_sc_done(is_success);
}

// Register control adapter
void Core::register_control_adapter(ServerAdapter *control_adapter) {
  if (this->get_state() != CMState::kCMStateIdle) {
    LOG_ERR("You can register control adapter on only idle state!");
    return;
  }
  std::unique_lock<std::mutex> lck(this->mControlAdaptersLock);
  control_adapter->enable_receiver_thread(Core::control_adapter_receive_loop);
  this->mControlAdapters.push_back(control_adapter);
}

// Register data adapter
void Core::register_data_adapter(ServerAdapter *data_adapter) {
  if (this->get_state() != CMState::kCMStateIdle) {
    LOG_ERR("You can register data adapter on only idle state!");
    return;
  }
  std::unique_lock<std::mutex> lck(this->mDataAdaptersLock);
  data_adapter->enable_receiver_thread(NULL);
  data_adapter->enable_sender_thread();
  this->mDataAdapters.push_back(data_adapter);
}

int Core::send(const void *dataBuffer, uint32_t dataLength) {
  CMState state = this->get_state();
  if (state != CMState::kCMStateReady) {
    LOG_ERR("Core is not started yet, so you cannot send the data");
    return -1;
  }
  uint32_t curr_offset = 0;
  int sent_bytes;
  ProtocolData pd;
  uint8_t *serialized_vector;
  uint32_t packet_size;

  /* Update statistics */
  this->mSendRequestSize.set_value(dataLength);
  this->mSendArrivalTime.arrive();

  /* Attach the protocol header to the payload */
  ProtocolManager::data_to_protocol_data((const uint8_t *)dataBuffer,
                                         dataLength, &pd);

  /* The serialized_vector buffer is allocated in here */
  packet_size =
      ProtocolManager::serialize(&pd, (const uint8_t *)dataBuffer, curr_offset,
                                 dataLength, &serialized_vector);
  assert(serialized_vector != NULL && packet_size > 0);

  /* Hand over the data to the Protocol Manager */
  sent_bytes = ProtocolManager::send_packet(serialized_vector, packet_size);
  if (unlikely(sent_bytes < 0)) {
    LOG_ERR("Sending stopped(%u/%u) by %d", curr_offset, dataLength,
            sent_bytes);
  }
  return sent_bytes;
}

int Core::receive(void **pDataBuffer) {
  CMState state = this->get_state();
  if (state != CMState::kCMStateReady) {
    LOG_ERR("Core is not started yet, so you cannot receive data");
    return -1;
  }
  int ret;
  uint32_t packet_size;
  uint8_t *packet;

  packet_size = ProtocolManager::recv_packet(&packet);
  *pDataBuffer = packet;

  return packet_size;
}

void Core::send_control_message(const void *dataBuffer, size_t dataLength) {
  int num_retries = 5;
  bool send_success = false;

  bool mute_warning_message = false;

  do {
    ServerAdapter *control_adapter = this->get_active_control_adapter();
    int res = control_adapter->send(dataBuffer, dataLength);

    if (control_adapter->is_disconnecting_on_purpose()) {
      // If it is disconnecting on purpose, wait until active control adapter
      // is ready.
      if (!mute_warning_message) {
        LOG_WARN("Send(Control Msg) Result: Now disconnecting on purpose. It "
                 "will wait until control adapter is ready.");
        mute_warning_message = true;
      }
      sleep(1);
      continue;
    }

    // Result handling
    if (res > 0) {
      // Success
      LOG_DEBUG("Send(Control Msg) Result: Success");
      send_success = true;
    } else {
      // Error handling
      if (errno == 0) {
        // Successful. Do nothing.
        LOG_DEBUG("Send(Control Msg) Result: Successful, but res=0");
        send_success = true;
      } else if (errno == EINTR) {
        // Handling interrupted system call
        LOG_DEBUG("Send(Control Msg) Result: interrupted");
        send_success = false;
      } else if (errno == EAGAIN) {
        // Kernel I/O buffer is full
        LOG_WARN("Send(Control Msg) Result: Failed. Kernel I/O buffer is full");
        send_success = false;
      } else {
        // Other errors
        LOG_WARN("Send(Control Msg) Result: Failed res=%d errno=%d(%s)", res,
                 errno, strerror(errno));
        send_success = false;
      }
    }
    num_retries--;
  } while (!send_success && num_retries > 0);

  if (!send_success) {
    LOG_WARN("Send(Control Msg): Failed");
  }
}

void Core::send_request(CtrlReq request_code, uint16_t adapter_id) {
  bool retry_check = true;
  while (retry_check) {
    ServerAdapter *control_adapter = this->get_active_control_adapter();
    ServerAdapterState controlAdapterState = control_adapter->get_state();
    if (controlAdapterState != ServerAdapterState::kActive) {
      LOG_WARN("Now switching control adapter... adapter_id=%d state=%d",
               control_adapter->get_id(), controlAdapterState);
    } else {
      retry_check = false;
    }
  }

  uint8_t net_request_code = (uint8_t)request_code;
  uint16_t net_adapter_id = htons(adapter_id);

  this->send_control_message(&net_request_code, 1);
  this->send_control_message(&net_adapter_id, 2);
}

void Core::send_request_connect(uint16_t adapter_id) {
  this->send_request(kCtrlReqConnect, adapter_id);
  LOG_VERB("Send(Control Msg): Request(Connect %d)", adapter_id);
}

void Core::send_request_disconnect(uint16_t adapter_id) {
  this->send_request(kCtrlReqDisconnect, adapter_id);
  LOG_VERB("Send(Control Msg): Request(Disconnect %d)", adapter_id);
}

void Core::send_request_disconnect_ack(uint16_t adapter_id) {
  this->send_request(kCtrlReqDisconnectAck, adapter_id);
  LOG_VERB("Send(Control Msg): Request(DisconnectAck %d)", adapter_id);
}

void Core::send_request_sleep(uint16_t adapter_id) {
  this->send_request(kCtrlReqSleep, adapter_id);
  LOG_VERB("Send(Control Msg): Request(Sleep %d)", adapter_id);
}

void Core::send_request_wake_up(uint16_t adapter_id) {
  this->send_request(kCtrlReqWakeUp, adapter_id);
  LOG_VERB("Send(Control Msg): Request(WakeUp %d)", adapter_id);
}

void Core::send_noti_private_data(PrivType priv_type, char *private_data_buf,
                                  uint32_t private_data_len) {
  bool retry_check = true;
  while (retry_check) {
    ServerAdapter *control_adapter = this->get_active_control_adapter();
    ServerAdapterState controlAdapterState = control_adapter->get_state();
    if (controlAdapterState != ServerAdapterState::kActive) {
      LOG_WARN("Now switching control adapter...");
    } else {
      retry_check = false;
    }
  }

  uint8_t request_code = kCtrlReqPriv;
  uint32_t net_priv_type = htonl((unsigned long)priv_type);
  uint32_t net_private_data_len = htonl(private_data_len);

  LOG_VERB("Send(Control Msg): Request(Priv Noti--Start)");
  this->send_control_message(&request_code, 1);
  this->send_control_message(&net_priv_type, 4);
  this->send_control_message(&net_private_data_len, 4);
  this->send_control_message(private_data_buf, private_data_len);
  LOG_VERB("Send(Control Msg): Request(Priv Noti '%s'; type=%d)",
           private_data_buf, priv_type);
}

void Core::control_adapter_receive_loop(ServerAdapter *adapter) {
  assert(adapter != NULL);

  while (adapter->is_receiver_loop_on()) {
    int res = Core::__control_adapter_receive_loop(adapter);
    if (errno == EINTR) {
      continue;
    } else if (errno == EAGAIN) {
      LOG_VERB("Receiving: Kernel I/O buffer is full at %s",
               adapter->get_name());
    } else if (res <= 0) {
      break;
    }
  } // End while

  // If control message loop is crashed, reconnect control adapter.
  LOG_DEBUG("Control message loop is finished");
}

int Core::__control_adapter_receive_loop(ServerAdapter *adapter) {
  Core *core = Core::get_instance();

  char data[512] = {
      0,
  };
  int res = 0;

  // Receive 1Byte: Control Request Code
  res = adapter->receive(data, 1);
  if (res <= 0) {
    return res;
  }

  char req_code = data[0];
  /* If the control message is 'connect adapter', */
  if (req_code == CtrlReq::kCtrlReqConnect ||
      req_code == CtrlReq::kCtrlReqSleep ||
      req_code == CtrlReq::kCtrlReqWakeUp ||
      req_code == CtrlReq::kCtrlReqDisconnect ||
      req_code == CtrlReq::kCtrlReqDisconnectAck) {
    // Receive 2Byte: Adapter ID
    res = adapter->receive(data, 2);
    if (res <= 0) {
      return res;
    }

    // convert adapter_id to n_adapter_id
    uint16_t n_adapter_id;
    uint16_t adapter_id;

    memcpy(&n_adapter_id, data, 2);
    adapter_id = ntohs(n_adapter_id);

    if (req_code == CtrlReq::kCtrlReqConnect) {
      LOG_VERB("Receive(Control Msg): Request(Connect %d)", adapter_id);
      NetworkSwitcher::get_instance()->connect_adapter_by_peer(adapter_id);
    } else if (req_code == CtrlReq::kCtrlReqSleep) {
      LOG_VERB("Receive(Control Msg): Request(Sleep %d)", adapter_id);
      NetworkSwitcher::get_instance()->sleep_adapter_by_peer(adapter_id);
    } else if (req_code == CtrlReq::kCtrlReqWakeUp) {
      LOG_VERB("Receive(Control Msg): Request(WakeUp %d)", adapter_id);
      NetworkSwitcher::get_instance()->wake_up_adapter_by_peer(adapter_id);
    } else if (req_code == CtrlReq::kCtrlReqDisconnect) {
      LOG_VERB("Receive(Control Msg): Request(Disconnect %d)", adapter_id);
      NetworkSwitcher::get_instance()->disconnect_adapter_by_peer(adapter_id);
    } else if (req_code == CtrlReq::kCtrlReqDisconnectAck) {
      LOG_VERB("Receive(Control Msg): Request(DisconnectAck %d)", adapter_id);
      ServerAdapter *disconnect_adapter =
          Core::get_instance()->find_adapter_by_id((int)adapter_id);
      if (disconnect_adapter == NULL) {
        LOG_WARN("Cannot find adapter %d", (int)adapter_id);
      } else {
        disconnect_adapter->peer_knows_disconnecting_on_purpose();
      }
    }
  } else if (req_code == CtrlReq::kCtrlReqPriv) {
    LOG_VERB("Receive(Control Msg): Request(Priv Noti--Start)");
    uint16_t n_priv_type;
    PrivType priv_type;
    uint32_t n_len;
    uint32_t len;

    // Receive 4Byte: Priv Type
    res = adapter->receive(&n_priv_type, 4);
    if (res <= 0) {
      return res;
    }

    priv_type = (ntohs(n_priv_type) == PrivType::kPrivTypeWFDInfo)
                    ? PrivType::kPrivTypeWFDInfo
                    : PrivType::kPrivTypeUnknown;

    // Receive 4Byte: Private Data Length
    res = adapter->receive(&n_len, 4);
    if (res <= 0) {
      return res;
    }

    len = ntohl(n_len);
    assert(len <= 512);

    // Receive nByte: Private Data
    res = adapter->receive(data, len);
    if (res <= 0) {
      return res;
    }
    LOG_VERB("Receive(Control Msg): Request(Priv Noti '%s'; type=%d)", data,
             priv_type);

    for (std::vector<ControlMessageListener *>::iterator it =
             core->mControlMessageListeners.begin();
         it != core->mControlMessageListeners.end(); it++) {
      ControlMessageListener *listener = *it;
      listener->on_receive_control_message(priv_type, data, len);
    }
  }
  return res;
}

// Transactions
// ----------------------------------------------------------------------------------

// Start Core
bool StartCoreTransaction::run(Core *caller) {
  if (sOngoing == NULL) {
    sOngoing = new StartCoreTransaction(caller);
    sOngoing->start();
    return true;
  } else {
    LOG_WARN("Already starting core");
    caller->done_start(false);
    return false;
  }
}
void StartCoreTransaction::start() {
  // Connect first control adapter
  std::unique_lock<std::mutex> lck(this->mCaller->mControlAdaptersLock);

  if (this->mCaller->mControlAdapters.empty()) {
    LOG_ERR("Connecting control adapter is failed");
    this->done(false);
    return;
  }

  this->mCaller->mControlAdapters.front()->connect(
      StartCoreTransaction::connect_first_control_adapter_callback, false);
}

void StartCoreTransaction::connect_first_control_adapter_callback(
    bool is_success) {
  if (!is_success) {
    LOG_ERR("Connecting control adapter is failed");
    sOngoing->done(false);
    return;
  }

  // Connect first data adapter
  std::unique_lock<std::mutex> lck(sOngoing->mCaller->mDataAdaptersLock);

  if (sOngoing->mCaller->mDataAdapters.empty()) {
    LOG_ERR("Connecting first data adapter is failed");
    sOngoing->done(false);
    return;
  }

  sOngoing->mCaller->mDataAdapters.front()->connect(
      StartCoreTransaction::connect_first_data_adapter_callback, false);
}

void StartCoreTransaction::connect_first_data_adapter_callback(
    bool is_success) {
  if (is_success) {
    // Done transaction
    sOngoing->done(true);
  } else {
    LOG_ERR("Connecting first data adapter is failed");
    sOngoing->done(false);
  }
}

void StartCoreTransaction::done(bool is_success) {
  this->mCaller->done_start(is_success);
  sOngoing = NULL;
}

// Stop Core
bool StopCoreTransaction::run(Core *caller) {
  if (sOngoing == NULL) {
    sOngoing = new StopCoreTransaction(caller);
    sOngoing->start();
    return true;
  } else {
    LOG_WARN("Already stopping core");
    caller->done_stop(false);
    return false;
  }
}
void StopCoreTransaction::start() {
  /* Disconnect all control adapters */
  std::unique_lock<std::mutex> lck(this->mCaller->mControlAdaptersLock);

  /* Get active control adapter count */
  this->mControlAdaptersCount = 0;
  for (std::vector<ServerAdapter *>::iterator it =
           this->mCaller->mControlAdapters.begin();
       it != this->mCaller->mControlAdapters.end(); it++) {
    ServerAdapter *control_adapter = *it;
    ServerAdapterState state = control_adapter->get_state();
    if (state != ServerAdapterState::kDisconnected &&
        state != ServerAdapterState::kDisconnecting) {
      this->mControlAdaptersCount++;
    }
  }

  /* Disconnect only active control adapters */
  for (std::vector<ServerAdapter *>::iterator it =
           this->mCaller->mControlAdapters.begin();
       it != this->mCaller->mControlAdapters.end(); it++) {
    ServerAdapter *control_adapter = *it;
    ServerAdapterState state = control_adapter->get_state();
    if (state != ServerAdapterState::kDisconnected &&
        state != ServerAdapterState::kDisconnecting) {
      control_adapter->disconnect(
          StopCoreTransaction::disconnect_control_adapter_callback, true, false,
          true);
    }
  }
}

void StopCoreTransaction::disconnect_control_adapter_callback(bool is_success) {
  if (!is_success) {
    if (sOngoing != NULL) {
      LOG_ERR("Disconnecting control adapter is failed");
      sOngoing->done(false);
    }
    return;
  }

  /* check if all the data adapters are disconnected */
  bool done_disconnect_all = false;
  {
    std::unique_lock<std::mutex> lck(sOngoing->mControlAdaptersCountLock);
    sOngoing->mControlAdaptersCount--;
    if (sOngoing->mControlAdaptersCount == 0) {
      done_disconnect_all = true;
    }
  }

  /* Done transaction */
  if (done_disconnect_all) {
    /* Disconnect all data adapters */
    std::unique_lock<std::mutex> lck(sOngoing->mCaller->mDataAdaptersLock);

    /* Get active data adapter count */
    sOngoing->mDataAdaptersCount = 0;
    for (std::vector<ServerAdapter *>::iterator it =
             sOngoing->mCaller->mDataAdapters.begin();
         it != sOngoing->mCaller->mDataAdapters.end(); it++) {
      ServerAdapter *data_adapter = *it;
      ServerAdapterState state = data_adapter->get_state();
      if (state != ServerAdapterState::kDisconnected &&
          state != ServerAdapterState::kDisconnecting) {
        sOngoing->mDataAdaptersCount++;
      }
    }

    /* Disconnect only active data adapters */
    for (std::vector<ServerAdapter *>::iterator it =
             sOngoing->mCaller->mDataAdapters.begin();
         it != sOngoing->mCaller->mDataAdapters.end(); it++) {
      ServerAdapter *data_adapter = *it;
      ServerAdapterState state = data_adapter->get_state();
      if (state != ServerAdapterState::kDisconnected &&
          state != ServerAdapterState::kDisconnecting) {
        data_adapter->disconnect(
            StopCoreTransaction::disconnect_data_adapter_callback, true, false,
            true);
      }
    }
  }
}

void StopCoreTransaction::disconnect_data_adapter_callback(bool is_success) {
  if (!is_success) {
    if (sOngoing != NULL) {
      LOG_ERR("Disconnecting data adapter is failed 2");
      sOngoing->done(false);
    }
    return;
  }

  /* check if all the data adapters are disconnected */
  bool done_disconnect_all = false;
  {
    std::unique_lock<std::mutex> lck(sOngoing->mDataAdaptersCountLock);
    sOngoing->mDataAdaptersCount--;
    if (sOngoing->mDataAdaptersCount == 0) {
      done_disconnect_all = true;
    }
  }

  /* Done transaction */
  if (done_disconnect_all) {
    sOngoing->done(true);
  }
}

void StopCoreTransaction::done(bool is_success) {
  this->mCaller->done_stop(is_success);
  sOngoing = NULL;
}

} /* namespace sc */