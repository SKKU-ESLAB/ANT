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

#include <string.h>
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

    // Print statistics
    this->print_stats(stats);

    // Handover
    NSState state = this->get_state();
    if (state == NSState::kNSStateRunning) {
      this->check_and_handover(stats);
    }

    usleep(SLEEP_USECS);
  }

  this->set_state(NSState::kNSStateInitialized);
}

void NetworkSwitcher::print_stats(Stats &stats) {
  if (Core::get_instance()->get_state() != CMState::kCMStateReady) {
    return;
  }

  const int k_state_str_length = 20;
  char state_str[k_state_str_length];
  switch (this->get_state()) {
  case NSState::kNSStateInitialized:
    strncpy(state_str, "Initialized", k_state_str_length);
    break;
  case NSState::kNSStateRunning:
    strncpy(state_str, "Ready", k_state_str_length);
    break;
  case NSState::kNSStateSwitching:
    strncpy(state_str, "Switching", k_state_str_length);
    break;
  default:
    strncpy(state_str, "Unknown", k_state_str_length);
    break;
  }

  const int k_mode_str_length = 20;
  char mode_str[k_mode_str_length];
  switch (this->get_mode()) {
  case NSMode::kNSModeEnergyAware:
    strncpy(mode_str, "Energy-Aware", k_mode_str_length);
    break;
  case NSMode::kNSModeLatencyAware:
    strncpy(mode_str, "Latency-Aware", k_mode_str_length);
    break;
  case NSMode::kNSModeCapDynamic:
    strncpy(mode_str, "Cap-Dynamic", k_mode_str_length);
    break;
  default:
    strncpy(mode_str, "Unknown", k_mode_str_length);
    break;
  }

  /* Print statistics
   *  - EMA(Send Request Size (B))
   *  - EMA(Queue Arrival Speed (B/s))
   *
   *  - Queue Data Size
   *
   *  - Total Bandwidth
   *  - EMA(Arrival Time (sec))
   */
  // printf("%3.3fB/s (R: %3.3fB, T_IA: %3.3fms) => [Q: %'dB ] => %'dB/s // ON: %'dB "
  //        "OFF: %dB // %s %s\n",
  //        stats.ema_queue_arrival_speed, stats.ema_send_request_size,
  //        (stats.ema_arrival_time_us / 1000), stats.now_queue_data_size,
  //        stats.now_total_bandwidth, this->get_init_energy_payoff_point(),
  //        this->get_idle_energy_payoff_point(stats.ema_arrival_time_us),
  //        mode_str, state_str);
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
  stats.now_total_bandwidth = core->get_total_bandwidth();

  /* Statistics used in Energy-aware & Latency-aware Policy */
  stats.ema_send_request_size = core->get_ema_send_request_size();
  stats.ema_arrival_time_us = core->get_ema_send_arrival_time();
  stats.now_queue_data_size = sm->get_queue_data_size(kSegSend) +
                              sm->get_failed_sending_queue_data_size();
}

void NetworkSwitcher::check_and_handover(Stats &stats) {
  /* Determine Increasing/Decreasing adapter */
  if (this->check_increase_adapter(stats)) {
    /* Maintain bandwidth when increasing */
    this->mBandwidthWhenIncreasing = stats.now_total_bandwidth;

    /* Increase Adapter */
    this->set_state(NSState::kNSStateSwitching);
    LOG_VERB("Increase adapter start!");
    bool res = this->increase_adapter();
    if (!res) {
      this->done_switch();
    }
  } else if (this->check_decrease_adapter(stats)) {
    /* Decrease Adapter */
    this->set_state(NSState::kNSStateSwitching);
    LOG_VERB("Decrease adapter start!");
    bool res = this->decrease_adapter();
    if (!res) {
      this->done_switch();
    }
  }
}

/* Energy constants (mJ) */
#define BT_TX_ENERGY_1KB_BASIS 5.0342202f
#define BT_TX_ENERGY_PER_1B 0.000882425f
#define WFD_TX_ENERGY_1KB_BASIS 2.055363f
#define WFD_TX_ENERGY_PER_1B 0.000200986f
#define WFD_INIT_ENERGY 3157.8248f
#define WFD_IDLE_ENERGY_PER_1SEC 156.13f

int NetworkSwitcher::get_init_energy_payoff_point(void) {
  /* 4634053B (4525KB) */
  return (int)((WFD_INIT_ENERGY) /
               (BT_TX_ENERGY_PER_1B - WFD_TX_ENERGY_PER_1B));
}
int NetworkSwitcher::get_idle_energy_payoff_point(int ema_arrival_time_us) {
  /*
   * 229118B(224KB) at 1sec
   * 6873542B(6712KB) at 30sec
   */
  return (int)((WFD_IDLE_ENERGY_PER_1SEC * ema_arrival_time_us / 1000000) /
               (BT_TX_ENERGY_PER_1B - WFD_TX_ENERGY_PER_1B));
}

/* Latency constants (us) */
#define BT_TX_LATENCY_1KB_BASIS 26970.0f
#define BT_TX_LATENCY_PER_1B 11.21951941f
#define WFD_TX_LATENCY_1KB_BASIS 10110.0f
#define WFD_TX_LATENCY_PER_1B 0.248092696f
#define WFD_INIT_LATENCY 6740000.0f

