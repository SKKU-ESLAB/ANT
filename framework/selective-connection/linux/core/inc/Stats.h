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

#ifndef __STATS_H__
#define __STATS_H__

namespace sc {
class Stats {
public:
  /* Statistics used to print present status */
  float ema_queue_arrival_speed = 0;

  /* Statistics used in CoolSpots Policy */
  int now_total_bandwidth = 0;
  float ema_media_rtt = 0;

  /* Statistics used in Energy-aware & Latency-aware Policy */
  float ema_send_request_size = 0;
  float ema_arrival_time_us = 0;
  int now_queue_data_size = 0;

  /* Statistics used to evaluate the policies */
  float ema_send_rtt = 0;
}; /* class Stats */
} /* namespace sc */

#endif /* !defined(__STATS_H__) */