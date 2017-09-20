/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
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
#include <string>
#include <iostream>
#include <unistd.h>

#include "AppCore.h"

AppCore gAppCore;
struct sigaction gActAppcore;
struct sigaction gActOld;

// Signal handler (SIGCHLD)
void sigchld_handler(int signo) {
  do {
    if(signo != SIGCHLD) break;

    gAppCore.onSignalSIGCHLD();
  } while(0);
}

// Parse arguments and set options
bool set_options(int argc, char** argv) {
  int opt;
  std::string input = "";
  bool flagA = false;
  bool flagB = false;

  // Retrieve the (non-option) argument:
  if ((argc <= 1) || (argv[argc-1] == NULL) || (argv[argc-1][0] == '-')) {
    // No arguments
  } else {
    input = argv[argc-1];
  }

  opterr = 0;

  // Retrieve the options:
  while ((opt = getopt(argc, argv, "d")) != -1 ) {
    switch (opt) {
      case 'd':
        printf("debugging apps: ON\n");
        gAppCore.setDebugApp(true);
        break;
      case '?':
      default:
        printf("Unknown option: '%c'!\n", char(optopt));
        return false;
        break;
    }
  }
  return true;
}

int main(int argc, char* argv[]) {
  // Spawn sigchld handler
  gActAppcore.sa_handler = sigchld_handler;
  gActAppcore.sa_flags = SA_SIGINFO;
  sigemptyset(&gActAppcore.sa_mask);
  sigaction(SIGCHLD, &gActAppcore, &gActOld);

  // Parse arguments and set options
  bool isGetOptionsSucceed = set_options(argc, argv);
  if(!isGetOptionsSucceed) {
    printf("Invalid options!\n");
    return false;
  }

  // Start appcore
  gAppCore.run();
}
