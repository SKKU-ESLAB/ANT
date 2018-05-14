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
      /* Singleton */
      static NetworkSwitcher* get_instance(void);
      void run(void);

      void run_switcher(void);

      void done_switch(void) {
        this->mStatus = 0;
      }

      void inc_queue_threshold(void) {
        this->mQueueThreshold += kSegQueueThreshold;
      }

      void dec_queue_threshold(void) {
        this->mQueueThreshold -= kSegQueueThreshold;
      }

    private:
      /* Singleton */
      static NetworkSwitcher* singleton;
      NetworkSwitcher();

      std::thread *mThread;

      int mStatus;
      uint32_t mQueueThreshold;
      uint8_t mTryDequeue;  // # of try to dequeue
  };
} /* namespace cm */

#endif /* INC_NETWORK_SWITCHER_H_ */
