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

#include "../../configs/NetworkSwitcherConfig.h"

using namespace sc;

float NetworkEstimator::latency_retain_bt(const Stats &stats) {
  return latency_retain(stats.now_queue_data_size,
                        stats.ema_arrival_time_us / 1000000, MAX_BANDWIDTH_BT);
}
float NetworkEstimator::latency_retain_wfd(const Stats &stats) {
  return latency_retain(stats.now_queue_data_size,
                        stats.ema_arrival_time_us / 1000000, MAX_BANDWIDTH_WFD);
}
float NetworkEstimator::latency_switch_to_bt(const Stats &stats) {
  return latency_switch(stats.now_queue_data_size,
                        stats.ema_arrival_time_us / 1000000, LATENCY_BT_ON,
                        MAX_BANDWIDTH_BT_ON, LATENCY_WFD_OFF,
                        MAX_BANDWIDTH_WFD_OFF, MAX_BANDWIDTH_BT);
}
float NetworkEstimator::latency_switch_to_wfd(const Stats &stats) {
  return latency_switch(stats.now_queue_data_size,
                        stats.ema_arrival_time_us / 1000000, LATENCY_WFD_ON,
                        MAX_BANDWIDTH_WFD_ON, LATENCY_BT_OFF,
                        MAX_BANDWIDTH_BT_OFF, MAX_BANDWIDTH_WFD);
}

float NetworkEstimator::energy_retain_bt(const Stats &stats) {
  return energy_retain(stats.now_queue_data_size,
                       stats.ema_arrival_time_us / 1000000, MAX_BANDWIDTH_BT,
                       POWER_BT_TRANSFER);
}

float NetworkEstimator::energy_retain_wfd(const Stats &stats) {
  return energy_retain(stats.now_queue_data_size,
                       stats.ema_arrival_time_us / 1000000, MAX_BANDWIDTH_WFD,
                       POWER_WFD_TRANSFER);
}

float NetworkEstimator::energy_switch_to_bt(const Stats &stats) {
  return energy_switch(stats.now_queue_data_size,
                       stats.ema_arrival_time_us / 1000000, LATENCY_BT_ON,
                       MAX_BANDWIDTH_BT_ON, POWER_BT_ON, LATENCY_WFD_OFF,
                       MAX_BANDWIDTH_WFD_OFF, POWER_WFD_OFF, MAX_BANDWIDTH_BT,
                       POWER_BT_TRANSFER);
}

float NetworkEstimator::energy_switch_to_wfd(const Stats &stats) {
  return energy_switch(stats.now_queue_data_size,
                       stats.ema_arrival_time_us / 1000000, LATENCY_WFD_ON,
                       MAX_BANDWIDTH_WFD_ON, POWER_WFD_ON, LATENCY_BT_OFF,
                       MAX_BANDWIDTH_BT_OFF, POWER_BT_OFF, MAX_BANDWIDTH_WFD,
                       POWER_WFD_TRANSFER);
}

float NetworkEstimator::energy_idle_bt(const Stats &stats) {
  return energy_idle(stats.ema_arrival_time_us / 1000000, POWER_BT_IDLE);
}

float NetworkEstimator::energy_idle_wfd(const Stats &stats) {
  return energy_idle(stats.ema_arrival_time_us / 1000000, POWER_WFD_IDLE);
}