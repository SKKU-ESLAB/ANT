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
#include "../inc/WfdP2PServer.h"

#include "../inc/WfdServerAdapter.h"

#include "../../common/inc/ChildProcess.h"
#include "../../common/inc/Counter.h"
#include "../../common/inc/DebugLog.h"

#include "../../core/inc/Core.h"

#include "../../configs/WfdConfig.h"

#include <mutex>
#include <thread>

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace sc;

struct sigaction WfdP2PServer::sSigaction;
struct sigaction WfdP2PServer::sSigactionOld;
bool WfdP2PServer::sDhcpdMonitoring = false;
int WfdP2PServer::sDhcpdPid = 0;
WfdP2PServer *WfdP2PServer::sDhcpdCaller = NULL;
bool WfdP2PServer::sDhcpdEnabled = false;

bool WfdP2PServer::allow_discover_impl(void) {
  char buf[1024];
  int ret;
  int ret_bool;

  // Set Wi-fi WPS Device Name
  ret = this->set_wps_device_name(this->mWfdDeviceName, buf, 1024);
  if (ret < 0)
    return false;

  // Add Wi-fi Direct P2P Group
  ret = this->wfd_add_p2p_group(buf, 1024);
  if (ret < 0)
    return false;

  // Check if Adding P2P Group is Successful
  {
    char *ptrptr;
    char *ptr = strtok_r(buf, "\t \n\'", &ptrptr);
    while (ptr != NULL) {
      if (strstr(ptr, "FAIL")) {
        LOG_ERR("allow_discover(%s): p2p_group_add error", this->get_name());
        return false;
      } else if (strstr(ptr, "OK")) {
        LOG_DEBUG("allow_discover(%s): p2p_group_add success",
                  this->get_name());
        break;
      }

      ptr = strtok_r(NULL, "\t \n\'", &ptrptr);
    }
  }

  // Retrieve WPA Interface Name from wpa-cli
  ret_bool = this->retrieve_wpa_interface_name();
  if (!ret_bool) {
    LOG_ERR("allow_discover(%s): retrieve_wpa_interface_name error",
            this->get_name());
    return false;
  }

  // Set Wi-fi Direct IP
  std::string default_ip_addr(DEFAULT_WFD_IP_ADDRESS);
  this->set_wfd_ip_addr(default_ip_addr.c_str());

  // Set DHCPD config
  WfdP2PServer::sDhcpdCaller = this;
  WfdP2PServer::sDhcpdPid = this->launch_dhcpd();

  // Retrieve and Send Wi-fi Direct Information
  {
    char wfdInfo[1024] = {
        '\0',
    };
    // MAC Address
    if (this->mP2PDeviceAddr.empty()) {
      ret = this->get_wfd_p2p_device_addr(buf, 256);
      if (ret < 0) {
        LOG_ERR("allow_discover(%s): get_wfd_p2p_device_addr error",
                this->get_name());
        return false;
      }
      this->mP2PDeviceAddr.assign(buf);
    }
    snprintf(wfdInfo, 1024, "%s", this->mP2PDeviceAddr.c_str());

    // WPS PIN
    ret = this->reset_wfd_server(buf, 256);
    if (ret < 0) {
      LOG_ERR("allow_discover(%s): reset_wfd_server error", this->get_name());
      return false;
    }
    strncat(wfdInfo, "\n", 1024);
    strncat(wfdInfo, buf, 1024);

    // IP Address
    if (this->mIPAddress.empty()) {
      for (int ip_wait_it = 0; ip_wait_it < 30; ip_wait_it++) {
        ret = this->get_wfd_ip_address(buf, 256);
        if (ret == 0) {
          break;
        }
        usleep(300000);
      }
      if (ret < 0) {
        LOG_ERR("allow_discover(%s): get_wfd_ip_address error",
                this->get_name());
        return false;
      }
      this->mIPAddress.assign(buf);
    }
    strncat(wfdInfo, "\n", 1024);
    strncat(wfdInfo, this->mIPAddress.c_str(), 1024);

/* Send WFD Info
 *  <Server MAC Address>
 *  <WPS PIN>
 *  <Server IP Address>
 */
#ifndef EXP_DONT_SEND_PRIV_CONTROL_MESSAGE
    Core::singleton()->get_control_sender()->send_noti_private_data(
        PrivType::kPrivTypeWFDInfo, wfdInfo, strlen(wfdInfo));
#endif

    // Notify IP address to the listeners
    for (std::vector<WfdIpAddressListener *>::iterator it =
             this->mIpAddrListeners.begin();
         it != this->mIpAddrListeners.end(); it++) {
      WfdIpAddressListener *listener = (*it);
      listener->on_change_ip_address(this->mIPAddress.c_str());
    }
  }

  return true;
}