int NetworkSwitcher::get_init_latency_payoff_point(void) {
  /* 612787B (598KB) */
  return (int)((WFD_INIT_LATENCY + WFD_TX_LATENCY_1KB_BASIS -
                BT_TX_LATENCY_1KB_BASIS) /
               (BT_TX_LATENCY_PER_1B - WFD_TX_LATENCY_PER_1B));
}

#define AVERAGE_WFD_ON_LATENCY_SEC 8.04f /* 8.04 sec */
#define MAX_BANDWIDTH 90000              /* 90000B/s */
bool NetworkSwitcher::check_increase_adapter(const Stats &stats) {
  /* Check the condition of adapter increase based on switching policy */
  if (!this->is_increaseable()) {
    return false;
  } else if (Core::get_instance()->get_state() != kCMStateReady) {
    return false;
  } else {
    switch (this->get_mode()) {
    case NSMode::kNSModeEnergyAware: {
      /*
       * Energy-aware Policy:
       *  - queue data size + EMA(send_request_size) > init energy payoff point
       */
      if (stats.ema_send_request_size + stats.now_queue_data_size >
          this->get_init_energy_payoff_point()) {
        return true;
      } else {
        return false;
      }
      break;
    } /* case NSMode::kNSModeEnergyAware */
    case NSMode::kNSModeLatencyAware: {
      /*
       * Latency-aware Policy:
       *  - queue data size + EMA(send_request_size) > init latency payoff point
       */
      if (stats.ema_send_request_size + stats.now_queue_data_size >
          this->get_init_latency_payoff_point()) {
        return true;
      } else {
        return false;
      }
      break;
    } /* case NSMode::kNSModeLatencyAware */
    case NSMode::kNSModeCapDynamic: {
      /*
       * Cap-dynamic Policy:
       */
      LOG_ERR("Unsupported mode!: %d", this->get_mode());
      return false;
      break;
    } /* case NSMode::kNSModeCapDynamic */
    default: {
      LOG_ERR("Unsupported mode!: %d", this->get_mode());
      return false;
      break;
    } /* default */
    } /* switch(this->get_mode()) */
  }
}

#define CHECK_DECREASING_OK_COUNT 2
bool NetworkSwitcher::check_decrease_adapter(const Stats &stats) {
  /* Check the condition of adapter decrease based on switching policy */
  if (!this->is_decreaseable()) {
    return false;
  } else if (Core::get_instance()->get_state() != kCMStateReady) {
    return false;
  } else {
    switch (this->get_mode()) {
    case NSMode::kNSModeEnergyAware: {
      /*
       * Energy-aware Policy:
       *  - if(wfd_idle_energy < wfd_init_energy) EMA(send_request_size)
       *      > idle energy payoff point
       *  - if(wfd_idle_energy > wfd_init_energy) always false
       */
      bool wfd_off;
      int wfd_idle_energy =
          WFD_IDLE_ENERGY_PER_1SEC * stats.ema_arrival_time_us / 1000000;
      if (WFD_INIT_ENERGY < wfd_idle_energy) {
        wfd_off = true;
      } else {
        int next_request_size = stats.ema_send_request_size;
        int idle_energy_payoff_point =
            get_idle_energy_payoff_point(stats.ema_arrival_time_us);
        if (next_request_size > idle_energy_payoff_point) {
          wfd_off = true;
        } else {
          wfd_off = false;
        }
      }

      if (wfd_off) {
        if (stats.ema_send_request_size <
            this->get_idle_energy_payoff_point(stats.ema_arrival_time_us)) {
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
      break;
    } /* case NSMode::kNSModeEnergyAware */
    case NSMode::kNSModeLatencyAware: {
      /*
       * Latency-aware Policy:
       *  - always false
       */
      return false;
      break;
    } /* case NSMode::kNSModeLatencyAware */
    case NSMode::kNSModeCapDynamic: {
      /*
       * Cap-dynamic Policy:
       */
      LOG_ERR("Unsupported mode!: %d", this->get_mode());
      break;
    } /* case NSMode::kNSModeCapDynamic */
    default: {
      LOG_ERR("Unsupported mode!: %d", this->get_mode());
      return false;
      break;
    }
    } /* switch(this->get_mode()) */
  }
} // namespace sc

bool NetworkSwitcher::increase_adapter(void) {
  if (Core::get_instance()->get_data_adapter_count() == 0) {
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
  if (Core::get_instance()->get_data_adapter_count() == 0) {
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
  // Switch Step 1/4
  // Increase/decrease active data adapter index
  this->mActiveDataAdapterIndex = next_index;

  return SwitchAdapterTransaction::run(prev_index, next_index);
}

bool NetworkSwitcher::is_increaseable(void) {
  /* Check the minimum condition of adapter increase such as adapters' count */
  int data_adapter_count = Core::get_instance()->get_data_adapter_count();
  int active_data_adapter_index = this->get_active_data_adapter_index();
  return ((data_adapter_count > 1) &&
          (active_data_adapter_index < (data_adapter_count - 1)));
}
bool NetworkSwitcher::is_decreaseable(void) {
  /* Check the minimum condition of adapter decrease such as adapters' count */
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
