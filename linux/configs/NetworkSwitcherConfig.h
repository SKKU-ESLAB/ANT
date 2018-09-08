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

#ifndef _NETWORK_SWITCHER_CONFIG_H_
#define _NETWORK_SWITCHER_CONFIG_H_

#define EXP_NO_CONTROL_ADAPTER_AFTER_SWITCHING 0 // TODO: remove
#define PREVENT_RECONNECT_CONTROL_ADAPTER 1
#define NETWORK_MONITOR_SLEEP_USECS (250 * 1000)

/* Energy constants (mJ) */
#define BT_TX_ENERGY_1KB_BASIS 5.0342202f
#define BT_TX_ENERGY_PER_1B 0.000882425f
#define WFD_TX_ENERGY_1KB_BASIS 2.055363f
#define WFD_TX_ENERGY_PER_1B 0.000200986f
#define WFD_INIT_ENERGY 3157.8248f
#define WFD_IDLE_ENERGY_PER_1SEC 156.13f

/* Latency constants (us) */
#define BT_TX_LATENCY_1KB_BASIS 26970.0f
#define BT_TX_LATENCY_PER_1B 11.21951941f
#define WFD_TX_LATENCY_1KB_BASIS 10110.0f
#define WFD_TX_LATENCY_PER_1B 0.248092696f
#define WFD_INIT_LATENCY 6740000.0f

/* CoolSpots */
#define AVERAGE_WFD_ON_LATENCY_SEC 8.04f /* 8.04 sec */
#define MAX_BANDWIDTH 90000              /* 90000B/s */

#endif /* !defined(_NETWORK_SWITCHER_CONFIG_H_) */