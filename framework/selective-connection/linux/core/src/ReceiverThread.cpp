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

    // Set state before receiver loop
    this->set_is_loop_ends(false);

    // Execute receiver loop
    LOG_DEBUG("%s: Receiver loop starts", this->mAdapter->get_name());
    this->receiver_loop();
    LOG_DEBUG("%s: Receiver loop ends", this->mAdapter->get_name());

    // Set state after receiver loop
    this->set_is_loop_enabled(false);
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
    if (res == -999) {
      continue;
    } else if (errno == EINTR) {
      continue;
    } else if (errno == EAGAIN || errno == EWOULDBLOCK ||
               errno == EINPROGRESS) {
      // Receive timeout
      continue;
    } else if (res != len) {
      if (!this->mAdapter->is_disconnecting_on_purpose()) {
        LOG_WARN("Receiving failed at %s (%d / %d; %s)",
                 this->mAdapter->get_name(), errno, res, strerror(errno));
      } else {
        LOG_DEBUG("Receiving broken at %s (%d / %d; %s)",
                  this->mAdapter->get_name(), errno, res, strerror(errno));
      }
      break;
    }

    uint32_t net_seq_no, net_len, net_flag, net_send_start_ts_sec,
        net_send_start_ts_usec, net_media_start_ts_sec, net_media_start_ts_usec;
    memcpy(&net_seq_no, buf, sizeof(uint32_t));
    memcpy(&net_len, (reinterpret_cast<uint8_t *>(buf) + 4), sizeof(uint32_t));
    memcpy(&net_flag, (reinterpret_cast<uint8_t *>(buf) + 8), sizeof(uint32_t));
    memcpy(&net_send_start_ts_sec, (reinterpret_cast<uint8_t *>(buf) + 12),
           sizeof(int));
    memcpy(&net_send_start_ts_usec, (reinterpret_cast<uint8_t *>(buf) + 16),
           sizeof(int));
    memcpy(&net_media_start_ts_sec, (reinterpret_cast<uint8_t *>(buf) + 20),
           sizeof(int));
    memcpy(&net_media_start_ts_usec, (reinterpret_cast<uint8_t *>(buf) + 24),
           sizeof(int));
    segment_to_receive->seq_no = ntohl(net_seq_no);
    segment_to_receive->len = ntohl(net_len);
    segment_to_receive->flag = ntohl(net_flag);
    segment_to_receive->send_start_ts_sec = ntohl(net_send_start_ts_sec);
    segment_to_receive->send_start_ts_usec = ntohl(net_send_start_ts_usec);
    segment_to_receive->media_start_ts_sec = ntohl(net_media_start_ts_sec);
    segment_to_receive->media_start_ts_usec = ntohl(net_media_start_ts_usec);

    bool is_ack = ((segment_to_receive->flag & kSegFlagAck) != 0);
    bool is_control = ((segment_to_receive->flag & kSegFlagControl) != 0);

    if (is_ack) {
      // Handle ACK message
      struct timeval end_ts;
      gettimeofday(&end_ts, NULL);

#ifdef VERBOSE_RECEIVE_ACK
      LOG_DEBUG("%s: Receive ACK - %d.%d ~ %ld.%ld (seqno=%d, len=%d, flag=%d)",
                this->mAdapter->get_name(),
                segment_to_receive->send_start_ts_sec,
                segment_to_receive->send_start_ts_usec, end_ts.tv_sec,
                end_ts.tv_usec, segment_to_receive->seq_no,
                segment_to_receive->len, segment_to_receive->flag);
#endif

      // Use 64-bit time value to prevent overflow during calculation
      long long send_start_us =
          (long long)segment_to_receive->send_start_ts_sec * 1000 * 1000 +
          segment_to_receive->send_start_ts_usec;
      long long media_start_us =
          (long long)segment_to_receive->media_start_ts_sec * 1000 * 1000 +
          segment_to_receive->media_start_ts_usec;
      long long end_us =
          (long long)end_ts.tv_sec * 1000 * 1000 + end_ts.tv_usec;
      int send_rtt = (int)(end_us - send_start_us);
      int media_rtt = (int)(end_us - media_start_us);
      Core::singleton()->set_send_rtt(send_rtt);
      Core::singleton()->set_media_rtt(media_rtt);
#ifdef VERBOSE_RECEIVE_ACK
      LOG_DEBUG("%s: ACK seqno=%d RTT=%d", this->mAdapter->get_name(),
                segment_to_receive->seq_no, send_rtt);
#endif

      continue;
    }

    if (is_control) {
#ifdef VERBOSE_ENQUEUE_RECV
      LOG_DEBUG("%s: Receive Segment (type=Ctrl, seqno=%d, len=%d, flag=%d)",
                this->mAdapter->get_name(), segment_to_receive->seq_no,
                segment_to_receive->len, segment_to_receive->flag);
#endif
      sm->enqueue(kSQRecvControl, segment_to_receive);
    } else {
#ifdef VERBOSE_ENQUEUE_RECV
      LOG_DEBUG("%s: Receive Segment (type=Data, seqno=%d, len=%d, flag=%d)",
                this->mAdapter->get_name(), segment_to_receive->seq_no,
                segment_to_receive->len, segment_to_receive->flag);
#endif
      sm->enqueue(kSQRecvData, segment_to_receive);
    }
    segment_to_receive = sm->get_free_segment();

#ifdef VERBOSE_SERVER_ADAPTER_RECEIVING
    LOG_DEBUG("%s: Received: %d", this->get_name(), res);
#endif
  }
}