int WfdP2PServer::set_wps_device_name(char *wfd_device_name, char ret[],
                                      size_t len) {
  char const *const params[] = {"wpa_cli", "set", "device_name",
                                wfd_device_name, NULL};

  return ChildProcess::run(WPA_CLI_PATH, params, ret, len, true);
}

int WfdP2PServer::wfd_add_p2p_group(char ret[], size_t len) {
  char const *const params[] = {"wpa_cli", "-i", DEFAULT_WFD_DEVICE_NAME,
                                "p2p_group_add", NULL};

  return ChildProcess::run(WPA_CLI_PATH, params, ret, len, true);
}

bool WfdP2PServer::retrieve_wpa_interface_name(void) {
  char buf[1024];

  // In the case of Wi-fi USB Dongle, it uses 'wlanX'.
  snprintf(this->mWpaIntfName, sizeof(this->mWpaIntfName),
           DEFAULT_WFD_DEVICE_NAME, strlen(DEFAULT_WFD_DEVICE_NAME));

  // In the case of Raspberry Pi 3 Internal Wi-fi Module, it uses 'p2p-wlanX-Y'.
  int ret = this->ping_wpa_cli(buf, 1024);
  if (ret < 0)
    return false;
  else {
    char *ptrptr;
    char *ptr = strtok_r(buf, "\t \n\'", &ptrptr);
    while (ptr != NULL) {
      if (strstr(ptr, "p2p-wlan")) {
        snprintf(this->mWpaIntfName, sizeof(this->mWpaIntfName), "%s", ptr);
      } else if (strstr(ptr, "FAIL")) {
        this->mWpaIntfName[0] = '\0';
        return false;
      }
      ptr = strtok_r(NULL, "\t \n\'", &ptrptr);
    }
  }
  return true;
}

int WfdP2PServer::ping_wpa_cli(char ret[], size_t len) {
  char const *const params[] = {"wpa_cli", "ping", NULL};

  return ChildProcess::run(WPA_CLI_PATH, params, ret, len, true);
}

int WfdP2PServer::set_wfd_ip_addr(const char *ip_addr) {
  assert(this->mWpaIntfName[0] != '\0');

  char const *const params[] = {"ifconfig", this->mWpaIntfName, ip_addr, NULL};
  char buf[256];

  return ChildProcess::run(IFCONFIG_PATH, params, buf, 256, true);
}

