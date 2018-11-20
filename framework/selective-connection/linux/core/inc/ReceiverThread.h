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

#ifndef __RECEIVER_THREAD_H__
#define __RECEIVER_THREAD_H__

#include "ServerAdapter.h"

#include <condition_variable>
#include <mutex>
#include <thread>

namespace sc {
class ServerAdapter;

class ReceiverThread {
public:
  void launch_thread(void) {
    if (this->mThread == NULL) {
      this->mThread = new std::thread(std::bind(&ReceiverThread::run, this));
      this->mThread->detach();
    }
  }

  void finish_thread(void) { this->set_is_thread_running(false); }

  void enable_loop(void) { this->set_is_loop_enabled(true); }

  void disable_loop(void) { this->set_is_loop_enabled(false); }

  void wait_until_disable_loop_done(void) { this->wait_until_loop_ends(); }

private:
  /* Main routine and thread loop */
  void run(void);

  /* Receiver loop */
  void receiver_loop(void);

private:
  /* Thread running */
  void set_is_thread_running(bool _is_thread_running) {
    std::unique_lock<std::mutex> lck(this->mIsThreadRunningMutex);
    this->mIsThreadRunning = _is_thread_running;
  }
  bool is_thread_running(void) {
    std::unique_lock<std::mutex> lck(this->mIsThreadRunningMutex);
    return this->mIsThreadRunning;
  }

  bool mIsThreadRunning;
  std::mutex mIsThreadRunningMutex;

private:
  /* Receiver loop end condition */
  void wait_until_loop_ends(void) {
    std::unique_lock<std::mutex> lck(this->mIsLoopEndsMutex);
    if (!this->mIsLoopEnds) {
      this->mLoopEndsCond.wait(lck);
    }
  }
  void set_is_loop_ends(bool is_loop_ends) {
    std::unique_lock<std::mutex> lck(this->mIsLoopEndsMutex);
    this->mIsLoopEnds = is_loop_ends;
    if (this->mIsLoopEnds) {
      this->mLoopEndsCond.notify_all();
    }
  }
  bool mIsLoopEnds;
  std::mutex mIsLoopEndsMutex;
  std::condition_variable mLoopEndsCond;

private:
  /* Receiver loop enable/disable */
  void set_is_loop_enabled(bool _is_loop_enabled) {
    std::unique_lock<std::mutex> lck(this->mIsLoopEnabledMutex);
    this->mIsLoopEnabled = _is_loop_enabled;
    if (this->mIsLoopEnabled) {
      this->mEnableLoopCond.notify_all();
    }
  }
  bool is_loop_enabled(void) {
    std::unique_lock<std::mutex> lck(this->mIsLoopEnabledMutex);
    return this->mIsLoopEnabled;
  }
  void wait_until_enable_loop(void) {
    std::unique_lock<std::mutex> lck(this->mIsLoopEnabledMutex);
    if(!this->mIsLoopEnabled) {
      this->mEnableLoopCond.wait(lck);
    }
  }

  bool mIsLoopEnabled;
  std::mutex mIsLoopEnabledMutex;
  std::condition_variable mEnableLoopCond;

public:
  /* Constructor */
  ReceiverThread(ServerAdapter *motherAdapter) {
    this->mThread = NULL;
    this->mIsLoopEnabled = false;

    this->mAdapter = motherAdapter;
  }

private:
  /* Mother adapter */
  ServerAdapter *mAdapter = NULL;

  /* Components */
  std::thread *mThread;

}; /* class ReceiverThread */
} /* namespace sc */

#endif /* !defined(__RECEIVER_THREAD_H__) */