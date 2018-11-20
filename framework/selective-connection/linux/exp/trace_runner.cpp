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

#include "TraceRunner.h"

#include <string>
#include <thread>

int main(int argc, char **argv) {
  /* Parse arguments */
  if (argc != 3 && argc != 4) {
    printf("[Usage] %s <packet trace filename> <policy number> <app trace "
           "filename>\n",
           argv[0]);
    printf("  - Energy-aware Only: 0\n");
    printf("  - Latency-aware Only: 1\n");
    printf("  - Cap-dynamic Only: 2\n");
    printf("  - App-aware: 3\n");
    printf("  - BT-only: 4\n");
    printf("  - WFD-only: 5\n");
    return -1;
  }

  std::string packet_trace_filename(argv[1]);
  int policy_number = atoi(argv[2]);

  TraceRunner *trace_runner;
  switch (policy_number) {
  case 0: {
    trace_runner = TraceRunner::energy_aware_only_runner(packet_trace_filename);
    break;
  }
  case 1: {
    trace_runner =
        TraceRunner::latency_aware_only_runner(packet_trace_filename);
    break;
  }
  case 2: {
    trace_runner = TraceRunner::cap_dynamic_only_runner(packet_trace_filename);
    break;
  }
  case 3: {
    std::string app_trace_filename(argv[3]);
    trace_runner = TraceRunner::app_aware_runner(packet_trace_filename,
                                                 app_trace_filename);
    break;
  }
  case 4: {
    trace_runner = TraceRunner::bt_only_runner(packet_trace_filename);
    break;
  }
  case 5: {
    trace_runner = TraceRunner::wfd_only_runner(packet_trace_filename);
    break;
  }
  default: { return -1; }
  }
  trace_runner->start();
  return 0;
}