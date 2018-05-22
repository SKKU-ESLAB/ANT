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

#include <network_switcher.h>

#include <network_manager.h>
#include <segment_manager.h>
#include <dbug_log.h>

#include <thread>
#include <unistd.h>

#define PRINT_DETAILS 0

#define SLEEP_USECS 250 * 1000

namespace cm {
NetworkSwitcher* NetworkSwitcher::singleton = NULL;

void NetworkSwitcher::run(void) {
  this->mThread = new std::thread(std::bind(&NetworkSwitcher::run_switcher, this));
  this->mThread->detach();
}

void NetworkSwitcher::run_switcher(void) {
  while(1) {
    switch(this->mStatus) {
      case kNSStatusNeedControlAdapter:
        break;
      case kNSStatusNeedDataAdapter:
        /* After connecting control adapter, at least one increase is required */
        this->mStatus = kNSStatusIncreasing;
        NetworkManager::get_instance()->increase_adapter(); 
        break;
      case kNSStatusReady:
        this->monitor_and_handover();
        break;
      case kNSStatusIncreasing:
      case kNSStatusDecreasing:
        /* Handover do not work during increasing or decreasing adapter */
        break;
    }

    usleep(SLEEP_USECS);
  }
}

void NetworkSwitcher::monitor_and_handover(void) {
  /* Monitor metrics */
  int send_request_speed;
  int send_queue_data_size;
  int total_bandwidth_now = 0;

  SegmentManager *segment_manager = SegmentManager::get_instance();
  send_request_speed = segment_manager->get_send_request_per_sec();
  send_queue_data_size = segment_manager->get_queue_data_size(kSegSend);

  NetworkManager *network_manager = NetworkManager::get_instance();
  int i = 0;
  std::list<NetworkAdapter *> control_adapters = network_manager->get_control_adapter_list();
  for(std::list<NetworkAdapter *>::iterator it = control_adapters.begin();
      it != control_adapters.end(); it++) {
    NetworkAdapter *adapter = *it;
    int bandwidth_up = adapter->get_bandwidth_up();
    int bandwidth_down = adapter->get_bandwidth_down();
    total_bandwidth_now += (bandwidth_up + bandwidth_down);
#if PRINT_DETAILS == 1
    LOG_VERB("- A%d (C: %s): Up=%d B/s Down=%d B/s",
        i, adapter->get_dev_name(), bandwidth_up, bandwidth_down);
#endif
    i++;
  }

  std::list<NetworkAdapter *> data_adapters = network_manager->get_data_adapter_list();
  for(std::list<NetworkAdapter *>::iterator it = data_adapters.begin();
      it != data_adapters.end(); it++) {
    NetworkAdapter *adapter = *it;
    int bandwidth_up = adapter->get_bandwidth_up();
    int bandwidth_down = adapter->get_bandwidth_down();
    total_bandwidth_now += (bandwidth_up + bandwidth_down);
#if PRINT_DETAILS == 1
    LOG_VERB("- A%d (D: %s): Up=%d B/s Down=%d B/s",
        i, adapter->get_dev_name(), bandwidth_up, bandwidth_down);
#endif
    i++;
  }

  /* Get average */
  put_values(send_request_speed, send_queue_data_size, total_bandwidth_now);

  int avg_send_request_speed = get_average_send_request_speed();
  int avg_send_queue_data_size = get_average_send_queue_data_size();
  int avg_total_bandwidth_now = get_average_total_bandwidth_now();

#if PRINT_DETAILS == 1
  LOG_VERB(" => r(t): %d B/s, |SQ(t)|: %d B, b(t): %d B/s",
      avg_send_request_speed, avg_send_queue_data_size, avg_total_bandwidth_now);
#else
  printf("%d %lu %d\n",
      avg_send_request_speed, avg_send_queue_data_size, avg_total_bandwidth_now);
#endif

    /* Determine Increasing/Decreasing adapter */
//    if(this->check_increase_adapter(avg_send_request_speed, avg_send_queue_data_size)) {
//      /* Maintain bandwidth when increasing */
//      this->mBandwidthWhenIncreasing = avg_total_bandwidth_now;
//
//      /* Increase Adapter */
//      this->mStatus = kNSStatusIncreasing;
//      NetworkManager::get_instance()->increase_adapter();
//    } else if(this->check_decrease_adapter(avg_total_bandwidth_now, this->mBandwidthWhenIncreasing)) {
//      /* Decrease Adapter */
//      this->mStatus = kNSStatusDecreasing;
//      NetworkManager::get_instance()->decrease_adapter();
//    }
}

#define AVERAGE_INCREASE_LATENCY_SEC 8.04f /* 8.04 sec */
#define MAX_BANDWIDTH 50000 /* 50000B/s */
bool NetworkSwitcher::check_increase_adapter(int send_request_speed, int send_queue_data_size) {
  /*
   * Increase condition: LHS > RHS
   * LHS: (average increase latency) * (r(t): send request speed) + (|SQ(t)|: send queue data size)
   * RHS: (average increase latency) * (maximum bluetooth bandwidth)
   */
  if(((float)AVERAGE_INCREASE_LATENCY_SEC * send_request_speed + send_queue_data_size) >
      ((float)AVERAGE_INCREASE_LATENCY_SEC * MAX_BANDWIDTH)) {
    return true;
  } else {
    return false;
  }
}

#define CHECK_DECREASING_OK_COUNT 6
bool NetworkSwitcher::check_decrease_adapter(int bandwidth_now, int bandwidth_when_increasing) {
  /*
   * Decrease condition: LHS < RHS (6 times)
   * LHS: (b(t): total bandwidth)
   * RHS: (b(t_wfdon): total bandwidth when increasing)
   */
  if(bandwidth_when_increasing == 0) return false;

  if(bandwidth_now < bandwidth_when_increasing) {
    this->mDecreasingCheckCount++;
    if(this->mDecreasingCheckCount >= CHECK_DECREASING_OK_COUNT) {
      this->mDecreasingCheckCount = 0;
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}
}
