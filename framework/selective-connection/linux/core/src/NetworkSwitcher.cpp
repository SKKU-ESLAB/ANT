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

#include "../inc/NetworkSwitcher.h"

#include "../inc/Core.h"
#include "../inc/SegmentManager.h"
#include "../inc/ServerAdapter.h"

#include "../../common/inc/DebugLog.h"

#include "../../configs/ExpConfig.h"
#include "../../configs/NetworkSwitcherConfig.h"

#include <string.h>
#include <thread>
#include <unistd.h>

using namespace sc;

NetworkSwitcher *NetworkSwitcher::sSingleton = NULL;

SwitchAdapterTransaction *SwitchAdapterTransaction::sOngoing = NULL;
ConnectRequestTransaction *ConnectRequestTransaction::sOngoing = NULL;
ReconnectAdapterTransaction *ReconnectAdapterTransaction::sOngoing = NULL;

void NetworkSwitcher::connect_adapter_by_peer(int adapter_id) {
  NSState state = this->get_state();
  if (state == NSState::kNSStateSwitching) {
    LOG_WARN("connect(adapter %d): FAILED - already switching", adapter_id);
    return;
  }

  this->set_state(NSState::kNSStateSwitching);
  ConnectRequestTransaction::run(adapter_id);
}

void NetworkSwitcher::disconnect_adapter_by_peer(int adapter_id,
                                                 uint32_t last_seq_no_control,
                                                 uint32_t last_seq_no_data) {
  NSState state = this->get_state();
  if (state == NSState::kNSStateSwitching) {
    LOG_WARN("disconnect(adapter %d): FAILED - already switching", adapter_id);
    return;
  }

  Core *core = Core::singleton();
  ServerAdapter *adapter = core->find_adapter_by_id(adapter_id);
  if (adapter == NULL) {
    LOG_WARN("Cannot find adapter %d", (int)adapter_id);
    return;
  }

  this->set_state(NSState::kNSStateSwitching);

  adapter->disconnect_on_peer_command(NULL, last_seq_no_control,
                                      last_seq_no_data);
}

void NetworkSwitcher::sleep_adapter_by_peer(int adapter_id) {
  NSState state = this->get_state();
  if (state == NSState::kNSStateSwitching) {
    LOG_VERB("It's now switching. Cannot connect to adapter %d.", adapter_id);
    return;
  }

  ServerAdapter *adapter = Core::singleton()->get_adapter(adapter_id);
  if (adapter != NULL) {
    this->set_state(NSState::kNSStateSwitching);
    adapter->sleep(NULL, false);
    this->set_state(NSState::kNSStateReady);
  }
}

void NetworkSwitcher::wake_up_adapter_by_peer(int adapter_id) {
  NSState state = this->get_state();
  if (state == NSState::kNSStateSwitching) {
    LOG_VERB("It's now switching. Cannot connect to adapter %d.", adapter_id);
    return;
  }

  ServerAdapter *adapter = Core::singleton()->get_adapter(adapter_id);
  if (adapter != NULL) {
    this->set_state(NSState::kNSStateSwitching);
    adapter->wake_up(NULL, false);
    this->set_state(NSState::kNSStateReady);
  }
}

void NetworkSwitcher::reconnect_adapter(ServerAdapter *adapter,
                                        bool retry_if_already_switching) {
  /* Wait until other adapter is disconnected */
  NSState state = this->get_state();
  if (retry_if_already_switching && state == NSState::kNSStateSwitching) {
    LOG_VERB("It's now switching. Cannot reconnect adapter %s",
             adapter->get_name());
    sleep(1);
    this->reconnect_adapter(adapter, retry_if_already_switching);
    return;
  }

  /* Start to reconnect the adapter */
  this->set_state(NSState::kNSStateSwitching);
  ReconnectAdapterTransaction::run(adapter);
}

