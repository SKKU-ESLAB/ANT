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

#include "../common/inc/DebugLog.h"
#include "../core/inc/API.h"
#include "../device/inc/NetworkInitializer.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

TraceRunner *TraceRunner::sTraceRunner = NULL;

void TraceRunner::start() {
  assert(!this->mPacketTraceFilename.empty());
  assert(this->mDecisionPolicy < kDPNum);

  printf("** TraceRunner Start\n");
  printf(" ** Trace File: %s\n", this->mPacketTraceFilename.c_str());
  printf(" ** Decision Policy: %d\n", this->mDecisionPolicy);

  // Network Interface Initialization
  sc::NetworkInitializer netInit;
  netInit.initialize();

  // Bluetooth adapter setting
  this->mBtAdapter = sc::BtServerAdapter::singleton(
      1, "Bt", "150e8400-1234-41d4-a716-446655440000");
  this->mBtAdapter->listen_state(this);
  sc::register_adapter(this->mBtAdapter);

  // Wi-fi Direct adapter setting
  this->mWfdAdapter = sc::WfdServerAdapter::singleton(2, "Wfd", 3455, "SelCon");
  this->mWfdAdapter->listen_state(this);
  sc::register_adapter(this->mWfdAdapter);

  // Initial network monitor mode setting
  switch (this->mDecisionPolicy) {
  case 0:
    // Energy-aware Only
    sc::set_switcher_mode(sc::kNSModeEnergyAware);
    break;
  case 1:
    // Latency-aware Only
    sc::set_switcher_mode(sc::kNSModeLatencyAware);
    break;
  case 2:
    // Cap-dynamic Only
    sc::set_switcher_mode(sc::kNSModeCapDynamic);
    break;
  case 3:
    // App-aware Only
    sc::set_switcher_mode(sc::kNSModeEnergyAware);
    break;
  case 4:
    // BT-only
    sc::set_switcher_mode(sc::kNSModeBTOnly);
    break;
  case 5:
    // WFD-only
    sc::set_switcher_mode(sc::kNSModeWFDOnly);
    break;
  default:
    // Unknown Policy
    printf("[ERROR] Unknown Policy! %d\n", this->mDecisionPolicy);
    return;
  }

  // Start selective connection
  TraceRunner::sTraceRunner = this;
  sc::start_sc(TraceRunner::on_start_sc);
}

void TraceRunner::on_start_sc(bool is_success) {
  TraceRunner *self = TraceRunner::sTraceRunner;

  if (!is_success) {
    printf("[ERROR] Failed to initialize!");
    return;
  }

  // Launch app receiving thread
  self->mThread =
      new std::thread(std::bind(&TraceRunner::receiving_thread, self));
  self->mThread->detach();

  // 1. Send test data to warm-up the initial connection
  printf(" ** Send Test Data\n");
  ::sleep(5);
  self->send_test_data();
  printf(" ** 40 sec. remains\n");
  ::sleep(10);
  self->send_test_data();
  printf(" ** 30 sec. remains\n");
  ::sleep(10);
  self->send_test_data();
  printf(" ** 20 sec. remains\n");
  ::sleep(10);
  printf(" ** 10 sec. remains\n");
  ::sleep(5);
  printf(" ** 5 sec. remains\n");
  ::sleep(5);

  printf(" ** Start Workload\n");

  /// Parse trace file and send data
  self->send_workload();

  printf(" ** Finish Workload...\n");
  ::sleep(600);

  // Stop selective connection
  sc::stop_sc(NULL);

  return;
}

void TraceRunner::send_test_data() {
  char *temp_buf = (char *)calloc(TEST_DATA_SIZE, sizeof(char));
  sc::send(temp_buf, TEST_DATA_SIZE);
  free(temp_buf);
}

