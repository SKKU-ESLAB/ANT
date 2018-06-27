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
#include <WfdP2pServer.h>
#include <Util.h>

#include <counter.h>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <stdio.h>

using namespace cm;

bool WfdP2pServer::allow_impl(void) {
  char wpa_cli_dev_name[256] = {0, };
  char wpa_cli_intf_name[256] = {0, };
  char buf[1024];
  int ret;

  ret = this->wifi_set_device_name(this->mWfdDeviceName, buf, 1024);
  if (ret != 0)
    return ret;

  ret = this->wifi_direct_p2p_group_add(buf, 1024);
  if (ret != 0)
    return ret;

  if (wpa_cli_dev_name[0] == '\0') {
    LOG_VERB("%s", buf);
    char *ptrptr;
    char *ptr = strtok_r(buf, "\t \n\'", &ptrptr);
    while (ptr != NULL) {
      if (strstr(ptr, "p2p-dev")) {
        snprintf(wpa_cli_dev_name, sizeof(wpa_cli_dev_name), "%s", ptr);
      } else if (strstr(ptr, "FAIL")) {
        LOG_WARN("P2P Group add failed");
        return -1;
      } else if (strstr(ptr, "OK")) {
        LOG_VERB("P2P Group added");
      }

      ptr = strtok_r(NULL, "\t \n\'", &ptrptr);
    }
  }

  assert(wpa_cli_dev_name[0] != '\0');

  ret = this->wifi_direct_ping(buf, 1024);
  if (ret != 0)
    return ret;

  char *ptrptr;
  char *ptr = strtok_r(buf, "\t \n\'", &ptrptr);
  while (ptr != NULL) {
    if (strstr(ptr, "p2p-wlan")) {
      snprintf(wpa_cli_intf_name, sizeof(wpa_cli_intf_name), "%s", ptr);
    } else if (strstr(ptr, "PONG")) {
      LOG_VERB("Added interface:%s", wpa_cli_intf_name);
    } else if (strstr(ptr, "FAIL")) {
      LOG_WARN("P2P Group add failed");
      wpa_cli_intf_name[0] = '\0';
      return -1;
    }
    ptr = strtok_r(NULL, "\t \n\'", &ptrptr);
  }

  ret = this->wifi_direct_ip_setup("192.168.49.1");
  
  ret = this->wifi_dhcp_setup();

  return ret;
}

int WfdP2pServer::wifi_set_device_name(char *wfd_device_name) {
  // TODO(esevan): Implementation.
  return 0;
}

int WfdP2pServer::wifi_direct_p2p_group_add(char ret[], size_t len) {
  char *const params[] = {"wpa_cli", "p2p_group_add", NULL};

  return Util::run_client(WPA_CLI_PATH, params, ret, len);
}

int WfdP2pServer::wifi_direct_ping(char ret[], size_t len) {
  char *const params[] = {"wpa_cli", "ping", NULL};

  return Util::run_client(WPA_CLI_PATH, params, ret, len);
}

int WfdP2pServer::wifi_direct_ip_setup(const char* ip_addr) {
  char *const params[] = {"ifconfig",
    wpa_cli_intf_name,
    ip_addr,
    NULL};
  char buf[256];

  assert(wpa_cli_intf_name[0] != '\0');

  return Util::run_client(IFCONFIG_PATH, params, buf, 256);
}

int WfdP2pServer::wifi_dhcp_setup(void) {
  char *const params[] = {"udhcpd", "dhcpd.conf", "-f", NULL};
  char buf[256];
  
  assert(wpa_cli_intf_name[0] != '\0');

  // Generate dhcp configuration
  int conf_fd = open(DHCPD_CONF_PATH, O_CREAT | O_WRONLY | O_TRUNC, 0644);
  char script[512];

  sprintf(script, "start 192.168.49.20\n"\
          "end 192.168.49.40\n"\
          "interface %s\n"\
          "max_leases 20\n"\
          "option subnet 255.255.255.0\n"\
          "option router 192.168.49.1\n"\
          "option lease 864000\n"\
          "option broadcast 192.168.49.255", wpa_cli_intf_name);

  write(conf_fd, script, strlen(script)+1);
  close(conf_fd);

  return Util::run_client(UDHCPD_PATH, params, buf, 256);
}

bool WfdP2pServer::disallow_impl(void) {
  // TODO:
}
