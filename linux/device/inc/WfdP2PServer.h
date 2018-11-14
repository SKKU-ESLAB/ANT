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

#ifndef __WFD_P2P_SERVER_H__
#define __WFD_P2P_SERVER_H__

#include "WfdIpAddressListener.h"

#include "../../core/inc/P2PServer.h"

#include <mutex>
#include <thread>
#include <vector>
#include <string>

#include <signal.h>
#include <stdio.h>

namespace sc {
class WfdP2PServer : public P2PServer {
public:
  virtual bool allow_discover_impl(void);
  virtual bool disallow_discover_impl(void);

  void add_wfd_ip_address_listener(WfdIpAddressListener *listener) {
    this->mIpAddrListeners.push_back(listener);
  }

  WfdP2PServer(const char *wfd_device_name, void *owner) : P2PServer("WFD") {
    snprintf(this->mWfdDeviceName, 100, "%s", wfd_device_name);
    this->mWpaIntfName[0] = '\0';

    this->mOwner = owner;
  }

  ~WfdP2PServer(void) {}

protected:
  std::vector<WfdIpAddressListener *> mIpAddrListeners;

  char mWfdDeviceName[100];

  char mWpaIntfName[256];

  std::string mP2PDeviceAddr;
  std::string mIPAddress;

  // In order to monitor the termination of child udhcpd process
  static struct sigaction sSigaction, sSigactionOld;
  static bool sDhcpdMonitoring;
  static int sDhcpdPid;
  static WfdP2PServer *sDhcpdCaller;
  static bool sDhcpdEnabled;

  void *mOwner;

private:
  int set_wps_device_name(char *wfd_device_name, char ret[], size_t len);
  int wfd_add_p2p_group(char ret[], size_t len);
  int wfd_remove_p2p_group(char ret[], size_t len);
  bool turn_off_wfd_interface();
  bool turn_on_wfd_interface();
  bool retrieve_wpa_interface_name(void);
  int ping_wpa_cli(char ret[], size_t len);
  int set_wfd_ip_addr(const char *ip_addr);
  int launch_dhcpd(void);
  int get_wfd_p2p_device_addr(char *dev_addr, size_t len);
  int get_wfd_status(char ret[], size_t len);
  int reset_wfd_server(char *pin, size_t len);
  int reset_wps_pin(char ret[], size_t len);
  int get_wfd_ip_address(char *buf, size_t len);
  static void sighandler_monitor_udhcpd(int signo, siginfo_t *sinfo,
                                        void *context);
  int kill_dhcpd(void);
}; /* class WfdP2PServer */
} /* namespace sc */

#endif /* !defined(__WFD_P2P_SERVER_H__) */