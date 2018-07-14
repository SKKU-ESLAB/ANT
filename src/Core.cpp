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

#include <DebugLog.h>
#include <ProtocolManager.h>
#include <SegmentManager.h>
#include <NetworkSwitcher.h>

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>

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

Core* Core::singleton = NULL;

bool StartCoreTransaction::sIsOngoing = false;
Core* StartCoreTransaction::sCaller = NULL;
bool StopCoreTransaction::sIsOngoing = false;
Core* StopCoreTransaction::sCaller = NULL;
int StopCoreTransaction::sDataAdaptersCount = 0;
std::mutex StopCoreTransaction::sDataAdaptersCountLock;

// Start core: connect initial adapters
bool Core::start(void) {
  if(this->get_state() != CMState::kCMStateIdle) {
    LOG_ERR("Core has already started.");
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
  StartCoreTransaction::start(this);
  return true;
}

void Core::done_start(bool is_success) {
  if(!is_success) {
    LOG_ERR("Failed to start core!");
    this->set_state(CMState::kCMStateIdle);
    return;
  }

  LOG_VERB("Succeed to start core!");
  this->set_state(CMState::kCMStateReady);
}

// Stop core: disconnect all the adapters
bool Core::stop(void) {
  CMState state = this->get_state();
  if(state == CMState::kCMStateStarting ||
      state == CMState::kCMStateStopping) {
    LOG_ERR("Cannot stop core during starting/stopping!");
    return false;
  } else if(state == CMState::kCMStateIdle) {
    LOG_ERR("core is already idle state!");
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
  StopCoreTransaction::start(this);
  return true;
}

void Core::done_stop(bool is_success) {
  if(!is_success) {
    LOG_ERR("Failed to stop core!");
    this->set_state(CMState::kCMStateReady);
  }

  LOG_VERB("Succeed to stop core!");
  this->set_state(CMState::kCMStateIdle);
}

// Register control adpater
void Core::register_control_adapter(ServerAdapter* control_adapter) {
  if(this->get_state() != CMState::kCMStateIdle) {
    LOG_ERR("You can register control adapter on only idle state!");
    return;
  }
  std::unique_lock<std::mutex> lck(this->mControlAdapterLock);
  if(this->mControlAdapter != NULL) {
    LOG_ERR("Another control adapter has already been registered!");
    return;
  }
  control_adapter->enable_receiver_thread(Core::receive_control_message_loop);
  this->mControlAdapter = control_adapter;
}

// Register data adpater
void Core::register_data_adapter(ServerAdapter* data_adapter) {
  if(this->get_state() != CMState::kCMStateIdle) {
    LOG_ERR("You can register data adapter on only idle state!");
    return;
  }
  std::unique_lock<std::mutex> lck(this->mDataAdaptersLock);
  data_adapter->enable_receiver_thread(NULL);
  data_adapter->enable_sender_thread();
  this->mDataAdapters.push_back(data_adapter);
  this->mDataAdapterCount++;
}

int Core::send(const void *dataBuffer, uint32_t dataLength) {
  CMState state = this->get_state();
  if(state != CMState::kCMStateReady) {
    LOG_ERR("Core is not started yet, so you cannot send the data");
    return -1;
  }
  uint32_t curr_offset = 0;
  int sent_bytes;
  ProtocolData pd;
  uint8_t *serialized_vector;
  uint32_t packet_size;

  // Attach the protocol header to the payload
  ProtocolManager::data_to_protocol_data((const uint8_t *) dataBuffer, dataLength, &pd);
  // The serialized_vector buffer is allocated in here
  packet_size = ProtocolManager::serialize(&pd,
                                           (const uint8_t *)dataBuffer,
                                           curr_offset,
                                           dataLength,
                                           &serialized_vector);
  assert(serialized_vector != NULL && packet_size > 0);

  // Hand over the data to the Protocol Manager
  sent_bytes = ProtocolManager::send_packet(serialized_vector, packet_size);
  if (unlikely(sent_bytes < 0)) {
    LOG_ERR("Sending stopped(%u/%u) by %d",
                 curr_offset,
                 dataLength,
                 sent_bytes);
  } 
  return sent_bytes;
}

int Core::receive(void **pDataBuffer) {
  CMState state = this->get_state();
  if(state != CMState::kCMStateReady) {
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

void Core::send_control_message(const void *data, size_t len) {
  CMState state = this->get_state();
  if(state != CMState::kCMStateReady) {
    LOG_ERR("Core is not started yet, so you cannot send the data");
    return;
  }
  ServerAdapter *control_adapter = this->get_control_adapter();
  control_adapter->send(data, len);
}

void Core::send_request_connect(uint16_t adapter_id) {
  CMState state = this->get_state();
  if(state != CMState::kCMStateReady) {
    LOG_ERR("Core is not started yet, so you cannot send the data");
    return;
  }

  uint8_t request_code = kCtrlReqConnect;
  uint16_t net_adapter_id = htons(adapter_id);

  this->send_control_message(&request_code, 1);
  this->send_control_message(&net_adapter_id, 2);
}

void Core::send_noti_private_data(uint16_t adapter_id,
    char* private_data_buf, uint32_t private_data_len) {
  CMState state = this->get_state();
  if(state != CMState::kCMStateReady) {
    LOG_ERR("Core is not started yet, so you cannot send the data");
    return;
  }
  uint8_t request_code = kCtrlReqPriv;
  uint16_t net_adapter_id = htons(adapter_id);
  uint32_t net_private_data_len = htonl(private_data_len);

  this->send_control_message(&request_code, 1);
  this->send_control_message(&net_adapter_id, 2);
  this->send_control_message(&net_private_data_len, 4);
  this->send_control_message(private_data_buf, private_data_len);
}

void Core::receive_control_message_loop(ServerAdapter* adapter) {
  assert(adapter != NULL);
  LOG_VERB("Ready to receive control message");
  
  char data[512] = {0, };
  int res = 0;

  Core* core = Core::get_instance();
  while (true) {
    // Receive 1Byte: Control Request Code
    res = adapter->receive(data, 1);
    if (res <= 0) {
      LOG_VERB("Control adapter could be closed");
      sleep(1);
      continue;
    }

    /*  If the control message is 'connect adapter', */
    if (data[0] == CtrlReq::kCtrlReqConnect) {
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

      LOG_DEBUG("Data Adapter Connect request arrived");
      NetworkSwitcher::get_instance()->connect_adapter(adapter_id);
    } else if (data[0] == CtrlReq::kCtrlReqPriv) {
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
        for(std::vector<ControlMessageListener*>::iterator it = core->mControlMessageListeners.begin();
            it != core->mControlMessageListeners.end();
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
  NetworkSwitcher::get_instance()->reconnect_control_adapter();
}

// Transactions ----------------------------------------------------------------------------------

// Start Core
bool StartCoreTransaction::start(Core* caller) {
  if(StartCoreTransaction::sIsOngoing) {
    LOG_ERR("Only one transaction can run at the same time.");
    StartCoreTransaction::sCaller->done_start(false);
    return false;
  }
  StartCoreTransaction::sIsOngoing = true;
  StartCoreTransaction::sCaller = caller;

  // Connect control adpater
  {
    std::unique_lock<std::mutex> lck(caller->mControlAdapterLock);
    bool res = caller->mControlAdapter->connect(StartCoreTransaction::connect_control_adapter_callback, false);
    if(!res) {
      LOG_ERR("Connecting control adapter is failed");
      StartCoreTransaction::sIsOngoing = false;
      caller->done_start(false);
      return false;
    }
  }
  return true;
}

void StartCoreTransaction::connect_control_adapter_callback(bool is_success) {
  Core* caller = StartCoreTransaction::sCaller;
  if(!is_success) {
    LOG_ERR("Connecting control adapter is failed 2");
    StartCoreTransaction::sIsOngoing = false;
    caller->done_start(false);
    return;
  }

  // Connect first data adapter
  {
    std::unique_lock<std::mutex> lck(caller->mDataAdaptersLock);
    bool res = caller->mDataAdapters.front()->connect(StartCoreTransaction::connect_first_data_adapter_callback, true);
    if(!res) {
      LOG_ERR("Connecting first data adapter is failed");
      StartCoreTransaction::sIsOngoing = false;
      caller->done_start(false);
      return;
    }
  }
}

void StartCoreTransaction::connect_first_data_adapter_callback(bool is_success) {
  Core* caller = StartCoreTransaction::sCaller;
  if(!is_success) {
    LOG_ERR("Connecting first data adapter is failed 2");
    StartCoreTransaction::sIsOngoing = false;
    caller->done_start(false);
    return;
  }

  // Done transaction
  StartCoreTransaction::sIsOngoing = false;
  caller->done_start(true);
}

// Stop Core
bool StopCoreTransaction::start(Core* caller) {
  if(StopCoreTransaction::sIsOngoing) {
    LOG_ERR("Only one transaction can run at the same time.");
    StopCoreTransaction::sCaller->done_stop(false);
    return false;
  }
  StopCoreTransaction::sIsOngoing = true;
  StopCoreTransaction::sCaller = caller;
  {
    std::unique_lock<std::mutex> lck(StopCoreTransaction::sDataAdaptersCountLock);
    StopCoreTransaction::sDataAdaptersCount
      = caller->get_data_adapter_count();
  }

  // Disconnect control adpater
  {
    std::unique_lock<std::mutex> lck(caller->mControlAdapterLock);
    bool res = caller->mControlAdapter->disconnect(StopCoreTransaction::disconnect_control_adapter_callback);
    if(!res) {
      LOG_ERR("Connecting control adapter is failed");
      StopCoreTransaction::sIsOngoing = false;
      caller->done_stop(false);
      return false;
    }
  }
  return true;
}

void StopCoreTransaction::disconnect_control_adapter_callback(bool is_success) {
  Core* caller = StopCoreTransaction::sCaller; 
  if(!is_success) {
    LOG_ERR("Connecting control adapter is failed 2");
    StopCoreTransaction::sIsOngoing = false;
    caller->done_stop(false);
    return;
  }

  // Disconnect all data adapters
  {
    std::unique_lock<std::mutex> lck(caller->mDataAdaptersLock);
    for(std::vector<ServerAdapter*>::iterator it = caller->mDataAdapters.begin();
        it != caller->mDataAdapters.end();
        it++) {
      ServerAdapter* data_adapter = *it;
      bool res = data_adapter->disconnect(StopCoreTransaction::disconnect_data_adapter_callback);
      if(!res) {
        LOG_ERR("Disconnecting data adapter is failed");
      }
    }
  }
}

void StopCoreTransaction::disconnect_data_adapter_callback(bool is_success) {
  Core* caller = StopCoreTransaction::sCaller; 
  if(!is_success) {
    LOG_ERR("Connecting first data adapter is failed 2");
    StopCoreTransaction::sIsOngoing = false;
    caller->done_stop(false);
    return;
  }

  bool done_disconnect_all = false;
  {
    std::unique_lock<std::mutex> lck(StopCoreTransaction::sDataAdaptersCountLock);
    StopCoreTransaction::sDataAdaptersCount--;
    if(StopCoreTransaction::sDataAdaptersCount == 0) {
      done_disconnect_all = true;
    }
  }

  // Done transaction
  if(done_disconnect_all) {
    StopCoreTransaction::sIsOngoing = false;
    caller->done_stop(true);
  }
}

} /* namespace cm */
