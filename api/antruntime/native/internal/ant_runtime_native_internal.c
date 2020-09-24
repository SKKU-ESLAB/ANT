#include "ant_runtime_native_internal.h"
#include "../../../common/native/ant_common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int ant_runtime_getPssInKB_internal() {
  FILE *cmd;
  char result[24] = {0x0};
  int pssInKB = -1;
  char commandLine[200];
  snprintf(commandLine, 200,
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