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

#include "../inc/SenderThread.h"

#include "../../common/inc/DebugLog.h"
#include "../inc/NetworkSwitcher.h"
#include "../inc/SegmentManager.h"
#include "../inc/ServerAdapter.h"

#include <condition_variable>
#include <mutex>
#include <thread>

#include <errno.h>
#include <string.h>

using namespace sc;

void SenderThread::run(void) {
  // Thread start
  LOG_ADAPTER_THREAD_LAUNCH(this->mAdapter->get_name(), "Sender");

  // Initialize state
  this->set_is_thread_running(true);
  this->set_is_loop_enabled(false);
  this->set_is_loop_paused(false);

  // Execute Thread loop
  while (this->is_thread_running()) {
    // Wait until enable loop (Initially, the loop is disabled)
    this->wait_until_enable_loop();

    // Set state before sender loop
    this->set_is_loop_ends(false);

    // Execute sender loop
    LOG_DEBUG("%s: Sender loop starts", this->mAdapter->get_name());
    this->sender_loop();
    LOG_DEBUG("%s: Sender loop ends", this->mAdapter->get_name());

    // Set state after sender loop
    this->set_is_loop_enabled(false);
    this->set_is_loop_paused(false);
    this->set_is_loop_ends(true);

// Reconnect the adapter if it is disconnected on failure
#ifdef EXP_RECONNECT_ADAPTER_ENABLED
    if (!this->mAdapter->is_disconnecting_on_purpose()) {
      NetworkSwitcher::singleton()->reconnect_adapter(this->mAdapter, true);
    }
#endif
  }

  // Set state for thread end
  this->set_is_thread_running(false);

  // Thread end
  LOG_ADAPTER_THREAD_FINISH(this->mAdapter->get_name(), "Sender");
  this->mThread = NULL;
}

void SenderThread::sender_loop(void) {
  while (this->is_loop_enabled()) {
    SegmentManager *sm = SegmentManager::singleton();
    Segment *segment_to_send = NULL;

    // If loop is paused, wait until resume loop
    do {
      if (!this->is_loop_paused()) {
        break;
      }
      LOG_VERB("Sender thread suspended: %s", this->mAdapter->get_name());
      this->mAdapter->set_state(ServerAdapterState::kSleeping);

      // Wait until resume loop
      this->wait_until_resume_loop();

      int adapterState = this->mAdapter->get_state();
      if (adapterState != ServerAdapterState::kDisconnecting &&
          adapterState != ServerAdapterState::kDisconnected) {
        this->mAdapter->set_state(ServerAdapterState::kActive);
      }
    } while (true);

    // Dequeue from a queue (one of the three queues)
    // Priority 1. Failed sending queue
    segment_to_send = sm->get_failed_sending();
#if defined(VERBOSE_DEQUEUE_SEND_CONTROL) || defined(VERBOSE_DEQUEUE_SEND_DATA)
    bool is_get_failed_segment = (segment_to_send != NULL);
#endif

    // Priority 2. Send control queue
    // Priority 3. Send data queue
    if (likely(segment_to_send == NULL)) {
      segment_to_send = sm->dequeue(kDeqSendControlData);
    }

    if (unlikely(segment_to_send == NULL)) {
      // Nothing to send.
      // SegmentManager::wake_up_dequeue_waiting() function may make this case
      continue;
    }

    bool is_control = ((segment_to_send->flag & kSegFlagControl) != 0);
#ifdef VERBOSE_DEQUEUE_SEND_CONTROL
    if (is_control) {
      LOG_DEBUG("%s: Send %s Segment (type=Ctrl, seqno=%d, len=%d, ts=%d.%d)",
                this->mAdapter->get_name(),
                (is_get_failed_segment ? "Failed" : "Normal"),
                segment_to_send->seq_no, (int)segment_to_send->len,
                segment_to_send->send_start_ts_sec,
                segment_to_send->send_start_ts_usec);
    }
#endif
#ifdef VERBOSE_DEQUEUE_SEND_DATA
    if (!is_control) {
      LOG_VERB("%s: Send %s Segment (type=Data, seqno=%d, len=%d, ts=%d.%d))",
                this->mAdapter->get_name(),
                (is_get_failed_segment ? "Failed" : "Normal"),
                segment_to_send->seq_no, (int)segment_to_send->len,
                segment_to_send->send_start_ts_sec,
                segment_to_send->send_start_ts_usec);
    }
#endif

    // If it is suspended, push the segment to the send-fail queue
    {
      if (this->is_loop_paused()) {
        LOG_VERB("Sending segment is pushed to failed queue at %s (suspended)",
                 this->mAdapter->get_name());

        sm->failed_sending(segment_to_send);
        continue;
      }
    }

    int len = kSegHeaderSize + kSegSize;
    void *data = segment_to_send->data;

    // Send data
    int res = this->mAdapter->send(data, len);

    // Error handling
    if (res <= 0) {
      if (this->mAdapter->is_disconnecting_on_purpose()) {
        // Now disconnecting on purpose... Send this segment by another
        // adapter.
        LOG_DEBUG("%s: Disconnecting on purpose (type=%d, seq_no=%lu)",
                  this->mAdapter->get_name(), (int)is_control,
                  segment_to_send->seq_no);
        sm->failed_sending(segment_to_send);
        break;
      } else if (errno == 0) {
        LOG_DEBUG("%s: Unknown error occured (type=%d, seq_no=%lu)",
                  this->mAdapter->get_name(), (int)is_control,
                  segment_to_send->seq_no);
        sm->failed_sending(segment_to_send);
        continue;
      } else if (errno == EINTR) {
        // Handling interrupted system call
        LOG_DEBUG("%s: Send interrupted (type=%d, seq_no=%lu)",
                  this->mAdapter->get_name(), (int)is_control,
                  segment_to_send->seq_no);
        sm->failed_sending(segment_to_send);
        continue;
      } else if (errno == EAGAIN) {
        // Kernel I/O buffer is full
        LOG_WARN(
            "%s: Send fail - Kernel I/O buffer is full (type=%d, seq_no=%lu)",
            this->mAdapter->get_name(), (int)is_control,
            segment_to_send->seq_no);
        sm->failed_sending(segment_to_send);
        continue;
      } else {
        // Other errors
        LOG_WARN("%s: Send fail - res=%d errno=%d(%s) (type=%d, seq_no=%lu)",
                 this->mAdapter->get_name(), res, errno, strerror(errno),
                 (int)is_control, segment_to_send->seq_no);
        sm->failed_sending(segment_to_send);
        break;
      }
    } else if (res != len) {
      LOG_WARN("%s: Send fail - sent_bytes are not accurate (type=%d, "
               "seq_no=%lu, expected=%d, sent=%d) - %d %s",
               this->mAdapter->get_name(), (int)is_control,
               segment_to_send->seq_no, len, res, errno, strerror(errno));
      sm->failed_sending(segment_to_send);
      break;
    }

    // If successful, update the last sequence number
    // Last sequence number is used to wait remaining segments on disconnection
    if (is_control) {
      this->set_last_seq_no_control(segment_to_send->seq_no);
    } else {
      this->set_last_seq_no_data(segment_to_send->seq_no);
    }

    sm->free_segment(segment_to_send);
  }
}