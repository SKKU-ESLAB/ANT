/* Copyright 2017 All Rights Reserved.
 *  Eunsoo Park (esevan.park@gmail.com)
 *  Injung Hwang (sinban04@gmail.com)
 *  
 * [Contact]
 *  Eunsoo Park (esevan.park@gmail.com)
 *  Injung Hwang (sinban04@gmail.com)
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

#include <segment_manager.h>
#include <dbug_log.h>
#include <protocol_manager.h>
#include <network_manager.h>

#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#include <list>

namespace cm {
static SegmentManager *sm = NULL;

SegmentManager::SegmentManager(void) {
  queue_size[kSegSend] = 0;
  queue_size[kSegRecv] = 0;
  next_seq_no[kSegSend] = 0;
  next_seq_no[kSegRecv] = 0;
  free_list_size = 0;
  seq_no = 0;
  queue_threshold = 0;
  try_dequeue = 0;
  is_changing_adapter = 0;

  is_start = 0;
  is_finish = 0;
}

SegmentManager *SegmentManager::get_instance(void) {
  if (sm == NULL)
    sm = new SegmentManager();
  return sm;
}

uint32_t SegmentManager::get_seq_no(uint32_t num_segments) {
  uint32_t res;
  seq_no_lock.lock();
  res = seq_no;
  seq_no += num_segments;
  seq_no_lock.unlock();
  return res;
}

void SegmentManager::serialize_segment_header(Segment *seg) {
  uint32_t net_seq_no = htonl(seg->seq_no);
  uint32_t net_flag_len = htonl(seg->flag_len); 
  memcpy(seg->data, &net_seq_no, sizeof(uint32_t));
  memcpy(seg->data+4, &net_flag_len, sizeof(uint32_t));
}


int SegmentManager::send_to_segment_manager(uint8_t *data, size_t len) {
  assert(data != NULL && len > 0);
  std::unique_lock<std::mutex> exp_lck(exp_lock);
  struct timeval temp, temp0;


  //fp2 = fopen("log2.txt","a");
  //gettimeofday(&start, NULL);
  uint32_t offset = 0;
  uint32_t num_of_segments =((len + kSegSize - 1) / kSegSize);
  assert((len + kSegSize - 1) / kSegSize < UINT32_MAX);

  /* Reserve sequence numbers to this thread */
  uint32_t allocated_seq_no = get_seq_no(num_of_segments);
  
  
  int seg_idx;
  for (seg_idx = 0; seg_idx < num_of_segments; seg_idx ++) {
    uint32_t seg_len =(len - offset < kSegSize)? len - offset : kSegSize;
    Segment *seg = get_free_segment();

    /* Set segment length */
    mSetSegLenBits(seg_len, seg->flag_len);

    /* Set segment sequence number */
    seg->seq_no = allocated_seq_no++;
 
    /* Set segment data */
    memcpy(&(seg->data[kSegHeaderSize]), data + offset, seg_len);
    offset += seg_len;

    /* Set segment MF flag */
    if (offset < len) mSetSegFlagBits(kSegFlagMF, seg->flag_len);

    /* Set segment header to data */
    serialize_segment_header(seg);

    enqueue(kSegSend, seg);
  }
 /* 
  OPEL_DBG_LOG("wait for lock release");
  is_finish = 1;
  exp_wait.wait(exp_lck);
  OPEL_DBG_LOG("lock released\n");
*/
  //gettimeofday(&end, NULL);
  //fprintf(fp2, " %ld \n",1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec));
  //printf("total: %ld \n",1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec));
  //fclose(fp2);
  
  return 0;
}

uint8_t *SegmentManager::recv_from_segment_manager(void *proc_data_handle) {
  assert(proc_data_handle != NULL);

  ProtocolData *pd = reinterpret_cast<ProtocolData *>(proc_data_handle);
  uint8_t *serialized = NULL;
  uint16_t offset = 0;
  size_t data_size = 0;
  bool cont = false;
  bool dequeued = false;
  Segment *seg;

  while(dequeued == false){
    seg = dequeue(kSegRecv);
    if(seg){
      dequeued = true;
    }
  }

  ProtocolManager::parse_header(&(seg->data[kSegHeaderSize]), pd);

  if (unlikely(pd->len == 0))
    return NULL;

  serialized = reinterpret_cast<uint8_t *>(calloc(pd->len, sizeof(uint8_t)));

  data_size = mGetSegLenBits(seg->flag_len) - kProtHeaderSize;
  memcpy(serialized + offset,
         &(seg->data[kSegHeaderSize]) + kProtHeaderSize,
         data_size);
  offset += data_size;

  cont = (mGetSegFlagBits(seg->flag_len) == kSegFlagMF);

  free_segment(seg);

  while (cont) {
    seg = dequeue(kSegRecv);
    data_size = mGetSegLenBits(seg->flag_len);
    memcpy(serialized + offset,
           &(seg->data[kSegHeaderSize]),
           data_size);
    cont =(mGetSegFlagBits(seg->flag_len) == kSegFlagMF);
    offset += data_size;
    free_segment(seg);
  }

  return serialized;
}
/*
 * This function is the end of the sending logic.
 * It enqeueus the data to the sending queue in order with the sequence number.
 */
