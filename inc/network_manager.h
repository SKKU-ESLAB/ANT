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

#ifndef INC_NETWORK_MANAGER_H_
#define INC_NETWORK_MANAGER_H_
#include <network_adapter.h>
#include <segment_manager.h>
#include <protocol_manager.h>

#include <stdint.h>
#include <list>
#include <mutex>
#include <thread>
#include <condition_variable>

#include <time.h>


namespace cm {
/* Ctrl header structure
 * |--|----|Data|
 *  CtrlReq (1B), DevId (2B) if Req >= 2, Data (-B) if Req == 4
 */

typedef enum {
  kCtrlReqOk = 0,
  kCtrlReqFail = 1,
  kCtrlReqIncr = 2,
  kCtrlReqDecr = 3,
  kCtrlReqPriv = 4 
} CtrlReq;

typedef enum {
  kNetCtrl = 0,
  kNetData = 1,
  kNetMaxPort = 2
} PortType;

typedef enum {
  kNetStatDiscon = 0,
  kNetStatConnecting = 1,
  kNetStatControl = 2,
  kNetStatIncr = 3,
  kNetStatDecr = 4,
  kNetStatData = 5
} NetStat;

class NetworkAdapter;
class NetworkManager {
  public:
    uint16_t decreasing_adapter_id;

    void install_data_adapter(NetworkAdapter *na);
    void remove_data_adapter(NetworkAdapter *na);

    void install_control_adapter(NetworkAdapter *na);
    void remove_control_adapter(NetworkAdapter *na);

    void increase_adapter(void);
    void decrease_adapter(void);
    void send_control_data(const void *data, size_t len);

    std::list<NetworkAdapter *> get_control_adapter_list() {
      return this->mAdapterList[kNetCtrl];
    }
    std::list<NetworkAdapter *> get_data_adapter_list() {
      return this->mAdapterList[kNetData];
    }

    /* Singleton */
    static NetworkManager *get_instance() {
      if (singleton == NULL)
        singleton = new NetworkManager();

      return singleton;
    }

  private:
    /* Singleton */
    static NetworkManager* singleton;
    NetworkManager(void) {
      this->mConnectingAdapter = NULL;
      mReceiverThread = NULL;
      this->mState = kNetStatDiscon;
      this->mPrevState = kNetStatDiscon;
    }

    NetStat mState;
    NetStat mPrevState;
    std::mutex mPortLocks[kNetMaxPort];
    std::list<NetworkAdapter *> mAdapterList[kNetMaxPort];
    NetworkAdapter *mConnectingAdapter;

    std::thread *mReceiverThread;

    bool is_data_adapter_on(void);
    static void install_control_cb_wrapper(DevState st);
    void install_control_cb(DevState st);
    void connect_control_adapter(void);
    void run_control_recver(void);
    void network_closed(void);

    NetworkAdapter *select_adapter(void);
    NetworkAdapter *select_adapter_on(void);

    static void increase_adapter_cb_wrapper(DevState stat);
    void increase_adapter_cb(DevState stat);

    static void decrease_adapter_cb_wrapper(DevState stat);
    void decrease_adapter_cb(DevState stat);
};
} /* namespace cm */
#endif  /* INC_NETWORK_MANAGER_H_ */