bool NetworkSwitcher::switch_adapters(int prev_index, int next_index) {
  // Switch Step 0
  this->set_state(NSState::kNSStateSwitching);
  return SwitchAdapterTransaction::run(prev_index, next_index);
}

void NetworkSwitcher::done_switch() {
  LOG_IMP("Switch adapter end!");
  NSState state = this->get_state();
  switch (state) {
  case NSState::kNSStateSwitching:
    this->set_state(NSState::kNSStateReady);
    break;
  case NSState::kNSStateReady:
    break;
  }
}

bool SwitchAdapterTransaction::run(int prev_index, int next_index) {
  if (sOngoing == NULL) {
    sOngoing = new SwitchAdapterTransaction(prev_index, next_index);
    LOG_VERB("Switch (%d->%d): START", prev_index, next_index);
    sOngoing->start();
    return true;
  } else {
    LOG_WARN("Already starting core");
    NetworkSwitcher *switcher = NetworkSwitcher::singleton();
    switcher->done_switch();
    return false;
  }
}

void SwitchAdapterTransaction::done(bool is_success) {
  // SWITCH TIME: disconnect end
  this->setDisconnectEndTS();
  uint64_t totalLatency = this->getTotalSwitchLatency();
  uint64_t connectLatency = this->getConnectLatency();
  uint64_t sleepLatency = this->getSleepLatency();
  uint64_t disconnectLatency = this->getDisconnectLatency();

  if (is_success) {
    LOG_IMP("Switch (%d->%d): SUCCESS (%lluus = %lluus + %lluus + %lluus)",
            sOngoing->mPrevIndex, sOngoing->mNextIndex, totalLatency,
            connectLatency, sleepLatency, disconnectLatency);
    Core::singleton()->set_active_adapter_index(sOngoing->mNextIndex);
  } else {
    LOG_ERR("Switch (%d->%d): FAILED (%lluus = %lluus + %lluus + %lluus)",
            sOngoing->mPrevIndex, sOngoing->mNextIndex, totalLatency,
            connectLatency, sleepLatency, disconnectLatency);
  }
  NetworkSwitcher::singleton()->done_switch();
  sOngoing = NULL;
}

void SwitchAdapterTransaction::start(void) {
  // Switch Step 0, 1-a
  Core *core = Core::singleton();

  // SWITCH TIME: connect start
  this->setConnectStartTS();

  ServerAdapter *next_adapter = core->get_adapter(this->mNextIndex);
  if (next_adapter == NULL) {
    LOG_ERR("Connecting next adapter is failed");
    this->done(false);
    return;
  }

  ServerAdapterState adapter_state = next_adapter->get_state();
  if (adapter_state == ServerAdapterState::kActive) {
    LOG_WARN("Next adapter is already active");
    SwitchAdapterTransaction::connect_next_adapter_callback(NULL, true);
    return;
  } else if (adapter_state == ServerAdapterState::kConnecting ||
             adapter_state == ServerAdapterState::kDisconnecting ||
             adapter_state == ServerAdapterState::kGoingSleeping ||
             adapter_state == ServerAdapterState::kWakingUp) {
    LOG_ERR("Switching is already in progress");
    this->done(false);
    return;
  }

  LOG_VERB("Switch (%d->%d): STEP 1. %s Adapter ON", sOngoing->mPrevIndex,
           sOngoing->mNextIndex, next_adapter->get_name());

// Connect or wake up the next adapter
#ifdef EXP_DONT_SEND_CONNECT_CONTROL_MESSAGE
  // For debugging
  next_adapter->connect_or_wake_up(
      SwitchAdapterTransaction::connect_next_adapter_callback, false);
#else
  next_adapter->connect_or_wake_up(
      SwitchAdapterTransaction::connect_next_adapter_callback, true);
#endif
}

