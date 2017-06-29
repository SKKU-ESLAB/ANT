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

/*  This function is the core function which manages the network adapter
 *  This tries to make the adapter in the target state
 */
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

      if (this->stat == kDevDisconnecting) {
        OPEL_DBG_WARN("Device is busy");
        return;
      }
      this->stat = kDevDisconnecting;
      close_connection_cb = cb;
      std::thread(std::bind(&NetworkAdapter::_close, this)).detach();
      break;
    case kDevCon:
      // Can connect the device only if it's in the disconnect state
      if (this->stat > kDevDiscon) {
        OPEL_DBG_WARN("Cannot connect this adapter: stat(%d)", this->stat);
        return;
      }

      this->stat = kDevConnecting;
      make_connection_cb = cb;
      std::thread(std::bind(&NetworkAdapter::_connect, this)).detach();
      break;
  }
}

void NetworkAdapter::set_data_adapter(void) {
  // Check if the adapter is initialized
  if (at == kATInitialized) {
    OPEL_DBG_WARN("Already initialized: %d", at & kATCtrl);
    return;
  }
  // Install the adapter as the data adapter
  NetworkManager::get_instance()->install_data_adapter(this);
  at |= kATInitialized;
}

/*  This function sets the adapter as a control adapter
 */
void NetworkAdapter::set_control_adapter(void) {
  // Check if the adapter is initialized
  if (at & kATInitialized) {
    OPEL_DBG_WARN("Already initialized: %d", at & kATCtrl);
    return;
  }
  
  // Check if the adapter is controllable
  if ((at & kATCtrlable) == 0) {
    OPEL_DBG_WARN("Not controllable adapter %d", dev_id);
    return;
  }
  // Install the control adapter
  NetworkManager::get_instance()->install_control_adapter(this);
  at |= kATCtrl | kATInitialized;
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
/*  This function is connecting function of the adapter as the name says.
 *  This is run by the independent thread created in the dev_switch() function. 
 */
void NetworkAdapter::_connect(void) {

  printf("connect thread created! tid: %d\n", (unsigned int)syscall(224));
  // It should be in the connecting state, when it really tries to connect
  if (stat != kDevConnecting)
    return;
  /*  make_connection() function is the function defined in each network adapter
   */
  bool res = make_connection();
  
  if (res) 
  {
    /*  Only if it's data adapter, run the sender & recver thread.
     *  Control adapter uses other thread (control_recver_thread)
     */
    if ((at & kATCtrl) == 0) {  // It's data adapter
      OPEL_DBG_LOG("Data adapter connected");
      th_sender =
          new std::thread(std::bind(&NetworkAdapter::run_sender, this));
      th_recver =
          new std::thread(std::bind(&NetworkAdapter::run_recver, this));
      th_sender->detach();
      th_recver->detach();
    } else {
      OPEL_DBG_LOG("Control adapter connected");
      th_sender = NULL;
      th_recver = NULL;
    }

    stat = kDevCon;
  }
  else {
    stat = kDevDiscon;
    OPEL_DBG_LOG("NetworkAdapter::_connect: Not connected!\n");
  }
  if (make_connection_cb){
    OPEL_DBG_LOG("connect callback called!\n");
    make_connection_cb(stat);
  }
  make_connection_cb = NULL;
}

void NetworkAdapter::_close(void) {
  __OPEL_FUNCTION_ENTER__;
  if (stat != kDevDisconnecting)
    return;


  bool res = close_connection();
  if (res) {
    OPEL_DBG_LOG("connection closed\n");
    if ((at & kATCtrl) == 0) {
      //SegmentManager::get_instance()->notify_queue();
      
      //OPEL_DBG_LOG("Join the sender & recver thread\n"); 
      //th_recver->join();
      //th_sender->join();

      OPEL_DBG_LOG("delete the sender & recver thread\n");

      delete th_sender;
      delete th_recver;

      th_sender = NULL;
      th_recver = NULL;
    }

    assert(th_sender == NULL && th_recver == NULL);
    
    stat = kDevDiscon;
  } else{
    stat = kDevCon;
  }

  if (close_connection_cb) {
    OPEL_DBG_LOG("Call close_connection callback\n");
    close_connection_cb(stat);
  } else{
    OPEL_DBG_LOG("No callback\n");
  }
  close_connection_cb = NULL;
}

void NetworkAdapter::return_sending_failed_packet(void *seg) {
  Segment *to_send = reinterpret_cast<Segment *>(seg);

  SegmentManager::get_instance()->failed_sending(to_send);
}

void NetworkAdapter::run_sender(void) {
  printf("sender thread created! tid: %d\n", (unsigned int)syscall(224));
  SegmentManager *sm = SegmentManager::get_instance();

  while (true) {
    Segment *to_send;

    if (likely((to_send = sm->get_failed_sending()) == NULL)){
      to_send = sm->dequeue(kSegSend);
    }

    if (to_send == NULL) {
      if (stat < kDevCon) break;
      else continue;
    }

    int len = kSegHeaderSize + kSegSize;
    const void *data = to_send->data;
    //OPEL_DBG_LOG("to_send seq_no: %d", to_send->seq_no);
    bool res = this->send(data, len); 
    if (!res) {
      OPEL_DBG_WARN("Sending failed at %s (%s)", dev_name, strerror(errno));
      return_sending_failed_packet(to_send);
      break;
    }
    sm->free_segment(to_send);
  }

  dev_switch(kDevDiscon, NULL);
}

void NetworkAdapter::run_recver(void) {
  printf("recever thread created! tid: %d\n", (unsigned int)syscall(224));
  SegmentManager *sm = SegmentManager::get_instance();
  Segment *free_seg = sm->get_free_segment();

  while (true) {
    void *buf = reinterpret_cast<void *>(free_seg->data);
    int len = kSegSize + kSegHeaderSize;
    int res = this->recv(buf, len);
    if (res < len) {
      OPEL_DBG_WARN("Recving failed at %s (%s)", dev_name, strerror(errno));
      sm->free_segment(free_seg);
      break;
    }


    uint32_t net_seq_no, net_flag_len;
    memcpy(&net_seq_no, buf, sizeof(uint32_t));
    memcpy(&net_flag_len,
           (reinterpret_cast<uint8_t *>(buf)+4), sizeof(uint32_t));
    free_seg->seq_no = ntohl(net_seq_no);
    free_seg->flag_len = ntohl(net_flag_len);

    OPEL_DBG_LOG("Recved:%d/%d(%d)", free_seg->seq_no, free_seg->flag_len, res);
    sm->enqueue(kSegRecv, free_seg);
    free_seg = sm->get_free_segment();
  }

  dev_switch(kDevDiscon, NULL);
}

void NetworkAdapter::set_controllable(void) {
  at = at | kATCtrlable;
}

void NetworkAdapter::send_ctrl_msg(const void *buf, int len) {
  uint8_t req = kCtrlReqPriv;
  uint16_t net_dev_id = htons(this->dev_id);
  uint32_t net_len = htonl(len);

  if ((at & kATCtrl) == kATCtrl) {
    OPEL_DBG_WARN("Cannot transfer private data in control adapter");
    return;
  }

  NetworkManager *nm = NetworkManager::get_instance();
  nm->send_control_data(&req, 1);
  nm->send_control_data(&net_dev_id, 2);
  nm->send_control_data(&net_len, 4);
  nm->send_control_data(buf, len);
}

}  /* namespace cm */
