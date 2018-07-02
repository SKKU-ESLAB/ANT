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

#include <Communicator.h>

#include <DebugLog.h>
#include <ProtocolManager.h>
#include <SegmentManager.h>

#include <string.h>
#include <errno.h>
#include <stdio.h>

/**
 * < When to Free Buffer >
 * [*] Sending
 * When ProtocolManager::serialize() method is invoked, memory will be allocated.
 * ProtocolManager is in charge of allocating this memory.
 * SegmentManager is in charge of freeing this memory.
 * [*] Receiving
 * SegmentManager invokes ProtocolManager::parse() with allocated serialized vector.
 * SegmentManager is in charge of allocating this memory.
 * After copying to application memory, serialized vector should be freed.
 * ProtocolManager is in charge of freeing this memory.
 */
namespace cm {
Communicator* Communicator::singleton = NULL;
bool SwitchAdapterTransaction::sIsOngoing = false;
int SwitchAdapterTransaction::sPrevIndex = 0;
int SwitchAdapterTransaction::sNextIndex = 0;
bool ConnectRequestTransaction::sIsOngoing = false;
bool DisconnectRequestTransaction::sIsOngoing = false;
bool ReconnectControlAdapterTransaction::sIsOngoing = false;

// Start communicator: connect initial adapters
bool Communicator::start(void) {
  if(this->get_state() != CMState::Idle) {
    LOG_ERR("Communicator has already started.");
    return false;
  } else if(this->mControlAdapter == NULL) {
    LOG_ERR("No control adapter is registered!");
    return false;
  } else if (this->mDataAdapters.empty()) {
    LOG_ERR("No data adapter is registered!");
    return false;
  }

  // Connect control adapter & first data adapter
  this->set_state(CMState::kCMStateStarting);
  StartCommunicator::start(this);
  return true;
}

void Communicator::done_start(bool is_success) {
  if(!is_success) {
    LOG_ERR("Failed to start communicator!");
    this->set_state(CMState::kCMStateIdle);
    return;
  }

  LOG_VERB("Succeed to start communicator!");
  this->set_state(CMState::kCMStateReady);
}

// Stop communicator: disconnect all the adapters
bool Communicator::stop(void) {
  CMState state = this->get_state()
  if(state == CMState::Connecting ||
      state == CMState::Disconnecting) {
    LOG_ERR("Cannot stop communicator during connecting/disconnecting!");
    return false;
  } else if(state == CMState::Starting ||
      state == CMState::Stopping) {
    LOG_ERR("Cannot stop communicator during starting/stopping!");
    return false;
  } else if(state == CMState::Idle) {
    LOG_ERR("communicator is already idle state!");
    return false;
  } else if(this->mControlAdapter == NULL) {
    LOG_ERR("No control adapter is registered!");
    return false;
  } else if (this->mDataAdapters.empty()) {
    LOG_ERR("No data adapter is registered!");
    return false;
  }

  // Disconnect all the adapters
  this->set_state(CMState::kCMStateStopping);
  StopCommunicatorTransaction::start(this);
  return true;
}

void Communicator::done_stop(bool is_success) {
  if(!is_success) {
    LOG_ERR("Failed to stop communicator!");
    this->set_state(CMState::kCMStateReady);
  }

  LOG_VERB("Succeed to stop communicator!");
  this->set_state(CMState::kCMStateIdle);
}

// Register control adpater
void Communicator::register_control_adapter(ServerAdapter* control_adapter) {
  if(this->get_state() != kCMStateIdle) {
    LOG_ERR("You can register control adapter on only idle state!");
    return;
  }
  std::unique_lock<std::mutex> lck(this->mControlAdapterLock);
  if(this->mControlAdapter != NULL) {
    LOG_ERR("Another control adapter has already been registered!");
    return;
  }
  control_adapter->enable_receiver_thread(Communicator::receive_control_message_loop);
  this->mControlAdapter = control_adapter;
}

// Register data adpater
void Communicator::register_data_adapter(ServerAdapter* data_adapter) {
  if(this->get_state() != kCMStateIdle) {
    LOG_ERR("You can register data adapter on only idle state!");
    return;
  }
  std::unique_lock<std::mutex> lck(this->mDataAdaptersLock);
  data_adapter->enable_receiver_thread(NULL);
  data_adapter->enable_sender_thread();
  this->mDataAdapters.push_back(data_adapter);
}

int Communicator::send(const void *buf, uint32_t len) {
  CMState state = this->get_state();
  if(state != kCMStateReady && state != kCMStateConnecting && state != lCMStateDisconnecting) {
    LOG_ERR("Communicator is not started yet, so you cannot send the data");
    return -1;
  }
  uint32_t curr_offset = 0;
  int sent_bytes;
  ProtocolData pd;
  uint8_t *serialized_vector;
  uint32_t packet_size;

  // Attach the protocol header to the payload
  ProtocolManager::data_to_protocol_data((const uint8_t *) buf, len, &pd);
  // The serialized_vector buffer is allocated in here
  packet_size = ProtocolManager::serialize(&pd,
                                           (const uint8_t *)buf,
                                           curr_offset,
                                           len,
                                           &serialized_vector);
  assert(serialized_vector != NULL && packet_size > 0);

  // Hand over the data to the Protocol Manager
  sent_bytes = ProtocolManager::send_packet(serialized_vector, packet_size);
  if (unlikely(sent_bytes < 0)) {
    LOG_ERR("Sending stopped(%u/%u) by %d",
                 curr_offset,
                 len,
                 sent_bytes);
  } 
  return sent_bytes;
}

int Communicator::receive(void **buf) {
  CMState state = this->get_state();
  if(state != kCMStateReady && state != kCMStateConnecting && state != lCMStateDisconnecting) {
    LOG_ERR("Communicator is not started yet, so you cannot receive data");
    return -1;
  }
  int ret;
  uint32_t packet_size;
  uint8_t *packet;

  packet_size = ProtocolManager::recv_packet(&packet);
  *buf = packet;

  return packet_size;
}

bool Communicator::increase_adapter(void) {
  CMState state = this->get_state();
  if(state != kCMStateReady) {
    if(state == kCMStateConnecting || state == kCMStateDisconnecting) {
      LOG_ERR("It's already connecting or disconnecting an adapter!");
      return;
    } else {
      LOG_ERR("Communicator is not started.");
      return;
    }
  } else if(this->mDataAdapters.empty()) {
    LOG_ERR("No data adapter is registered!");
    return;
  } else if(this->mDataAdapters.size() == 1) {
    LOG_ERR("More than one data adapters are required!");
    return;
  } else if(this->mActiveDataAdapterIndex == this->mDataAdapter.size() - 1) {
    LOG_WARN("Cannot increase adapter!");
    return;
  }

  int prev_index = this->mActiveDataAdapterIndex;
  int next_index = this->mActiveDataAdapterIndex + 1;

  return this->switch_adapters(prev_index, next_index);
}

bool Communicator::decrease_adapter(void) {
  if(state != kCMStateReady) {
    if(state == kCMStateConnecting || state == kCMStateDisconnecting) {
      LOG_ERR("It's already connecting or disconnecting an adapter!");
      return;
    } else {
      LOG_ERR("Communicator is not started.");
      return;
    }
  } else if(this->mDataAdapters.empty()) {
    LOG_ERR("No data adapter is registered!");
    return;
  } else if(this->mDataAdapters.size() == 1) {
    LOG_ERR("More than one data adapters are required!");
    return;
  } else if(this->mActiveDataAdapterIndex == 0) {
    LOG_WARN("Cannot deccrease adapter!");
    return;
  }

  int prev_index = this->mActiveDataAdapterIndex;
  int next_index = this->mActiveDataAdapterIndex - 1;

  return this->switch_adapters(prev_index, next_index);
}

bool Communicator::switch_adapters(int prev_index, int next_index) {
  if(state != kCMStateReady) {
    if(state == kCMStateConnecting || state == kCMStateDisconnecting) {
      LOG_ERR("It's already connecting or disconnecting an adapter!");
      return false;
    } else {
      LOG_ERR("Communicator is not started.");
      return false;
    }
  }
  // Switch Step 1/4
  // Increase/decrease active data adapter index
  this->mActiveDataAdapterIndex = next_index;

  return SwitchAdapterTransaction::start(this, prev_index, next_index);
}

void Communicator::send_control_message(const void *data, size_t len) {
  CMState state = this->get_state();
  if(state != kCMStateReady && state != kCMStateConnecting && state != lCMStateDisconnecting) {
    LOG_ERR("Communicator is not started yet, so you cannot send the data");
    return -1;
  }
  ServerAdapter *control_adapter = this->get_control_adapter();
  control_adapter->send(data, len);
}

void Communicator::send_private_control_data(uint8_t request_code,
    uint16_t adapter_id, uint32_t private_data_len) {
  CMState state = this->get_state();
  if(state != kCMStateReady && state != kCMStateConnecting && state != lCMStateDisconnecting) {
    LOG_ERR("Communicator is not started yet, so you cannot send the data");
    return -1;
  }
  uint8_t req = kCtrlReqPriv;
  uint16_t net_adapter_id = htons(adapter_id);
  uint32_t net_private_datalen = htonl(private_data_len);

  this->send_control_message(&request_code, 1);
  this->send_control_message(&net_adapter_id, 2);
  this->send_control_message(&net_private_data_len, 4);
  this->send_control_message(buf, len);
}

void Communicator::receive_control_message_loop(ServerAdapter* adapter) {
  assert(adapter != NULL);
  LOG_VERB("Ready to receive control message");

  /* Notify network switcher that control adapter is ready */
  NetworkSwitcher *network_switcher = NetworkSwitcher::get_instance();
  network_switcher->control_adapter_ready();
  
  char data[512] = {0, };
  int res = 0;
  while (true) {
    // Receive 1Byte: Control Request Code
    res = adapter->receive(data, 1);
    if (res <= 0) {
      LOG_VERB("Control adapter could be closed");
      sleep(1);
      continue;
    }

    /*  If the control message is 'connect adapter', */
    if (data[0] == kCtrlReqConnect || data[0] == kCtrlReqDisconnect) {
      // Receive 2Byte: Adapter ID 
      res = adapter->receive(data, 2);
      if (res <= 0) {
        LOG_DEBUG("Control adapter has been closed");
        break;
      }

      // convert adapter_id to n_adapter_id
      uint16_t n_adapter_id;
      uint16_t adapter_id;

      memcpy(&n_adapter_id, data, 2);
      adapter_id = ntohs(n_adapter_id);

      if(data[0] == kCtrlReqConnect) {
        LOG_DEBUG("Data Adapter Connect request arrived");
        ConnectRequestTransaction::start(this, adapter_id);
      } else if (data[0] == kCtrlReqDisconnect) {
        LOG_DEBUG("Data Adapter Disconnect request arrived");
        DisconnectRequestTransaction::start(this, adapter_id);
      }
    } else if (data[0] == kCtrlReqPriv) {
      LOG_VERB("Private data arrived");
      uint16_t n_adapter_id;
      uint16_t adapter_id;
      uint32_t nlen;
      uint32_t len;

      // Receive 2Byte: Adapter ID 
      res = adapter->receive(&n_adapter_id, 2);
      if (res <= 0) break;
      adapter_id = ntohs(n_adapter_id);

      // Receive 4Byte: Private Data Length
      res = adapter->receive(&nlen, 4);
      if (res <= 0) break;
      len = ntohl(nlen);
      assert(len <= 512);

      // Receive nByte: Private Data
      res = adapter->receive(data, len);
      if (res > 0) {
        for(it = this->mControlMessageListeners.begin();
            it != this->mControlMessageListeners.end();
            it++) {
          ControlMessageListener* listener = *it;
          listener->on_receive_control_message(adapter_id, data, len);
        }
      } else {
        LOG_DEBUG("Control adapter closed");
        break;
      }
    }
  } // End while

  // If control message loop is crashed, reconnect control adapter.
  ReconnectControlAdapterTransaction::start(this);
}

// Transactions ----------------------------------------------------------------------------------

// Start Communicator
bool StartCommunicatorTransaction::start(Communicator* caller) {
  if(StartCommunicatorTransaction::sIsOngoing) {
    LOG_ERR("Only one transaction can run at the same time.");
    StartCommunicatorTransaction::sCaller->done_start(false);
    return;
  }
  StartCommunicatorTransaction::sIsOngoing = true;

  // Connect control adpater
  {
    Communicator* sCaller = StopCommunicatorTransaction::sCaller;
    std::unique_lock<std::mutex> lck(sCaller->mControlAdapterLock);
    bool res = sCaller->mControlAdapter->connect(StartCommunicatorTransaction::connect_control_adapter_callback);
    if(!res) {
      LOG_ERR("Connecting control adapter is failed");
      StartCommunicatorTransaction::sIsOngoing = false;
      StartCommunicatorTransaction::sCaller->done_start(false);
      return;
    }
  }
}

void StartCommunicatorTransaction::connect_control_adapter_callback(bool is_success) {
  if(!is_success) {
    LOG_ERR("Connecting control adapter is failed 2");
    StartCommunicatorTransaction::sIsOngoing = false;
    StartCommunicatorTransaction::sCaller->done_start(false);
    return;
  }

  // Connect first data adapter
  {
    Communicator* sCaller = StopCommunicatorTransaction::sCaller;
    std::unique_lock<std::mutex> lck(sCaller->mDataAdaptersLock);
    sCaller->mCurrentDataAdapter = 0;
    bool res = sCaller->mDataAdapters.front()->connect(StartCommunicatorTransaction::connect_first_data_adapter_callback);
    if(!res) {
      LOG_ERR("Connecting first data adapter is failed");
      StartCommunicatorTransaction::sIsOngoing = false;
      StartCommunicatorTransaction::sCaller->done_start(false);
      return;
    }
  }
}

void StartCommunicatorTransaction::connect_first_data_adapter_callback(bool is_success) {
  if(!is_success) {
    LOG_ERR("Connecting first data adapter is failed 2");
    StartCommunicatorTransaction::sIsOngoing = false;
    StartCommunicatorTransaction::sCaller->done_start(false);
    return;
  }

  // Done transaction
  StartCommunicatorTransaction::sIsOngoing = false;
  StartCommunicatorTransaction::sCaller->done_start(true);
}

// Stop Communicator
bool StopCommunicatorTransaction::start(Communicator* caller) {
  if(StopCommunicatorTransaction::sIsOngoing) {
    LOG_ERR("Only one transaction can run at the same time.");
    StopCommunicatorTransaction::sCaller->done_stop(false);
    return;
  }
  StopCommunicatorTransaction::sIsOngoing = true;
  {
    std::unique_lock<std::mutex> lck(StopCommunicatorTransaction::sDataAdaptersCountLock);
    StopCommunicatorTransaction::sDataAdaptersCount
      = StopCommunicatorTransaction::caller->get_data_adapter_count;
  }

  // Disconnect control adpater
  {
    Communicator* sCaller = StopCommunicatorTransaction::sCaller;
    std::unique_lock<std::mutex> lck(sCaller->mControlAdapterLock);
    bool res = sCaller->mControlAdapter->disconnect(StopCommunicatorTransaction::disconnect_control_adapter_callback);
    if(!res) {
      LOG_ERR("Connecting control adapter is failed");
      StopCommunicatorTransaction::sIsOngoing = false;
      StopCommunicatorTransaction::sCaller->done_stop(false);
      return;
    }
  }
}

void StopCommunicatorTransaction::disconnect_control_adapter_callback(bool is_success) {
  if(!is_success) {
    LOG_ERR("Connecting control adapter is failed 2");
    StopCommunicatorTransaction::sIsOngoing = false;
    StopCommunicatorTransaction::sCaller->done_stop(false);
    return;
  }

  // Disconnect all data adapters
  {
    Communicator* sCaller = StopCommunicatorTransaction::sCaller;
    std::unique_lock<std::mutex> lck(sCaller->mDataAdaptersLock);
    for(std::vector<ServerAdapter*>::iterator it = sCaller->mDataAdapter.begin();
        it != sCaller->mDataAdapter.end();
        it++) {
      ServerAdapter* data_adapter = *it;
      bool res = data_adapter->disconnect(StopCommunicatorTransaction::disconnect_data_adapter_callback);
      if(!res) {
        LOG_ERR("Disconnecting data adapter is failed");
      }
    }
  }
}

void StopCommunicatorTransaction::disconnect_data_adapter_callback(bool is_success) {
  if(!is_success) {
    LOG_ERR("Connecting first data adapter is failed 2");
    StopCommunicatorTransaction::sIsOngoing = false;
    StopCommunicatorTransaction::sCaller->done_stop(false);
    return;
  }

  bool done_disconnect_all = false;
  {
    std::unique_lock<std::mutex> lck(StopCommunicatorTransaction::sDataAdaptersCountLock);
    StopCommunicatorTransaction::sDataAdaptersCount--;
    if(StopCommunicatorTransaction::sDataAdaptersCount == 0) {
      done_disconnect_all = true;
    }
  }

  // Done transaction
  if(done_disonnect_all) {
    StopCommunicatorTransaction::sIsOngoing = false;
    StopCommunicatorTransaction::sCaller->done_stop(true);
  }
}

// Connect Request
bool ConnectRequestTransaction::start(Communicator* caller, int adapter_id) {
  if(ConnectRequestTransaction::sIsOngoing) {
    LOG_ERR("Only one transaction can run at the same time.");
    return;
  }
  ConnectRequestTransaction::sIsOngoing = true;
  ConnectRequestTransaction::sAdapterId = adapter_id;

  // Connect requested adapter
  ServerAdapter* adapter = caller->find_data_adapter_by_id(ConnectRequestTransaction::sAdapterId);
  if(adapter == NULL) {
    LOG_ERR("Connecting requested data adapter is failed 1");
    ConnectRequestTransaction::sIsOngoing = false;
    return;
  }
  bool res = adapter->connect(ConnectRequestTransaction::connect_callback);
  if(!res) {
    LOG_ERR("Connecting requested data adapter is failed 2");
    ConnectRequestTransaction::sIsOngoing = false;
    return;
  }
}

void ConnectRequestTransaction::connect_callback(bool is_success) {
  if(!is_success) {
    LOG_ERR("Connecting requested data adapter is failed 3");
    ConnectRequestTransaction::sIsOngoing = false;
    return;
  }
  LOG_ERR("Connecting requested data adapter is done");
}

// Disconnect Request
bool DisconnectRequestTransaction::start(Communicator* caller, int adapter_id) {
  if(DisconnectRequestTransaction::sIsOngoing) {
    LOG_ERR("Only one transaction can run at the same time.");
    return;
  }
  DisconnectRequestTransaction::sIsOngoing = true;
  DisconnectRequestTransaction::sAdapterId = adapter_id;

  // Connect requested adapter
  ServerAdapter* adapter = caller->find_data_adapter_by_id(DisconnectRequestTransaction::sAdapterId);
  if(adapter == NULL) {
    LOG_ERR("Disconnecting requested data adapter is failed 1");
    DisconnectRequestTransaction::sIsOngoing = false;
    return;
  }
  bool res = adapter->disconnect(DisconnectRequestTransaction::disconnect_callback);
  if(!res) {
    LOG_ERR("Disconnecting requested data adapter is failed 2");
    DisconnectRequestTransaction::sIsOngoing = false;
    return;
  }
}

void DisconnectRequestTransaction::disconnect_callback(bool is_success) {
  if(!is_success) {
    LOG_ERR("Disconnecting requested data adapter is failed 3");
    DisconnectRequestTransaction::sIsOngoing = false;
    return;
  }
  LOG_ERR("Disconnecting requested data adapter is done");
}

// Reconnect Control Adapter
bool ReconnectControlAdapterTransaction::start(Communicator* caller) {
  if(ReconnectControlAdapterTransaction::sIsOngoing) {
    LOG_ERR("Only one transaction can run at the same time.");
    return;
  }
  ReconnectControlAdapterTransaction::sIsOngoing = true;

  // disconnect control adapter
  ServerAdapter* control_adapter = caller->get_control_adapter();
  if(control_adapter == NULL) {
    LOG_ERR("Reconnecting control adapter is failed 1: retry");
    ReconnectControlAdapterTransaction::sIsOngoing = false;
    ReconnectControlAdapterTransaction::start(ReconnectControlAdapterTransaction::sCaller);
    return;
  }
  bool res = adapter->disconnect(ReconnectControlAdapterTransaction::disconnect_callback);
  if(!res) {
    LOG_ERR("Reconnecting control adapter is failed 2: retry");
    ReconnectControlAdapterTransaction::sIsOngoing = false;
    ReconnectControlAdapterTransaction::start(ReconnectControlAdapterTransaction::sCaller);
    return;
  }
}

void ReconnectControlAdapterTransaction::disconnect_callback(bool is_success) {
  if(!is_success) {
    LOG_ERR("Reconnecting control adapter is failed 3: retry");
    ReconnectControlAdapterTransaction::sIsOngoing = false;
    ReconnectControlAdapterTransaction::start(ReconnectControlAdapterTransaction::sCaller);
    return;
  }
  // connect control adapter
  ServerAdapter* control_adapter = caller->get_control_adapter();
  if(control_adapter == NULL) {
    LOG_ERR("Reconnecting control adapter is failed 4: retry");
    ReconnectControlAdapterTransaction::sIsOngoing = false;
    ReconnectControlAdapterTransaction::start(ReconnectControlAdapterTransaction::sCaller);
    return;
  }
  bool res = adapter->connect(ReconnectControlAdapterTransaction::disconnect_callback);
  if(!res) {
    LOG_ERR("Disconnecting control adapter is failed 5: retry");
    ReconnectControlAdapterTransaction::sIsOngoing = false;
    ReconnectControlAdapterTransaction::start(ReconnectControlAdapterTransaction::sCaller);
    return;
  }
}

void ReconnectControlAdapterTransaction::connect_callback(bool is_success) {
  if(!is_success) {
    LOG_ERR("Reconnecting control adapter is failed 6: retry");
    ReconnectControlAdapterTransaction::sIsOngoing = false;
    ReconnectControlAdapterTransaction::start(ReconnectControlAdapterTransaction::sCaller);
    return;
  }
  LOG_ERR("Reconnecting control adapter is done");
}

void SwitchAdapterTransaction::start(Communicator* caller, int prev_index, int next_index) {
  // Switch Step 2/4
  if(SwitchAdapterTransaction::sIsOngoing) {
    LOG_ERR("Only one transaction can run at the same time.");
    NetworkSwitcher::get_instance()->done_switch();
    return;
  }
  SwitchAdapterTransaction::sIsOngoing = true;
  SwitchAdapterTransaction::sPrevIndex = prev_index;
  SwitchAdapterTransaction::sNextIndex = next_index;

  // Connect next active adapter
  ServerAdapter* next_adapter = caller->get_data_adapter(SwitchAdapterTransaction::sNextIndex);
  if(next_adapter == NULL) {
    LOG_ERR("Connecting next data adapter is failed 1");
    NetworkSwitcher::get_instance()->done_switch();
    SwitchAdapterTransaction::sIsOngoing = false;
    return;
  }
  bool res = next_adapter->connect(SwitchAdapterTransaction::connect_callback);
  if(!res) {
    LOG_ERR("Connecting next data adapter is failed 2");
    NetworkSwitcher::get_instance()->done_switch();
    SwitchAdapterTransaction::sIsOngoing = false;
    return;
  }
}

void SwitchAdapterTransaction::connect_callback(bool is_success) {
  // Switch Step 3/4
  if(!is_success) {
    LOG_ERR("Connecting next data adapter is failed 3");
    NetworkSwitcher::get_instance()->done_switch();
    SwitchAdapterTransaction::sIsOngoing = false;
    return;
  }
  // Disconnect previous active adapter
  ServerAdapter* prev_adapter = caller->get_data_adapter(SwitchAdapterTransaction::sPrevIndex);
  if(next_adapter == NULL) {
    LOG_ERR("Disconnecting next data adapter is failed 1");
    NetworkSwitcher::get_instance()->done_switch();
    SwitchAdapterTransaction::sIsOngoing = false;
    return;
  }
  bool res = next_adapter->disconnect(SwitchAdapterTransaction::disconnect_callback);
  if(!res) {
    LOG_ERR("Disconnecting next data adapter is failed 2");
    NetworkSwitcher::get_instance()->done_switch();
    SwitchAdapterTransaction::sIsOngoing = false;
    return;
  }
}

void SwitchAdapterTransaction::disconnect_callback(bool is_success) {
  // Switch Step 4/4
  if(!is_success) {
    LOG_ERR("Connecting next data adapter is failed 3");
    NetworkSwitcher::get_instance()->done_switch();
    SwitchAdapterTransaction::sIsOngoing = false;
    return;
  }

  LOG_DEBUG("Switch from %d to %d done.",
      SwitchAdapterTransaction::sPrevIndex, SwitchAdapterTransaction::sNextIndex);
  NetworkSwitcher::get_instance()->done_switch();
  this->mActiveDataAdapterIndex = SwitchAdapterTransaction::sNextIndex;
  SwitchAdapterTransaction::sIsOngoing = false;
  return;
}

} /* namespace cm */
