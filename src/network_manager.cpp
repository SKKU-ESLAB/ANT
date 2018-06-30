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

#include <network_manager.h>
#include <counter.h>

#include <dbug_log.h>
#include <segment_manager.h>
#include <network_switcher.h>
#include <ServerAdapter.h>

#include <string.h>
#include <thread>
#include <functional>
#include <list>
#include <arpa/inet.h>
#include <unistd.h>

namespace cm {
/* Singleton */
NetworkManager* NetworkManager::singleton = NULL;

/*  This function is to install the data adapter
 *  This process is simply pushing the adapter to the list
 */
void NetworkManager::install_data_adapter(ServerAdapter *adapter) {
  std::unique_lock<std::mutex> lck(this->mPortLocks[kNetData]);
  mAdapterList[kNetData].push_back(adapter);
}

void NetworkManager::remove_data_adapter(ServerAdapter *adapter) {
  std::unique_lock<std::mutex> lck(this->mPortLocks[kNetData]);
  mAdapterList[kNetData].remove(adapter);
}

void NetworkManager::network_closed(void) {
  // Close data adapters here
  ServerAdapter *control_adapter = mAdapterList[kNetCtrl].front();
  control_adapter->stat = kDevDisconnecting;

  std::list<ServerAdapter *>::iterator it;

  // Disconnect all the data adapters
  for (it = mAdapterList[kNetData].begin();
       it != mAdapterList[kNetData].end();
       ++it) {
    ServerAdapter *data_adapter = *it;
    data_adapter->disconnect();
  }

  // Disconnect control adpater
  control_adapter->disconnect();

  SegmentManager::get_instance()->reset();

  this->mState = kNetStatDiscon;
}

/*  This function is the logic run by control receving thread.
 */
void NetworkManager::run_control_recver(void) {
  printf("control recver thread created! tid: %d\n", (unsigned int)syscall(224));
  char data[512] = {0, };
  int res = 0;
  
  ServerAdapter *adapter = mAdapterList[kNetCtrl].front();
  assert(adapter != NULL);
  LOG_VERB("Control recver activated");

  /* Notify network switcher that control adapter is ready */
  NetworkSwitcher *network_switcher = NetworkSwitcher::get_instance();
  network_switcher->control_adapter_ready();
  
  while (true) {
    // Control data parsing
    res = adapter->recv(data, 1);
    if (res <= 0) {
      LOG_VERB("Control adapter could be closed");
      sleep(1);
    } else {
    /*  If the control message is 'increase adapter', */
    if (data[0] == kCtrlReqIncr) {
      LOG_DEBUG("DataIncr request arrived");
      res = adapter->recv(data, 2);
      if (res <= 0) {
        LOG_DEBUG("Control adapter has been closed");
        break;
      }

      // convert dev_id to ndev_id
      uint16_t ndev_id;
      uint16_t dev_id;

      memcpy(&ndev_id, data, 2);
      dev_id = ntohs(ndev_id);

      LOG_DEBUG("Data adapter increasing %x", dev_id);

      ServerAdapter *data_na = NULL;

      std::list<ServerAdapter *>::iterator walker;
      for (walker = mAdapterList[kNetData].begin();
           walker != mAdapterList[kNetData].end();
           ++walker) {
        ServerAdapter *na_walker = *walker;
        if (na_walker->get_id() == dev_id) {
          data_na = na_walker;
          break;
        }
      }
      assert(data_na != NULL);

      this->mPrevState = this->mState;
      this->mState = kNetStatIncr;

      adapter->dev_switch(kDevCon, increase_adapter_cb_wrapper);
    } else if (data[0] == kCtrlReqDecr) {
    } else if (data[0] == kCtrlReqPriv) {
      LOG_VERB("Private data arrived");
      uint16_t ndev_id;
      uint16_t dev_id;
      uint32_t nlen;
      uint32_t len;

      do {
        res = adapter->recv(&ndev_id, 2);
        if (res <= 0) break;

        res = adapter->recv(&nlen, 4);
        if (res <= 0) break;

        dev_id = ntohs(ndev_id);
        len = ntohl(nlen);
        assert(len <= 512);

        res = adapter->recv(data, len);
        if (res <= 0) break;

        std::list<ServerAdapter *>::iterator it;
        for (it = mAdapterList[kNetData].begin();
             it != mAdapterList[kNetData].end();
             ++it) {
          ServerAdapter *walker = *it;
          if (walker->get_id() == dev_id) {
            walker->on_control_recv(data, len);
            break;
          }
        }
      } while (false);

      if (res <= 0) {
        LOG_DEBUG("Control adapter closed");
        break;
      }
    }
    }
  } // End while

  network_closed();
  NetworkManager::get_instance()->connect_control_adapter();
}

void NetworkManager::send_control_data(const void *data, size_t len) {
  if (this->mState < kNetStatControl) {
    LOG_WARN("Communicator disconnected");
    return;
  }

  if (this->mState != kNetStatIncr && this->mState != kNetStatDecr) {
    LOG_WARN("Control data MUST BE sent in only increasing or decreasing adapters");
    return;
  }

  ServerAdapter *adapter = mAdapterList[kNetCtrl].front();
  adapter->send(data, len);
}

void NetworkManager::install_control_cb_wrapper(DevState st) {
  NetworkManager::get_instance()->install_control_cb(st);
}
void NetworkManager::install_control_cb(DevState res) {
  // After the adapter is connected
  if (res == kDevCon) {
    NetworkManager::get_instance()->mState = kNetStatControl;
    LOG_DEBUG("Communicator successfully connected");
    if (mReceiverThread != NULL) {
      delete mReceiverThread;
      mReceiverThread = NULL;
    }
    // Create control_recver_thread
    mReceiverThread = new std::thread(std::bind(&NetworkManager::run_control_recver, this));
    mReceiverThread->detach();
  } else {
    sleep(1);
    NetworkManager::get_instance()->connect_control_adapter();
    LOG_WARN("Communicator re-trying to connect...");
  }
}

void NetworkManager::connect_control_adapter() {
  if (unlikely(this->mState > kNetStatDiscon)) {
    LOG_WARN("Invalid trial:Control adapter is not in disconnected state");
    return;
  }
  ServerAdapter *adapter = mAdapterList[kNetCtrl].front();
  this->mState = kNetStatConnecting;
  // Connect the device
  adapter->dev_switch(kDevCon, install_control_cb_wrapper);
}

void NetworkManager::install_control_adapter(ServerAdapter *adapter) {
  // Keep only one control adapter in connection 
  if (this->mState > kNetStatDiscon || mAdapterList[kNetCtrl].size() > 0) {
    LOG_WARN("Control port already installed");
    return;
  }
  // Already in the connecting state
  if (this->mState == kNetStatConnecting) {
    LOG_WARN("Already connting control port");
    return;
  }

  std::unique_lock<std::mutex> lck(this->mPortLocks[kNetCtrl]);
  mAdapterList[kNetCtrl].push_back(adapter);

  // Make connection of control port
  connect_control_adapter();
}

void NetworkManager::remove_control_adapter(ServerAdapter *adapter) {
  std::unique_lock<std::mutex> lck(this->mPortLocks[kNetCtrl]);
  mAdapterList[kNetCtrl].remove(adapter);
}

void NetworkManager::increase_adapter_cb(DevState stat) {
  unsigned char buf[512];

  assert(this->mState == kNetStatIncr);

  if (stat == kDevCon) {
    LOG_VERB("Adapter connected :%d ", this->mConnectingAdapter->dev_id);
    this->mState = kNetStatData;
    this->mConnectingAdapter = NULL;
  } else {
    // Failed to connect, roll back state
    LOG_VERB("Adapter Connection Failed\n");
    this->mState = this->mPrevState;
  }
  NetworkSwitcher::get_instance()->done_switch();
}

void NetworkManager::increase_adapter_cb_wrapper(DevState stat) {
  NetworkManager::get_instance()->increase_adapter_cb(stat);
}

void NetworkManager::increase_adapter() {
  // The adapter is already increasing adapter
  if (this->mState == kNetStatIncr || this->mState == kNetStatDecr) {
    LOG_WARN("Data ports are busy");
    NetworkSwitcher::get_instance()->done_switch();
    return;
  }

  if (this->mState <= kNetStatConnecting) {
    LOG_ERR("Control port is not opened yet");
    NetworkSwitcher::get_instance()->done_switch();
    return;
  }

  this->mPrevState = this->mState;
  this->mState = kNetStatIncr;

  /* Select the data adapter available */
  ServerAdapter *adapter = select_adapter();
  if (adapter == NULL) {
    this->mState = this->mPrevState;
    NetworkSwitcher::get_instance()->done_switch();
    return;
  }

  /* send control message to turn on the new data adapter */
  unsigned char buf[512];
  buf[0] = kCtrlReqIncr;
  uint16_t ndev_id = htons(adapter->dev_id);
  memcpy(buf+1, &ndev_id, 2);
  send_control_data((const void *)buf, 3);

  this->mConnectingAdapter = adapter;
  adapter->dev_switch(kDevCon, increase_adapter_cb_wrapper);
}

bool NetworkManager::is_data_adapter_on(){
  std::list<ServerAdapter *>::iterator it = mAdapterList[kNetData].begin();

  while (it != mAdapterList[kNetData].end()) {
    ServerAdapter *walker = *it;
    if (walker->stat == kDevCon) {
      return true;
    }
    it++;
  }
  return false;
}

void NetworkManager::decrease_adapter_cb(DevState stat) {
  unsigned char buf[512];

  assert(this->mState == kNetStatDecr);

  if (stat == kDevDiscon) {
    LOG_VERB("Adapter disconnected :%d ", this->mConnectingAdapter->dev_id);
    if(is_data_adapter_on()){ 
      this->mState = kNetStatData;
    } else {
      this->mState = kNetStatControl;
    }
    this->mConnectingAdapter = NULL;
  } else {
    LOG_VERB("Adapter is not disabled\n");
    // If failed to connect, roll back state
    this->mState = this->mPrevState;
  }
  NetworkSwitcher::get_instance()->done_switch();
}

void NetworkManager::decrease_adapter_cb_wrapper(DevState stat) {
  NetworkManager::get_instance()->decrease_adapter_cb(stat);
}

void NetworkManager::decrease_adapter() {
  // The adapter is already increasing or decreasing adapter
  if (this->mState == kNetStatIncr || this->mState == kNetStatDecr) {
    LOG_WARN("Data ports are busy");
    NetworkSwitcher::get_instance()->done_switch();
    return;
  }

  if (this->mState <= kNetStatConnecting){
    LOG_ERR("Control port is not opened yet");
    NetworkSwitcher::get_instance()->done_switch();
    return;
  }

  this->mPrevState = this->mState;
  this->mState = kNetStatDecr;

  /* Select one of the data adapters on */
  ServerAdapter *adapter = select_adapter_on();
  if(adapter == NULL){
    LOG_DEBUG("All devices are already down");
    this->mState = this->mPrevState;
    NetworkSwitcher::get_instance()->done_switch();
    return;
  }
  decreasing_adapter_id = adapter->dev_id;

  /* Check the network device, which the adapter is using, is used by other adapter
   * if it's used by other adapters, the device should not be turned off
   */
  /* adapter->delete_threads(); */

  /* Send control message to turn off the working data adapter */

  unsigned char buf[512];
  buf[0] = kCtrlReqDecr;
  uint16_t ndev_id = htons(adapter->dev_id);
  memcpy(buf+1, &ndev_id, 2);
  send_control_data((const void *)buf, 3);

  this->mConnectingAdapter = adapter;
  adapter->dev_switch(kDevDiscon, decrease_adapter_cb_wrapper); 
  __FUNCTION_EXIT__; 
}

/*  This function selects the adapter off in the list to use additionally.
 *  This selecting algorithm can be optimized for better output.
 */
ServerAdapter *NetworkManager::select_adapter() { 
  std::list<ServerAdapter *>::iterator it = mAdapterList[kNetData].begin();

  ServerAdapter *res = NULL;

  while (it != mAdapterList[kNetData].end()) {
    ServerAdapter *walker = *it;
    if (walker->stat == kDevDiscon) {
      res = walker;
      break;
    }
    it++;
  }
  return res;
}

/*  This function selects the adapter on in the list to use additionally.
 *  This selecting algorithm can be optimized for better output.
 */
ServerAdapter *NetworkManager::select_adapter_on() {
  std::list<ServerAdapter *>::reverse_iterator it = mAdapterList[kNetData].rbegin();
  ServerAdapter *res = NULL;
  int num_on = 0;

  while (it != mAdapterList[kNetData].rend()) {
    ServerAdapter *walker = *it;
    if (walker->stat == kDevCon) { 
      if(res == NULL){
        res = walker;
      }
      num_on++;
    }
    it++;
  }
  if(num_on > 1){
    LOG_DEBUG("working adapter is more than once\n");
    return res;
  } else {
    LOG_DEBUG("working adapter is one or 0\n");
    return NULL;
  }
}
} /* namespace cm */
