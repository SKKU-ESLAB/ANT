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

#include "../inc/Core.h"

#include "../inc/NetworkSwitcher.h"
#include "../inc/ProtocolManager.h"
#include "../inc/SegmentManager.h"

#include "../../common/inc/DebugLog.h"

#include "../../configs/ExpConfig.h"

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
using namespace sc;

Core *Core::sSingleton = NULL;

StartCoreTransaction *StartCoreTransaction::sOngoing = NULL;
StopCoreTransaction *StopCoreTransaction::sOngoing = NULL;

// Start core: connect initial adapter
void Core::start() {
  setlocale(LC_ALL, "");

  CoreState state = this->get_state();
  if (state != CoreState::kCoreStateIdle) {
    LOG_ERR("start(Core): FAILED - state (%d)", state);
    this->done_start(false);
    return;
  } else if (this->mAdapters.empty()) {
    LOG_ERR("start(Core): FAILED - no adapters");
    this->done_start(false);
    return;
  }

  // Connect first adapter
  this->set_state(CoreState::kCoreStateStarting);
  StartCoreTransaction::run(this);
}

void Core::done_start(bool is_success) {
  if (is_success) {
    LOG_VERB("start(Core): SUCCESS");
    this->set_state(CoreState::kCoreStateReady);

    // Launch control message receiving thread
    this->mControlMessageReceiver->start_receiving_thread();
  } else {
    LOG_ERR("start(Core): FAILED - error");
    this->set_state(CoreState::kCoreStateIdle);
    return;
  }

  // Execute callback
  start_sc_done(is_success);
}

// Stop core: disconnect all the adapters
void Core::stop() {
  CoreState state = this->get_state();
  if (state == CoreState::kCoreStateStarting ||
      state == CoreState::kCoreStateStopping ||
      state == CoreState::kCoreStateIdle) {
    LOG_ERR("done(Core): FAIL - state (%d)", state);
    this->done_stop(false);
    return;
  } else if (this->mAdapters.empty()) {
    LOG_ERR("done(Core): FAIL - no adapters");
    this->done_stop(false);
    return;
  }

  // Disconnect all the adapters
  this->set_state(CoreState::kCoreStateStopping);
  StopCoreTransaction::run(this);
}

void Core::done_stop(bool is_success) {
  if (is_success) {
    LOG_VERB("done(Core): SUCCESS");
    this->set_state(CoreState::kCoreStateIdle);

    // Finish control message receiving thread
    this->mControlMessageReceiver->stop_receiving_thread();
  } else {
    LOG_ERR("done(Core): FAIL - error");
    this->set_state(CoreState::kCoreStateReady);
  }

  stop_sc_done(is_success);
}

// Register adapter
void Core::register_adapter(ServerAdapter *adapter) {
  if (this->get_state() != CoreState::kCoreStateIdle) {
    LOG_ERR("You can register adapter on only idle state!");
    return;
  }
  std::unique_lock<std::mutex> lck(this->mAdaptersLock);
  this->mAdapters.push_back(adapter);

  adapter->launch_threads();
}

int Core::send(const void *dataBuffer, uint32_t dataLength, bool is_control) {
  CoreState state = this->get_state();
  if (state != CoreState::kCoreStateReady) {
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
  sent_bytes =
      ProtocolManager::send_packet(serialized_vector, packet_size, is_control);
  if (unlikely(sent_bytes < 0)) {
    LOG_ERR("Sending stopped(%u/%u) by %d", curr_offset, dataLength,
            sent_bytes);
  }
  free(serialized_vector);
  return sent_bytes;
}

int Core::receive(void **pDataBuffer, bool is_control) {
  CoreState state = this->get_state();
  if (state != CoreState::kCoreStateReady) {
    LOG_ERR("Core is not started yet, so you cannot receive data");
    return -1;
  }
  int ret;
  uint32_t packet_size;
  uint8_t *packet;

  packet_size = ProtocolManager::recv_packet(&packet, is_control);
  *pDataBuffer = packet;

  return packet_size;
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
  // Connect first adapter
  std::unique_lock<std::mutex> lck(this->mCaller->mAdaptersLock);

  if (this->mCaller->mAdapters.empty()) {
    LOG_ERR("Connecting adapter is failed");
    this->done(false);
    return;
  }

  this->mCaller->mAdapters.front()->connect(
      StartCoreTransaction::connect_first_adapter_callback, false);
}

void StartCoreTransaction::connect_first_adapter_callback(
    ServerAdapter *adapter, bool is_success) {
  if (is_success) {
    // Done transaction
    sOngoing->done(true);
  } else {
    LOG_ERR("Connecting first adapter is failed");
    sOngoing->done(false);
  }
}

void StartCoreTransaction::done(bool is_success) {
  // Execute StartCore callback
  this->mCaller->done_start(is_success);

  // Execute callback
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
  /* Disconnect all adapters */
  std::unique_lock<std::mutex> lck(this->mCaller->mAdaptersLock);

  /* Get active adapter count */
  this->mAdaptersCount = 0;
  for (std::vector<ServerAdapter *>::iterator it =
           this->mCaller->mAdapters.begin();
       it != this->mCaller->mAdapters.end(); it++) {
    ServerAdapter *adapter = *it;
    ServerAdapterState state = adapter->get_state();
    if (state != ServerAdapterState::kDisconnected &&
        state != ServerAdapterState::kDisconnecting) {
      this->mAdaptersCount++;
    }
  }

  /* Disconnect only active adapters */
  for (std::vector<ServerAdapter *>::iterator it =
           this->mCaller->mAdapters.begin();
       it != this->mCaller->mAdapters.end(); it++) {
    ServerAdapter *adapter = *it;
    ServerAdapterState state = adapter->get_state();
    if (state != ServerAdapterState::kDisconnected &&
        state != ServerAdapterState::kDisconnecting) {
      adapter->disconnect_on_command(
          StopCoreTransaction::disconnect_adapter_callback);
    }
  }
}

void StopCoreTransaction::disconnect_adapter_callback(ServerAdapter *adapter,
                                                      bool is_success) {
  if (!is_success) {
    if (sOngoing != NULL) {
      LOG_ERR("Disconnecting adapter is failed 2");
      sOngoing->done(false);
    }
    return;
  }

  adapter->finish_thread();

  /* check if all the adapters are disconnected */
  bool done_disconnect_all = false;
  {
    std::unique_lock<std::mutex> lck(sOngoing->mAdaptersCountLock);
    sOngoing->mAdaptersCount--;
    if (sOngoing->mAdaptersCount == 0) {
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