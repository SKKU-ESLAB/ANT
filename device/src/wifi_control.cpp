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

#include <wifi_control.h>
#include <dbug_log.h>

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/wait.h>

#define WPA_CLI_PATH "/sbin/wpa_cli"
#define IFCONFIG_PATH "/sbin/ifconfig"

namespace wifi {
static char wpa_cli_dev_name[256] = {0, };
static char wpa_cli_intf_name[256] = {0, };

static int run_cli(char *res_buf, size_t len, char *const params[], int v = 0) {
  int fd[2];
  int pid;
  int bk;

  if (pipe(fd) < 0) {
    OPEL_DBG_ERR("pipe open error");
    return errno;
  }
  dup2(1, bk);

  if ((pid = fork()) < 0) {
    OPEL_DBG_ERR("fork error");
    return errno;
  } else if (pid > 0) {
    /* Parent process */
    close(fd[1]);

    char buf[1024];
    int read_bytes = read(fd[0], buf, 1024);

    if (read_bytes < 0) {
      OPEL_DBG_ERR("read error");
      return errno;
    }

    waitpid(pid, NULL, 0);

    memcpy(res_buf, buf, read_bytes < len ? read_bytes:len);
    dup2(bk, 1);

    close(fd[0]);

    return 0;
  } else {
    /* Child process */
    close(fd[0]);
    dup2(fd[1], 1);

    if (v == 0)
      execv(WPA_CLI_PATH, params);
    else
      execv(IFCONFIG_PATH, params);
  }
}
inline static int wifi_direct_ping(char ret[], size_t len) {
  char *const params[] = {"wpa_cli", "ping", NULL};

  return run_cli(ret, len, params);
}
inline static int wifi_direct_p2p_group_remove(char ret[], size_t len) {
  char *const params[] = {"wpa_cli",
    "-i",
    wpa_cli_dev_name,
    "p2p_group_remove",
    wpa_cli_intf_name,
    NULL};

  assert(wpa_cli_dev_name[0] != '\0' && wpa_cli_intf_name[0] != '\0');

  return run_cli(ret, len, params);
}

inline static int wifi_direct_p2p_group_add(char ret[], size_t len) {
  char *const params[] = {"wpa_cli", "p2p_group_add", NULL};

  return run_cli(ret, len, params);
}

inline static int wifi_direct_wpa_cli_pin_reset(char ret[], size_t len) {
  char *const params[] = {"wpa_cli", "wps_pin", "any", "12345670", NULL};

  assert(wpa_cli_intf_name[0] != '\0');

  return run_cli(ret, len, params);
}

inline static int wifi_direct_wpa_cli_get_status(char ret[], size_t len) {
  char *const params[] = {"wpa_cli", "status", NULL};

  assert(wpa_cli_intf_name[0] != '\0');

  return run_cli(ret, len, params);
}

/* Do not use this API */
inline static int wifi_direct_ip_setup(void) {
  char *const params[] = {"ifconfig",
    wpa_cli_intf_name,
    "192.168.49.1",
    "up",
    NULL};
  char buf[256];

  assert(wpa_cli_intf_name[0] != '\0');

  return run_cli(buf, 256, params, 1);
}

static int wifi_set_device_name(char *dev_name, char *buf, int len) {
  char *const params[] = {"wpa_cli",
    "set",
    "device_name",
    dev_name,
    NULL};

  return run_cli(buf, len, params);
}
extern int wifi_direct_server_up(char *device_name) {
  char buf[1024];
  int ret;

  if (wpa_cli_intf_name[0] != '\0') {
    OPEL_DBG_LOG("WFD is already up-%s", wpa_cli_intf_name);
    return -1;
  }

  ret = wifi_set_device_name(device_name, buf, 1024);
  if (ret != 0)
    return ret;

  ret = wifi_direct_p2p_group_add(buf, 1024);
  if (ret != 0)
    return ret;

  if (wpa_cli_dev_name[0] == '\0') {
    OPEL_DBG_LOG("%s", buf);
    char *ptrptr;
    char *ptr = strtok_r(buf, "\t \n\'", &ptrptr);
    while (ptr != NULL) {
      if (strstr(ptr, "p2p-dev")) {
        snprintf(wpa_cli_dev_name, sizeof(wpa_cli_dev_name), "%s", ptr);
      } else if (strstr(ptr, "FAIL")) {
        OPEL_DBG_WARN("P2P Group add failed");
        return -1;
      } else if (strstr(ptr, "OK")) {
        OPEL_DBG_LOG("P2P Group added");
      }

      ptr = strtok_r(NULL, "\t \n\'", &ptrptr);
    }
  }

  assert(wpa_cli_dev_name[0] != '\0');

  ret = wifi_direct_ping(buf, 1024);
  if (ret != 0)
    return ret;

  char *ptrptr;
  char *ptr = strtok_r(buf, "\t \n\'", &ptrptr);
  while (ptr != NULL) {
    if (strstr(ptr, "p2p-wlan")) {
      snprintf(wpa_cli_intf_name, sizeof(wpa_cli_intf_name), "%s", ptr);
    } else if (strstr(ptr, "PONG")) {
      OPEL_DBG_LOG("Added interface:%s", wpa_cli_intf_name);
    } else if (strstr(ptr, "FAIL")) {
      OPEL_DBG_WARN("P2P Group add failed");
      wpa_cli_intf_name[0] = '\0';
      return -1;
    }
    ptr = strtok_r(NULL, "\t \n\'", &ptrptr);
  }

  return ret;
}

extern int wifi_direct_server_down(void) {
  char buf[1024];
  int ret;

  if (wpa_cli_intf_name[0] == '\0') {
    OPEL_DBG_LOG("WFD is not up");
    return -1;
  }

  ret = wifi_direct_p2p_group_remove(buf, 1024);

  char *ptrptr;
  char *ptr = strtok_r(buf, "\t \n\'", &ptrptr);
  while (ptr != NULL) {
    if (strstr(ptr, "OK")) {
      OPEL_DBG_LOG("Succedded to remove p2p group");
      wpa_cli_intf_name[0] = '\0';
      return ret;
    } else if (strstr(ptr, "FAIL")) {
      OPEL_DBG_LOG("Failed to remove p2p group");
      return -1;
    }
  }

  OPEL_DBG_ERR("Assertion: Must not reach here");
  return -1;
}

extern int wifi_direct_server_reset(char *pin, size_t len) {
  char buf[1024] = {0, };
  int ret;
  if (wpa_cli_intf_name[0] == '\0') {
    OPEL_DBG_LOG("WFD is not up");
    return -1;
  }

  ret = wifi_direct_wpa_cli_pin_reset(buf, 1024);
  if (ret != 0)
    return ret;

  // Selected interface 'p2p-wlan0-9' 33996608
  OPEL_DBG_LOG("Pin parsing %s", buf);
  char *ptrptr;
  char *ptr = strtok_r(buf, "\t \n\'", &ptrptr);  // Selected
  ptr = strtok_r(NULL, "\t \n\'", &ptrptr);  // interface
  ptr = strtok_r(NULL, "\t \n\'", &ptrptr);  // p2p-wlan0-9
  ptr = strtok_r(NULL, "\t \n\'", &ptrptr);  // 33996608

  snprintf(pin, len, "%s", ptr);
  OPEL_DBG_LOG("Pin:%s", ptr);

  return 0;
}

extern int wifi_get_p2p_device_addr(char *dev_addr, size_t len) {
  char buf[1024];
  int ret;
  if (wpa_cli_intf_name[0] == '\0') {
    OPEL_DBG_LOG("WFD is not up");
    return -1;
  }

  ret = wifi_direct_wpa_cli_get_status(buf, 1024);
  //OPEL_DBG_LOG("Status: %s", buf);
  if (ret != 0)
    return ret;

  char *ptrptr;
  char *ptr = strtok_r(buf, "\t \n\'", &ptrptr);
  while (ptr != NULL) {
    if (strstr(ptr, "p2p_device_address")) {
      // p2p_device_address=XX
      sscanf(ptr, "%*[^=]=%s", dev_addr);
      OPEL_DBG_LOG("p2p_device_address=%s", dev_addr);
      break;
    } else if (strstr(ptr, "FAIL")) {
      OPEL_DBG_WARN("Get p2p device address failed");
      return -1;
    }

    ptr = strtok_r(NULL, "\t \n\'", &ptrptr);
  }
  return 0;
}

extern int wifi_get_device_name(char *dev_name) {
  // TODO(esevan): Implementation.
  return 0;
}

extern int wifi_set_device_name(char *dev_name) {
  // TODO(esevan): Implementation.
  return 0;
}

extern int wifi_direct_ip_addr(char *buf, size_t len) {
  int ret = wifi_direct_wpa_cli_get_status(buf, 1024);
  if (ret != 0)
    return ret;
  //OPEL_DBG_LOG("Status: %s", buf);

  char *ptrptr;
  char *ptr = strtok_r(buf, "\t \n\'", &ptrptr);
  while (ptr != NULL) {
    if (strstr(ptr, "ip_address")) {
      sscanf(ptr, "%*[^=]=%s", buf);
      OPEL_DBG_LOG("IP_Device_Addrress = %s", buf);
      return 0;
    }

    ptr = strtok_r(NULL, "\t \n\'", &ptrptr);
  }
  OPEL_DBG_WARN("Get IP address failed");
  return -1;
}
}  /* namespace wifi */
