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
#include "../inc/ChildProcess.h"

#include "../../common/inc/DebugLog.h"

#include "../../configs/ExpConfig.h"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifdef VERBOSE_CHILD_PROCESS_RUN
#include <string>
#endif

using namespace sc;

int ChildProcess::run(const char *path, char const *const params[],
                      char *res_buf, size_t len, bool is_wait_child) {
  int internal_pipe[2];
  int pid, bk, res;

#ifdef VERBOSE_CHILD_PROCESS_RUN
  std::string command_string("");
  int i = 0;
  while (params[i] != NULL) {
    command_string.append(params[i]);
    if (params[i + 1] != NULL) {
      command_string.append(" ");
    }
    i++;
  }
  LOG_DEBUG("RUN: %s", command_string.c_str());
#endif

  res = pipe(internal_pipe);
  if (res < 0) {
    LOG_ERR("pipe open error");
    return errno;
  }

  dup2(1, bk);

  pid = fork();
  if (pid < 0) {
    LOG_ERR("Fork error: %d %s", errno, strerror(errno));
    return errno;
  } else if (pid > 0) { // LOG_VERB("Forked PID : %d", pid);
    /* Parent process */
    close(internal_pipe[1]);

    char buf[1024];
    const int kMaxTries = 5;
    int read_bytes;
    for (int tries = 0; tries < kMaxTries; tries++) {
      read_bytes = read(internal_pipe[0], buf, 1024);

      if (read_bytes < 0) {
        LOG_DEBUG("%s(pid %d): read error(%s) / retry %d", path, pid,
                  strerror(errno), tries);
      } else {
        break;
      }
    }
    if (read_bytes < 0) {
      return errno;
    }

    if (is_wait_child) {
      int status;
      waitpid(pid, &status, 0);
#ifdef VERBOSE_CHILD_PROCESS_RUN
      LOG_DEBUG("DONE: %s - %d", command_string.c_str(), status);
#endif
    } else {
#ifdef VERBOSE_CHILD_PROCESS_RUN
      LOG_DEBUG("DAEMON: %s", command_string.c_str());
#endif
    }

    memcpy(res_buf, buf, read_bytes < len ? read_bytes : len);
    dup2(bk, 1);

    close(internal_pipe[0]);

    return pid;
  } else {
    /* Child process */
    close(internal_pipe[0]);
    dup2(internal_pipe[1], 1);

    execv(path, (char **)params);
    return 0;
  }
}

int ChildProcess::run(const char *path, char const *const params[],
                      bool is_wait_child) {
  int pid;

#ifdef VERBOSE_CHILD_PROCESS_RUN
  std::string command_string("");
  int i = 0;
  while (params[i] != NULL) {
    command_string.append(params[i]);
    command_string.append(" ");
    i++;
  }
  LOG_VERB("Run command: %s", command_string.c_str());
#endif

  if ((pid = fork()) < 0) {
    LOG_ERR("Fork error: %d %s", errno, strerror(errno));
    return errno;
  } else if (pid > 0) { // LOG_VERB("Forked PID : %d", pid);
    /* Parent process */
    if (is_wait_child) {
      int status;
      waitpid(pid, &status, 0);
#ifdef VERBOSE_CHILD_PROCESS_RUN
      LOG_VERB("DONE: %s - %d", command_string.c_str(), status);
#endif
    } else {
#ifdef VERBOSE_CHILD_PROCESS_RUN
      LOG_VERB("DAEMON: %s", command_string.c_str());
#endif
    }
    return pid;
  } else {
    execv(path, (char **)params);
    return 0;
  }
}