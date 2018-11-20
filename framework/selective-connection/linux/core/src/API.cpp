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

#include "../inc/APIInternal.h"
#include "../inc/NetworkMonitor.h"

#include <condition_variable>
#include <mutex>

using namespace sc;

std::mutex g_wait_lock_start_sc;
std::condition_variable g_wait_cond_start_sc;
bool g_is_start_sc_done;
bool g_start_sc_success;

std::mutex g_wait_lock_stop_sc;
std::condition_variable g_wait_cond_stop_sc;
bool g_is_stop_sc_done;
bool g_stop_sc_success;

// TODO: make it into synchronous call
void sc::start_sc(StartCallback startCallback) {
  // Core start procedure
  g_is_start_sc_done = false;
  Core::singleton()->start();
  NetworkMonitor::singleton()->start();

  // Wait until core start thread ends
  if(!g_is_start_sc_done) {
    std::unique_lock<std::mutex> lck(g_wait_lock_start_sc);
    g_wait_cond_start_sc.wait(lck);
  }

  // Execute callback
  if(startCallback != NULL) {
    startCallback(g_start_sc_success);
  }
}

void sc::start_sc_done(bool is_success) {
  g_is_start_sc_done = true;
  g_start_sc_success = is_success;
  g_wait_cond_start_sc.notify_all();
}

// TODO: make it into synchronous call
void sc::stop_sc(StopCallback stopCallback) {
  // Core stop procedure
  g_is_stop_sc_done = false;
  NetworkMonitor::singleton()->stop();
  Core::singleton()->stop();

  // Wait until core stop thread ends
  if(!g_is_stop_sc_done) {
    std::unique_lock<std::mutex> lck(g_wait_lock_stop_sc);
    g_wait_cond_stop_sc.wait(lck);
  }

  // Execute callback
  if(stopCallback != NULL) {
    stopCallback(g_stop_sc_success);
  }
}

void sc::stop_sc_done(bool is_success) {
  g_is_stop_sc_done = true;
  g_stop_sc_success = is_success;
  g_wait_cond_stop_sc.notify_all();
}

void sc::set_switcher_mode(NSMode switcher_mode) {
  NetworkMonitor::singleton()->set_mode(switcher_mode);
}