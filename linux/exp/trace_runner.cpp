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

#include "../core/inc/API.h"

#include "../device/inc/BtServerAdapter.h"
#include "../device/inc/CommInitializer.h"
#include "../device/inc/WfdServerAdapter.h"

#include "csv.h"

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

char g_trace_file_name[512];

sc::BtServerAdapter *bt;
sc::WfdServerAdapter *wfd;

int main(int argc, char **argv) {
  /* Parse arguments */
  if (argc != 2) {
    printf("[Usage] %s <trace file name>\n", argv[0]);
    return -1;
  }

  // Communication Interface Initialization
  CommInitializer commInit;
  commInit.initialize();

  snprintf(g_trace_file_name, 512, "%s", argv[1]);
  printf("Trace File: %s\n", g_trace_file_name);

  bt = sc::BtServerAdapter::singleton(
      1, "Bt", "150e8400-1234-41d4-a716-446655440000");
  wfd = sc::WfdServerAdapter::singleton(2, "Wfd", 3455, "SelCon");

  printf("Step 1. Initializing Network Adapters\n");

  printf("  Adapter 1: RFCOMM over Bluetooth\n");
  sc::register_adapter(bt);
  printf("  Adapter 2: TCP over Wi-fi Direct\n");
  sc::register_adapter(wfd);

  sc::start_sc(on_connect);
  return 0;
}

void on_connect(bool is_success) {
  int iter = 0;
  char sending_buf[8192];
  int ret, numbytes;
  char *buffer;
  char input[100];
  char file_name[200];
  char file_dir[200];
  char *temp_buf;

  std::thread(receiving_thread).detach();

#define TEST_DATA_SIZE (5 * 1024)
  printf("Step 2. Send Test Data (%dB)\n", TEST_DATA_SIZE);
  int i;
  printf("Wait for 2 seconds...\n");
  sleep(2);

  temp_buf = (char *)calloc(TEST_DATA_SIZE, sizeof(char));
  sc::send(temp_buf, TEST_DATA_SIZE);
  free(temp_buf);

  printf("Wait for 2 seconds...\n");
  sleep(2);

#define BUFFER_SIZE (20 * 1024 * 1024)
#define SOURCE_LOCALHOST_BT "localhost (ANT-0)"
#define SOURCE_LOCALHOST_WFD "192.168.0.33"
  printf("Step 3. Send Workload (%s)\n", g_trace_file_name);

  /* Initialize CSV Parser */
  io::CSVReader<3, io::trim_chars<>, io::double_quote_escape<',', '\"'>> in(
      g_trace_file_name);
  in.read_header(io::ignore_extra_column, "Time", "Source", "Payload");
  std::string timestr;
  std::string source;
  int payload_length;

  /* Read CSV File */
  int recent_sent_sec = 0;
  int recent_sent_usec = 0;
  while (in.read_row(timestr, source, payload_length)) {
#if DEBUG_SHOW_DATA == 1
    printf("%s %s %d\n", timestr.c_str(), source.c_str(), payload_length);
#endif
    if (payload_length == 0) {
      /* In case of ACK message */
      continue;
    }

    if (source.find(SOURCE_LOCALHOST_BT) == std::string::npos &&
        source.find(SOURCE_LOCALHOST_WFD) == std::string::npos) {
      /* Only replay packets sent from this device */
      continue;
    }

    /* Parse Time */
    int time_sec;
    int time_usec;
    sscanf(timestr.c_str(), "%d.%d", &(time_sec), &(time_usec));
    time_usec = time_usec / 1000;

    /* Sleep for wait */
    int sleep_us = (int)(time_sec * 1000 * 1000 + time_usec) -
                   (int)(recent_sent_sec * 1000 * 1000 + recent_sent_usec);
    if (sleep_us < 0) {
      printf("[Error] Invalid sleep time: %d(%d:%d -> %d:%d)\n", sleep_us,
             recent_sent_sec, recent_sent_usec, time_sec, time_usec);
      return;
    }
    /* printf(" * Packet %d (Length: %d / Time: %d:%d) Wait for %d us...\n",
        iter, payload_length, time_sec, time_usec, sleep_us); */
    if (iter % 1000 == 0) {
      printf(" * Packet %d\n", iter);
    }
    usleep(sleep_us);

    buffer = (char *)calloc(payload_length, sizeof(char));
    if (buffer == NULL) {
      fprintf(stderr, "[Error] Buffer allocation failed: size=%d\n",
              payload_length);
      return;
    } else {
#if DEBUG_SHOW_TIME == 1
      gettimeofday(&start, NULL);
#endif
      /* Generate String */
      rand_string(buffer, payload_length);

      /* Send Data */
      ret = sc::send(buffer, payload_length);

      free(buffer);

#if DEBUG_SHOW_DATA == 1
      printf("   - Packet %d : Sent!\n", iter);
#endif
    }

    recent_sent_sec = time_sec;
    recent_sent_usec = time_usec;
    iter++;
  }

  printf("Finish Workload. Sleep 600 secs...\n");

  sleep(600);

  sc::stop_sc(NULL);

  return;
}
