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

#include <API.h>
#include <APIInternal.h>

#include <mutex>
#include <condition_variable>

using namespace sc;

std::mutex g_wait_lock_start_sc;
std::condition_variable g_wait_cond_start_sc;
bool g_start_sc_success;

std::mutex g_wait_lock_stop_sc;
std::condition_variable g_wait_cond_stop_sc;
bool g_stop_sc_success;

void sc::start_sc(StartCallback startCallback) {
  Core::get_instance()->start();
  NetworkSwitcher::get_instance()->start();

  // Wait until connect thread ends
  std::unique_lock<std::mutex> lck(g_wait_lock_start_sc);
  g_wait_cond_start_sc.wait(lck);

  // Execute callback
  startCallback(g_start_sc_success);
}

void sc::start_sc_done(bool is_success) {
  g_start_sc_success = is_success;
  g_wait_cond_start_sc.notify_all();
}

void sc::stop_sc(StopCallback stopCallback) {
  NetworkSwitcher::get_instance()->stop();
  Core::get_instance()->stop();
  
  // Wait until connect thread ends
  std::unique_lock<std::mutex> lck(g_wait_lock_stop_sc);
  g_wait_cond_stop_sc.wait(lck);

  // Execute callback
  stopCallback(g_stop_sc_success);
}

void sc::stop_sc_done(bool is_success) {
  g_stop_sc_success = is_success;
  g_wait_cond_stop_sc.notify_all();
}