/* Copyright 2015-2016 CISS, and contributors. All rights reserved
 * 
 * Contact: Eunsoo Park <esevan.park@gmail.com>
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

#include <network_adapter.h>

#include <dbug_log.h>
#include <network_manager.h>
#include <segment_manager.h>

#include <string.h>
#include <errno.h>
#include <netinet/in.h>

#include <functional>

namespace cm {
NetworkAdapter::NetworkAdapter() {
  NetworkManager::get_instance()->install_network_adapter(this);
  stat = kDevOff;
  device_on_cb = NULL;
  device_off_cb = NULL;
  make_connection_cb = NULL;
  close_connection_cb = NULL;
  snprintf(dev_name, sizeof(dev_name), "UNKNOWN");

  th_sender = NULL;
  th_recver = NULL;
}
NetworkAdapter::~NetworkAdapter() {
  dev_off();
  NetworkManager::get_instance()->remove_network_adapter(this);
}

void NetworkAdapter::dev_switch(DevState stat, DevStatCb cb) {
  switch (stat) {
    case kDevOn:
      device_on_cb = cb;
      std::thread(&NetworkAdapter::dev_on, this).detach();
      break;
    case kDevOff:
      device_off_cb = cb;
      std::thread(&NetworkAdapter::dev_off, this).detach();
      break;
    case kDevDiscon:
      close_connection_cb = cb;
      std::thread(&NetworkAdapter::close, this).detach();
      break;
    case kDevCon:
      make_connection_cb = cb;
      std::thread(&NetworkAdapter::connect, this).detach();
      break;
  }
}

DevState NetworkAdapter::get_stat() {
  return stat;
}

void NetworkAdapter::dev_on(void) {
  std::unique_lock<std::mutex> lck(dev_on_lock);
  if (stat == kDevOff) {
    bool res = this->device_on();
    if (res)
      stat = kDevOn;
  }

  if (device_on_cb) device_on_cb(stat);

  device_on_cb = NULL;
}

void NetworkAdapter::dev_off(void) {
  std::unique_lock<std::mutex> lck(dev_off_lock);
  if (stat == kDevOn) {
    bool res = this->device_off();
    if (res)
      stat = kDevOff;
  }

  if (device_off_cb) device_off_cb(stat);
  device_off_cb = NULL;
}

void NetworkAdapter::connect(void) {
  std::unique_lock<std::mutex> lck(connect_lock);
  if (stat == kDevDiscon || stat == kDevOn) {
    bool res = make_connection();
    if (res) {
      th_sender = new std::thread(std::bind(&NetworkAdapter::run_sender, this));
      th_recver = new std::thread(std::bind(&NetworkAdapter::run_recver, this));

      stat = kDevCon;
    }
  }

  if (make_connection_cb) make_connection_cb(stat);
  make_connection_cb = NULL;
}

void NetworkAdapter::close(void) {
  std::unique_lock<std::mutex> lck(close_lock);
  if (stat == kDevCon) {
    bool res = close_connection();
    if (res) {
      th_sender->join();
      th_recver->join();

      delete th_sender;
      delete th_recver;
      th_sender = NULL;
      th_recver = NULL;

      stat = kDevDiscon;
    }
  }

  if (close_connection_cb) close_connection_cb(stat);
  close_connection_cb = NULL;
}

void NetworkAdapter::return_sending_failed_packet(void *seg) {
  Segment *to_send = reinterpret_cast<Segment *>(seg);

  SegmentManager::get_instance()->failed_sending(to_send);
}

void NetworkAdapter::run_sender(void) {
  SegmentManager *sm = SegmentManager::get_instance();

  while (true) {
    Segment *to_send;
    if (likely((to_send = sm->get_failed_sending()) == NULL)) {
      to_send = sm->dequeue(kSegSend);
    }

    size_t len = kSegHeaderSize + kSegSize;
    const void *data = to_send->data;
    bool res = this->send(data, len);
    if (!res) {
      OPEL_DBG_WARN("Sending failed at %s (%s)", dev_name, strerror(errno));
      sm->failed_sending(to_send);
      break;
    }
    sm->free_segment(to_send);
  }
}

void NetworkAdapter::run_recver(void) {
  SegmentManager *sm = SegmentManager::get_instance();
  Segment *free_seg = sm->get_free_segment();

  while (true) {
    void *buf = reinterpret_cast<void *>(free_seg->data);
    size_t len = kSegSize + kSegHeaderSize;
    bool res = this->recv(buf, len);
    if (!res) {
      OPEL_DBG_WARN("Recving failed at %s (%s)", dev_name, strerror(errno));
      break;
    }

    uint16_t net_seq_no, net_flag_len;
    memcpy(&net_seq_no, buf, sizeof(uint16_t));
    memcpy(&net_flag_len, ((uint8_t *)buf+2), sizeof(uint16_t));
    free_seg->seq_no = ntohs(net_seq_no);
    free_seg->flag_len = ntohs(net_flag_len);

    sm->enqueue(kSegRecv, free_seg);
    free_seg = sm->get_free_segment();
  }
}
}  /* namespace cm */
