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

#include "../configs/WfdConfig.h"

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define UDHCPD_CONFIG_PATH "dhcpd.conf"

using namespace sc;

struct sigaction sSigaction;
struct sigaction sSigactionOld;
bool sDhcpdMonitoring = false;
int sDhcpdPid = 0;

void sighandler_monitor_udhcpd(int signo, siginfo_t *sinfo, void *context) {
  do {
    if (signo != SIGCHLD || sDhcpdPid == 0)
      break;

    if (signo == SIGCHLD && sinfo->si_pid == sDhcpdPid) {
      int status;
      while (waitpid(sDhcpdPid, &status, WNOHANG) > 0) {
      }
      sDhcpdPid = 0;
    }
  } while (0);
}

int set_dhcpd_config(const char *intfName) {
  if (!sDhcpdMonitoring) {
    sSigaction.sa_flags = SA_SIGINFO;
    sSigaction.sa_sigaction = sighandler_monitor_udhcpd;
    sigaction(SIGCHLD, &sSigaction, &sSigactionOld);

    sDhcpdMonitoring = true;
  }

  char const *const params[] = {"udhcpd", UDHCPD_CONFIG_PATH, "-f", NULL};

  // Generate dhcp configuration
  int config_fd = open(UDHCPD_CONFIG_PATH, O_CREAT | O_WRONLY | O_TRUNC, 0644);
  char script[512];

  sprintf(script,
          "start 192.168.49.20\n"
          "end 192.168.49.40\n"
          "interface %s\n"
          "max_leases 20\n"
          "option subnet 255.255.255.0\n"
          "option router 192.168.49.1\n"
          "option lease 864000\n"
          "option broadcast 192.168.49.255",
          intfName);

  write(config_fd, script, strlen(script) + 1);
  close(config_fd);

  ChildProcess::run(UDHCPD_PATH, params, false);

  return 0;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("arguments: <p2p-%s-*>\n", DEFAULT_WFD_DEVICE_NAME);
  }
  char intfName[100];
  snprintf(intfName, 100, "p2p-%s-%s", DEFAULT_WFD_DEVICE_NAME, argv[1]);
  set_dhcpd_config(intfName);

  while (1) {
  }
  return 0;
}