void SwitchAdapterTransaction::connect_next_adapter_callback(
    ServerAdapter *adapter, bool is_success) {
  // Switch Step 1-b, 2-a
  Core *core = Core::singleton();
  NetworkSwitcher *switcher = NetworkSwitcher::singleton();

  // SWITCH TIME: sleep start
  sOngoing->setSleepStartTS();

  // Check if the connection result is successful
  if (!is_success) {
    LOG_ERR("Connecting next adapter is failed");
    sOngoing->done(false);
    return;
  }

  // Get previous adapter
  ServerAdapter *prev_adapter = core->get_adapter(sOngoing->mPrevIndex);
  if (prev_adapter == NULL) {
    LOG_ERR("Disconnecting previous adapter is failed");
    sOngoing->done(false);
    return;
  }

  // Check previous adapter's state
  ServerAdapterState adapter_state = prev_adapter->get_state();
  if (adapter_state == ServerAdapterState::kDisconnected) {
    LOG_WARN("Prev adapter is already disconnected");
    // Proceed to Step 4
    sOngoing->done(true);
    return;
  } else if (adapter_state == ServerAdapterState::kSleeping ||
             adapter_state == ServerAdapterState::kGoingSleeping) {
    LOG_WARN("Prev adapter is already sleeping or going to sleeping");
    // Proceed to Step 3-a
    sOngoing->sleep_prev_adapter_callback(NULL, true);
  } else if (adapter_state == ServerAdapterState::kConnecting ||
             adapter_state == ServerAdapterState::kDisconnecting ||
             adapter_state == ServerAdapterState::kWakingUp) {
    // Failure
    LOG_ERR("Switching is already in progress");
    sOngoing->done(false);
    return;
  }

  LOG_VERB("Switch (%d->%d): STEP 2. %s Adapter SLEEP", sOngoing->mPrevIndex,
           sOngoing->mNextIndex, prev_adapter->get_name());

  // Disconnect or sleep previous adapter
  // TODO: Actually, it needs sleep request. For now, since sleep does not
  // support ACK message, sleep request message makes out-of-order of sleep()
  // and disconnect().
  prev_adapter->sleep(SwitchAdapterTransaction::sleep_prev_adapter_callback,
                      false);
}

void SwitchAdapterTransaction::sleep_prev_adapter_callback(
    ServerAdapter *adapter, bool is_success) {
  // Switch Step 2-b, 3-a
  Core *core = Core::singleton();
  NetworkSwitcher *switcher = NetworkSwitcher::singleton();

  // SWITCH TIME: disconnect start
  sOngoing->setDisconnectStartTS();

  if (!is_success) {
    LOG_ERR("Sleeping next adapter is failed");
    sOngoing->done(false);
    return;
  }

  ServerAdapter *prev_adapter = core->get_adapter(sOngoing->mPrevIndex);
  if (prev_adapter == NULL) {
    LOG_ERR("Sleeping previous adapter is failed");
    sOngoing->done(false);
    return;
  }

  ServerAdapterState adapter_state = prev_adapter->get_state();
  if (adapter_state == ServerAdapterState::kDisconnected) {
    // Proceed to Step 4
    LOG_WARN("Prev adapter is already disconnected");
    sOngoing->done(true);
    return;
  } else if (adapter_state == ServerAdapterState::kGoingSleeping) {
    // Wait until the adapter become sleeping state
    LOG_VERB("Wait for going to sleeping");
    do {
      adapter_state = prev_adapter->get_state();
      ::sleep(1);
    } while (adapter_state != ServerAdapterState::kSleeping);
  } else if (adapter_state == ServerAdapterState::kConnecting ||
             adapter_state == ServerAdapterState::kDisconnecting ||
             adapter_state == ServerAdapterState::kWakingUp) {
    // Failure
    LOG_ERR("Switching is already in progress");
    sOngoing->done(false);
    return;
  }

  LOG_VERB("Switch (%d->%d): STEP 3. %s Adapter OFF", sOngoing->mPrevIndex,
           sOngoing->mNextIndex, prev_adapter->get_name());

  prev_adapter->disconnect_on_command(
      SwitchAdapterTransaction::disconnect_prev_adapter_callback);
}

