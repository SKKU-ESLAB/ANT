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

#define SLEEP_USECS 1 * 1000 * 1000

namespace cm {
NetworkSwitcher* NetworkSwitcher::singleton = NULL;

NetworkSwitcher* NetworkSwitcher::get_instance(void) {
  if(NetworkSwitcher::singleton == NULL) {
    NetworkSwitcher::singleton = new NetworkSwitcher();
  }
  return NetworkSwitcher::singleton;
}

NetworkSwitcher::NetworkSwitcher(void) {
  this->mThread = NULL;
  this->mStatus = kNSStatusIdle;
  this->mTryDequeue = 0;
}

void NetworkSwitcher::run(void) {
  this->mThread = new std::thread(std::bind(&NetworkSwitcher::run_switcher, this));
  this->mThread->detach();
}

void NetworkSwitcher::run_switcher(void) {
  while(1) {
    SegmentManager *segmentManager = SegmentManager::get_instance();
    uint32_t sendQueueSize = segmentManager->get_queue_size(kSegSend);

    if (sendQueueSize > this->mQueueThreshold){
      /* 
       * Dynamic adapter control 1
       * Increase adapter when queue size is over threshold.
       */
      if (this->mStatus == kNSStatusIdle) {
        /* Increase adapter */
        this->mStatus = kNSStatusIncreasing;
        NetworkManager::get_instance()->increase_adapter();
      }
    } else if (sendQueueSize == 0) {
      /* 
       * Dynamic adapter control 2
       * Decrease adapter when queue size is under threshold for a while.
       */
      if (this->mTryDequeue > 1) {
        this->mTryDequeue = 0;
        if (this->mStatus == kNSStatusIdle) {
          /* Decrease adapter */
          this->mStatus = kNSStatusDecreasing;
          NetworkManager::get_instance()->decrease_adapter();
        }
      } else {
        this->mTryDequeue++;
        LOG_VERB("TryDequeue++: %d\n", this->mTryDequeue);
      }
    }
    usleep(SLEEP_USECS);
  }
}
}
