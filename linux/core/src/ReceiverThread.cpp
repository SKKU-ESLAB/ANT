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

#include "../inc/ReceiverThread.h"

#include "../../common/inc/DebugLog.h"
#include "../inc/NetworkSwitcher.h"
#include "../inc/SegmentManager.h"
#include "../inc/ServerAdapter.h"

#include <condition_variable>
#include <mutex>
#include <thread>

#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

using namespace sc;

void ReceiverThread::run(void) {
  // Thread start
  LOG_ADAPTER_THREAD_LAUNCH(this->mAdapter->get_name(), "Receiver");

  // Initialize state
  this->set_is_thread_running(true);
  this->set_is_loop_enabled(false);

  // Execute Thread loop
  while (this->is_thread_running()) {
    // Wait until enable loop (Initially, the loop is disabled)
    this->wait_until_enable_loop();

    // Execute receiver loop
    this->receiver_loop();

    // Set state after receiver loop
    this->set_is_loop_enabled(false);

    // Reconnect the adapter if it is disconnected on failure
    if (!this->mAdapter->is_disconnecting_on_purpose()) {
      NetworkSwitcher::singleton()->reconnect_adapter(this->mAdapter, true);
    }

    // Notify that the receiver loop ends!
    this->notify_loop_ends();
  }

  // Set state for thread end
  this->set_is_thread_running(false);

  // Thread end
  LOG_ADAPTER_THREAD_FINISH(this->mAdapter->get_name(), "Receiver");
  this->mThread = NULL;
}

void ReceiverThread::receiver_loop(void) {
  while (this->is_loop_enabled()) {
    SegmentManager *sm = SegmentManager::singleton();
    Segment *segment_to_receive = sm->get_free_segment();
    void *buf = reinterpret_cast<void *>(segment_to_receive->data);
    int len = kSegSize + kSegHeaderSize;

#ifdef VERBOSE_SERVER_ADAPTER_RECEIVING
    LOG_DEBUG("%s: Receiving...", this->get_name());
#endif
    int res = this->mAdapter->receive(buf, len);
    if (errno == EINTR) {
      continue;
    } else if (errno == EAGAIN) {
      LOG_WARN("Kernel I/O buffer is full at %s", this->mAdapter->get_name());
      continue;
    } else if (res < len && errno != 0) {
      if (!this->mAdapter->is_disconnecting_on_purpose()) {
        LOG_WARN("Receiving failed at %s (%d / %d; %s)",
                 this->mAdapter->get_name(), errno, res, strerror(errno));
      } else {
        LOG_DEBUG("Receiving broken at %s (%d / %d; %s)",
                  this->mAdapter->get_name(), errno, res, strerror(errno));
      }
      break;
    }

    uint32_t net_seq_no, net_len, net_flag;
    memcpy(&net_seq_no, buf, sizeof(uint32_t));
    memcpy(&net_len, (reinterpret_cast<uint8_t *>(buf) + 4), sizeof(uint32_t));
    memcpy(&net_flag, (reinterpret_cast<uint8_t *>(buf) + 8), sizeof(uint32_t));
    segment_to_receive->seq_no = ntohl(net_seq_no);
    segment_to_receive->len = ntohl(net_len);
    segment_to_receive->flag = ntohl(net_flag);

    bool is_control = ((segment_to_receive->flag & kSegFlagControl) != 0);

    if (is_control) {
#ifdef VERBOSE_ENQUEUE_RECV
      LOG_DEBUG("%s: Receive Segment (type=Ctrl, seqno=%d)",
                this->mAdapter->get_name(), segment_to_receive->seq_no);
#endif
      sm->enqueue(kSQRecvControl, segment_to_receive);
    } else {
#ifdef VERBOSE_ENQUEUE_RECV
      LOG_DEBUG("%s: Receive Segment (type=Data, seqno=%d)",
                this->mAdapter->get_name(), segment_to_receive->seq_no);
#endif
      sm->enqueue(kSQRecvData, segment_to_receive);
    }
    segment_to_receive = sm->get_free_segment();

#ifdef VERBOSE_SERVER_ADAPTER_RECEIVING
    LOG_DEBUG("%s: Received: %d", this->get_name(), res);
#endif
  }
}