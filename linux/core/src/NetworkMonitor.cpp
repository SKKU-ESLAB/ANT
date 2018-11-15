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

#include "../inc/Core.h"
#include "../inc/NetworkEstimator.h"
#include "../inc/NetworkSwitcher.h"
#include "../inc/Stats.h"

#include "../../configs/ExpConfig.h"
#include "../../configs/NetworkSwitcherConfig.h"

#include <math.h>
#include <string.h>

using namespace sc;

NetworkMonitor *NetworkMonitor::sSingleton = NULL;

void NetworkMonitor::start(void) {
  this->mMonitorThreadOn = true;
  this->mMonitorThread =
      new std::thread(std::bind(&NetworkMonitor::monitor_thread, this));
  this->mMonitorThread->detach();

  this->start_logging();
}

void NetworkMonitor::stop(void) {
  this->mMonitorThreadOn = false;
  this->stop_logging();
}

void NetworkMonitor::start_logging(void) {
  this->mLoggingThreadOn = true;
  this->mLoggingThread =
      new std::thread(std::bind(&NetworkMonitor::logging_thread, this));
  this->mLoggingThread->detach();
}

void NetworkMonitor::stop_logging(void) { this->mLoggingThreadOn = false; }

void NetworkMonitor::monitor_thread(void) {
  int count = 0;
  while (this->mMonitorThreadOn) {
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

void NetworkMonitor::logging_thread(void) {
  FILE *fp = ::fopen(MONITOR_LOG_FILE_NAME, "w");
  if (fp == NULL) {
    LOG_ERR("Failed to open log file");
    return;
  }

  // Write header
  fprintf(fp, "#Timeval(sec), EMA_ReqSize(B), EMA_IAT(ms), SQ_Length(B), "
              "Bandwidth(B/s), EMA_RTT(ms), BT_State, WFD_State\n");

  // Setting first timeval
  struct timeval first_tv;
  gettimeofday(&first_tv, NULL);
  long long first_tv_us =
      (long long)first_tv.tv_sec * 1000 * 1000 + (long long)first_tv.tv_usec;

  while (this->mLoggingThreadOn) {
    // Get statistics
    Core *core = Core::singleton();

    // Get relative now timeval
    struct timeval now_tv;
    gettimeofday(&now_tv, NULL);
    long long now_tv_us =
        (long long)now_tv.tv_sec * 1000 * 1000 + (long long)now_tv.tv_usec;
    long long relative_now_tv_us = now_tv_us - first_tv_us;
    int relative_now_tv_sec = (int)(relative_now_tv_us) / (1000 * 1000);
    int relative_now_tv_usec = (int)(relative_now_tv_us) % (1000 * 1000);

    // Get EMA send RTT
    // Use get_stats() instead
    Stats stats;
    this->get_stats(stats);

    ServerAdapterState btState = core->get_adapter(0)->get_state();
    ServerAdapterState wfdState = core->get_adapter(1)->get_state();
    int bt_on =
        (btState == ServerAdapterState::kConnecting)
            ? 1
            : ((btState == ServerAdapterState::kActive)
                   ? 2
                   : ((btState == ServerAdapterState::kDisconnecting) ? 3 : 0));
    int wfd_on =
        (wfdState == ServerAdapterState::kConnecting)
            ? 1
            : ((wfdState == ServerAdapterState::kActive)
                   ? 2
                   : ((wfdState == ServerAdapterState::kDisconnecting) ? 3
                                                                       : 0));

    ::fprintf(
        fp, "%ld.%ld, %d, %3.3f, %d, %d, %3.3f, %d, %d\n", relative_now_tv_sec,
        relative_now_tv_usec, (int)stats.ema_send_request_size,
        (stats.ema_arrival_time_us / 1000), stats.now_queue_data_size,
        stats.now_total_bandwidth, stats.ema_send_rtt / 1000, bt_on, wfd_on);
    ::fflush(fp);
    ::usleep(250 * 1000);
  }

  ::fclose(fp);
}

void NetworkMonitor::print_stats(Stats &stats) {
#ifndef PRINT_NETWORK_MONITOR_STATISTICS
  return;
#else
  if (Core::singleton()->get_state() != CoreState::kCoreStateReady) {
    return;
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
  switch (this->get_mode()) {
  case NSMode::kNSModeEnergyAware: {
    float energy_retain, energy_switch;
    if (this->is_increaseable()) {
      energy_retain = NetworkEstimator::energy_retain_bt(stats);
      energy_switch = NetworkEstimator::energy_switch_to_wfd(stats);
      if (stats.now_queue_data_size == 0) {
        energy_retain += NetworkEstimator::energy_idle_bt(stats);
        energy_switch += NetworkEstimator::energy_idle_wfd(stats);
      }
    } else {
      energy_retain = NetworkEstimator::energy_retain_wfd(stats);
      energy_switch = NetworkEstimator::energy_switch_to_bt(stats);
      if (stats.now_queue_data_size == 0) {
        energy_retain += NetworkEstimator::energy_idle_wfd(stats);
        energy_switch += NetworkEstimator::energy_idle_bt(stats);
      }
    }
    printf("R: %dB (IAT: %3.3fms) %3.3fB/s => [Q: %dB ] => %dB/s // Energy: "
           "%4.2fmJ vs. "
           "%4.2fmJ (%d) // RTT=%3.3fms\n",
           (int)stats.ema_send_request_size, (stats.ema_arrival_time_us / 1000),
           stats.ema_queue_arrival_speed, stats.now_queue_data_size,
           stats.now_total_bandwidth, energy_retain, energy_switch,
           this->mEADecreasingCheckCount, stats.ema_send_rtt / 1000);
    break;
  }
  case NSMode::kNSModeLatencyAware: {
    float latency_retain, latency_switch;
    if (this->is_increaseable()) {
      latency_retain = NetworkEstimator::latency_retain_queue_bt(stats);
      latency_switch = NetworkEstimator::latency_switch_queue_to_wfd(stats);
    } else {
      latency_retain = NetworkEstimator::latency_retain_queue_wfd(stats);
      latency_switch = NetworkEstimator::latency_switch_queue_to_bt(stats);
    }
    printf("R: %dB (IAT: %3.3fms) %3.3fB/s => [Q: %dB ] => %dB/s // Latency: "
           "%4.2fs vs. "
           "%4.2fs // RTT=%3.3fms\n",
           (int)stats.ema_send_request_size, (stats.ema_arrival_time_us / 1000),
           stats.ema_queue_arrival_speed, stats.now_queue_data_size,
           stats.now_total_bandwidth, latency_retain, latency_switch,
           stats.ema_send_rtt / 1000);
    break;
  }
  case NSMode::kNSModeCapDynamic: {
    printf("R: %dB (IAT: %3.3fms) %3.3fB/s => [Q: %dB ] => %dB/s // "
           "Cap-Dynamic // "
           "RTT=%3.3fms\n",
           (int)stats.ema_send_request_size, (stats.ema_arrival_time_us / 1000),
           stats.ema_queue_arrival_speed, stats.now_queue_data_size,
           stats.now_total_bandwidth, stats.ema_send_rtt / 1000);
    break;
  }
  default:
    break;
  }
#endif
}

void NetworkMonitor::get_stats(Stats &stats) {
  Core *core = Core::singleton();
  SegmentManager *sm = SegmentManager::singleton();

  /* Statistics used to print present status */
  stats.ema_queue_arrival_speed = sm->get_ema_queue_arrival_speed();

  /* Statistics used in CoolSpots Policy */
  stats.now_total_bandwidth = core->get_total_bandwidth();

  /* Statistics used in Energy-aware & Latency-aware Policy */
  stats.ema_send_request_size = core->get_ema_send_request_size();
  stats.ema_arrival_time_us = core->get_ema_send_arrival_time();
  stats.now_queue_data_size = sm->get_queue_data_size(kSQSendData) +
                              sm->get_queue_data_size(kSQSendControl) +
                              sm->get_failed_sending_queue_data_size();

  /* Statistics used to evaluate the policies */
  stats.ema_send_rtt = core->get_ema_send_rtt();
  stats.ema_media_rtt = core->get_ema_media_rtt();
}

void NetworkMonitor::check_and_decide_switching(Stats &stats) {
  /* Determine Increasing/Decreasing adapter */
  if (this->check_increase_adapter(stats)) {
    /* Increase Adapter */
    this->increase_adapter();
  } else if (this->check_decrease_adapter(stats)) {
    /* Decrease Adapter */
    this->decrease_adapter();
  }
}

#define CHECK_CD_INCREASING_OK_COUNT 5
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
       * Energy-aware Policy
       */
      float energy_retain = NetworkEstimator::energy_retain_bt(stats);
      float energy_switch = NetworkEstimator::energy_switch_to_wfd(stats);
      if (stats.now_queue_data_size == 0) {
        energy_retain += NetworkEstimator::energy_idle_bt(stats);
        energy_switch += NetworkEstimator::energy_idle_wfd(stats);
      }

      if (isinf(energy_retain) && isinf(energy_switch)) {
        return true;
      }

      if (energy_retain > energy_switch) {
        return true;
      } else {
        return false;
      }

      break;
    } /* case NSMode::kNSModeEnergyAware */
    case NSMode::kNSModeLatencyAware: {
      /*
       * Latency-aware Policy
       */
      float latency_retain = NetworkEstimator::latency_retain_queue_bt(stats);
      float latency_switch =
          NetworkEstimator::latency_switch_queue_to_wfd(stats);

      if (isinf(latency_retain) && isinf(latency_switch)) {
        return true;
      }

      if (latency_retain > latency_switch) {
        return true;
      } else {
        return false;
      }

      break;
    } /* case NSMode::kNSModeLatencyAware */
    case NSMode::kNSModeCapDynamic: {
      /*
       * Cap-dynamic Policy
       */
      if (stats.ema_media_rtt > RTT_THRESHOLD_US) {
        this->mCDIncreasingCheckCount++;
        if (this->mCDIncreasingCheckCount >= CHECK_CD_INCREASING_OK_COUNT) {
          this->mCDIncreasingCheckCount = 0;
          this->mCDBandwidthWhenIncreasing = stats.now_total_bandwidth;
          return true;
        } else {
          return false;
        }
      } else {
        return false;
      }
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

#define CHECK_EA_DECREASING_OK_COUNT 8
#define CHECK_CD_DECREASING_OK_COUNT 15
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
       * Energy-aware Policy
       */
      float energy_retain = NetworkEstimator::energy_retain_wfd(stats);
      float energy_switch = NetworkEstimator::energy_switch_to_bt(stats);
      if (stats.now_queue_data_size == 0) {
        energy_retain += NetworkEstimator::energy_idle_wfd(stats);
        energy_switch += NetworkEstimator::energy_idle_bt(stats);
      }

      if (isinf(energy_retain) && isinf(energy_switch)) {
        return false;
      }

      if (energy_retain > energy_switch) {
        this->mEADecreasingCheckCount++;
        if (this->mEADecreasingCheckCount > CHECK_EA_DECREASING_OK_COUNT) {
          this->mEADecreasingCheckCount = 0;
          return true;
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
       * Latency-aware Policy
       */
      float latency_retain = NetworkEstimator::latency_retain_queue_wfd(stats);
      float latency_switch = NetworkEstimator::latency_switch_queue_to_bt(stats);

      if (isinf(latency_retain) && isinf(latency_switch)) {
        return false;
      }

      if (latency_retain > latency_switch) {
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
      if (stats.now_total_bandwidth < this->mCDBandwidthWhenIncreasing) {
        this->mCDDecreasingCheckCount++;
        if (this->mCDDecreasingCheckCount >= CHECK_CD_DECREASING_OK_COUNT) {
          this->mCDDecreasingCheckCount = 0;
          return true;
        } else {
          return false;
        }
      } else {
        return false;
      }
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
  int active_adapter_index = core->get_active_adapter_index();
  return ((adapter_count > 1) && (active_adapter_index < (adapter_count - 1)));
}
bool NetworkMonitor::is_decreaseable(void) {
  /* Check the minimum condition of adapter decrease such as adapters' count
   */
  Core *core = Core::singleton();
  int adapter_count = core->get_adapter_count();
  int active_adapter_index = core->get_active_adapter_index();
  return ((adapter_count > 1) && (active_adapter_index > 0));
}

bool NetworkMonitor::increase_adapter(void) {
  Core *core = Core::singleton();
  if (core->get_adapter_count() == 0) {
    LOG_ERR("No adapter is registered!");
    return false;
  } else if (!this->is_increaseable()) {
    LOG_WARN("Cannot increase adapter!");
    return false;
  }

  int prev_index = core->get_active_adapter_index();
  int next_index = prev_index + 1;

  return NetworkSwitcher::singleton()->switch_adapters(prev_index, next_index);
}

bool NetworkMonitor::decrease_adapter(void) {
  Core *core = Core::singleton();
  if (core->get_adapter_count() == 0) {
    LOG_ERR("No adapter is registered!");
    return false;
  } else if (!this->is_decreaseable()) {
    LOG_WARN("Cannot deccrease adapter!");
    return false;
  }

  int prev_index = core->get_active_adapter_index();
  int next_index = prev_index - 1;

  return NetworkSwitcher::singleton()->switch_adapters(prev_index, next_index);
}