/* Copyright (c) 2017-2020 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "../../../common/native/ant_common.h"
#include "./ant_runtime_native_internal.h"

int ant_runtime_getPssInKB_internal() {
  FILE *cmd;
  char result[24] = {0x0};
  int pssInKB = -1;
  char commandLine[200];
  snprintf(commandLine, sizeof(commandLine),
           "cat /proc/%d/smaps | grep -i pss |  awk '{Total+=$2} END "
           "{print Total}'",
           (int)getpid());
  cmd = popen(commandLine, "r");
  while (fgets(result, sizeof(result), cmd) != NULL) {
    sscanf(result, "%d", &pssInKB);
  }
  pclose(cmd);

  return pssInKB;
}

void initANTRuntime(void) {
  // Empty function
}
