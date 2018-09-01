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

#include <Core.h>
#include <DebugLog.h>
#include <SegmentManager.h>
#include <ServerAdapter.h>

#include <thread>
#include <unistd.h>

namespace sc {
NetworkSwitcher *NetworkSwitcher::singleton = NULL;

SwitchAdapterTransaction *SwitchAdapterTransaction::sOngoing = NULL;
ConnectRequestTransaction *ConnectRequestTransaction::sOngoing = NULL;
ReconnectControlAdapterTransaction
    *ReconnectControlAdapterTransaction::sOngoing = NULL;

void NetworkSwitcher::start(void) {
  this->mSwitcherThreadOn = true;
  this->mThread =
      new std::thread(std::bind(&NetworkSwitcher::switcher_thread, this));
  this->mThread->detach();
}

void NetworkSwitcher::stop(void) { this->mSwitcherThreadOn = false; }

void NetworkSwitcher::switcher_thread(void) {
  this->set_state(NSState::kNSStateRunning);

  int count = 0;
  while (this->mSwitcherThreadOn) {
    // Get statistics
    Stats stats;
    this->get_stats(stats);
    float avg_arrival_time_sec = stats.ema_arrival_time_us / 1000000;

    // Get present state
    NSState state = this->get_state();

    // Print present state & statistics
    char state_str[512];
    switch (state) {
    case NSState::kNSStateInitialized:
      strncpy(state_str, "Initialized", 512);
      break;
    case NSState::kNSStateRunning:
      strncpy(state_str, "Ready", 512);
      break;
    case NSState::kNSStateSwitching:
      strncpy(state_str, "Switching", 512);
      break;
    }
    LOG_VERB("%s %d %d %d %0.4f", state_str, stats.ema_queue_arrival_speed,
             stats.now_queue_data_size, stats.now_total_bandwidth,
             avg_arrival_time_sec);

    // Handover
    if (state == NSState::kNSStateRunning) {
      this->check_and_handover(stats);
    }

    usleep(SLEEP_USECS);
  }

  this->set_state(NSState::kNSStateInitialized);
}

void NetworkSwitcher::connect_adapter(int adapter_id) {
  NSState state = this->get_state();
  if (state == NSState::kNSStateSwitching) {
    LOG_VERB("It's now switching. Cannot connect to adapter %d.", adapter_id);
    return;
  } else if (state != NSState::kNSStateRunning) {
    LOG_VERB("Switcher is not running. Cannot connect to adapter %d.",
             adapter_id);
    return;
  }
  this->set_state(NSState::kNSStateSwitching);
  ConnectRequestTransaction::run(adapter_id);
}

bool NetworkSwitcher::sleep_adapter(int adapter_id) {
  NSState state = this->get_state();
  if (state == NSState::kNSStateSwitching) {
    LOG_VERB("It's now switching. Cannot connect to adapter %d.", adapter_id);
    return false;
  } else if (state != NSState::kNSStateRunning) {
    LOG_VERB("Switcher is not running. Cannot connect to adapter %d.",
             adapter_id);
    return false;
  }

  ServerAdapter *adapter = Core::get_instance()->get_data_adapter(adapter_id);
  if (adapter != NULL) {
    this->set_state(NSState::kNSStateSwitching);
    bool res = adapter->sleep(false);
    this->set_state(NSState::kNSStateRunning);
    return res;
  } else {
    return false;
  }
}

bool NetworkSwitcher::wake_up_adapter(int adapter_id) {
  NSState state = this->get_state();
  if (state == NSState::kNSStateSwitching) {
    LOG_VERB("It's now switching. Cannot connect to adapter %d.", adapter_id);
    return false;
  } else if (state != NSState::kNSStateRunning) {
    LOG_VERB("Switcher is not running. Cannot connect to adapter %d.",
             adapter_id);
    return false;
  }

  ServerAdapter *adapter = Core::get_instance()->get_data_adapter(adapter_id);
  if (adapter != NULL) {
    this->set_state(NSState::kNSStateSwitching);
    bool res = adapter->wake_up(false);
    this->set_state(NSState::kNSStateRunning);
    return res;
  } else {
    return false;
  }
}

void NetworkSwitcher::reconnect_control_adapter(void) {
  NSState state = this->get_state();
  if (state == NSState::kNSStateSwitching) {
    LOG_VERB("It's now switching. Cannot reconnect control adapter.");
    this->reconnect_control_adapter();
    return;
  } else if (state != NSState::kNSStateRunning) {
    LOG_VERB("Switcher is not running. Cannot reconnect control adapter.");
    this->reconnect_control_adapter();
    return;
  }
  this->set_state(NSState::kNSStateSwitching);
  ReconnectControlAdapterTransaction::run();
}

void NetworkSwitcher::get_stats(Stats &stats) {
  // TODO: consider peer's request_speed, now_queue_data_size
  Core *core = Core::get_instance();
  SegmentManager *sm = SegmentManager::get_instance();

  /* Statistics used to print present status */
  this->mQueueArrivalSpeed.set_value(sm->get_send_request_per_sec());
  stats.ema_queue_arrival_speed = this->mQueueArrivalSpeed.get_em_average();

  /* Statistics used in CoolSpots Policy */
  stats.now_total_bandwidth = Core::get_instance()->get_total_bandwidth();

  /* Statistics used in Energy-aware & Latency-aware Policy */
  stats.ema_send_request_size = core->get_ema_send_request_size();
  stats.now_queue_data_size = sm->get_queue_data_size(kSegSend) +
                              sm->get_failed_sending_queue_data_size();
  stats.ema_arrival_time_us = sm->get_average_arrival_time(
      AVERAGE_ARRIVAL_TIME_WINDOW_SIZE_US); // TODO: remake it with Counter
      // TODO: move it to Core
}

void NetworkSwitcher::check_and_handover(Stats &stats) {
  /* Determine Increasing/Decreasing adapter */
  if (this->check_increase_adapter(stats.ema_queue_arrival_speed,
                                   stats.now_queue_data_size)) {
    /* Maintain bandwidth when increasing */
    this->mBandwidthWhenIncreasing = stats.now_total_bandwidth;

    /* Increase Adapter */
    this->set_state(NSState::kNSStateSwitching);
    bool res = this->increase_adapter();
    if (!res) {
      this->done_switch();
    }
  } else if (this->check_decrease_adapter(stats.ema_queue_arrival_speed,
                                          stats.ema_arrival_time_us)) {
    /* Decrease Adapter */
    this->set_state(NSState::kNSStateSwitching);
    bool res = this->decrease_adapter();
    if (!res) {
      this->done_switch();
    }
  }
}

/* Energy constants (mJ) */
#define BT_TX_ENERGY_1B_BASIS 5.0342f
#define BT_TX_ENERGY_PER_1B 0.9018f
#define WFD_TX_ENERGY_1B_BASIS 2.0554f
#define WFD_TX_ENERGY_PER_1B 0.2054f
#define WFD_INIT_ENERGY 3157.8248f
#define WFD_IDLE_ENERGY_PER_1SEC 156.13f

int NetworkSwitcher::get_init_energy_payoff_point(void) {
  /* 4534B */
  return (int)((WFD_INIT_ENERGY) /
               (BT_TX_ENERGY_PER_1B - WFD_TX_ENERGY_PER_1B));
}
int NetworkSwitcher::get_idle_energy_payoff_point(int ema_arrival_time_us) {
  /*
   * 224B at 1sec
   * 6726B at 30sec
   */
  return (int)((WFD_IDLE_ENERGY_PER_1SEC * ema_arrival_time_us / 1000000) /
               (BT_TX_ENERGY_PER_1B - WFD_TX_ENERGY_PER_1B));
}

#define AVERAGE_WFD_ON_LATENCY_SEC 8.04f /* 8.04 sec */
#define MAX_BANDWIDTH 90000              /* 90000B/s */
bool NetworkSwitcher::check_increase_adapter(int queue_arrival_speed,
                                             int now_queue_data_size) {
  /*
   * Increase condition:
   *   send queue data size + queue arrival speed > energy payoff point
   */
  if (!this->is_increaseable()) {
    return false;
  } else if (Core::get_instance()->get_state() != kCMStateReady) {
    return false;
  } else if (queue_arrival_speed + now_queue_data_size >
             this->get_init_energy_payoff_point()) {
    return true;
  } else {
    return false;
  }
}

#define CHECK_DECREASING_OK_COUNT 2
bool NetworkSwitcher::check_decrease_adapter(int queue_arrival_speed,
                                             int ema_arrival_time_us) {
  /*
   * Decrease condition: LHS < RHS (6 times)
   * LHS: (b(t): total bandwidth)
   * RHS: (b(t_wfdon): total bandwidth when increasing)
   */
  if (!this->is_decreaseable()) {
    return false;
  } else if (Core::get_instance()->get_state() != kCMStateReady) {
    return false;
  } else {
    bool wfd_off;
    if (WFD_INIT_ENERGY <
        (WFD_IDLE_ENERGY_PER_1SEC * ema_arrival_time_us / 1000000)) {
      wfd_off = true;
    } else {
      //int next_arrival_speed = ;
      int idle_energy_payoff_point =
          get_idle_energy_payoff_point(ema_arrival_time_us);
      if (next_arrival_speed > idle_energy_payoff_point) {
        wfd_off = true;
      } else {
        wfd_off = false;
      }
    }

    if (wfd_off) {
      if (queue_arrival_speed <
          this->get_idle_energy_payoff_point(ema_arrival_time_us)) {
        this->mDecreasingCheckCount++;
        if (this->mDecreasingCheckCount >= CHECK_DECREASING_OK_COUNT) {
          this->mDecreasingCheckCount = 0;
          return true;
        } else {
          return false;
        }
      } else {
        return false;
      }
    } else {
      return false;
    }
  }
}

bool NetworkSwitcher::increase_adapter(void) {
  NSState state = this->get_state();
  if (state != NSState::kNSStateRunning) {
    if (state == NSState::kNSStateSwitching) {
      LOG_ERR("It's already connecting or disconnecting an adapter!");
    } else {
      LOG_ERR("Core is not started.");
    }
    return false;
  } else if (Core::get_instance()->get_data_adapter_count() == 0) {
    LOG_ERR("No data adapter is registered!");
    return false;
  } else if (!this->is_increaseable()) {
    LOG_WARN("Cannot increase adapter!");
    return false;
  }

  int prev_index = this->mActiveDataAdapterIndex;
  int next_index = this->mActiveDataAdapterIndex + 1;

  return this->switch_adapters(prev_index, next_index);
}

bool NetworkSwitcher::decrease_adapter(void) {
  NSState state = this->get_state();
  if (state != NSState::kNSStateRunning) {
    if (state == NSState::kNSStateSwitching) {
      LOG_ERR("It's already connecting or disconnecting an adapter!");
    } else {
      LOG_ERR("Core is not started.");
    }
    return false;
  } else if (Core::get_instance()->get_data_adapter_count() == 0) {
    LOG_ERR("No data adapter is registered!");
    return false;
  } else if (!this->is_decreaseable()) {
    LOG_WARN("Cannot deccrease adapter!");
    return false;
  }

  int prev_index = this->mActiveDataAdapterIndex;
  int next_index = this->mActiveDataAdapterIndex - 1;

  return this->switch_adapters(prev_index, next_index);
}

bool NetworkSwitcher::switch_adapters(int prev_index, int next_index) {
  NSState state = this->get_state();
  if (state != NSState::kNSStateRunning) {
    if (state == NSState::kNSStateSwitching) {
      LOG_ERR("It's already connecting or disconnecting an adapter!");
    } else {
      LOG_ERR("Core is not started.");
    }
    return false;
  }
  // Switch Step 1/4
  // Increase/decrease active data adapter index
  this->mActiveDataAdapterIndex = next_index;

  return SwitchAdapterTransaction::run(prev_index, next_index);
}

bool NetworkSwitcher::is_increaseable(void) {
  int data_adapter_count = Core::get_instance()->get_data_adapter_count();
  int active_data_adapter_index = this->get_active_data_adapter_index();
  return ((data_adapter_count > 1) &&
          (active_data_adapter_index < (data_adapter_count - 1)));
}
bool NetworkSwitcher::is_decreaseable(void) {
  int data_adapter_count = Core::get_instance()->get_data_adapter_count();
  int active_data_adapter_index = this->get_active_data_adapter_index();
  return ((data_adapter_count > 1) && (active_data_adapter_index > 0));
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
  NetworkSwitcher *switcher = NetworkSwitcher::get_instance();
  if (is_success) {
    LOG_DEBUG("Switch from %d to %d done.", sOngoing->mPrevIndex,
              sOngoing->mNextIndex);
    switcher->set_active_data_adapter_index(sOngoing->mNextIndex);
  } else {
    LOG_DEBUG("Switch from %d to %d failed.", sOngoing->mPrevIndex,
              sOngoing->mNextIndex);
  }
  switcher->done_switch();
  sOngoing = NULL;
}

void SwitchAdapterTransaction::start(void) {
  // Switch Step 2/4
  Core *core = Core::get_instance();

  ServerAdapter *next_adapter = core->get_data_adapter(this->mNextIndex);
  if (next_adapter == NULL) {
    LOG_ERR("Connecting next data adapter is failed");
    this->done(false);
    return;
  }

  ServerAdapterState adapter_state = next_adapter->get_state();
  if (adapter_state == ServerAdapterState::kDisconnected) {
    // If next active adapter is disconnected now, connect the adapter
    bool res =
        next_adapter->connect(SwitchAdapterTransaction::connect_callback, true);
    if (!res) {
      LOG_ERR("Connecting next data adapter is failed");
      this->done(false);
      return;
    }
  } else if (adapter_state == ServerAdapterState::kSleeping) {
    // If next active adapter is sleeping now, wake up the adapter
    bool res = next_adapter->wake_up(true);
    if (res) {
      sOngoing->connect_callback(true);
    } else {
      LOG_ERR("Waking up next data adapter is failed");
      this->done(false);
      return;
    }
  } else if (adapter_state == ServerAdapterState::kActive) {
    LOG_ERR("Next data adapter is already active");
    this->done(false);
    return;
  } else {
    LOG_ERR("Switching is already in progress");
    this->done(false);
    return;
  }
}

void SwitchAdapterTransaction::connect_callback(bool is_success) {
  // Switch Step 3/4
  Core *core = Core::get_instance();
  NetworkSwitcher *switcher = NetworkSwitcher::get_instance();
  if (!is_success) {
    LOG_ERR("Connecting next data adapter is failed");
    sOngoing->done(false);
    return;
  }

  ServerAdapter *prev_adapter = core->get_data_adapter(sOngoing->mPrevIndex);
  if (prev_adapter == NULL) {
    LOG_ERR("Disconnecting previous data adapter is failed");
    sOngoing->done(false);
    return;
  }
  if (prev_adapter->is_sleeping_allowed()) {
    // Sleep previous active adapter
    bool res = prev_adapter->sleep(true);
    if (res) {
      sOngoing->disconnect_callback(true);
    } else {
      LOG_ERR("Disconnecting previous data adapter is failed");
      sOngoing->done(false);
      return;
    }
  } else {
    // Disconnect previous active adapter
    bool res =
        prev_adapter->disconnect(SwitchAdapterTransaction::disconnect_callback);
    if (!res) {
      LOG_ERR("Disconnecting previous data adapter is failed");
      sOngoing->done(false);
      return;
    }
  }
}

void SwitchAdapterTransaction::disconnect_callback(bool is_success) {
  // Switch Step 4/4
  Core *core = Core::get_instance();
  NetworkSwitcher *switcher = NetworkSwitcher::get_instance();
  if (is_success) {
    // Success!
    sOngoing->done(true);
  } else {
    LOG_ERR("Disconnecting previous data adapter is failed");
    sOngoing->done(false);
  }
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
  bool res =
      adapter->connect(ConnectRequestTransaction::connect_callback, false);
  if (!res) {
    LOG_ERR("Connecting requested data adapter is failed");
    this->done();
    return false;
  }
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
  ServerAdapter *control_adapter = Core::get_instance()->get_control_adapter();
  if (control_adapter == NULL) {
    LOG_ERR("Reconnecting control adapter is failed: retry");
    this->done(true);
    return false;
  }
  bool res = control_adapter->disconnect(
      ReconnectControlAdapterTransaction::disconnect_callback);
  if (!res) {
    LOG_ERR("Reconnecting control adapter is failed: retry");
    this->done(true);
    return false;
  }
  return true;
}

void ReconnectControlAdapterTransaction::disconnect_callback(bool is_success) {
  if (!is_success) {
    LOG_ERR("Reconnecting control adapter is failed: retry");
    sOngoing->done(true);
    return;
  }
  // connect control adapter
  ServerAdapter *control_adapter = Core::get_instance()->get_control_adapter();
  if (control_adapter == NULL) {
    LOG_ERR("Reconnecting control adapter is failed: retry");
    sOngoing->done(true);
    return;
  }
  bool res = control_adapter->connect(
      ReconnectControlAdapterTransaction::disconnect_callback, false);
  if (!res) {
    LOG_ERR("Disconnecting control adapter is failed: retry");
    sOngoing->done(true);
    return;
  }
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
