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
float NetworkEstimator::latency_retain_queue(const Stats &stats,
                                             AdapterType a1) {
  float latency_transfer = latency_transfer_queue(stats, a1, a1);
  return latency_transfer;
}
float NetworkEstimator::latency_switch_queue(const Stats &stats, AdapterType a1,
                                             AdapterType a2) {
  float latency_switch = latency_switch_only(stats, a1, a2);
  float latency_transfer = latency_transfer_queue(stats, a1, a2);
  return (latency_switch + latency_transfer);
}

/* Energy-aware Policy: Energy (mJ) */
float NetworkEstimator::energy_retain(const Stats &stats, AdapterType a1) {
  if (stats.now_queue_data_size != 0) {
    // Queue length != 0
    return energy_transfer_queue(stats, a1, a1);
  } else {
    // Queue length == 0
    if (a1 == A_BT) {
      float time_transfer_duration = IDLE_ENERGY_ESTIMATION_TIME_SEC;
      float energy_transfer_a1 =
          energy_transfer_idle(stats, time_transfer_duration, A_BT);
      float energy_idle_a1 =
          energy_idle(stats.ema_send_request_size,
                      stats.ema_arrival_time_us / 1000000, A_BT, A_BT);
      return (energy_idle_a1 + energy_transfer_a1);
    } else {
      return energy_retain_idle_wfd(stats);
    }
  }
}

float NetworkEstimator::energy_switch(const Stats &stats, AdapterType a1,
                                      AdapterType a2) {
  if (stats.now_queue_data_size != 0) {
    // Queue length != 0
    float energy_switch = energy_switch_only(a1, a2);
    float energy_transfer_a2 = energy_transfer_queue(stats, a1, a2);
    return (energy_switch + energy_transfer_a2);
  } else {
    // Queue length == 0
    float energy_switch = energy_switch_only(a1, a2);
    float time_transfer_duration =
        IDLE_ENERGY_ESTIMATION_TIME_SEC - LATENCY_ON(a2) - LATENCY_OFF(a1);
    float energy_transfer_a2 =
        energy_transfer_idle(stats, time_transfer_duration, a2);
    float energy_idle_a2 = energy_idle(stats, a1, a2);
    return (energy_switch + energy_transfer_a2 + energy_idle_a2);
  }
}

/* Basic Latency */
float NetworkEstimator::latency_transfer_queue(const Stats &stats,
                                               AdapterType a1, AdapterType a2) {
  float queue_length = stats.now_queue_data_size;
  float queue_arrival_speed = stats.ema_queue_arrival_speed;
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

float NetworkEstimator::latency_switch_only(const Stats &stats, AdapterType a1,
                                            AdapterType a2) {
  float queue_length = stats.now_queue_data_size;
  float queue_arrival_speed = stats.ema_queue_arrival_speed;
  return (LATENCY_ON(a2) + LATENCY_OFF(a1));
}

/* Basic Energy */

float NetworkEstimator::energy_retain_idle_bt(const Stats &stats) {
  float time_transfer_duration = IDLE_ENERGY_ESTIMATION_TIME_SEC;
  float energy_transfer_a1 =
      energy_transfer_idle(stats, time_transfer_duration, A_BT);
  float energy_idle_a1 =
      energy_idle(stats.ema_send_request_size,
                  stats.ema_arrival_time_us / 1000000, A_BT, A_BT);
  return (energy_transfer_a1 + energy_idle_a1);
}
float NetworkEstimator::energy_retain_idle_wfd(const Stats &stats) {
  float time_transfer_duration = IDLE_ENERGY_ESTIMATION_TIME_SEC;
  float energy_transfer_a1 = energy_transfer_idle(
      stats.ema_send_request_size, stats.ema_arrival_time_us / 1000000,
      time_transfer_duration, A_WFD);
  float energy_idle_a1 =
      energy_idle(stats.ema_send_request_size,
                  stats.ema_arrival_time_us / 1000000, A_WFD, A_WFD);
  return (energy_transfer_a1 + energy_idle_a1);
}
float NetworkEstimator::energy_switch_idle_to_bt(const Stats &stats) {
  float energy_switch = energy_switch_only(
      stats.now_queue_data_size, stats.ema_queue_arrival_speed, A_WFD, A_BT);
  float time_transfer_duration =
      IDLE_ENERGY_ESTIMATION_TIME_SEC - LATENCY_ON(A_BT) - LATENCY_OFF(A_WFD);
  float energy_transfer_a2 = energy_transfer_idle(
      stats.ema_send_request_size, stats.ema_arrival_time_us / 1000000,
      time_transfer_duration, A_WFD);
  float energy_idle_a2 =
      energy_idle(stats.ema_send_request_size,
                  stats.ema_arrival_time_us / 1000000, A_WFD, A_BT);
  return (energy_switch + energy_transfer_a2 + energy_idle_a2);
}
float NetworkEstimator::energy_switch_idle_to_wfd(const Stats &stats) {
  float energy_switch = energy_switch_only(
      stats.now_queue_data_size, stats.ema_queue_arrival_speed, A_BT, A_WFD);
  float time_transfer_duration =
      IDLE_ENERGY_ESTIMATION_TIME_SEC - LATENCY_ON(A_WFD) - LATENCY_OFF(A_BT);
  float energy_transfer_a2 = energy_transfer_idle(
      stats.ema_send_request_size, stats.ema_arrival_time_us,
      time_transfer_duration, A_WFD);
  float energy_idle_a2 =
      energy_idle(stats.ema_send_request_size,
                  stats.ema_arrival_time_us / 1000000, A_BT, A_WFD);
  return (energy_switch + energy_transfer_a2 + energy_idle_a2);
}

float NetworkEstimator::energy_transfer_queue(const Stats &stats,
                                              AdapterType a1, AdapterType a2) {
  float time_retain = latency_transfer_queue(stats, a1, a2);
  float ret = (time_retain * POWER_TRANSFER(a2));
  return ret;
}

float NetworkEstimator::time_transfer_idle(const Stats &stats,
                                           float time_transfer_duration,
                                           AdapterType a2) {
  float avg_request_size = stats.ema_send_request_size;
  float inter_arrival_time = stats.ema_arrival_time_us / 1000000;
  return (time_transfer_duration / inter_arrival_time) *
         (avg_request_size / BANDWIDTH_TRANSFER(a2));
}

float NetworkEstimator::energy_transfer_idle(const Stats &stats,
                                             float time_transfer_duration,
                                             AdapterType a2) {
  float time_transfer = time_transfer_idle(stats, time_transfer_duration, a2);
  return time_transfer * POWER_TRANSFER(a2);
}

float NetworkEstimator::energy_idle(const Stats &stats, AdapterType a1,
                                    AdapterType a2) {
  float avg_request_size = stats.ema_send_request_size;
  float inter_arrival_time = stats.ema_arrival_time_us;
  if (a1 == a2) {
    // Retain
    return (IDLE_ENERGY_ESTIMATION_TIME_SEC -
            time_transfer_idle(stats, inter_arrival_time,
                               IDLE_ENERGY_ESTIMATION_TIME_SEC) *
                POWER_TRANSFER(a1));
  } else {
    // Switch
    return (IDLE_ENERGY_ESTIMATION_TIME_SEC - LATENCY_ON(a2) -
            LATENCY_OFF(a1)) *
           POWER_TRANSFER(a1);
  }
}

float NetworkEstimator::energy_switch_only(AdapterType a1, AdapterType a2) {
  return (POWER_ON(a2) * LATENCY_ON(a2) + POWER_OFF(a1) * LATENCY_OFF(a1));
}