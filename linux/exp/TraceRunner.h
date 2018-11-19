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

#ifndef __TRACE_RUNNER_H__
#define __TRACE_RUNNER_H__

#include "csv.h"

#include "../common/inc/DebugLog.h"
#include "../core/inc/ServerAdapterStateListener.h"
#include "../device/inc/BtServerAdapter.h"
#include "../device/inc/WfdServerAdapter.h"

#include <string>
#include <thread>

#define DEBUG_SHOW_DATA 0
#define DEBUG_SHOW_TIME 0

#define TEST_DATA_SIZE (128)

typedef enum {
  kDPEnergyAwareOnly = 0,
  kDPLatencyAwareOnly = 1,
  kDPCapDynamicOnly = 2,
  kDPAppAware = 3,
  kDPBTOnly = 4,
  kDPWFDOnly = 5,
  kDPNum = 6
} DecisionPolicy;

class AppTraceReader {
public:
  void read_header() {
    this->mCSVReader->read_header(io::ignore_extra_column, "Time", "PolicyNum");
  }

  bool read_column(int &ts_sec, int &ts_usec, int &policy_num) {
    std::string ts_str, policy_num_str;
    bool ret = this->mCSVReader->read_row(ts_str, policy_num_str);
    if(ret) {
      LOG_VERB("ROW:%s / %s", ts_str.c_str(), policy_num_str.c_str());
      sscanf(ts_str.c_str(), "%d.%d", &(ts_sec), &(ts_usec));
      policy_num = std::stoi(policy_num_str);
    }

    return ret;
  }

  AppTraceReader(std::string app_trace_filename) {
    this->mCSVReader = new io::CSVReader<2, io::trim_chars<>,
                                         io::double_quote_escape<',', '\"'>>(
        app_trace_filename.c_str());
  }

private:
  io::CSVReader<2, io::trim_chars<>, io::double_quote_escape<',', '\"'>>
      *mCSVReader;
}; /* class AppTraceReader */

class TraceRunner : sc::ServerAdapterStateListener {
public:
  void start();
  static void on_start_sc(bool is_success);

private:
  static TraceRunner *sTraceRunner;

private:
  void send_test_data(void);

  void send_workload(void);
  int sleep_workload(int next_ts_sec, int next_ts_usec);
  int check_app_policy(AppTraceReader *app_trace_reader, int next_ts_sec,
                       int next_ts_usec);

  void receiving_thread(void);

private:
  virtual void onUpdateServerAdapterState(sc::ServerAdapter *adapter,
                                          sc::ServerAdapterState old_state,
                                          sc::ServerAdapterState new_state);

private:
  static char *generate_random_string(char *str, size_t size);
  static char *generate_simple_string(char *str, size_t size);

public:
  static TraceRunner *
  energy_aware_only_runner(std::string packet_trace_filename);
  static TraceRunner *
  latency_aware_only_runner(std::string packet_trace_filename);
  static TraceRunner *
  cap_dynamic_only_runner(std::string packet_trace_filename);
  static TraceRunner *app_aware_runner(std::string packet_trace_filename,
                                       std::string app_trace_filename);
  static TraceRunner *bt_only_runner(std::string packet_trace_filename);
  static TraceRunner *wfd_only_runner(std::string packet_trace_filename);

private:
  static TraceRunner *simple_runner(std::string packet_trace_filename,
                                    DecisionPolicy switcher_mode);

private:
  TraceRunner(std::string packet_trace_filename, DecisionPolicy decision_policy,
              std::string app_trace_filename) {
    this->mPacketTraceFilename.assign(packet_trace_filename);
    this->mDecisionPolicy = decision_policy;
    this->mAppTraceFilename.assign(app_trace_filename);
  }

private:
  /* Attributes */
  std::string mPacketTraceFilename;
  std::string mAppTraceFilename;
  DecisionPolicy mDecisionPolicy;

  int mRecentTSSec = 0;
  int mRecentTSUsec = 0;

  int mNextAppTSSec = 0;
  int mNextAppTSUsec = 0;
  int mNextAppPolicyNum = 0;
  bool mNextAppTSReady = false;

  /* Components */
  std::thread *mThread;
  sc::BtServerAdapter *mBtAdapter;
  sc::WfdServerAdapter *mWfdAdapter;
}; /* class TraceRunner */

#endif /* !defined(__TRACE_RUNNER_H__) */