int WfdP2PServer::launch_dhcpd(void) {
  WfdP2PServer::sDhcpdEnabled = true;

  if (!WfdP2PServer::sDhcpdMonitoring) {
    WfdP2PServer::sSigaction.sa_flags = SA_SIGINFO;
    WfdP2PServer::sSigaction.sa_sigaction =
        WfdP2PServer::sighandler_monitor_udhcpd;
    sigaction(SIGCHLD, &WfdP2PServer::sSigaction, &WfdP2PServer::sSigactionOld);

    WfdP2PServer::sDhcpdMonitoring = true;
  }

  char const *const params[] = {"udhcpd", UDHCPD_CONFIG_PATH, "-f", NULL};

  // Generate dhcp configuration
  int config_fd = open(UDHCPD_CONFIG_PATH, O_CREAT | O_WRONLY | O_TRUNC, 0644);
#define LINEBUF_SIZE 128
#define SCRIPT_SIZE 512
  char linebuf[LINEBUF_SIZE];
  char script[SCRIPT_SIZE];

  assert(this->mWpaIntfName[0] != '\0');

  snprintf(linebuf, LINEBUF_SIZE, "start %s\n", WFD_DHCPD_LEASES_START_ADDRESS);
  strncpy(script, linebuf, SCRIPT_SIZE);
  snprintf(linebuf, LINEBUF_SIZE, "end %s\n", WFD_DHCPD_LEASES_END_ADDRESS);
  strncat(script, linebuf, SCRIPT_SIZE);
  snprintf(linebuf, LINEBUF_SIZE, "interface %s\n", this->mWpaIntfName);
  strncat(script, linebuf, SCRIPT_SIZE);
  snprintf(linebuf, LINEBUF_SIZE, "max_leases %d\n", WFD_DHCPD_MAX_LEASES);
  strncat(script, linebuf, SCRIPT_SIZE);
  snprintf(linebuf, LINEBUF_SIZE, "option subnet %s\n", WFD_DHCPD_SUBNET_MASK);
  strncat(script, linebuf, SCRIPT_SIZE);
  snprintf(linebuf, LINEBUF_SIZE, "option router %s\n", DEFAULT_WFD_IP_ADDRESS);
  strncat(script, linebuf, SCRIPT_SIZE);
  snprintf(linebuf, LINEBUF_SIZE, "option lease %d\n", WFD_DHCPD_LEASE);
  strncat(script, linebuf, SCRIPT_SIZE);
  snprintf(linebuf, LINEBUF_SIZE, "option broadcast %s\n",
           WFD_DHCPD_BROADCAST_ADDRESS);
  strncat(script, linebuf, SCRIPT_SIZE);

  write(config_fd, script, strlen(script) + 1);
  close(config_fd);

  int dhcpdPid = ChildProcess::run(UDHCPD_PATH, params, false);

  return dhcpdPid;
}

int WfdP2PServer::get_wfd_p2p_device_addr(char *dev_addr, size_t len) {
  char buf[1024];
  int ret;
  if (this->mWpaIntfName[0] == '\0') {
    LOG_WARN("%s: WFD is not up", this->get_name());
    return -1;
  }

  ret = this->get_wfd_status(buf, 1024);
  if (ret < 0)
    return ret;

  std::string buf_str(buf);

  int header_pos = buf_str.find("p2p_device_address=");
  if (header_pos == std::string::npos) {
    LOG_WARN("%s: Get p2p device address failed", this->get_name());
    return -1;
  }

  int target_pos = header_pos + strlen("p2p_device_address=");
  int end_pos = buf_str.find("\n", target_pos);
  int target_length = end_pos - target_pos;
  std::string target_str = buf_str.substr(target_pos, target_length);

  strncpy(dev_addr, target_str.c_str(), target_str.size());
  dev_addr[target_length] = '\0';

  return 0;
}

int WfdP2PServer::get_wfd_status(char ret[], size_t len) {
  char const *const params[] = {"wpa_cli", "status", NULL};

  return ChildProcess::run(WPA_CLI_PATH, params, ret, len, true);
}

int WfdP2PServer::reset_wfd_server(char *pin, size_t len) {
  char buf[1024] = {
      0,
  };
  int ret;
  if (this->mWpaIntfName[0] == '\0') {
    LOG_WARN("%s: WFD is not up", this->get_name());
    return -1;
  }

  ret = this->reset_wps_pin(buf, 1024);
  if (ret < 0)
    return ret;

  // Selected interface 'p2p-wlanx-y' 33996608
  char *ptrptr;
  char *ptr = strtok_r(buf, "\t \n\'", &ptrptr); // Selected
  ptr = strtok_r(NULL, "\t \n\'", &ptrptr);      // interface
  ptr = strtok_r(NULL, "\t \n\'", &ptrptr);      // p2p-wlanx-y
  ptr = strtok_r(NULL, "\t \n\'", &ptrptr);      // 33996608

  snprintf(pin, len, "%s", ptr);

  if (WfdP2PServer::sDhcpdPid == 0) {
    LOG_DEBUG("%s: UDHCP is off > turn it up", this->get_name());
    WfdP2PServer::sDhcpdPid = this->launch_dhcpd();
  }

  return 0;
}

int WfdP2PServer::reset_wps_pin(char ret[], size_t len) {
  char const *const params[] = {"wpa_cli", "wps_pin", "any", "12345670", NULL};

  assert(this->mWpaIntfName[0] != '\0');

  return ChildProcess::run(WPA_CLI_PATH, params, ret, len, true);
}

