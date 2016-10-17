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

#ifndef INC_NETWORK_MANAGER_H_
#define INC_NETWORK_MANAGER_H_
#include <network_adapter.h>
#include <segment_manager.h>
#include <protocol_manager.h>

#include <stdint.h>
#include <list>
#include <mutex>
#include <condition_variable>


namespace cm {
class NetworkManager {
 public:
  static NetworkManager *get_instance(void);

  void install_data_adapter(NetworkAdapter *na);
  void remove_data_adapter(NetworkAdapter *na);

  void install_control_adapter(NetworkAdapter *na);
  void remove_control_adapter(NetworkAdapter *na);

  void increase_adapter(void);
  void decrease_adapter(void);


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

  NetworkManager(void);

  unsigned short connecting_dev_id;
  NetStat state;
  NetStat prev_state;
  std::mutex lock[kNetMaxPort];
  std::list<NetworkAdapter *> adapter_list[kNetMaxPort];
  NetworkAdapter *connecting_adapter;

  std::thread *th_recver;

  static void install_control_cb_wrapper(DevState st);
  void install_control_cb(DevState st);
  void connect_control_adapter(void);
  void run_control_recver(void);
  void send_control_data(const void *data, size_t len);
  void network_closed(void);

  NetworkAdapter *select_device(void);
  
  static void increase_adapter_cb_wrapper(DevState stat);
  void increase_adapter_cb(DevState stat);

  static void decrease_adapter_cb_wrapper(DevState stat);
  void decrease_adapter_cb(DevState stat);
};
} /* namespace cm */
#endif  /* INC_NETWORK_MANAGER_H_ */