#define SOURCE_LOCALHOST_BT "localhost ("
#define SOURCE_LOCALHOST_WFD "192.168.0.33"
void TraceRunner::send_workload(void) {
  // Initialize parser
  io::CSVReader<4, io::trim_chars<>, io::double_quote_escape<',', '\"'>>
      packetTraceReader(this->mPacketTraceFilename.c_str());
  packetTraceReader.read_header(io::ignore_extra_column, "Time", "Source",
                                "PayloadBT", "Payload");

  AppTraceReader *appTraceReader = NULL;
  if (this->mDecisionPolicy == kDPAppAware) {
    appTraceReader = new AppTraceReader(this->mAppTraceFilename);
    appTraceReader->read_header();
  }

  int iter = 0;

  // Parse packet trace file
  std::string ts_str, source_str, payload_length_rfcomm_str,
      payload_length_tcp_str;
  this->mNextAppTSReady = false;
  while (packetTraceReader.read_row(
      ts_str, source_str, payload_length_rfcomm_str, payload_length_tcp_str)) {
#if DEBUG_SHOW_DATA == 1
    printf("%s %s %d\n", ts_str.c_str(), source_str.c_str(), data_size);
#endif

    // Parsing
    int ts_sec, ts_usec, data_size;
    {
      // Filter invalid packets
      if (source_str.find(SOURCE_LOCALHOST_BT) == std::string::npos &&
          source_str.find(SOURCE_LOCALHOST_WFD) == std::string::npos) {
        // Only replay packets sent from this device
        continue;
      }

      // Get payload length
      if (payload_length_rfcomm_str.length() != 0) {
        data_size = std::stoi(payload_length_rfcomm_str);
      } else if (payload_length_tcp_str.length() != 0) {
        data_size = std::stoi(payload_length_tcp_str);
      } else {
        continue;
      }

      // Filter invalid packets
      if (data_size == 0) {
        continue;
      }

      // Get timestamp
      sscanf(ts_str.c_str(), "%d.%d", &(ts_sec), &(ts_usec));
    }

    if (iter % 1000 == 0) {
      printf(" ** Send Workload Packet (No. %d) - %d.%d\n", iter, ts_sec,
             ts_usec);
    }

    // Sleep for wait
    int res = this->sleep_workload(ts_sec, ts_usec);
    if (res < 0) {
      printf("[ERROR] sleep_workload error %d\n", res);
      return;
    }

    // In case of app-aware workload,
    // check if the policy should be applied by NetworkMonitor
    if (this->mDecisionPolicy == kDPAppAware) {
      res = this->check_app_policy(appTraceReader, ts_sec, ts_usec);
      if (res < 0) {
        printf("[ERROR] check_app_policy error %d\n", res);
        return;
      }
    }

#if DEBUG_SHOW_TIME == 1
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
#endif

    // Allocate data buffer
    char *data_buffer = (char *)calloc(data_size, sizeof(char));
    if (data_buffer == NULL) {
      fprintf(stderr, "[Error] Data buffer allocation failed: size=%d\n",
              data_size);
      return;
    }

    // Generate string to data buffer
    TraceRunner::generate_simple_string(data_buffer, data_size);

    // Send data
    sc::send(data_buffer, data_size);

#if DEBUG_SHOW_TIME == 1
    gettimeofday(&end_time, NULL);
    printf("%ld %ld \n", end_time.tv_sec - start_time.tv_sec,
           end_time.tv_usec - start_time.tv_usec);
#endif

#if DEBUG_SHOW_DATA == 1
    printf("   - Packet %d : Sent!\n", iter);
#endif

    free(data_buffer);
    iter++;
  }
}

int TraceRunner::sleep_workload(int next_ts_sec, int next_ts_usec) {
  int sleep_us = (int)(next_ts_sec * 1000 * 1000 + next_ts_usec) -
                 (int)(this->mRecentTSSec * 1000 * 1000 + this->mRecentTSUsec);
  if (sleep_us < 0) {
    printf("[Error] Invalid sleep time: %d(%d:%d -> %d:%d)\n", sleep_us,
           this->mRecentTSSec, this->mRecentTSUsec, next_ts_sec, next_ts_usec);
    return -1;
  }

  // Sleep
  ::usleep(sleep_us);

  // Update recent timestamp
  this->mRecentTSSec = next_ts_sec;
  this->mRecentTSUsec = next_ts_usec;

  return 0;
}

