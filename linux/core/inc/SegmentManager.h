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

#include "../../common/inc/Counter.h"

#include "../../configs/ExpConfig.h"

#include <condition_variable>
#include <list>
#include <mutex>
#include <stdint.h>

#include <sys/time.h>
#include <unistd.h>

namespace sc {
#define kSegSize 512

#define kSegFreeThreshold 256
#define kSegHeaderSize 8

#define kSegLenOffset 0
#define kSegLenMask 0x00003FFF
#define kSegFlagOffset 14
#define kSegFlagMask 0x0000C000
#define mGetSegLenBits(x) (((x)&kSegLenMask) >> kSegLenOffset)
#define mGetSegFlagBits(x) (((x)&kSegFlagMask) >> kSegFlagOffset)
#define mSetSegBits(x, dest, offset, mask)                                     \
  do {                                                                         \
    dest |= ((x << offset) & mask);                                            \
  } while (0)
#define mSetSegLenBits(x, dest) mSetSegBits(x, dest, kSegLenOffset, kSegLenMask)
#define mSetSegFlagBits(x, dest)                                               \
  mSetSegBits(x, dest, kSegFlagOffset, kSegFlagMask)

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
  kNumDeq = 3
} SegDequeueType; /* enum SegDequeueType */

/**
 * Types of Flag
 */
typedef enum {
  kSegFlagMF = 1,
  kSegFlagControl = 2
} SegFlagVal; /* enum SegFlagVal */

/**
 * Types of Sequence Number
 */
typedef enum {
  kSNData = 0,
  kSNControl = 1,
  kNumSN = 2
} SegSeqNumType; /* enum SegSeqNumType */

/**
 * < Data Structure of Segment > - Handled by Segment Manager
 * Segment is the minimum unit of sending data through the network.
 * The partial of the segment cannot be sent.
 *
 * (*c.f.) Segment Header (seq_no + flag_len) is delicate to memroy alignment.
 *    You need to be careful with the segment header.
 */
#define SEGMENT_DATA_SIZE (kSegSize + kSegHeaderSize)
typedef struct {
  uint32_t seq_no;
  uint32_t flag_len; // To present the size of the segment(consider the flag)
  uint8_t data[SEGMENT_DATA_SIZE];
} Segment; /* struct Segment */

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
    return this->mQueueLength[type].get_value();
  }

  int get_queue_data_size(int type) {
    return this->mQueueLength[type].get_value() * SEGMENT_DATA_SIZE;
  }

  int get_failed_sending_queue_data_size() {
    return this->mFailedSendingQueueLength.get_value() * SEGMENT_DATA_SIZE;
  }

  int get_send_request_per_sec() { return this->mSendRequest.get_speed(); }

  /* Singleton */
  static SegmentManager *singleton(void) {
    if (sSingleton == NULL)
      sSingleton = new SegmentManager();
    return sSingleton;
  }

private:
  /* Singleton */
  static SegmentManager *sSingleton;
  SegmentManager(void) {
    for (int i = 0; i < kNumSN; i++) {
      this->mNextSeqNo[i] = 0;
    }
    for (int i = 0; i < kNumSQ; i++) {
      this->mExpectedSeqNo[i] = 0;
    }
    this->mFreeSegmentListSize = 0;

    is_start = 0;
    is_finish = 0;
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

  uint32_t mExpectedSeqNo[kNumSQ];
  std::list<Segment *> mQueues[kNumSQ];
  std::list<Segment *> mSendFailQueue;
  std::list<Segment *> mPendingQueues[kNumSQ];

  /* Statistics */
  Counter mSendRequest;
  Counter mQueueLength[kNumSQ];
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

  uint32_t get_last_seq_no_control(void) {
    return this->mNextSeqNo[kSNControl] - 1;
  }
  uint32_t get_last_seq_no_data(void) { return this->mNextSeqNo[kSNData] - 1; }

private:
  void check_receiving_done() {
    bool is_wakeup = false;
    {
      std::unique_lock<std::mutex> lck(this->mWaitReceivingMutex);
      if (this->mIsWaitReceiving &&
          this->mExpectedSeqNo[kSQRecvControl] >= this->mWaitSeqNoControl &&
          this->mExpectedSeqNo[kSQRecvData] >= this->mWaitSeqNoData) {
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