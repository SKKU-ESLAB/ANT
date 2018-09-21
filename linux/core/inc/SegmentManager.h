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
 * Sequence Number Type
 */
typedef enum {
  kSNSend = 0,
  kSNRecv = 1,
  kNumSN = 2
} SegSeqNumType; /* enum SegSeqNumType */

/**
 * Types of Flag
 */
typedef enum {
  kSegFlagMF = 1,
  kSegFlagControl = 2
} SegFlagVal; /* enum SegFlagVal */

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
    this->mNextGlobalSeqNo = 0;
    for (int i = 0; i < kNumSN; i++) {
      this->mNextSeqNo[i] = 0;
    }
    this->mFreeSegmentListSize = 0;

    is_start = 0;
    is_finish = 0;
  }

  std::mutex mNextGlobalSeqNoLock;
  uint32_t mNextGlobalSeqNo;
  uint32_t get_next_global_seq_no(uint32_t num_segments);

  // for experiment
  int is_start, is_finish;
  struct timeval start, end;
  FILE *fp2;

  /* When access to queue, lock should be acquired */
  std::mutex mDequeueLock[kNumDeq];
  std::mutex mSendFailQueueLock;
  std::condition_variable mDequeueCond[kNumDeq];

  uint32_t mNextSeqNo[kNumSN];
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

  void serialize_segment_header(Segment *seg);

  void release_segment_from_free_list(uint32_t threshold);

  void reset_send_queue(void);
  void reset_recv_queue(void);
}; /* class SegmentManager */
} /* namespace sc */

#endif // !defined(__SEGMENT_MANAGER_H__)