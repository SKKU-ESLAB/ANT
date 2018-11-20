/* Copyright 2017-2018 All Rights Reserved.
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *  Eunsoo Park (esevan.park@gmail.com)
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

#include "../inc/SegmentManager.h"

#include "../inc/ProtocolManager.h"

#include "../../common/inc/DebugLog.h"

#include "../../configs/ExpConfig.h"

#include <list>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

using namespace sc;

/* Singleton */
SegmentManager *SegmentManager::sSingleton = NULL;

uint32_t SegmentManager::get_next_seq_no(SegSeqNumType seq_num_type,
                                         uint32_t num_segments) {
  uint32_t res;
  this->mNextSeqNoLock[seq_num_type].lock();
  res = this->mNextSeqNo[seq_num_type];
  this->mNextSeqNo[seq_num_type] += num_segments;
  this->mNextSeqNoLock[seq_num_type].unlock();
  return res;
}

void SegmentManager::serialize_segment_header(Segment *seg) {
  uint32_t net_seq_no = htonl(seg->seq_no);
  uint32_t net_len = htonl(seg->len);
  uint32_t net_flag = htonl(seg->flag);
  int32_t net_send_start_ts_sec = htonl(seg->send_start_ts_sec);
  int32_t net_send_start_ts_usec = htonl(seg->send_start_ts_usec);
  memcpy(seg->data, &net_seq_no, sizeof(uint32_t));
  memcpy(seg->data + 4, &net_len, sizeof(uint32_t));
  memcpy(seg->data + 8, &net_flag, sizeof(uint32_t));
  memcpy(seg->data + 12, &net_send_start_ts_sec, sizeof(int32_t));
  memcpy(seg->data + 16, &net_send_start_ts_usec, sizeof(int32_t));
}

int SegmentManager::send_to_segment_manager(uint8_t *data, size_t len,
                                            bool is_control) {
  assert(data != NULL && len > 0);

  uint32_t offset = 0;
  uint32_t num_of_segments = ((len + kSegSize - 1) / kSegSize);
  assert((len + kSegSize - 1) / kSegSize < UINT32_MAX);

  SegSeqNumType seq_num_type = (is_control) ? kSNControl : kSNData;

  /* Reserve sequence numbers to this thread */
  uint32_t allocated_seq_no = get_next_seq_no(seq_num_type, num_of_segments);

  struct timeval send_start_ts;
  gettimeofday(&send_start_ts, NULL);

  int seg_idx;
  for (seg_idx = 0; seg_idx < num_of_segments; seg_idx++) {
    uint32_t seg_len = (len - offset < kSegSize) ? len - offset : kSegSize;
    Segment *seg = this->get_free_segment();

    /* Set segment sequence number */
    seg->seq_no = allocated_seq_no++;

    /* Set segment length */
    seg->len = seg_len;

    /* Set segment MF flag and/or control segment flag */
    uint32_t flag = 0;
    if (offset + seg_len < len)
      flag = flag | kSegFlagMF;
    if (is_control)
      flag = flag | kSegFlagControl;
    seg->flag = flag;

    /* Set segment enqueued timestamp */
    seg->send_start_ts_sec = (int)send_start_ts.tv_sec;
    seg->send_start_ts_usec = (int)send_start_ts.tv_usec;

    /* Set segment header to data */
    this->serialize_segment_header(seg);

    /* Set segment data */
    memcpy(seg->data + kSegHeaderSize, data + offset, seg_len);
    offset += seg_len;

    if (is_control) {
#ifdef VERBOSE_ENQUEUE_SEND
      LOG_DEBUG("Enqueue(control) IsControl: %d / SeqNo: %lu / len: %d / TS: %ld.%ld",
                is_control, seg->seq_no, len, send_start_ts.tv_sec, send_start_ts.tv_usec);
#endif
      this->enqueue(kSQSendControl, seg);
    } else {
#ifdef VERBOSE_ENQUEUE_SEND
      LOG_DEBUG("Enqueue(data) IsControl: %d / SeqNo: %lu / TS: %ld.%ld", is_control,
                seg->seq_no, send_start_ts.tv_sec, send_start_ts.tv_usec);
#endif
      this->enqueue(kSQSendData, seg);
    }
  }

  return 0;
}

