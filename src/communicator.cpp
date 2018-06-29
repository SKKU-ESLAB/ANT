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

void Communicator::register_control_adapter(ServerAdapter* adapter) {
  std::unique_lock<std::mutex> lck(this->mControlAdapterLock);
  if(this->mControlAdapter != NULL) {
    LOG_ERR("Another control adapter has already been registered!");
  }
  this->mControlAdapter = adapter;
}

void Communicator::register_data_adapter(ServerAdapter* adapter) {
  std::unique_lock<std::mutex> lck(this->mDataAdaptersLock);
  this->mDataAdapters.push_back(adapter);
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
    bool res = this->mControlAdapter->connect();
    if(!res) {
      LOG_ERR("Connecting control adapter is failed");
      return;
    }
  }

  // Connect first data adapter
  {
    std::unique_lock<std::mutex> lck(this->mDataAdaptersLock);
    this->mCurrentDataAdapter = 0;
    bool res = this->mDataAdapters.front()->connect();
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

  // Disconnect all data adapters
  {
    std::unique_lock<std::mutex> lck(this->mControlAdapterLock);
    bool res = this->mControlAdapter->disconnect();
    if(!res) {
      LOG_ERR("Disconnecting control adapter is failed");
    }
  }

  // Disconnect control adapter
  {
    std::unique_lock<std::mutex> lck(this->mDataAdaptersLock);
    for(std::vector<ServerAdapter*>::iterator it = this->mDataAdapter.begin();
        it != this->mDataAdapter.end();
        it++) {
      ServerAdapter* data_adapter = *it;
      bool res = data_adapter->disconnect();
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

  return this->mSwitchAdapterTransaction.start(this, prev_index, next_index);
}

void SwitchAdapterTransaction::start(Communicator* caller, int prev_index, int next_index) {
  // Switch Step 2/4
  if(this->mIsOngoing) {
    LOG_ERR("Only one transaction can run at the same time.");
    NetworkSwitcher::get_instance()->done_switch();
    return;
  }
  this->mIsOngoing = true;
  this->mPrevIndex = prev_index;
  this->mNextIndex = next_index;

  // Connect next active adapter
  ServerAdapter* next_adapter = caller->get_data_adapter(this->mNextIndex);
  if(next_adapter == NULL) {
    LOG_ERR("Connecting next data adapter is failed 1");
    NetworkSwitcher::get_instance()->done_switch();
    this->mIsOngoing = false;
    return;
  }
  bool res = next_adapter->connect();
  if(!res) {
    LOG_ERR("Connecting next data adapter is failed 2");
    NetworkSwitcher::get_instance()->done_switch();
    this->mIsOngoing = false;
    return;
  }
}

void SwitchAdapterTransaction::connect_callback(bool is_success) {
  // Switch Step 3/4
  if(!is_success) {
    LOG_ERR("Connecting next data adapter is failed 3");
    NetworkSwitcher::get_instance()->done_switch();
    this->mIsOngoing = false;
    return;
  }
  // Disconnect previous active adapter
  ServerAdapter* prev_adapter = caller->get_data_adapter(this->mPrevIndex);
  if(next_adapter == NULL) {
    LOG_ERR("Disconnecting next data adapter is failed 1");
    NetworkSwitcher::get_instance()->done_switch();
    this->mIsOngoing = false;
    return;
  }
  bool res = next_adapter->disconnect();
  if(!res) {
    LOG_ERR("Disconnecting next data adapter is failed 2");
    NetworkSwitcher::get_instance()->done_switch();
    this->mIsOngoing = false;
    return;
  }
}

void SwitchAdapterTransaction::disconnect_callback(bool is_success) {
  // Switch Step 4/4
  if(!is_success) {
    LOG_ERR("Connecting next data adapter is failed 3");
    NetworkSwitcher::get_instance()->done_switch();
    this->mIsOngoing = false;
    return;
  }

  LOG_DEBUG("Switch from %d to %d done.", this->mPrevIndex, this->mNextIndex);
  NetworkSwitcher::get_instance()->done_switch();
  this->mIsOngoing = false;
  return;
}

void Communicator::send_control_data(const void *data, size_t len) {
  // NetworkManager::send_control_data()
  // TODO:
}

} /* namespace cm */
