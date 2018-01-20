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

#ifndef INC_NETWORK_MANAGER_H_
#define INC_NETWORK_MANAGER_H_
#include <network_adapter.h>
#include <segment_manager.h>
#include <protocol_manager.h>

#include <stdint.h>
#include <list>
#include <mutex>
#include <condition_variable>

#include <time.h>


namespace cm {
typedef enum {
  kCtrlReqOk = 0,
  kCtrlReqFail = 1,
  kCtrlReqIncr = 2,
  kCtrlReqDecr = 3,
  kCtrlReqPriv = 4 
} CtrlReq;

class NetworkManager {
 public:
  uint16_t decreasing_adapter_id;
 
  static NetworkManager *get_instance(void);

  void install_data_adapter(NetworkAdapter *na);
  void remove_data_adapter(NetworkAdapter *na);

  void install_control_adapter(NetworkAdapter *na);
  void remove_control_adapter(NetworkAdapter *na);

  void increase_adapter(void);
  void decrease_adapter(void);
  void send_control_data(const void *data, size_t len);
 private:
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

  typedef struct networkdevicestatus {
    int num_bt;
    int num_eth;
    int num_wfd;
  } netdevstat;

  /* Ctrl header structure
   * |--|----|Data|
   *  CtrlReq (1B), DevId (2B) if Req >= 2, Data (-B) if Req == 4
   */

  NetworkManager(void);

  unsigned short connecting_dev_id;
  NetStat state;
  NetStat prev_state;
  std::mutex lock[kNetMaxPort];
  std::list<NetworkAdapter *> adapter_list[kNetMaxPort];
  NetworkAdapter *connecting_adapter;

  struct timespec increase_time;

  std::thread *th_recver;

  bool is_data_adapter_on(void);
  static void install_control_cb_wrapper(DevState st);
  void install_control_cb(DevState st);
  void connect_control_adapter(void);
  void run_control_recver(void);
  void network_closed(void);

  NetworkAdapter *select_device(void);
  NetworkAdapter *select_device_on(void);

  static void increase_adapter_cb_wrapper(DevState stat);
  void increase_adapter_cb(DevState stat);

  static void decrease_adapter_cb_wrapper(DevState stat);
  void decrease_adapter_cb(DevState stat);
};
} /* namespace cm */
#endif  /* INC_NETWORK_MANAGER_H_ */
