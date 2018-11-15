/* Copyright 2017-2018 All Rights Reserved.
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *  Eunsoo Park (esevan.park@gmail.com)
 *  Injung Hwang (sinban04@gmail.com)
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

#ifndef __SEGMENT_MANAGER_H__
#define __SEGMENT_MANAGER_H__

#include "Segment.h"
#include "SegmentQueue.h"

#include "../../common/inc/Counter.h"

#include "../../configs/ExpConfig.h"

#include <condition_variable>
#include <list>
#include <mutex>
#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>

namespace sc {

#define kSegFreeThreshold 256

/**
 * Queue Type
 */
typedef enum {
  kSQSendData = 0,
  kSQRecvData = 1,
  kSQSendControl = 2,
  kSQRecvControl = 3,
  kNumSQ = 4,
  kSQUnknown = 999
} SegQueueType; /* enum SegQueueType */

/**
 * Dequeue Type
 */
typedef enum {
  kDeqSendControlData = 0,
  kDeqRecvData = 1,
  kDeqRecvControl = 2,
  kNumDeq = 3,
  kDeqUnknown = 999
} SegDequeueType; /* enum SegDequeueType */

/**
 * Types of Flag
 */
typedef enum {
  kSegFlagMF = 1,
  kSegFlagControl = 2,
  kSegFlagAck = 4
} SegFlagVal; /* enum SegFlagVal */

/**
 * Types of Sequence Number
 */
typedef enum {
  kSNData = 0,
  kSNControl = 1,
  kNumSN = 2
} SegSeqNumType; /* enum SegSeqNumType */

class SegmentManager {
public:
  /* Used in protocol manager */
  int send_to_segment_manager(uint8_t *data, size_t len, bool is_control);
  uint8_t *recv_from_segment_manager(void *proc_data_handle, bool is_control);

  void failed_sending(Segment *seg);
  Segment *get_failed_sending(void);
  void enqueue(SegQueueType queue_type, Segment *seg);
  Segment *dequeue(SegDequeueType dequeue_type);
  Segment *get_free_segment(void);
  void free_segment(Segment *seg);
  void free_segment_all(void);

  void wake_up_dequeue_waiting(SegDequeueType dequeue_type) {
    this->mDequeueCond[dequeue_type].notify_all();
  }

  int get_queue_length(int type) {
    return this->mSegmentQueues[type].get_queue_length();
  }

  int get_queue_data_size(int type) {
    return this->mSegmentQueues[type].get_queue_length() * SEGMENT_DATA_SIZE;
  }

  int get_failed_sending_queue_data_size() {
    return this->mFailedSendingQueueLength.get_value() * SEGMENT_DATA_SIZE;
  }

  int get_send_request_per_sec() { return this->mSendRequest.get_speed(); }

  void update_queue_arrival_speed() {
    int send_request_per_sec = this->get_send_request_per_sec();
    this->mQueueArrivalSpeed.set_value(send_request_per_sec);
  }

  int get_ema_queue_arrival_speed() {
    return this->mQueueArrivalSpeed.get_em_average();
  }

  /* Singleton */
  static SegmentManager *singleton(void) {
    if (sSingleton == NULL)
      sSingleton = new SegmentManager();
    return sSingleton;
  }

private:
  SegDequeueType queue_type_to_dequeue_type(SegQueueType queue_type);
  SegQueueType get_target_queue_type(SegDequeueType dequeue_type);

  void wait_for_dequeue_locked(SegDequeueType dequeue_type,
                               std::unique_lock<std::mutex> &dequeue_lock);

private:
  /* Singleton */
  static SegmentManager *sSingleton;
  SegmentManager(void) {
    for (int i = 0; i < kNumSN; i++) {
      this->mNextSeqNo[i] = 0;
    }
    this->mFreeSegmentListSize = 0;

    is_start = 0;
    is_finish = 0;

    this->mSendRequest.start_measure_speed();
  }

  // for experiment
  int is_start, is_finish;
  struct timeval start, end;
  FILE *fp2;

  /* When access to queue, lock should be acquired */
  std::mutex mDequeueLock[kNumDeq];
  std::mutex mSendFailQueueLock;
  std::condition_variable mDequeueCond[kNumDeq];

  std::mutex mNextSeqNoLock[kNumSN];
  uint32_t mNextSeqNo[kNumSN];
  uint32_t get_next_seq_no(SegSeqNumType seq_num_type, uint32_t num_segments);

  SegmentQueue mSegmentQueues[kNumSQ];
  std::list<Segment *> mSendFailQueue;

  /* Statistics */
  Counter mSendRequest;
  Counter mQueueArrivalSpeed; /* to achieve the ema of queue arrival speed */
  Counter mFailedSendingQueueLength;

  /* Reserved free segment list */
  std::mutex mFreeSegmentListLock;
  std::list<Segment *> mFreeList;
  uint32_t mFreeSegmentListSize;

  /* Wait data before disconnection */
public:
  void wait_receiving(uint32_t wait_seq_no_control, uint32_t wait_seq_no_data) {
    std::unique_lock<std::mutex> lck(this->mWaitReceivingMutex);
    this->mIsWaitReceiving = true;
    this->mWaitSeqNoControl = wait_seq_no_control;
    this->mWaitSeqNoData = wait_seq_no_data;
    this->mWaitReceivingCond.wait(lck);
  }

private:
  void check_receiving_done() {
    bool is_wakeup = false;
    {
      std::unique_lock<std::mutex> lck(this->mWaitReceivingMutex);
      uint32_t expected_seq_no_control =
          this->mSegmentQueues[kSQRecvControl].get_expected_seq_no();
      uint32_t expected_seq_no_data =
          this->mSegmentQueues[kSQRecvData].get_expected_seq_no();
      if (this->mIsWaitReceiving &&
          expected_seq_no_control >= this->mWaitSeqNoControl &&
          expected_seq_no_data >= this->mWaitSeqNoData) {
        is_wakeup = true;
      }
    }
    if (is_wakeup) {
      this->mWaitReceivingCond.notify_all();
      this->mIsWaitReceiving = false;
    }
  }

  bool mIsWaitReceiving = false;
  uint32_t mWaitSeqNoControl = 0;
  uint32_t mWaitSeqNoData = 0;
  std::mutex mWaitReceivingMutex;
  std::condition_variable mWaitReceivingCond;

private:
  void serialize_segment_header(Segment *seg);

  void release_segment_from_free_list(uint32_t threshold);

  void reset_send_queue(void);
  void reset_recv_queue(void);
}; /* class SegmentManager */
} /* namespace sc */

#endif // !defined(__SEGMENT_MANAGER_H__)