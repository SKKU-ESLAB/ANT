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

#include "../inc/NetworkEstimator.h"
#include "../inc/Stats.h"

using namespace sc;

/* Latency-aware Policy: Latency (sec) */
float NetworkEstimator::latency_retain_queue_bt(const Stats &stats) {
  return latency_transfer_queue(stats.now_queue_data_size,
                                stats.ema_queue_arrival_speed, A_BT, A_BT);
}
float NetworkEstimator::latency_retain_queue_wfd(const Stats &stats) {
  return latency_transfer_queue(stats.now_queue_data_size,
                                stats.ema_queue_arrival_speed, A_WFD, A_WFD);
}
float NetworkEstimator::latency_switch_queue_to_bt(const Stats &stats) {
  float latency_switch = latency_switch_only(
      stats.now_queue_data_size, stats.ema_queue_arrival_speed, A_WFD, A_BT);
  float latency_transfer = latency_transfer_queue(
      stats.now_queue_data_size, stats.ema_queue_arrival_speed, A_WFD, A_BT);
  return (latency_switch + latency_transfer);
}
float NetworkEstimator::latency_switch_queue_to_wfd(const Stats &stats) {
  float latency_switch = latency_switch_only(
      stats.now_queue_data_size, stats.ema_queue_arrival_speed, A_BT, A_WFD);
  float latency_transfer = latency_transfer_queue(
      stats.now_queue_data_size, stats.ema_queue_arrival_speed, A_BT, A_WFD);
  return (latency_switch + latency_transfer);
}

/* Energy-aware Policy: Energy (mJ) */
float NetworkEstimator::energy_retain_bt(const Stats &stats) {
  if (stats.now_queue_data_size != 0) {
    return energy_retain_queue_bt(stats);
  } else {
    return energy_retain_idle_bt(stats);
  }
}

float NetworkEstimator::energy_retain_wfd(const Stats &stats) {
  if (stats.now_queue_data_size != 0) {
    return energy_retain_queue_wfd(stats);
  } else {
    return energy_retain_idle_wfd(stats);
  }
}

float NetworkEstimator::energy_switch_to_bt(const Stats &stats) {
  if (stats.now_queue_data_size != 0) {
    return energy_switch_queue_to_bt(stats);
  } else {
    return energy_switch_idle_to_bt(stats);
  }
}
float NetworkEstimator::energy_switch_to_wfd(const Stats &stats) {
  if (stats.now_queue_data_size != 0) {
    return energy_switch_queue_to_wfd(stats);
  } else {
    return energy_switch_idle_to_wfd(stats);
  }
}

/* Basic Latency */
float NetworkEstimator::latency_transfer_queue(float queue_length,
                                               float queue_arrival_speed,
                                               int a1, int a2) {
  if (a1 == a2) {
    // Retain
    float nr = queue_length;
    float dn = BANDWIDTH_TRANSFER(a1) - queue_arrival_speed;
    dn = (dn > 0) ? dn : 0;

    float ret = (dn != 0) ? (nr / dn) : std::numeric_limits<float>::infinity();
    return ret;
  } else {
    // Switch
    float nr = queue_length - LATENCY_ON(a2) * BANDWIDTH_OFF(a2) -
               LATENCY_OFF(a1) * BANDWIDTH_OFF(a1);
    nr = (nr > 0) ? nr : 0;
    float dn = BANDWIDTH_TRANSFER(a2) - queue_arrival_speed;
    dn = (dn > 0) ? dn : 0;

    float ret = (dn != 0) ? (nr / dn) : std::numeric_limits<float>::infinity();
    return ret;
  }
}

float NetworkEstimator::latency_switch_only(float queue_length,
                                            float queue_arrival_speed, int a1,
                                            int a2) {
  return (LATENCY_ON(a2) + LATENCY_OFF(a1));
}

/* Basic Energy */
float NetworkEstimator::energy_retain_queue_bt(const Stats &stats) {
  return energy_transfer_queue(stats.now_queue_data_size,
                               stats.ema_queue_arrival_speed, A_BT, A_BT);
}

float NetworkEstimator::energy_retain_queue_wfd(const Stats &stats) {
  return energy_transfer_queue(stats.now_queue_data_size,
                               stats.ema_queue_arrival_speed, A_WFD, A_WFD);
}

float NetworkEstimator::energy_switch_queue_to_bt(const Stats &stats) {
  float energy_switch = energy_switch_only(
      stats.now_queue_data_size, stats.ema_queue_arrival_speed, A_WFD, A_BT);
  float energy_transfer_a2 = energy_transfer_queue(
      stats.now_queue_data_size, stats.ema_queue_arrival_speed, A_WFD, A_BT);
  return (energy_switch + energy_transfer_a2);
}

float NetworkEstimator::energy_switch_queue_to_wfd(const Stats &stats) {
  float energy_switch = energy_switch_only(
      stats.now_queue_data_size, stats.ema_queue_arrival_speed, A_BT, A_WFD);
  float energy_transfer_a2 = energy_transfer_queue(
      stats.now_queue_data_size, stats.ema_queue_arrival_speed, A_BT, A_WFD);
  return (energy_switch + energy_transfer_a2);
}

float NetworkEstimator::energy_retain_idle_bt(const Stats &stats) {
  float energy_transfer_a1;
  float energy_idle_a1;
  return (energy_transfer_a1 + energy_idle_a1);
}
float NetworkEstimator::energy_retain_idle_wfd(const Stats &stats) {
  float energy_transfer_a1;
  float energy_idle_a1;
  return (energy_transfer_a1 + energy_idle_a1);
}
float NetworkEstimator::energy_switch_idle_to_bt(const Stats &stats) {
  float energy_switch = energy_switch_only(
      stats.now_queue_data_size, stats.ema_queue_arrival_speed, A_WFD, A_BT);
  float energy_transfer_a2;
  float energy_idle_a2;
  return (energy_switch + energy_transfer_a2 + energy_idle_a2);
}
float NetworkEstimator::energy_switch_idle_to_wfd(const Stats &stats) {
  float energy_switch = energy_switch_only(
      stats.now_queue_data_size, stats.ema_queue_arrival_speed, A_BT, A_WFD);
  float energy_transfer_a2;
  float energy_idle_a2;
  return (energy_switch + energy_transfer_a2 + energy_idle_a2);
}

float NetworkEstimator::energy_transfer_queue(float queue_length,
                                              float queue_arrival_speed, int a1,
                                              int a2) {
  float time_retain =
      latency_transfer_queue(queue_arrival_speed, queue_arrival_speed, a1, a2);
  float ret = (time_retain * POWER_TRANSFER(a2));
  return ret;
}

float NetworkEstimator::energy_transfer_idle(float avg_request_size, float inter_arrival_time, int a1, int a2) {
  if(a1 == a2) {
    // Retain
    float bandwidth_a1 = BANDWIDTH();
    float time_transfer_idle = (IDLE_ENERGY_ESTIMATION_TIME_SEC / inter_arrival_time) * (avg_request_size / )
  } else {
    // Switch

  }
}

float NetworkEstimator::energy_switch_only(float queue_length,
                                           float queue_arrival_speed, int a1,
                                           int a2) {
  return (POWER_ON(a2) * LATENCY_ON(a2) + POWER_OFF(a1) * LATENCY_OFF(a1));
}