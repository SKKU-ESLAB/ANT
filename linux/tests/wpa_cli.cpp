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

using namespace sc;

int main() {
  char ret[512];
  size_t len = 512;

  {
    char const *const params[] = {"wpa_cli", "p2p_group_add", NULL};
    ChildProcess::run(WPA_CLI_PATH, params, ret, len, true);
    printf("wpa_cli p2p_group_add: %s\n", ret);
  }
  {
    char const *const params[] = {"wpa_cli", "status", NULL};
    ChildProcess::run(WPA_CLI_PATH, params, ret, len, true);
    printf("wpa_cli status: %s\n", ret);
  }
  {
    char const *const params[] = {"wpa_cli", "ping", NULL};
    ChildProcess::run(WPA_CLI_PATH, params, ret, len, true);
    printf("wpa_cli ping: %s\n", ret);
  }

  return 0;
}