uint8_t *SegmentManager::recv_from_segment_manager(void *proc_data_handle,
                                                   bool is_control) {
  assert(proc_data_handle != NULL);

  ProtocolData *pd = reinterpret_cast<ProtocolData *>(proc_data_handle);
  uint8_t *serialized = NULL;
  uint16_t offset = 0;
  size_t data_size = 0;
  bool cont = false;
  bool dequeued = false;
  Segment *seg;

  while (dequeued == false) {
    if (is_control) {
      seg = dequeue(kDeqRecvControl);
    } else {
      seg = dequeue(kDeqRecvData);
    }
    if (seg) {
      dequeued = true;
    }
  }

  ProtocolManager::parse_header(&(seg->data[kSegHeaderSize]), pd);

  if (unlikely(pd->len == 0))
    return NULL;

  serialized = reinterpret_cast<uint8_t *>(calloc(pd->len, sizeof(uint8_t)));

  data_size = seg->len - kProtHeaderSize;
  memcpy(serialized + offset, &(seg->data[kSegHeaderSize]) + kProtHeaderSize,
         data_size);
  offset += data_size;

  cont = ((seg->flag & kSegFlagMF) != 0);

  free_segment(seg);

  while (cont) {
    if (is_control) {
      seg = dequeue(kDeqRecvControl);
    } else {
      seg = dequeue(kDeqRecvData);
    }
    data_size = seg->len;
    memcpy(serialized + offset, &(seg->data[kSegHeaderSize]), data_size);
    cont = ((seg->flag & kSegFlagMF) != 0);
    offset += data_size;
    free_segment(seg);
  }

  return serialized;
}

SegDequeueType
SegmentManager::queue_type_to_dequeue_type(SegQueueType queue_type) {
  SegDequeueType dequeue_type;
  switch (queue_type) {
  case SegQueueType::kSQRecvControl:
    dequeue_type = SegDequeueType::kDeqRecvControl;
    break;
  case SegQueueType::kSQRecvData:
    dequeue_type = SegDequeueType::kDeqRecvData;
    break;
  case SegQueueType::kSQSendControl:
  case SegQueueType::kSQSendData:
    dequeue_type = SegDequeueType::kDeqSendControlData;
    break;
  default:
    LOG_ERR("Unknown queue type: %d", queue_type);
    dequeue_type = SegDequeueType::kDeqUnknown;
    break;
  }
  return dequeue_type;
}

/*
 * This function is the end of the sending logic.
 * It enqeueus the data to the sending queue in order with the sequence number.
 */
void SegmentManager::enqueue(SegQueueType queue_type, Segment *seg) {
  assert(queue_type < kNumSQ);
  SegDequeueType dequeue_type = queue_type_to_dequeue_type(queue_type);
  assert(dequeue_type < kNumDeq);

  // Dequeue Lock
  std::unique_lock<std::mutex> dequeue_lock(this->mDequeueLock[dequeue_type]);

  // Enqueue the segment to the corresponding segment queue
  // If the segment has already been enqueued, the segment can be ignored.
  bool continuous_segment_enqueued =
      this->mSegmentQueues[queue_type].enqueue(seg);

  // If enqueue is successful, wake up threads that wait for dequeue
  if (continuous_segment_enqueued) {
    this->wake_up_dequeue_waiting(dequeue_type);
  }

  // Update statistics
  if (queue_type == kSQSendControl || queue_type == kSQSendData) {
    this->mSendRequest.add(SEGMENT_DATA_SIZE);
    this->update_queue_arrival_speed();
  }

  // Check if all the remaining segments are received
  // It is used for the pre-condition of disconnection
  this->check_receiving_done();
}

void SegmentManager::wait_for_dequeue_locked(
    SegDequeueType dequeue_type, std::unique_lock<std::mutex> &dequeue_lock) {
  /* If queue is empty, wait until some segment is enqueued */
  bool is_wait_required = false;
  switch (dequeue_type) {
  case SegDequeueType::kDeqSendControlData: {
    int send_control_queue_length =
        this->mSegmentQueues[SegQueueType::kSQSendControl].get_queue_length();
    int send_data_queue_length =
        this->mSegmentQueues[SegQueueType::kSQSendData].get_queue_length();
    is_wait_required =
        ((send_control_queue_length == 0) && (send_data_queue_length == 0));
    break;
  }
  case SegDequeueType::kDeqRecvControl: {
    int recv_control_queue_length =
        this->mSegmentQueues[SegQueueType::kSQRecvControl].get_queue_length();
    is_wait_required = (recv_control_queue_length == 0);
    break;
  }
  case SegDequeueType::kDeqRecvData: {
    int recv_data_queue_length =
        this->mSegmentQueues[SegQueueType::kSQRecvData].get_queue_length();
    is_wait_required = (recv_data_queue_length == 0);
    break;
  }
  }
  if (is_wait_required) {
#ifdef VERBOSE_SEGMENT_QUEUE_WAITING
    if (dequeue_type == kDeqSendControlData) {
      LOG_DEBUG("sending queue is empty. wait for another");
    } else {
      LOG_DEBUG("receiving queue is empty. wait for another");
    }
#endif

    this->mDequeueCond[dequeue_type].wait(dequeue_lock);
  }
}