void SwitchAdapterTransaction::disconnect_prev_adapter_callback(
    ServerAdapter *adapter, bool is_success) {
  // Switch Step 3-b, 4
  if (!is_success) {
    LOG_ERR("Disconnecting prev adapter is failed");
    sOngoing->done(false);
    return;
  }

  // Switching adapter is successfully done!
  sOngoing->done(true);
}

bool ConnectRequestTransaction::run(int adapter_id) {
  if (sOngoing == NULL) {
    sOngoing = new ConnectRequestTransaction(adapter_id);
    sOngoing->start();
    return true;
  } else {
    LOG_WARN("Already starting core");
    NetworkSwitcher *switcher = NetworkSwitcher::singleton();
    switcher->done_switch();
    return false;
  }
}

void ConnectRequestTransaction::done() {
  NetworkSwitcher *switcher = NetworkSwitcher::singleton();
  switcher->done_switch();
  sOngoing = NULL;
}

// Connect Request
bool ConnectRequestTransaction::start() {
  // Connect requested adapter
  ServerAdapter *adapter =
      Core::singleton()->find_adapter_by_id(this->mAdapterId);
  if (adapter == NULL) {
    LOG_ERR("Connecting requested adapter is failed");
    this->done();
    return false;
  }
  adapter->connect(ConnectRequestTransaction::connect_callback, false);
  return true;
}

void ConnectRequestTransaction::connect_callback(ServerAdapter *adapter,
                                                 bool is_success) {
  if (!is_success) {
    LOG_ERR("Connecting requested adapter is failed");
    sOngoing->done();
    return;
  }
  LOG_VERB("Connecting requested adapter is done");
  sOngoing->done();
}

bool ReconnectAdapterTransaction::run(ServerAdapter *targetAdapter) {
  if (sOngoing == NULL) {
    sOngoing = new ReconnectAdapterTransaction(targetAdapter);
    sOngoing->start();
    return true;
  } else {
    LOG_WARN("Already starting core");
    NetworkSwitcher *switcher = NetworkSwitcher::singleton();
    switcher->done_switch();
    return false;
  }
}

void ReconnectAdapterTransaction::done(bool require_restart) {
  sOngoing = NULL;
  if (require_restart) {
    ReconnectAdapterTransaction::start();
  } else {
    sleep(1);

    NetworkSwitcher *switcher = NetworkSwitcher::singleton();
    switcher->done_switch();
  }
}

// Reconnect Adapter
bool ReconnectAdapterTransaction::start() {
  // disconnect adapter
  if (this->mTargetAdapter == NULL) {
    LOG_ERR("Reconnecting adapter is failed: retry");
    this->done(true);
    return false;
  }
  this->mTargetAdapter->disconnect_on_failure(
      ReconnectAdapterTransaction::disconnect_callback);
  return true;
}

void ReconnectAdapterTransaction::disconnect_callback(ServerAdapter *adapter,
                                                      bool is_success) {
  if (!is_success) {
    LOG_ERR("Reconnecting adapter is failed: retry");
    sOngoing->done(true);
    return;
  }
  // connect adapter
  if (sOngoing->mTargetAdapter == NULL) {
    LOG_ERR("Reconnecting adapter is failed: retry");
    sOngoing->done(true);
    return;
  }
  sOngoing->mTargetAdapter->connect(
      ReconnectAdapterTransaction::disconnect_callback, false);
}

void ReconnectAdapterTransaction::connect_callback(ServerAdapter *adapter,
                                                   bool is_success) {
  if (!is_success) {
    LOG_ERR("Reconnecting adapter is failed 6: retry");
    sOngoing->done(true);
    return;
  }
  LOG_VERB("Reconnecting adapter is done");
  NetworkSwitcher::singleton()->done_switch();
}