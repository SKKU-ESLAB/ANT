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

#ifndef INC_NETWORK_SWITCHER_H_
#define INC_NETWORK_SWITCHER_H_

#include <thread>

#define kSegThreshold 512
#define kSegQueueThreshold 50*(kSegThreshold / 512)

namespace cm {
typedef enum {
  kNSStatusIdle = 0,
  kNSStatusIncreasing = 1,
  kNSStatusDecreasing = 2
} NSStatus;

class NetworkSwitcher {
  public:
    void run(void);

    void run_switcher(void);

    bool check_increase_adapter(uint64_t send_request_speed, uint32_t send_queue_data_size);
    bool check_decrease_adapter(uint64_t bandwidth_now, uint64_t bandwidth_when_increasing);

    void done_switch(void) {
      this->mStatus = 0;
    }

    void inc_queue_threshold(void) {
      this->mQueueThreshold += kSegQueueThreshold;
    }

    void dec_queue_threshold(void) {
      this->mQueueThreshold -= kSegQueueThreshold;
    }

    /* Singleton */
    static NetworkSwitcher* get_instance(void) {
      if(NetworkSwitcher::singleton == NULL) {
        NetworkSwitcher::singleton = new NetworkSwitcher();
      }
      return NetworkSwitcher::singleton;
    }

  private:
    /* Singleton */
    static NetworkSwitcher* singleton;
    NetworkSwitcher(void) {
      this->mThread = NULL;
      this->mStatus = kNSStatusIdle;
      this->mBandwidthWhenIncreasing = 0;
      this->mCheckDecreasingOk = 0;
    }

    std::thread *mThread;

    int mStatus;
    uint32_t mQueueThreshold;
    uint64_t mBandwidthWhenIncreasing;
    int mCheckDecreasingOk;
};
} /* namespace cm */

#endif /* INC_NETWORK_SWITCHER_H_ */
