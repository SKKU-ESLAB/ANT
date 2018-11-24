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

#ifndef __SEGMENT_QUEUE_H__
#define __SEGMENT_QUEUE_H__

#include "Segment.h"

#include "../../common/inc/Counter.h"
#include "../../common/inc/DebugLog.h"

#include <condition_variable>
#include <list>
#include <mutex>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

namespace sc {
class SegmentQueue {
public:
  bool enqueue(Segment *seg) {
    bool continuous_segment_enqueued = false;
    if (seg->seq_no == this->mExpectedSeqNo) {
      /*
       * If the sequence number is the right next one,
       * it executes enqueuing logic normally.
       */
      this->mExpectedSeqNo++;
      this->mMainQueue.push_back(seg);
      this->mQueueLength.increase();
      continuous_segment_enqueued = true;
    } else {
      /*
       * If the sequence number is not the next expected one,
       * it enqueues its segments to the pending queue, not normal queue.
       */
      if (seg->seq_no <= this->mExpectedSeqNo) {
        LOG_ERR("Sequence # Error!: %d > %d", seg->seq_no,
                this->mExpectedSeqNo);
      }
      assert(seg->seq_no > this->mExpectedSeqNo);

      std::list<Segment *>::iterator curr_it = this->mPendingQueue.begin();

      /*
       * If we received a unsequential segment, put it into pending queue.
       * Pending queue should retain segments in order of sequence number.
       */
      while (curr_it != this->mPendingQueue.end()) {
        Segment *walker = *curr_it;
        assert(walker->seq_no != seg->seq_no);
        if (walker->seq_no > seg->seq_no)
          break;
        curr_it++;
      }
      this->mPendingQueue.insert(curr_it, seg);
    }

    /*
     * Finally, we put all the consequent segments into type queue.
     * If no segment in the pending queue matches to the next seq_no,
     * Then this process is just skipped.
     */
    std::list<Segment *>::iterator curr_it = this->mPendingQueue.begin();
    while (curr_it != this->mPendingQueue.end() &&
           (*curr_it)->seq_no == this->mExpectedSeqNo) {
      this->mExpectedSeqNo++;
      this->mMainQueue.push_back(*curr_it);
      this->mQueueLength.increase();
      continuous_segment_enqueued = true;

      std::list<Segment *>::iterator to_erase = curr_it++;
      this->mPendingQueue.erase(to_erase);
    }

    return continuous_segment_enqueued;
  }

  Segment *dequeue(void) {
    // Check the main queue
    Segment *segment_dequeued = this->mMainQueue.front();
    if (segment_dequeued == NULL) {
      return NULL;
    } else {
      // Update main queue
      this->mMainQueue.pop_front();
      this->mQueueLength.decrease();
      return segment_dequeued;
    }
  }

  int get_queue_length(void) { return this->mQueueLength.get_value(); }

  uint32_t get_expected_seq_no(void) { return this->mExpectedSeqNo; }

  SegmentQueue(void) { this->mExpectedSeqNo = 0; }

private:
  std::list<Segment *> mMainQueue;    /* In-order & Continuous */
  std::list<Segment *> mPendingQueue; /* In-order & Noncontinuous */

  Counter mQueueLength;

  uint32_t mExpectedSeqNo;
}; /* class SegmentQueue */
} /* namespace sc */

#endif /* !defined(__SEGMENT_QUEUE_H__) */