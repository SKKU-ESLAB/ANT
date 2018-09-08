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

#include <NetworkSwitcher.h>

#include <ExpConfig.h>
#include <NetworkSwitcherConfig.h>

#include <Core.h>
#include <DebugLog.h>
#include <SegmentManager.h>
#include <ServerAdapter.h>

#include <string.h>
#include <thread>
#include <unistd.h>

namespace sc {
NetworkSwitcher *NetworkSwitcher::singleton = NULL;

SwitchAdapterTransaction *SwitchAdapterTransaction::sOngoing = NULL;
ConnectRequestTransaction *ConnectRequestTransaction::sOngoing = NULL;
ReconnectControlAdapterTransaction
    *ReconnectControlAdapterTransaction::sOngoing = NULL;

void NetworkSwitcher::connect_adapter(int adapter_id) {
  NSState state = this->get_state();
  if (state == NSState::kNSStateSwitching) {
    LOG_VERB("It's now switching. Cannot connect to adapter %d.", adapter_id);
    return;
  }

  this->set_state(NSState::kNSStateSwitching);
  ConnectRequestTransaction::run(adapter_id);
}

void NetworkSwitcher::sleep_adapter(int adapter_id) {
  NSState state = this->get_state();
  if (state == NSState::kNSStateSwitching) {
    LOG_VERB("It's now switching. Cannot connect to adapter %d.", adapter_id);
    return;
  }

  ServerAdapter *adapter = Core::get_instance()->get_data_adapter(adapter_id);
  if (adapter != NULL) {
    this->set_state(NSState::kNSStateSwitching);
    adapter->sleep(NULL, false);
    this->set_state(NSState::kNSStateReady);
  }
}

void NetworkSwitcher::wake_up_adapter(int adapter_id) {
  NSState state = this->get_state();
  if (state == NSState::kNSStateSwitching) {
    LOG_VERB("It's now switching. Cannot connect to adapter %d.", adapter_id);
    return;
  }

  ServerAdapter *adapter = Core::get_instance()->get_data_adapter(adapter_id);
  if (adapter != NULL) {
    this->set_state(NSState::kNSStateSwitching);
    adapter->wake_up(NULL, false);
    this->set_state(NSState::kNSStateReady);
  }
}

void NetworkSwitcher::reconnect_control_adapter(void) {
  NSState state = this->get_state();
  if (state == NSState::kNSStateSwitching) {
    LOG_VERB("It's now switching. Cannot reconnect control adapter.");
    sleep(1);
    this->reconnect_control_adapter();
    return;
  }
  this->set_state(NSState::kNSStateSwitching);
  ReconnectControlAdapterTransaction::run();
}

bool NetworkSwitcher::switch_adapters(int prev_index, int next_index) {
  // Switch Step 1
  // Increase/decrease active data adapter index
  Core::get_instance()->set_active_adapter_index(next_index);

  return SwitchAdapterTransaction::run(prev_index, next_index);
}

bool SwitchAdapterTransaction::run(int prev_index, int next_index) {
  if (sOngoing == NULL) {
    sOngoing = new SwitchAdapterTransaction(prev_index, next_index);
    sOngoing->start();
    return true;
  } else {
    LOG_WARN("Already starting core");
    NetworkSwitcher *switcher = NetworkSwitcher::get_instance();
    switcher->done_switch();
    return false;
  }
}

void SwitchAdapterTransaction::done(bool is_success) {
  if (is_success) {
    LOG_DEBUG("Switch from %d to %d done.", sOngoing->mPrevIndex,
              sOngoing->mNextIndex);
    Core::get_instance()->set_active_adapter_index(sOngoing->mNextIndex);
  } else {
    LOG_DEBUG("Switch from %d to %d failed.", sOngoing->mPrevIndex,
              sOngoing->mNextIndex);
  }
  NetworkSwitcher::get_instance()->done_switch();
  sOngoing = NULL;
}

void SwitchAdapterTransaction::start(void) {
  // Switch Step 1, 2-a
  Core *core = Core::get_instance();

  ServerAdapter *next_data_adapter = core->get_data_adapter(this->mNextIndex);
  if (next_data_adapter == NULL) {
    LOG_ERR("Connecting next data adapter is failed");
    this->done(false);
    return;
  }

  ServerAdapterState adapter_state = next_data_adapter->get_state();
  if (adapter_state == ServerAdapterState::kActive) {
    LOG_ERR("Next data adapter is already active");
    this->done(false);
    return;
  } else if (adapter_state == ServerAdapterState::kConnecting ||
             adapter_state == ServerAdapterState::kDisconnecting ||
             adapter_state == ServerAdapterState::kGoingSleeping ||
             adapter_state == ServerAdapterState::kWakingUp) {
    LOG_ERR("Switching is already in progress");
    this->done(false);
    return;
  }

  // Connect or wake up the next data adapter
  next_data_adapter->connect_or_wake_up(
      SwitchAdapterTransaction::connect_next_data_callback, true);
}

void SwitchAdapterTransaction::connect_next_data_callback(bool is_success) {
  // Switch Step 2-b, 3-a
  Core *core = Core::get_instance();
  NetworkSwitcher *switcher = NetworkSwitcher::get_instance();
  if (!is_success) {
    LOG_ERR("Connecting next data adapter is failed");
    sOngoing->done(false);
    return;
  }

  ServerAdapter *prev_data_adapter =
      core->get_data_adapter(sOngoing->mPrevIndex);
  if (prev_data_adapter == NULL) {
    LOG_ERR("Disconnecting previous data adapter is failed");
    sOngoing->done(false);
    return;
  }

  ServerAdapterState adapter_state = prev_data_adapter->get_state();
  if (adapter_state == ServerAdapterState::kDisconnected) {
    LOG_ERR("Prev data adapter is already disconnected");
    sOngoing->done(false);
    return;
  } else if (adapter_state == ServerAdapterState::kConnecting ||
             adapter_state == ServerAdapterState::kDisconnecting ||
             adapter_state == ServerAdapterState::kGoingSleeping ||
             adapter_state == ServerAdapterState::kWakingUp) {
    LOG_ERR("Switching is already in progress");
    sOngoing->done(false);
    return;
  }

  // Disconnect or sleep previous data adapter
  prev_data_adapter->disconnect_or_sleep(
      SwitchAdapterTransaction::disconnect_prev_data_callback, true);
}

void SwitchAdapterTransaction::disconnect_prev_data_callback(bool is_success) {
  // Switch Step 3-b, 4-a
  Core *core = Core::get_instance();
  NetworkSwitcher *switcher = NetworkSwitcher::get_instance();
  if (!is_success) {
    LOG_ERR("Disconnecting previous data adapter is failed");
    sOngoing->done(false);
    return;
  }

  ServerAdapter *next_control_adapter =
      core->get_control_adapter(sOngoing->mNextIndex);
  if (next_control_adapter == NULL) {
    LOG_ERR("Connecting next control adpater is failed");
    sOngoing->done(false);
    return;
  }

  ServerAdapterState adapter_state = next_control_adapter->get_state();
  if (adapter_state == ServerAdapterState::kActive) {
    LOG_ERR("Next control adapter is already active");
    sOngoing->done(false);
    return;
  } else if (adapter_state == ServerAdapterState::kConnecting ||
             adapter_state == ServerAdapterState::kDisconnecting ||
             adapter_state == ServerAdapterState::kGoingSleeping ||
             adapter_state == ServerAdapterState::kWakingUp) {
    LOG_ERR("Switching is already in progress");
    sOngoing->done(false);
    return;
  }

  next_control_adapter->connect_or_wake_up(
      SwitchAdapterTransaction::connect_next_control_callback, true);
}

void SwitchAdapterTransaction::connect_next_control_callback(bool is_success) {
  // Switch Step 4-b, 5-a
  Core *core = Core::get_instance();
  NetworkSwitcher *switcher = NetworkSwitcher::get_instance();
  if (!is_success) {
    LOG_ERR("Connecting next control adapter is failed");
    sOngoing->done(false);
    return;
  }

  ServerAdapter *prev_control_adapter =
      core->get_control_adapter(sOngoing->mPrevIndex);
  if (prev_control_adapter == NULL) {
    LOG_ERR("Disconnecting prev control adapter is failed");
    sOngoing->done(false);
    return;
  }

  ServerAdapterState adapter_state = prev_control_adapter->get_state();
  if (adapter_state == ServerAdapterState::kDisconnected) {
    LOG_ERR("Next control adapter is already disconnected");
    sOngoing->done(false);
    return;
  } else if (adapter_state == ServerAdapterState::kConnecting ||
             adapter_state == ServerAdapterState::kDisconnecting ||
             adapter_state == ServerAdapterState::kGoingSleeping ||
             adapter_state == ServerAdapterState::kWakingUp) {
    LOG_ERR("Switching is already in progress");
    sOngoing->done(false);
    return;
  }

  prev_control_adapter->disconnect_or_sleep(
      SwitchAdapterTransaction::disconnect_prev_control_callback, true);
}

void SwitchAdapterTransaction::disconnect_prev_control_callback(
    bool is_success) {
  // Switch Step 5-b, 6
  if (!is_success) {
    LOG_ERR("Disconnecting prev control adapter is failed");
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
    NetworkSwitcher *switcher = NetworkSwitcher::get_instance();
    switcher->done_switch();
    return false;
  }
}

void ConnectRequestTransaction::done() {
  NetworkSwitcher *switcher = NetworkSwitcher::get_instance();
  switcher->done_switch();
  sOngoing = NULL;
}

// Connect Request
bool ConnectRequestTransaction::start() {
  // Connect requested adapter
  ServerAdapter *adapter =
      Core::get_instance()->find_data_adapter_by_id(this->mAdapterId);
  if (adapter == NULL) {
    LOG_ERR("Connecting requested data adapter is failed");
    this->done();
    return false;
  }
  adapter->connect(ConnectRequestTransaction::connect_callback, false);
  return true;
}

void ConnectRequestTransaction::connect_callback(bool is_success) {
  if (!is_success) {
    LOG_ERR("Connecting requested data adapter is failed");
    sOngoing->done();
    return;
  }
  LOG_VERB("Connecting requested data adapter is done");
  sOngoing->done();
}

bool ReconnectControlAdapterTransaction::run() {
  if (sOngoing == NULL) {
    sOngoing = new ReconnectControlAdapterTransaction();
    sOngoing->start();
    return true;
  } else {
    LOG_WARN("Already starting core");
    NetworkSwitcher *switcher = NetworkSwitcher::get_instance();
    switcher->done_switch();
    return false;
  }
}

void ReconnectControlAdapterTransaction::done(bool require_restart) {
  sOngoing = NULL;
  if (require_restart) {
    ReconnectControlAdapterTransaction::start();
  } else {
    NetworkSwitcher *switcher = NetworkSwitcher::get_instance();
    switcher->done_switch();
  }
}

// Reconnect Control Adapter
bool ReconnectControlAdapterTransaction::start() {
  // disconnect control adapter
  ServerAdapter *control_adapter =
      Core::get_instance()->get_active_control_adapter();
  if (control_adapter == NULL) {
    LOG_ERR("Reconnecting control adapter is failed: retry");
    this->done(true);
    return false;
  }
  control_adapter->disconnect(
      ReconnectControlAdapterTransaction::disconnect_callback);
  return true;
}

void ReconnectControlAdapterTransaction::disconnect_callback(bool is_success) {
  if (!is_success) {
    LOG_ERR("Reconnecting control adapter is failed: retry");
    sOngoing->done(true);
    return;
  }
  // connect control adapter
  ServerAdapter *control_adapter =
      Core::get_instance()->get_active_control_adapter();
  if (control_adapter == NULL) {
    LOG_ERR("Reconnecting control adapter is failed: retry");
    sOngoing->done(true);
    return;
  }
  control_adapter->connect(
      ReconnectControlAdapterTransaction::disconnect_callback, false);
}

void ReconnectControlAdapterTransaction::connect_callback(bool is_success) {
  if (!is_success) {
    LOG_ERR("Reconnecting control adapter is failed 6: retry");
    sOngoing->done(true);
    return;
  }
  LOG_VERB("Reconnecting control adapter is done");
  NetworkSwitcher::get_instance()->done_switch();
}

} /* namespace sc */
