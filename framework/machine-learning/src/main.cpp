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
#include <string.h>

#include "MLDaemon.h"
#include "ANTdbugLog.h"

MLDaemon gMLDaemon;

int main(int argc, char* argv[]) {
  if(argc == 2) {
    // Run machine learning daemon in testing mode
    if(strcmp(argv[1], "--test-motion") == 0) {
      // Test motion classifier
      gMLDaemon.runTest("motionclassifier");
    } else if(strcmp(argv[1], "--test-image") == 0) {
      // Test image classifier
      gMLDaemon.runTest("imageclassifier");
    } else {
      ANT_DBG_ERR("Invalid arguments!" \
          "Expected: %s [--test-motion or --test-image]",
          argv[0]);
    }
  } else {
    // Start machine learning daemon
    gMLDaemon.run();
  }
}
