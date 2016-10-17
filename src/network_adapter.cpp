/* Copyright 2016 Eunsoo Park (esevan.park@gmail.com). All rights reserved
 * 
 * Contact: Eunsoo Park (esevan.park@gmail.com)
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
  at = kATUninitialized;
  stat = kDevDiscon;
  device_on_cb = NULL;
  device_off_cb = NULL;
  make_connection_cb = NULL;
  close_connection_cb = NULL;
  snprintf(dev_name, sizeof(dev_name), "UNKNOWN");

  th_sender = NULL;
  th_recver = NULL;
}
NetworkAdapter::~NetworkAdapter() {
  if (at == kATUninitialized)
    return;

  dev_off();

  if (at & kATCtrl)
    NetworkManager::get_instance()->remove_control_adapter(this);
  else
    NetworkManager::get_instance()->remove_data_adapter(this);
}

void NetworkAdapter::dev_switch(DevState stat, DevStatCb cb) {
  switch (stat) {
    case kDevOn:
      /* Not implemented
      device_on_cb = cb;
      std::thread(std::bind(&NetworkAdapter::dev_on, this)).detach();
      break;
      */
    case kDevOff:
      /* Not implemented
      device_off_cb = cb;
      std::thread(std::bind(&NetworkAdapter::dev_off, this)).detach();
      */
      OPEL_DBG_ERR("Not implemented bug");
      break;
    case kDevDiscon:
      if (this->stat == kDevDiscon) {
        OPEL_DBG_WARN("Already disconnected");
        return;
      }

      if (this->stat == kDevConnecting) {
        OPEL_DBG_WARN("Device is busy");
        return;
      }
      this->stat = kDevDisconnecting;
      close_connection_cb = cb;
      std::thread(std::bind(&NetworkAdapter::close, this)).detach();
      break;
    case kDevCon:
      if (this->stat > kDevDiscon) {
        OPEL_DBG_WARN("Cannot connect this adapter: stat(%d)", this->stat);
        return;
      }

      this->stat = kDevConnecting;
      make_connection_cb = cb;
      std::thread(std::bind(&NetworkAdapter::connect, this)).detach();
      break;
  }
}

void NetworkAdapter::set_data_adapter(void) {
  if (at == kATInitialized) {
    OPEL_DBG_WARN("Already initialized: %d", at & kATCtrl);
    return;
  }
  OPEL_DBG_LOG("Data adapter installing");
  NetworkManager::get_instance()->install_data_adapter(this);
  at |= kATInitialized;
  OPEL_DBG_LOG("Data adapter installed");
}

void NetworkAdapter::set_control_adapter(void) {
  if (at & kATInitialized) {
    OPEL_DBG_WARN("Already initialized: %d", at & kATCtrl);
    return;
  }

  if ((at & kATCtrlable) == 0) {
    OPEL_DBG_WARN("Not controllable adapter %d", dev_id);
    return;
  }
  OPEL_DBG_LOG("Control adapter installing");
  NetworkManager::get_instance()->install_control_adapter(this);
  at |= kATCtrl | kATInitialized;

  OPEL_DBG_LOG("Control adapter installed");
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
    else
      stat = kDevOff;
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
    else
      stat = kDevOn;
  }

  if (device_off_cb) device_off_cb(stat);
  device_off_cb = NULL;
}

void NetworkAdapter::connect(void) {
  if (stat != kDevConnecting)
    return;
  
  bool res = make_connection();

  if (res) {
    if ((at & kATCtrl) == 0) {
      OPEL_DBG_LOG("Data adapter connected");
      th_sender =
          new std::thread(std::bind(&NetworkAdapter::run_sender, this));
      th_recver =
          new std::thread(std::bind(&NetworkAdapter::run_recver, this));
    } else {
      OPEL_DBG_LOG("Control adapter connected");
      th_sender = NULL;
      th_recver = NULL;
    }

    stat = kDevCon;
  }
  else
    stat = kDevDiscon;

  if (make_connection_cb) make_connection_cb(stat);
  make_connection_cb = NULL;
}

void NetworkAdapter::close(void) {
  if (stat != kDevDisconnecting)
    return;

  bool res = close_connection();
  if (res) {
    if (at & kATCtrl == 0) {
      th_sender->join();
      th_recver->join();

      delete th_sender;
      delete th_recver;

      th_sender = NULL;
      th_recver = NULL;
    }

    assert(th_sender == NULL && th_recver == NULL);

    stat = kDevDiscon;
  }
  else
    stat = kDevCon;

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
    //OPEL_DBG_LOG("to_send seq_no: %d", to_send->seq_no);
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
    int res = this->recv(buf, len);
    if (res < len) {
      OPEL_DBG_WARN("Recving failed at %s (%s)", dev_name, strerror(errno));
      sm->free_segment(free_seg);
      break;
    }

    uint16_t net_seq_no, net_flag_len;
    memcpy(&net_seq_no, buf, sizeof(uint16_t));
    memcpy(&net_flag_len,
           (reinterpret_cast<uint8_t *>(buf)+2), sizeof(uint16_t));
    free_seg->seq_no = ntohs(net_seq_no);
    free_seg->flag_len = ntohs(net_flag_len);

    sm->enqueue(kSegRecv, free_seg);
    free_seg = sm->get_free_segment();
  }
}

void NetworkAdapter::set_controllable(void) {
  at = at | kATCtrlable;
}
}  /* namespace cm */
