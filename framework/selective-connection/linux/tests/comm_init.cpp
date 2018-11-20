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

#include "../common/inc/ChildProcess.h"
#include "../common/inc/DebugLog.h"

#include "../configs/ExpConfig.h"
#include "../configs/PathConfig.h"
#include "../configs/WfdConfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

using namespace sc;

int kill_dhcpd(void);
void retrieve_wpa_interface_name(std::string &wpaIntfName);

int main(void) {
  char cmdLine[500] = {
      0,
  };

  // Step 1. Bluetooth OFF
  LOG_VERB("Init Step 1. Bluetooth OFF");

  snprintf(cmdLine, 500, "%s hci0 down", HCICONFIG_PATH);
  system(cmdLine);

  // Step 2. Bluetooth ON
  LOG_VERB("Init Step 2. Bluetooth ON");

  snprintf(cmdLine, 500, "%s hci0 up piscan", HCICONFIG_PATH);
  system(cmdLine);

  // Step 3. Wi-fi Direct OFF
  LOG_VERB("Init Step 3. Wi-fi Direct OFF");

  snprintf(cmdLine, 500, "killall udhcpd");
  system(cmdLine);

  snprintf(cmdLine, 500, "%s %s", IFDOWN_PATH, DEFAULT_WFD_DEVICE_NAME);
  system(cmdLine);

  snprintf(cmdLine, 500, "%s %s down", IFCONFIG_PATH, DEFAULT_WFD_DEVICE_NAME);
  system(cmdLine);

  std::string wpaIntfName("");
  retrieve_wpa_interface_name(wpaIntfName);

  if (!wpaIntfName.empty() && wpaIntfName.compare("(null)") == 0) {
    snprintf(cmdLine, 500, "%s p2p_group_remove %s", WPA_CLI_PATH,
             wpaIntfName.c_str());
    system(cmdLine);
  }

  // Step 4. Wi-fi ON
  LOG_VERB("Init Step 4. Wi-fi ON");

  snprintf(cmdLine, 500, "%s %s", IFUP_PATH, DEFAULT_WFD_DEVICE_NAME);
  system(cmdLine);

  snprintf(cmdLine, 500, "%s %s up", IFCONFIG_PATH, DEFAULT_WFD_DEVICE_NAME);
  system(cmdLine);

  return 0;
}

int ping_wpa_cli(char ret[], size_t len) {
  char const *const params[] = {"wpa_cli", "ping", NULL};

  return ChildProcess::run(WPA_CLI_PATH, params, ret, len, true);
}

void retrieve_wpa_interface_name(std::string &wpaIntfName) {
  char wpaIntfNameCstr[100];
  char buf[1024];

  // In the case of Wi-fi USB Dongle, it uses 'wlanX'.
  snprintf(wpaIntfNameCstr, sizeof(wpaIntfNameCstr), DEFAULT_WFD_DEVICE_NAME,
           strlen(DEFAULT_WFD_DEVICE_NAME));

  // In the case of Raspberry Pi 3 Internal Wi-fi Module, it uses 'p2p-wlanX-Y'.
  int ret = ping_wpa_cli(buf, 1024);
  if (ret < 0) {
    LOG_ERR("P2P ping call failed");
    return;
  } else {
    char *ptrptr;
    char *ptr = strtok_r(buf, "\t \n\'", &ptrptr);
    while (ptr != NULL) {
      if (strstr(ptr, "p2p-wlan")) {
        snprintf(wpaIntfNameCstr, sizeof(wpaIntfNameCstr), "%s", ptr);
      } else if (strstr(ptr, "FAIL")) {
        LOG_ERR("P2P ping failed");
        return;
      }
      ptr = strtok_r(NULL, "\t \n\'", &ptrptr);
    }
  }
  wpaIntfName.assign(wpaIntfNameCstr, strlen(wpaIntfNameCstr));
}