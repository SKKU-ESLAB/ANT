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

#include "../inc/API.h"

#include "../device/inc/BtServerAdapter.h"
#include "../device/inc/EthServerAdapter.h"
#include "../device/inc/WfdServerAdapter.h"

#include <thread>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

using namespace sc;

#define DEBUG_SHOW_DATA 0
#define DEBUG_SHOW_TIME 0

#if DEBUG_SHOW_TIME == 1
struct timeval start, end;
#endif

FILE *fp;
std::condition_variable end_lock;

void receiving_thread() {
  void *buf = NULL;
  printf("[INFO] Receiving thread created! tid: %d\n",
         (unsigned int)syscall(224));

  while (true) {
    int ret = sc::receive(&buf);
#if DEBUG_SHOW_DATA == 1
    printf("Recv %d> %s\n\n", ret, reinterpret_cast<char *>(buf));
#endif
#if DEBUG_SHOW_TIME == 1
    gettimeofday(&end, NULL);
    printf("%ld %ld \n", end.tv_sec - start.tv_sec,
           end.tv_usec - start.tv_usec);
#endif

    if (buf)
      free(buf);
    end_lock.notify_one();
  }
}

static char *rand_string(char *str, size_t size) {
  const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  if (size) {
    --size;
    for (size_t n = 0; n < size; n++) {
      int key = rand() % (int)(sizeof charset - 1);
      str[n] = charset[key];
    }
    str[size] = '\0';
  }
  return str;
}

void on_connect(bool is_success);

sc::BtServerAdapter *btControl;
sc::BtServerAdapter *btData;

int main(int argc, char **argv) {

  // EthServerAdapter ethAdapter(2345, "Eth", 2345);
  btControl = new sc::BtServerAdapter(1, "BtCt",
                                      "150e8400-1234-41d4-a716-446655440000");
  btData = new sc::BtServerAdapter(11, "BtDt",
                                   "150e8400-1234-41d4-a716-446655440001");

  printf("Step 1. Initializing Network Adapters\n");

  // printf("  a) Control Adapter: TCP over Ethernet\n");
  // sc::register_control_adapter(&ethAdapter);
  printf("  a) Control Adapter: RFCOMM over Bluetooth\n");
  sc::register_control_adapter(btControl);
  printf("  b) Data Adapter: RFCOMM over Bluetooth\n");
  sc::register_data_adapter(btData);

  sc::start_sc(on_connect);
}

void on_connect(bool is_success) {
  if (!is_success) {
    sc::start_sc(on_connect);
    return;
  }

  char *temp_buf;

  std::thread(receiving_thread).detach();

#define TEST_DATA_SIZE (5 * 1024)
  printf("Step 2. Send Test Data (%dB)\n", TEST_DATA_SIZE);
  int i;
  for (i = 0; i < 1; i++) {
    sleep(2);
    temp_buf = (char *)calloc(TEST_DATA_SIZE, sizeof(char));
    sc::send(temp_buf, TEST_DATA_SIZE);
    sleep(10);
    free(temp_buf);
  }

  printf("Wait for 10 seconds...\n");
  sleep(10);

  printf("Finish Workload\n");

  sc::stop_sc(NULL);

  return;
}
