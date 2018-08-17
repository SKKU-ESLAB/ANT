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

#include <P2PServer.h>

#include <WfdIpAddressListener.h>

#include <vector>
#include <thread>
#include <mutex>

#include <stdio.h>
#include <signal.h>

namespace cm {

class WfdP2PServer : public P2PServer {
public:
  virtual bool allow_impl(void);
  virtual bool disallow_impl(void);

  void add_wfd_ip_address_listener(WfdIpAddressListener* listener) {
    this->mIpAddrListeners.push_back(listener);
  }

  WfdP2PServer(const char* wfd_device_name, void* owner) {
    snprintf(this->mWfdDeviceName, 100, "%s", wfd_device_name);
    this->mWfdDeviceName[0] = '\0';
    this->mWpaDevName[0] = '\0';
    this->mWpaIntfName[0] = '\0';

    this->mOwner = owner;
  }

  ~WfdP2PServer(void) {
  }

protected:
  std::vector<WfdIpAddressListener*> mIpAddrListeners;

  char mWfdDeviceName[100];

  char mWpaDevName[256];
  char mWpaIntfName[256];

  // In order to monitor the termination of child udhcpd process
  static struct sigaction sSigaction, sSigactionOld;
  static bool sDhcpdMonitoring;
  static int sDhcpdPid;

  void* mOwner;

private:
  int set_wps_device_name(char *wfd_device_name, char ret[], size_t len);
  int wfd_add_p2p_group(char ret[], size_t len);
  int wfd_remove_p2p_group(char ret[], size_t len);
  int ping_wpa_cli(char ret[], size_t len);
  int set_wfd_ip_addr(char* ip_addr);
  int set_dhcpd_config(void);
  int get_wfd_p2p_device_addr(char *dev_addr, size_t len);
  int get_wfd_status(char ret[], size_t len);
  int reset_wfd_server(char *pin, size_t len);
  int reset_wps_pin(char ret[], size_t len);
  int get_wfd_ip_address(char *buf, size_t len);
  static void sighandler_monitor_udhcpd(int signo, siginfo_t *sinfo, void *context);
  int kill_dhcpd(void);
}; /* class WfdP2PServer */

} /* namespace cm */

#endif /* !defined(_WFD_P2P_SERVER_H_) */
