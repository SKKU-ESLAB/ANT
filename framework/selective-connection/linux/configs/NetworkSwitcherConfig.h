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

#ifndef __NETWORK_SWITCHER_CONFIG_H__
#define __NETWORK_SWITCHER_CONFIG_H__

#define NETWORK_MONITOR_SLEEP_USECS (250 * 1000)

/* Max Bandwidth (B/s; float) */
#define MAX_BANDWIDTH_BT 91585.0f
#define MAX_BANDWIDTH_WFD 3735044.0f
#define MAX_BANDWIDTH_BT_ON 922126.0f
#define MAX_BANDWIDTH_BT_OFF 352779.0f
#define MAX_BANDWIDTH_WFD_ON 30674.0f
#define MAX_BANDWIDTH_WFD_OFF 38531.0f

/* Switch Latency (sec; float) */
#define LATENCY_BT_ON 5.72f
#define LATENCY_BT_OFF 3.41f
#define LATENCY_WFD_ON 7.66f
#define LATENCY_WFD_OFF 2.61f

/* Power (mW; float) */
#define POWER_BT_TRANSFER 77.41f
#define POWER_WFD_TRANSFER 704.71f
#define POWER_BT_ON 160.77f
#define POWER_BT_OFF 263.60f
#define POWER_WFD_ON 242.31f
#define POWER_WFD_OFF 173.62f
#define POWER_BT_IDLE 1.53f
#define POWER_WFD_IDLE 129.97f

/* CoolSpots */
#define AVERAGE_WFD_ON_LATENCY_SEC 8.04f /* 8.04 sec */
#define MAX_BANDWIDTH 90000              /* 90000B/s */
#define RTT_THRESHOLD_US 250000          /* 250ms */

#endif /* !defined(__NETWORK_SWITCHER_CONFIG_H__) */