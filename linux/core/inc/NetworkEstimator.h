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

#ifndef __NETWORK_ESTIMATOR_H__
#define __NETWORK_ESTIMATOR_H__

#include "Stats.h"

#include <limits>

namespace sc {
class NetworkEstimator {
private:
  /* Basic Information */
  static float latency_retain(float queue_length, float queue_arrival_speed,
                              float bandwidth_a1) {
    float nr = queue_length;
    float dn = bandwidth_a1 - queue_arrival_speed;
    dn = (dn > 0) ? dn : 0;

    float ret = (dn != 0) ? (nr / dn) : std::numeric_limits<float>::infinity();
    return ret;
  }
  static float
  latency_remaining_transfer(float queue_length, float queue_arrival_speed,
                             float time_a2_on, float bandwidth_a2_on,
                             float time_a1_off, float bandwidth_a1_off,
                             float bandwidth_a2) {
    float nr = queue_length - time_a2_on * bandwidth_a2_on -
               time_a1_off * bandwidth_a1_off;
    nr = (nr > 0) ? nr : 0;
    float dn = bandwidth_a2_on - queue_arrival_speed;
    dn = (dn > 0) ? dn : 0;

    float ret = (dn != 0) ? (nr / dn) : std::numeric_limits<float>::infinity();
    return ret;
  }
  static float latency_switch(float queue_length, float queue_arrival_speed,
                              float time_a2_on, float bandwidth_a2_on,
                              float time_a1_off, float bandwidth_a1_off,
                              float bandwidth_a2) {
    float time_a2_transfer = latency_remaining_transfer(
        queue_length, queue_arrival_speed, time_a2_on, bandwidth_a2_on,
        time_a1_off, bandwidth_a1_off, bandwidth_a2);
    return (time_a2_on + time_a1_off + time_a2_transfer);
  }

public:
  /* Latency-aware Policy: Latency (sec) */
  static float latency_retain_bt(const Stats &stats);
  static float latency_retain_wfd(const Stats &stats);
  static float latency_switch_to_bt(const Stats &stats);
  static float latency_switch_to_wfd(const Stats &stats);

private:
  /* Energy-aware Policy: Energy (mJ) */
  static float energy_retain(float queue_length, float queue_arrival_speed,
                             float bandwidth_a1, float power_a1_transfer) {
    float time_retain =
        latency_retain(queue_arrival_speed, queue_arrival_speed, bandwidth_a1);
    float ret = time_retain * power_a1_transfer;
    return ret;
  }
  static float energy_switch(float queue_length, float queue_arrival_speed,
                             float time_a2_on, float bandwidth_a2_on,
                             float power_a2_on, float time_a1_off,
                             float bandwidth_a1_off, float power_a1_off,
                             float bandwidth_a2, float power_a2_transfer) {
    float time_a2_transfer = latency_remaining_transfer(
        queue_length, queue_arrival_speed, time_a2_on, bandwidth_a2_on,
        time_a1_off, bandwidth_a1_off, bandwidth_a2);
    float ret = time_a2_on * power_a2_on + time_a1_off * power_a1_off +
                time_a2_transfer * power_a2_transfer;
    return ret;
  }

  static float energy_idle(float idle_time, float power_a1_idle) {
    return idle_time * power_a1_idle;
  }

public:
  static float energy_retain_bt(const Stats &stats);
  static float energy_retain_wfd(const Stats &stats);
  static float energy_switch_to_bt(const Stats &stats);
  static float energy_switch_to_wfd(const Stats &stats);
  static float energy_idle_bt(const Stats &stats);
  static float energy_idle_wfd(const Stats &stats);
}; /* class NetworkEstimator */
} /* namespace sc */

#endif /* !defined(__NETWORK_ESTIMATOR_H__) */