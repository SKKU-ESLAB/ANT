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
#include "../core/inc/ServerAdapterStateListener.h"

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

FILE *g_file_pointer;

char g_trace_file_name[512];

sc::BtServerAdapter *g_bt_adapter;
sc::WfdServerAdapter *g_wfd_adapter;

class MyAdapterStateListener : ServerAdapterStateListener {
public:
  virtual void onUpdateServerAdapterState(ServerAdapter *adapter,
                                          ServerAdapterState old_state,
                                          ServerAdapterState new_state) {
    ServerAdapterState bt_state = g_bt_adapter->get_state();
    ServerAdapterState wfd_state = g_wfd_adapter->get_state();
    std::string bt_state_str(
        ServerAdapter::server_adapter_state_to_string(bt_state));
    std::string wfd_state_str(
        ServerAdapter::server_adapter_state_to_string(wfd_state));

    LOG_VERB("[STATE] BT: %s / WFD: %s", bt_state_str.c_str(),
             wfd_state_str.c_str());
  }
}; /* class ServerAdapterStateListener */

void receiving_thread() {
  void *buf = NULL;
  LOG_THREAD_LAUNCH("App Receiving");

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
  }

  LOG_THREAD_FINISH("App Receiving");
}

static char *rand_string(char *str, size_t size) {
  const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  if (size) {
    --size;
    for (size_t n = 0; n < size; n++) {
      // int key = rand() % (int)(sizeof charset - 1);
      // str[n] = charset[key];
      str[n] = (char)255;
    }
    str[size] = '\0';
  }
  return str;
}

void on_connect(bool is_success);

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
  printf(" ** Trace File: %s\n", g_trace_file_name);

  g_bt_adapter = sc::BtServerAdapter::singleton(
      1, "Bt", "150e8400-1234-41d4-a716-446655440000");
  g_wfd_adapter = sc::WfdServerAdapter::singleton(2, "Wfd", 3455, "SelCon");

  sc::register_adapter(g_bt_adapter);
  sc::register_adapter(g_wfd_adapter);

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
  printf(" ** Send Test Data (%dB)\n", TEST_DATA_SIZE);
  int i;
  printf(" ** Wait for 5 seconds...\n");
  sleep(5);

  temp_buf = (char *)calloc(TEST_DATA_SIZE, sizeof(char));
  sc::send(temp_buf, TEST_DATA_SIZE);
  free(temp_buf);

  printf(" ** Wait for 2 seconds...\n");
  sleep(2);

#define BUFFER_SIZE (20 * 1024 * 1024)
#define SOURCE_LOCALHOST_BT "localhost ("
#define SOURCE_LOCALHOST_WFD "192.168.0.33"
  printf("Step 3. Send Workload (%s)\n", g_trace_file_name);

  /* Initialize CSV Parser */
  io::CSVReader<4, io::trim_chars<>, io::double_quote_escape<',', '\"'>> in(
      g_trace_file_name);
  in.read_header(io::ignore_extra_column, "Time", "Source", "PayloadBT",
                 "Payload");
  std::string timestr;
  std::string source;
  std::string payload_bt;
  std::string payload_tcp;

  /* Read CSV File */
  int recent_sent_sec = 0;
  int recent_sent_usec = 0;
  while (in.read_row(timestr, source, payload_bt, payload_tcp)) {
    int payload_length;
    if (payload_bt.length() != 0) {
      payload_length = std::stoi(payload_bt);
    } else if (payload_tcp.length() != 0) {
      payload_length = std::stoi(payload_tcp);
    } else {
      continue;
    }

#if DEBUG_SHOW_DATA == 1
    printf("%s %s %d\n", timestr.c_str(), source.c_str(), payload_length);
#endif

    if (source.find(SOURCE_LOCALHOST_BT) == std::string::npos &&
        source.find(SOURCE_LOCALHOST_WFD) == std::string::npos) {
      /* Only replay packets sent from this device */
      continue;
    }

    /* Parse Time */
    int time_sec;
    int time_usec;
    sscanf(timestr.c_str(), "%d.%d", &(time_sec), &(time_usec));

    /* Sleep for wait */
    int sleep_us = (int)(time_sec * 1000 * 1000 + time_usec) -
                   (int)(recent_sent_sec * 1000 * 1000 + recent_sent_usec);
    if (sleep_us < 0) {
      printf("[Error] Invalid sleep time: %d(%d:%d -> %d:%d)\n", sleep_us,
             recent_sent_sec, recent_sent_usec, time_sec, time_usec);
      return;
    }
    if (iter % 1000 == 0) {
      printf(" ** Send Message (No. %d)\n", iter);
    }
    usleep(sleep_us);

    if(payload_length == 0) {
      recent_sent_sec = time_sec;
      recent_sent_usec = time_usec;
      continue;
    }

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

  printf(" ** Finish Workload...\n");

  sleep(600);

  sc::stop_sc(NULL);

  return;
}