SegQueueType
SegmentManager::get_target_queue_type(SegDequeueType dequeue_type) {
  SegQueueType target_queue_type = SegQueueType::kSQUnknown;
  switch (dequeue_type) {
  case SegDequeueType::kDeqSendControlData: {
    if (this->mSegmentQueues[SegQueueType::kSQSendControl].get_queue_length() !=
        0) {
      // Priority 1. Dequeue send control queue
      target_queue_type = kSQSendControl;
    } else if (this->mSegmentQueues[SegQueueType::kSQSendData]
                   .get_queue_length() != 0) {
      // Priority 2. Dequeue send data queue
      target_queue_type = kSQSendData;
    } else {
      // No segments in both two send queues
      break;
    }
    break;
  }
  case SegDequeueType::kDeqRecvControl: {
    target_queue_type = kSQRecvControl;
    break;
  }
  case SegDequeueType::kDeqRecvData: {
    target_queue_type = kSQRecvData;
    break;
  }
  default: {
    LOG_ERR("Invalid dequeue type (dequeue=%d)", dequeue_type);
    break;
  }
  }
  return target_queue_type;
}

/*
 * Dequeue the segment from the queue.
 * Note that this function is used for sending & receiving queue.
 */
Segment *SegmentManager::dequeue(SegDequeueType dequeue_type) {
  assert(dequeue_type < kNumDeq);
  std::unique_lock<std::mutex> dequeue_lock(this->mDequeueLock[dequeue_type]);

  // Wait for dequeue
  this->wait_for_dequeue_locked(dequeue_type, dequeue_lock);

  // Dequeue from queue
  SegQueueType target_queue_type = get_target_queue_type(dequeue_type);
  if (target_queue_type >= kNumSQ) {
    // It is not failure situation.
    return NULL;
  }

  // Check queue type
  if (target_queue_type >= SegQueueType::kNumSQ) {
    LOG_ERR("Dequeue failed: invalid queue type (dequeue=%d, queue=%d)",
            dequeue_type, target_queue_type);
    return NULL;
  }

  // Check the dequeued segment
  Segment *segment_dequeued = this->mSegmentQueues[target_queue_type].dequeue();
  if (segment_dequeued == NULL) {
    LOG_DEBUG("Dequeue interrupted: empty queue (queue=%d, dequeue=%d)",
              target_queue_type, dequeue_type);
  }

  return segment_dequeued;
}

/*  This function returns a free segment from the list of free segments.
 *  If there is no one available, allocate new one
 */
Segment *SegmentManager::get_free_segment(void) {
  std::unique_lock<std::mutex> lck(this->mFreeSegmentListLock);
  Segment *ret = NULL;
  if (this->mFreeSegmentListSize == 0) {
    ret = reinterpret_cast<Segment *>(calloc(1, sizeof(Segment)));
  } else {
    ret = this->mFreeList.front();
    this->mFreeList.pop_front();
    this->mFreeSegmentListSize--;
  }

  ret->seq_no = 0;
  ret->len = 0;
  ret->flag = 0;
  assert(ret != NULL);
  return ret;
}

void SegmentManager::release_segment_from_free_list(uint32_t threshold) {
  while (this->mFreeSegmentListSize > threshold) {
    Segment *to_free = this->mFreeList.front();
    this->mFreeList.pop_front();
    free(to_free);
    this->mFreeSegmentListSize--;
  }
}

/*  This function releases the segment which is not further used.
 */
void SegmentManager::free_segment(Segment *seg) {
  std::unique_lock<std::mutex> lck(this->mFreeSegmentListLock);
  this->mFreeList.push_front(seg);
  this->mFreeSegmentListSize++;

  // If the free list is big enough, release the half elements of the list
  if (unlikely(this->mFreeSegmentListSize > kSegFreeThreshold)) {
    release_segment_from_free_list(kSegFreeThreshold / 2);
  }
}

void SegmentManager::free_segment_all(void) {
  std::unique_lock<std::mutex> lck(this->mFreeSegmentListLock);
  release_segment_from_free_list(0);
}

void SegmentManager::failed_sending(Segment *seg) {
  std::unique_lock<std::mutex> lck(this->mSendFailQueueLock);
  this->mSendFailQueue.push_back(seg);
  this->mFailedSendingQueueLength.increase();
}

Segment *SegmentManager::get_failed_sending(void) {
  std::unique_lock<std::mutex> lck(this->mSendFailQueueLock);
  if (this->mSendFailQueue.size() == 0)
    return NULL;

  Segment *res = this->mSendFailQueue.front();
  this->mSendFailQueue.pop_front();
  this->mFailedSendingQueueLength.decrease();

  return res;
}