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

#include "../inc/NetworkMonitor.h"

#include "../inc/NetworkSwitcher.h"

#include "../../configs/ExpConfig.h"
#include "../../configs/NetworkSwitcherConfig.h"

#include <string.h>

using namespace sc;

NetworkMonitor *NetworkMonitor::singleton = NULL;

void NetworkMonitor::start(void) {
  this->mSwitcherThreadOn = true;
  this->mThread =
      new std::thread(std::bind(&NetworkMonitor::switcher_thread, this));
  this->mThread->detach();
}

void NetworkMonitor::stop(void) { this->mSwitcherThreadOn = false; }

void NetworkMonitor::switcher_thread(void) {
  int count = 0;
  while (this->mSwitcherThreadOn) {
    // Get statistics
    Stats stats;
    this->get_stats(stats);

    // Print statistics
    this->print_stats(stats);

    // Check and switch
    // If the switcher is already switching,
    NSState switcher_state = NetworkSwitcher::singleton()->get_state();
    if (switcher_state == NSState::kNSStateReady) {
      this->check_and_decide_switching(stats);
    }

    usleep(NETWORK_MONITOR_SLEEP_USECS);
  }
}

void NetworkMonitor::print_stats(Stats &stats) {
#ifndef PRINT_NETWORK_MONITOR_STATISTICS
  return;
#else
  if (Core::singleton()->get_state() != CoreState::kCoreStateReady) {
    return;
  }

  const int k_state_str_length = 20;
  char state_str[k_state_str_length];
  switch (NetworkSwitcher::singleton()->get_state()) {
  case NSState::kNSStateReady:
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
  printf(
      "%3.3fB/s (R: %3.3fB, T_IA: %3.3fms) => [Q: %'dB ] => %'dB/s // ON:%'dB "
      "OFF: %dB // %s %s\n",
      stats.ema_queue_arrival_speed, stats.ema_send_request_size,
      (stats.ema_arrival_time_us / 1000), stats.now_queue_data_size,
      stats.now_total_bandwidth, this->get_init_energy_payoff_point(),
      this->get_idle_energy_payoff_point(stats.ema_arrival_time_us), mode_str,
      state_str);
#endif
}

void NetworkMonitor::get_stats(Stats &stats) {
  // TODO: consider peer's request_speed, now_queue_data_size
  Core *core = Core::singleton();
  SegmentManager *sm = SegmentManager::singleton();

  /* Statistics used to print present status */
  this->mQueueArrivalSpeed.set_value(sm->get_send_request_per_sec());
  stats.ema_queue_arrival_speed = this->mQueueArrivalSpeed.get_em_average();

  /* Statistics used in CoolSpots Policy */
  stats.now_total_bandwidth = core->get_total_bandwidth();

  /* Statistics used in Energy-aware & Latency-aware Policy */
  stats.ema_send_request_size = core->get_ema_send_request_size();
  stats.ema_arrival_time_us = core->get_ema_send_arrival_time();
  stats.now_queue_data_size = sm->get_queue_data_size(kSegSendData) +
                              sm->get_queue_data_size(kSegSendControl) +
                              sm->get_failed_sending_queue_data_size();
}

void NetworkMonitor::check_and_decide_switching(Stats &stats) {
  /* Determine Increasing/Decreasing adapter */
  if (this->check_increase_adapter(stats)) {
    /* CoolSpots: Maintain bandwidth when increasing */
    this->mBandwidthWhenIncreasing = stats.now_total_bandwidth;

    /* Increase Adapter */
    LOG_VERB("Increase adapter start!");
    this->increase_adapter();
  } else if (this->check_decrease_adapter(stats)) {
    /* Decrease Adapter */
    LOG_VERB("Decrease adapter start!");
    this->decrease_adapter();
  }
}

int NetworkMonitor::get_init_energy_payoff_point(void) {
  /* 4634053B (4525KB) */
  return (int)((WFD_INIT_ENERGY) /
               (BT_TX_ENERGY_PER_1B - WFD_TX_ENERGY_PER_1B));
}
int NetworkMonitor::get_idle_energy_payoff_point(int ema_arrival_time_us) {
  /*
   * 229118B(224KB) at 1sec
   * 6873542B(6712KB) at 30sec
   */
  return (int)((WFD_IDLE_ENERGY_PER_1SEC * ema_arrival_time_us / 1000000) /
               (BT_TX_ENERGY_PER_1B - WFD_TX_ENERGY_PER_1B));
}

int NetworkMonitor::get_init_latency_payoff_point(void) {
  /* 612787B (598KB) */
  return (int)((WFD_INIT_LATENCY + WFD_TX_LATENCY_1KB_BASIS -
                BT_TX_LATENCY_1KB_BASIS) /
               (BT_TX_LATENCY_PER_1B - WFD_TX_LATENCY_PER_1B));
}

bool NetworkMonitor::check_increase_adapter(const Stats &stats) {
  /* Check the condition of adapter increase based on switching policy */
  if (!this->is_increaseable()) {
    return false;
  } else if (Core::singleton()->get_state() != kCoreStateReady) {
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
bool NetworkMonitor::check_decrease_adapter(const Stats &stats) {
  /* Check the condition of adapter decrease based on switching policy */
  if (!this->is_decreaseable()) {
    return false;
  } else if (Core::singleton()->get_state() != kCoreStateReady) {
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
}

bool NetworkMonitor::is_increaseable(void) {
  /* Check the minimum condition of adapter increase such as adapters' count
   */
  Core *core = Core::singleton();
  int adapter_count = core->get_adapter_count();
  int active_data_adapter_index = core->get_active_adapter_index();
  return ((adapter_count > 1) &&
          (active_data_adapter_index < (adapter_count - 1)));
}
bool NetworkMonitor::is_decreaseable(void) {
  /* Check the minimum condition of adapter decrease such as adapters' count
   */
  Core *core = Core::singleton();
  int adapter_count = core->get_adapter_count();
  int active_data_adapter_index = core->get_active_adapter_index();
  return ((adapter_count > 1) && (active_data_adapter_index > 0));
}

bool NetworkMonitor::increase_adapter(void) {
  Core *core = Core::singleton();
  if (core->get_adapter_count() == 0) {
    LOG_ERR("No data adapter is registered!");
    return false;
  } else if (!this->is_increaseable()) {
    LOG_WARN("Cannot increase adapter!");
    return false;
  }

  int prev_index = core->get_active_adapter_index();
  int next_index = prev_index + 1;

  return NetworkSwitcher::singleton()->switch_adapters(prev_index,
                                                          next_index);
}

bool NetworkMonitor::decrease_adapter(void) {
  Core *core = Core::singleton();
  if (core->get_adapter_count() == 0) {
    LOG_ERR("No data adapter is registered!");
    return false;
  } else if (!this->is_decreaseable()) {
    LOG_WARN("Cannot deccrease adapter!");
    return false;
  }

  int prev_index = core->get_active_adapter_index();
  int next_index = prev_index - 1;

  return NetworkSwitcher::singleton()->switch_adapters(prev_index,
                                                          next_index);
}