void SegmentManager::enqueue(SegQueueType type, Segment *seg) {
  assert(type < kSegMaxQueueType);
  // Get lock for the queue
  std::unique_lock<std::mutex> lck(lock[type]);
  bool segment_enqueued = false;

  /* If the sequence number is the right next one,
   * It executes enqueuing logic normally.
   */
  if (seg->seq_no == next_seq_no[type]) {
    next_seq_no[type]++;
    queue[type].push_back(seg);
    queue_size[type]++;
    segment_enqueued = true;
  }
  /* If the sequence number is not the next expected one,
   * It enqueues its segments to the pending queue, not normal queue.
   */
  else {
    if (seg->seq_no <= next_seq_no[type]){
      OPEL_DBG_ERR("%d > %d, %s", seg->seq_no, next_seq_no[type], type == kSegSend? "Send":"Recv");
    }
    assert(seg->seq_no > next_seq_no[type]);
    std::list<Segment *>::iterator curr_it = pending_queue[type].begin();

    /* First, if we received unsequential data, 
     * we put a requested segment into pending queue */
    while (curr_it != pending_queue[type].end()) {
      Segment *walker = *curr_it;
      assert(walker->seq_no != seg->seq_no);

      if (walker->seq_no > seg->seq_no)
        break;

      curr_it++;
    }
    pending_queue[type].insert(curr_it, seg);

  }

  /*  Finally, we put all consequent segments into type queue 
   *  (If no segment in the pending queue matches to the next seq_no,
   *  Then this process is just skipped.)
   */
  std::list<Segment *>::iterator curr_it = pending_queue[type].begin();
  while (curr_it != pending_queue[type].end() &&
         (*curr_it)->seq_no == next_seq_no[type]) {
    next_seq_no[type]++;
    queue[type].push_back(*curr_it);
    queue_size[type]++;
    segment_enqueued = true;

    std::list<Segment *>::iterator to_erase = curr_it++;
    pending_queue[type].erase(to_erase);
  }

  /*  Dynamic adapter control
   *  Increase the adapter with the queue's threshold 
   */
  if (type == kSegSend ) {
    if (queue_size[type] > queue_threshold && is_changing_adapter == 0){ 
      is_changing_adapter = 1;
      NetworkManager::get_instance()->increase_adapter();
      OPEL_DBG_LOG("Increase adapter!"); 
    }
  }

  if (segment_enqueued) not_empty[type].notify_all();
}

/*  Dequeue the segment from the queue.
 *  Note that this function is used for sending & receiving queue.
 */
Segment *SegmentManager::dequeue(SegQueueType type) { 
  assert(type < kSegMaxQueueType);
  std::unique_lock<std::mutex> lck(lock[type]);

  if (queue_size[type] == 0) {
   
 /*  
    if(type == 0){
      if(is_finish == 1){
        exp_wait.notify_one();
        OPEL_DBG_LOG("notify one");
        is_finish = 0;
      }
    }

*/

    // When it's sending queue
    if(type == kSegSend){
      /*
      if(try_dequeue > 8){
        try_dequeue = 0;
        if(is_changing_adapter == 0){
          is_changing_adapter = 2;
          NetworkManager::get_instance()->decrease_adapter();
          OPEL_DBG_LOG("Decrease Adapter!\n");
        } else {
          OPEL_DBG_LOG("cannot decrease adapter, now\n");
        }

      } else {
        try_dequeue++;
        OPEL_DBG_LOG("try_dequeue ++\n");
      }
*/      
      not_empty[type].wait(lck);

    } else { // When it's receiving queue
      not_empty[type].wait(lck);

    } 

  }

  if (queue_size[type] == 0){
    
    return NULL;   
 
  }

  Segment *ret = queue[type].front();
  if (ret == NULL) {
    OPEL_DBG_LOG("Queue[%s] is NULL(empty)", type==0? "send":"recv");
    return NULL;
  }
  queue[type].pop_front();
  queue_size[type]--;
  return ret;
}

/*  This function returns a free segment from the list of free segments.
 *  If there is no one available, allocate new one
 */
Segment *SegmentManager::get_free_segment(void) {
  std::unique_lock<std::mutex> lck(free_list_lock);
  Segment *ret = NULL;
  if (free_list_size == 0) {
    ret = reinterpret_cast<Segment *>(calloc(1, sizeof(Segment)));
  } else {
    ret = free_list.front();
    free_list.pop_front();
    free_list_size--;
  }

  ret->seq_no = 0;
  ret->flag_len = 0;
  assert(ret != NULL);
  return ret;
}

void SegmentManager::release_segment_from_free_list(uint32_t threshold) {
  while (free_list_size > threshold) {
    Segment *to_free = free_list.front();
    free_list.pop_front();
    free(to_free);
    free_list_size--;
  }
}

/*  This function releases the segment which is not further used.
 */
void SegmentManager::free_segment(Segment *seg) {
  std::unique_lock<std::mutex> lck(free_list_lock);
  free_list.push_front(seg);
  free_list_size++;

  // If the free list is big enough, release the half elements of the list
  if (unlikely(free_list_size > kSegFreeThreshold)) {
    release_segment_from_free_list(kSegFreeThreshold / 2);
  }
}

void SegmentManager::free_segment_all(void) {
  std::unique_lock<std::mutex> lck(free_list_lock);
  release_segment_from_free_list(0);
}

void SegmentManager::failed_sending(Segment *seg) {
  std::unique_lock<std::mutex> lck(failed_lock);
  failed.push_back(seg);
}

Segment *SegmentManager::get_failed_sending(void) {
  std::unique_lock<std::mutex> lck(failed_lock);
  if (failed.size() == 0) return NULL;

  Segment *res = failed.front();
  failed.pop_front();

  return res;
}

void SegmentManager::reset(void) {
  
}

void SegmentManager::notify_queue() {
  OPEL_DBG_LOG("notify all\n");
  not_empty[kSegSend].notify_all();
  not_empty[kSegRecv].notify_all();
}

}; /* namespace cm */
