/* Copyright 2017 Eunsoo Park (esevan.park@gmail.com). All rights reserved
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

#include <bt_control.h>

namespace bt {
#define HCICONFIG_PATH "/usr/sbin/hciconfig"
static int run_cli(char *res_buf, size_t len, char *const params[]) {
  int fd[2];
  int pid;
  int bk;
  static int initialized;

  if (pipe(fd) < 0) {
    OPEL_DBG_ERR("pipe open error");
    return errno;
  }
  dup2(1, bk);

  if ((pid = fork()) < 0) {
    OPEL_DBG_ERR("fork error");
    return errno;
  } else if (pid > 0) {
    //OPEL_DBG_LOG("Forked PID : %d", pid);
    /* Parent process */
    close(fd[1]);

    char buf[1024];
    int read_bytes = read(fd[0], buf, 1024);

    if (read_bytes < 0) {
      OPEL_DBG_ERR("%d/%d read error", pid, udhcpd_pid);
      return errno;
    }

    memcpy(res_buf, buf, read_bytes < len ? read_bytes:len);
    dup2(bk, 1);

    close(fd[0]);

    return 0;
  } else {
    /* Child process */
    close(fd[0]);
    dup2(fd[1], 1);

    execv(HCICONFIG_PATH, params);
  }
}
void bt_init() {
}
}
