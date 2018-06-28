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
    this->mWfdDeviceName[0] = NULL;
    this->mWpaDevName[0] = NULL;
    this->mWpaIntfName[0] = NULL;
  }

  ~WfdP2pServer(void) {
  }

protected:
  char mWfdDeviceName[100];

  char mWpaDevName[256];
  char mWpaIntfName[256];

  static struct sigaction sSigaction, sSigactionOld;
  static bool sDhcpdMonitoring;
  static int sDhcpdPid;

private:
  int set_wps_device_name(char *wfd_device_name);
  int wfd_add_p2p_group(char ret[], size_t len);
  int wfd_remove_p2p_group(char ret[], size_t len);
  int ping_wpa_cli(char ret[], size_t len);
  int set_wfd_ip_addr(const char* ip_addr);
  int set_dhcpd_config(void);
  static void sighandler_monitor_udhcpd(int signo, siginfo_t *sinfo, void *context);
}; /* class WfdP2pServer */

} /* namespace cm */

#endif /* !defined(_WFD_P2P_SERVER_H_) */
