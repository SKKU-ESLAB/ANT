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

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <dbug_log.h>
#include <communicator.h>
#include <protocol_manager.h>
#include <segment_manager.h>
#include <network_manager.h>

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

int Communicator::send(const void *buf, uint32_t len) {
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
  int ret;
  uint32_t packet_size;
  uint8_t *packet;

  packet_size = ProtocolManager::recv_packet(&packet);
  *buf = packet;

  return packet_size;
}

void Communicator::register_control_adapter(ServerAdapter* control_adapter) {
  std::unique_lock<std::mutex> lck(this->mControlAdapterLock);
  if(this->mControlAdapter != NULL) {
    LOG_ERR("Another control adapter has already been registered!");
  }
  control_adapter->enable_receiver_thread(Communicator::receive_control_message_loop);
  this->mControlAdapter = control_adapter;
}

void Communicator::register_data_adapter(ServerAdapter* data_adapter) {
  std::unique_lock<std::mutex> lck(this->mDataAdaptersLock);
  data_adapter->enable_receiver_thread(NULL);
  data_adapter->enable_sender_thread();
  this->mDataAdapters.push_back(data_adapter);
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
    } else {
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
            // TODO: implement listener
            listener->on_receive_control_message(adapter_id, data, len);
          }
        } else {
          LOG_DEBUG("Control adapter closed");
          break;
        }
      }
    }
  } // End while

  ReconnectControlAdapterTransaction::start(this);
}

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

void Communicator::start(void) {
  if(this->mControlAdapter == NULL) {
    LOG_ERR("No control adapter is registered!");
    return;
  } else if (this->mDataAdapters.empty()) {
    LOG_ERR("No data adapter is registered!");
    return;
  }

  // Connect control adpater
  {
    std::unique_lock<std::mutex> lck(this->mControlAdapterLock);
    bool res = this->mControlAdapter->connect(NULL);
    if(!res) {
      LOG_ERR("Connecting control adapter is failed");
      return;
    }
  }

  // Connect first data adapter
  {
    std::unique_lock<std::mutex> lck(this->mDataAdaptersLock);
    this->mCurrentDataAdapter = 0;
    bool res = this->mDataAdapters.front()->connect(NULL);
    if(!res) {
      LOG_ERR("Connecting data adapter is failed");
      return;
    }
  }
}
void Communicator::stop(void) {
  if(this->mControlAdapter == NULL) {
    LOG_ERR("No control adapter is registered!");
    return;
  } else if (this->mDataAdapters.empty()) {
    LOG_ERR("No data adapter is registered!");
    return;
  }

  // Disconnect control adapter
  {
    std::unique_lock<std::mutex> lck(this->mControlAdapterLock);
    bool res = this->mControlAdapter->disconnect(NULL);
    if(!res) {
      LOG_ERR("Disconnecting control adapter is failed");
    }
  }

  // Disconnect all data adapters
  {
    std::unique_lock<std::mutex> lck(this->mDataAdaptersLock);
    for(std::vector<ServerAdapter*>::iterator it = this->mDataAdapter.begin();
        it != this->mDataAdapter.end();
        it++) {
      ServerAdapter* data_adapter = *it;
      bool res = data_adapter->disconnect(NULL);
      if(!res) {
        LOG_ERR("Disconnecting data adapter is failed");
      }
    }
  }
}

bool Communicator::increase_adapter(void) {
  // Increase active data adapter index
  if(this->mDataAdapters.empty()) {
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
  // Increase active data adapter index
  if(this->mDataAdapters.empty()) {
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
  // Switch Step 1/4
  // Increase/decrease active data adapter index
  this->mActiveDataAdapterIndex = next_index;

  return SwitchAdapterTransaction::start(this, prev_index, next_index);
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

void Communicator::send_control_message(const void *data, size_t len) {
  ServerAdapter *control_adapter = this->get_control_adapter();
  control_adapter->send(data, len);
}

void Communicator::send_private_control_data(uint8_t request_code,
    uint16_t adapter_id, uint32_t private_data_len) {
  uint8_t req = kCtrlReqPriv;
  uint16_t net_adapter_id = htons(adapter_id);
  uint32_t net_private_datalen = htonl(private_data_len);

  this->send_control_message(&request_code, 1);
  this->send_control_message(&net_adapter_id, 2);
  this->send_control_message(&net_private_data_len, 4);
  this->send_control_message(buf, len);
}

} /* namespace cm */