int WfdP2PServer::get_wfd_ip_address(char *buf, size_t len) {
  int ret = get_wfd_status(buf, 1024);
  if (ret < 0)
    return ret;

  char *ptrptr;
  char *ptr = strtok_r(buf, "\t \n\'", &ptrptr);
  while (ptr != NULL) {
    if (strstr(ptr, "ip_address") || strstr(ptr, "p2p_device_address")) {
      sscanf(ptr, "%*[^=]=%s", buf);
      return 0;
    }

    ptr = strtok_r(NULL, "\t \n\'", &ptrptr);
  }

  LOG_WARN("%s: Get IP address failed", this->get_name());
  return -1;
}

void WfdP2PServer::sighandler_monitor_udhcpd(int signo, siginfo_t *sinfo,
                                             void *context) {
  if (signo != SIGCHLD || WfdP2PServer::sDhcpdPid == 0)
    return;

  if (sinfo->si_pid == WfdP2PServer::sDhcpdPid) {
    int status;
    while (waitpid(WfdP2PServer::sDhcpdPid, &status, WNOHANG) > 0) {
    }
    WfdP2PServer::sDhcpdPid = 0;

    LOG_DEBUG("udhcpd terminated");
    if (WfdP2PServer::sDhcpdCaller != NULL && WfdP2PServer::sDhcpdEnabled) {
      LOG_WARN("Relaunch udhcpd");
      WfdP2PServer::sDhcpdPid = WfdP2PServer::sDhcpdCaller->launch_dhcpd();
    }
  }
}

bool WfdP2PServer::disallow_discover_impl(void) {
  char buf[1024];
  int ret;

  if (this->mWpaIntfName[0] == '\0') {
    LOG_WARN("%s: WFD is not up", this->get_name());
    return false;
  }

  this->kill_dhcpd();

  // Remove Wi-fi Direct P2P Group
  ret = this->wfd_remove_p2p_group(buf, 1024);
  LOG_VERB("wpa_supplicant p2p_group_remove result: %s", buf);

  char *ptrptr;
  char *ptr = strtok_r(buf, "\t \n\'", &ptrptr);
  while (ptr != NULL) {
    if (strstr(ptr, "OK")) {
      LOG_VERB("%s: Succedded to remove p2p group", this->get_name());
      return true;
    } else if (strstr(ptr, "FAIL")) {
      LOG_WARN("%s: Failed to remove p2p group", this->get_name());
      return false;
    }

    ptr = strtok_r(NULL, "\t \n\'", &ptrptr);
  }

  LOG_WARN("%s: wpa_supplicant crashed!", this->get_name());
  // LOG_WARN("%s: wpa_supplicant crashed. revive it.", this->get_name());
  // /* Revive wfd interface */
  // this->turn_off_wfd_interface();
  // sleep(1);
  // this->turn_on_wfd_interface();
  return true;
}

bool WfdP2PServer::turn_off_wfd_interface() {
  char const *const params[] = {"ifdown", DEFAULT_WFD_DEVICE_NAME, NULL};

  return ChildProcess::run(IFDOWN_PATH, params, true);
}

bool WfdP2PServer::turn_on_wfd_interface() {
  char const *const params[] = {"ifup", DEFAULT_WFD_DEVICE_NAME, NULL};

  return ChildProcess::run(IFUP_PATH, params, true);
}

int WfdP2PServer::wfd_remove_p2p_group(char ret[], size_t len) {
  char const *const params[] = {"wpa_cli", "p2p_group_remove",
                                this->mWpaIntfName, NULL};

  assert(this->mWpaIntfName[0] != '\0');

  return ChildProcess::run(WPA_CLI_PATH, params, ret, len, true);
}

int WfdP2PServer::kill_dhcpd(void) {
  WfdP2PServer::sDhcpdEnabled = false;
  WfdP2PServer::sDhcpdMonitoring = false;

  if (unlink(UDHCPD_CONFIG_PATH) != 0)
    LOG_WARN("%s: kill udhcpd failed %s", this->get_name(), strerror(errno));

  if (WfdP2PServer::sDhcpdPid == 0) {
    LOG_DEBUG("%s: no udhcpd to kill", this->get_name());
    return 0;
  }

  kill(WfdP2PServer::sDhcpdPid, SIGKILL);

  return 0;
}