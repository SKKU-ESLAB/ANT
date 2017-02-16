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

#include <network_manager.h>

#include <dbug_log.h>
#include <segment_manager.h>

#include <string.h>
#include <thread>
#include <functional>
#include <list>
#include <arpa/inet.h>
#include <unistd.h>

namespace cm {
static NetworkManager *net_manager = NULL;

NetworkManager *NetworkManager::get_instance() {
  if (net_manager == NULL)
    net_manager = new NetworkManager();

  return net_manager;
}

NetworkManager::NetworkManager(void) {
  connecting_dev_id = 0;
  connecting_adapter = NULL;
  th_recver = NULL;

  state = kNetStatDiscon;
  prev_state = kNetStatDiscon;
}

void NetworkManager::install_data_adapter(NetworkAdapter *na) {
  std::unique_lock<std::mutex> lck(lock[kNetData]);
  adapter_list[kNetData].push_back(na);
}

void NetworkManager::remove_data_adapter(NetworkAdapter *na) {
  std::unique_lock<std::mutex> lck(lock[kNetData]);
  adapter_list[kNetData].remove(na);
}

void NetworkManager::network_closed(void) {
  // Close data adapters here
  NetworkAdapter *ca = adapter_list[kNetCtrl].front();
  ca->stat = kDevDisconnecting;

  std::list<NetworkAdapter *>::iterator it;

  for (it = adapter_list[kNetData].begin();
       it != adapter_list[kNetData].end();
       ++it) {
    NetworkAdapter *walker = *it;
    walker->dev_switch(kDevDiscon, NULL);
  }

  ca->_close();

  SegmentManager::get_instance()->reset();

  state = kNetStatDiscon;
}

void NetworkManager::run_control_recver(void) {
  char data[512] = {0, };
  int res = 0;

  while (true) {
    NetworkAdapter *na = adapter_list[kNetCtrl].front();
    assert(na != NULL);

    // Control data parsing
    OPEL_DBG_LOG("Control recver activated");
    res = na->recv(data, 1);
    if (res <= 0) {
      OPEL_DBG_VERB("Control adapter has been closed");
      break;
    }

    if (data[0] == kCtrlReqIncr) {
      OPEL_DBG_VERB("DataIncr request arrived");
      res = na->recv(data, 2);
      if (res <= 0) {
        OPEL_DBG_VERB("Control adapter has been closed");
        break;
      }

      uint16_t ndev_id;
      uint16_t dev_id;

      memcpy(&ndev_id, data, 2);
      dev_id = ntohs(ndev_id);

      OPEL_DBG_VERB("Data adapter increasing %x", dev_id);

      NetworkAdapter *data_na = NULL;

      std::list<NetworkAdapter *>::iterator walker;
      for (walker = adapter_list[kNetData].begin();
           walker != adapter_list[kNetData].end();
           ++walker) {
        NetworkAdapter *na_walker = *walker;
        if (na_walker->get_id() == dev_id) {
          data_na = na_walker;
          break;
        }
      }
      assert(data_na != NULL);

      prev_state = state;
      state = kNetStatIncr;

      na->dev_switch(kDevCon, increase_adapter_cb_wrapper);
    } else if (data[0] == kCtrlReqDecr) {
    } else if (data[0] == kCtrlReqPriv) {
      OPEL_DBG_LOG("Private data arrived");
      uint16_t ndev_id;
      uint16_t dev_id;
      uint32_t nlen;
      uint32_t len;

      do {
        res = na->recv(&ndev_id, 2);
        if (res <= 0) break;

        res = na->recv(&nlen, 4);
        if (res <= 0) break;

        dev_id = ntohs(ndev_id);
        len = ntohl(nlen);
        assert(len <= 512);

        res = na->recv(data, len);
        if (res <= 0) break;

        std::list<NetworkAdapter *>::iterator it;
        for (it = adapter_list[kNetData].begin();
             it != adapter_list[kNetData].end();
             ++it) {
          NetworkAdapter *walker = *it;
          if (walker->get_id() == dev_id) {
            walker->on_control_recv(data, len);
            break;
          }
        }
      } while (false);

      if (res <= 0) {
        OPEL_DBG_VERB("Control adapter closed");
        break;
      }
    }
  }

  network_closed();
  NetworkManager::get_instance()->connect_control_adapter();
}

void NetworkManager::send_control_data(const void *data, size_t len) {
  if (state < kNetStatControl) {
    OPEL_DBG_WARN("Communicator disconnected");
    return;
  }

  if (state != kNetStatIncr && state != kNetStatDecr) {
    OPEL_DBG_WARN("Control data MUST BE sent in only increasing or decreasing adapters");
    return;
  }

  NetworkAdapter *na = adapter_list[kNetCtrl].front();
  na->send(data, len);
}

void NetworkManager::install_control_cb_wrapper(DevState st) {
  NetworkManager::get_instance()->install_control_cb(st);
}
void NetworkManager::install_control_cb(DevState res) {
  if (res == kDevCon) {
    NetworkManager::get_instance()->state = kNetStatControl;
    OPEL_DBG_VERB("Communicator successfully connected");
    if (th_recver != NULL) {
      delete th_recver;
      th_recver = NULL;
    }
    th_recver =
        new std::thread(std::bind(&NetworkManager::run_control_recver, this));
    th_recver->detach();
  } else {
    sleep(1);
    NetworkManager::get_instance()->connect_control_adapter();
    OPEL_DBG_WARN("Communicator re-trying to connect...");
  }
}

void NetworkManager::connect_control_adapter() {
  if (unlikely(state > kNetStatDiscon)) {
    OPEL_DBG_WARN("Invalid trial:Control port already connected");
    return;
  }
  NetworkAdapter *na = adapter_list[kNetCtrl].front();
  state = kNetStatConnecting;
  na->dev_switch(kDevCon, install_control_cb_wrapper);
}

void NetworkManager::install_control_adapter(NetworkAdapter *na) {
  if (state > kNetStatDiscon || adapter_list[kNetCtrl].size() > 0) {
    OPEL_DBG_WARN("Control port already installed");
    return;
  }

  if (state == kNetStatConnecting) {
    OPEL_DBG_WARN("Already connting control port");
    return;
  }

  std::unique_lock<std::mutex> lck(lock[kNetCtrl]);
  adapter_list[kNetCtrl].push_back(na);

  // Make connection of control port
  connect_control_adapter();
}

void NetworkManager::remove_control_adapter(NetworkAdapter *na) {
  std::unique_lock<std::mutex> lck(lock[kNetCtrl]);
  adapter_list[kNetCtrl].remove(na);
}

void NetworkManager::increase_adapter_cb(DevState stat) {
  unsigned char buf[512];

  assert(state == kNetStatIncr);

  if (stat == kDevCon) {
    OPEL_DBG_LOG("Adapter connected :%d ", connecting_adapter->dev_id);
    state = kNetStatData;
    connecting_adapter = NULL;
    SegmentManager::get_instance()->queue_threshold += kSegQueueThreshold;
  } else {
    // Failed to connect, roll back state
    state = prev_state;
  }
}

void NetworkManager::increase_adapter_cb_wrapper(DevState stat) {
  NetworkManager::get_instance()->increase_adapter_cb(stat);
}

void NetworkManager::increase_adapter() {
  if (state == kNetStatIncr || state == kNetStatDecr) {
    OPEL_DBG_WARN("Data ports are busy");
    return;
  }

  if (state <= kNetStatConnecting) {
    OPEL_DBG_ERR("Control port is not opened yet");
    return;
  }

  OPEL_DBG_LOG("Increasing data adapter...");
  prev_state = state;
  state = kNetStatIncr;

  NetworkAdapter *na = select_device();
  if (na == NULL) {
    OPEL_DBG_WARN("All devices are already up");
    state = prev_state;
    return;
  }

  OPEL_DBG_LOG("%d dev is upping", na->dev_id);
  unsigned char buf[512];
  buf[0] = kCtrlReqIncr;
  uint16_t ndev_id = htons(na->dev_id);
  memcpy(buf+1, &ndev_id, 2);
  send_control_data((const void *)buf, 3);

  connecting_adapter = na;
  na->dev_switch(kDevCon, increase_adapter_cb_wrapper);
}

void NetworkManager::decrease_adapter() {
  if (state == kNetStatIncr || kNetStatDecr) {
    OPEL_DBG_WARN("Data ports are busy");
    return;
  }
}

NetworkAdapter *NetworkManager::select_device() {
  std::list<NetworkAdapter *>::iterator it = adapter_list[kNetData].begin();
  NetworkAdapter *res = NULL;

  while (it != adapter_list[kNetData].end()) {
    NetworkAdapter *walker = *it;

    if (walker->stat == kDevDiscon) {
      res = walker;
      break;
    }
    it++;
  }
  return res;
}

} /* namespace cm */