int TraceRunner::check_app_policy(AppTraceReader *app_trace_reader,
                                  int next_packet_ts_sec,
                                  int next_packet_ts_usec) {
  if (app_trace_reader == NULL) {
    return -1;
  }

  // Next app timestamp
  int next_packet_ts_us =
      next_packet_ts_sec * 1000 * 1000 + next_packet_ts_usec;

  // Next packet timestamp
  int next_app_ts_us = this->mNextAppTSSec * 1000 * 1000 + this->mNextAppTSUsec;

  // Read a column from app trace file
  if (!this->mNextAppTSReady) {
    bool ret = app_trace_reader->read_column(
        this->mNextAppTSSec, this->mNextAppTSUsec, this->mNextAppPolicyNum);
    if (ret) {
      next_app_ts_us = this->mNextAppTSSec * 1000 * 1000 + this->mNextAppTSUsec;
      this->mNextAppTSReady = true;
    }
  }

  // If the time is for app, apply the policy
  if (this->mNextAppTSReady && next_app_ts_us <= next_packet_ts_us &&
      next_app_ts_us != 0) {
    switch (this->mNextAppPolicyNum) {
    case 0:
      sc::set_switcher_mode(sc::kNSModeEnergyAware);
      this->mNextAppTSReady = false;
      break;
    case 1:
      sc::set_switcher_mode(sc::kNSModeLatencyAware);
      this->mNextAppTSReady = false;
      break;
    default:
      return -2;
    }
    return 1;
  } else {
    return 0;
  }
}

void TraceRunner::receiving_thread(void) {
  LOG_THREAD_LAUNCH("App Receiving");

  void *buf = NULL;
  while (true) {
    int ret = sc::receive(&buf);
#if DEBUG_SHOW_DATA == 1
    printf("Recv %d> %s\n\n", ret, reinterpret_cast<char *>(buf));
#endif

    if (buf)
      free(buf);
  }

  LOG_THREAD_FINISH("App Receiving");
}

void TraceRunner::onUpdateServerAdapterState(sc::ServerAdapter *adapter,
                                             sc::ServerAdapterState old_state,
                                             sc::ServerAdapterState new_state) {
  sc::ServerAdapterState bt_state = this->mBtAdapter->get_state();
  sc::ServerAdapterState wfd_state = this->mWfdAdapter->get_state();
  std::string bt_state_str(
      sc::ServerAdapter::server_adapter_state_to_string(bt_state));
  std::string wfd_state_str(
      sc::ServerAdapter::server_adapter_state_to_string(wfd_state));

  LOG_VERB("[STATE] BT: %s / WFD: %s", bt_state_str.c_str(),
           wfd_state_str.c_str());
}

char *TraceRunner::generate_random_string(char *str, size_t size) {
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

char *TraceRunner::generate_simple_string(char *str, size_t size) {
  memset(str, 'a', size - 1);
  memset(str + size - 1, '\0', 1);
  return str;
}

TraceRunner *
TraceRunner::energy_aware_only_runner(std::string packet_trace_filename) {
  return TraceRunner::simple_runner(packet_trace_filename, kDPEnergyAwareOnly);
}
TraceRunner *
TraceRunner::latency_aware_only_runner(std::string packet_trace_filename) {
  return TraceRunner::simple_runner(packet_trace_filename, kDPLatencyAwareOnly);
}
TraceRunner *
TraceRunner::cap_dynamic_only_runner(std::string packet_trace_filename) {
  return TraceRunner::simple_runner(packet_trace_filename, kDPCapDynamicOnly);
}
TraceRunner *TraceRunner::app_aware_runner(std::string packet_trace_filename,
                                           std::string app_trace_filename) {
  return new TraceRunner(packet_trace_filename, kDPAppAware,
                         app_trace_filename);
}
TraceRunner *TraceRunner::bt_only_runner(std::string packet_trace_filename) {
  return TraceRunner::simple_runner(packet_trace_filename, kDPBTOnly);
}
TraceRunner *TraceRunner::wfd_only_runner(std::string packet_trace_filename) {
  return TraceRunner::simple_runner(packet_trace_filename, kDPWFDOnly);
}

TraceRunner *TraceRunner::simple_runner(std::string packet_trace_filename,
                                        DecisionPolicy switcher_mode) {
  return new TraceRunner(packet_trace_filename, switcher_mode, "");
}