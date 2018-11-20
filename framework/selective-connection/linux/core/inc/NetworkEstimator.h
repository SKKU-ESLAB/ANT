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

#include "../../configs/NetworkSwitcherConfig.h"

#include <limits>
#include <stdio.h>

typedef enum { A_BT = 0, A_WFD = 1 } AdapterType;

#define IDLE_ENERGY_ESTIMATION_TIME_SEC 30.0f
#define BANDWIDTH_TRANSFER(a)                                                  \
  ((a == A_BT) ? MAX_BANDWIDTH_BT : MAX_BANDWIDTH_WFD)
#define BANDWIDTH_ON(a)                                                        \
  ((a == A_BT) ? MAX_BANDWIDTH_BT_ON : MAX_BANDWIDTH_WFD_ON)
#define BANDWIDTH_OFF(a)                                                       \
  ((a == A_BT) ? MAX_BANDWIDTH_BT_OFF : MAX_BANDWIDTH_WFD_OFF)

#define LATENCY_ON(a) ((a == A_BT) ? LATENCY_BT_ON : LATENCY_WFD_ON)
#define LATENCY_OFF(a) ((a == A_BT) ? LATENCY_BT_OFF : LATENCY_WFD_OFF)

#define POWER_TRANSFER(a) ((a == A_BT) ? POWER_BT_TRANSFER : POWER_WFD_TRANSFER)
#define POWER_ON(a) ((a == A_BT) ? POWER_BT_ON : POWER_WFD_ON)
#define POWER_OFF(a) ((a == A_BT) ? POWER_BT_OFF : POWER_WFD_OFF)
#define POWER_IDLE(a) ((a == A_BT) ? POWER_BT_IDLE : POWER_WFD_IDLE)

namespace sc {
class NetworkEstimator {
public:
  /* Latency-aware Policy: Latency (sec) */
  static float latency_retain_queue(const Stats &stats, AdapterType a1);
  static float latency_switch_queue(const Stats &stats, AdapterType a1,
                                    AdapterType a2);

public:
  /* Energy-aware Policy: Energy (mJ) */
  static float energy_retain(const Stats &stats, AdapterType a1);
  static float energy_switch(const Stats &stats, AdapterType a1,
                             AdapterType a2);

private:
  /* Basic Latency */
  static float latency_transfer_queue(const Stats &stats, AdapterType a1,
                                      AdapterType a2);
  static float latency_switch_only(const Stats &stats, AdapterType a1,
                                   AdapterType a2);

private:
  /* Basic Energy */
  static float energy_transfer_queue(const Stats &stats, AdapterType a1,
                                     AdapterType a2);
  static float energy_transfer_idle(const Stats &stats,
                                    float time_transfer_duration,
                                    AdapterType a2);

  static float time_transfer_idle(const Stats &stats,
                                  float time_transfer_duration, AdapterType a2);
  static float energy_idle(const Stats &stats, float estimation_time,
                           AdapterType a1, AdapterType a2);
  static float energy_switch_only(const Stats &stats, AdapterType a1,
                                  AdapterType a2);

}; /* class NetworkEstimator */
} /* namespace sc */

#endif /* !defined(__NETWORK_ESTIMATOR_H__) */