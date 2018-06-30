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
          ConnectTransaction::start(this, adapter_id);
        } else if (data[0] == kCtrlReqDisconnect) {
          LOG_DEBUG("Data Adapter Disconnect request arrived");
          DisconnectTransaction::start(this, adapter_id);
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

  // Disconnect control adapter
  {
    std::unique_lock<std::mutex> lck(this->mControlAdapterLock);
    // TODO: callback
    bool res = this->mControlAdapter->disconnect(NULL);
    if(!res) {
      LOG_ERR("Disconnecting control adapter is failed");
    }
  }

  // Connect control adpater
  // TODO: make in callback
  {
    std::unique_lock<std::mutex> lck(this->mControlAdapterLock);
    bool res = this->mControlAdapter->connect(NULL);
    if(!res) {
      LOG_ERR("Connecting control adapter is failed");
      return;
    }
  }
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
    // TODO: callback
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
    // TODO: callback
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
    // TODO: callback
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
      // TODO: callback
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
  }
  int prev_index = this->mActiveDataAdapterIndex;
  int next_index = (this->mActiveDataAdapterIndex + 1) % this->mDataAdapters.size();

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
  }
  int prev_index = this->mActiveDataAdapterIndex;
  int next_index = (this->mActiveDataAdapterIndex - 1) % this->mDataAdapters.size();

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
  SwitchAdapterTransaction::sIsOngoing = false;
  return;
}

void Communicator::send_control_data(const void *data, size_t len) {
  // NetworkManager::send_control_data()
  // TODO:
}

} /* namespace cm */
