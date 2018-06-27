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

#ifndef _WFD_P2P_SERVER_H_
#define _WFD_P2P_SERVER_H_

#include <P2pServer.h>

#include <counter.h>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <stdio.h>

namespace cm {

class WfdP2pServer : P2pServer {
public:
  virtual bool allow_impl(void);
  virtual bool disallow_impl(void);

  WfdP2pServer(const char* wfd_device_name) {
    snprintf(this->mWfdDeviceName, 100, "%s", wfd_device_name);
  }

  ~WfdP2pServer(void) {
  }

protected:
  char mWfdDeviceName[100];

private:
  int wifi_set_device_name(char *wfd_device_name);
  int wifi_direct_p2p_group_add(char ret[], size_t len);
  int wifi_direct_ping(char ret[], size_t len);
  int wifi_direct_ip_setup(const char* ip_addr);
  int wifi_dhcp_setup(void);
}; /* class WfdP2pServer */

} /* namespace cm */

#endif /* !defined(_WFD_P2P_SERVER_H_) */
