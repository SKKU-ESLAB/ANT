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

#ifndef __SENDER_THREAD_H__
#define __SENDER_THREAD_H__

#include "ServerAdapter.h"

#include <condition_variable>
#include <mutex>
#include <thread>

namespace sc {
class ServerAdapter;

class SenderThread {
public:
  void launch_thread(void) {
    if (this->mThread == NULL) {
      this->mThread = new std::thread(std::bind(&SenderThread::run, this));
      this->mThread->detach();
    }
  }

  void finish_thread(void) { this->set_is_thread_running(false); }

  void enable_loop(void) { this->set_is_loop_enabled(true); }

  void disable_loop(void) { this->set_is_loop_enabled(false); }

  void wait_until_disable_loop_done(void) { this->wait_until_loop_ends(); }

  void resume_loop(void) { this->set_is_loop_paused(false); }

  void pause_loop(void) { this->set_is_loop_paused(true); }

private:
  /* Main routine and thread loop */
  void run(void);

  /* Sender loop */
  void sender_loop(void);

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
  /* Sender loop end condition */
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
  /* Sender loop enable/disable */
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
    if (!this->mIsLoopEnabled) {
      this->mEnableLoopCond.wait(lck);
    }
  }

  bool mIsLoopEnabled;
  std::mutex mIsLoopEnabledMutex;
  std::condition_variable mEnableLoopCond;

public:
  /* Sender loop pause/resume */
  bool is_loop_paused(void) {
    std::unique_lock<std::mutex> lck(this->mIsLoopPausedMutex);
    return this->mIsLoopPaused;
  }

private:
  /* Sender loop pause/resume */
  void set_is_loop_paused(bool _is_loop_paused) {
    std::unique_lock<std::mutex> lck(this->mIsLoopPausedMutex);
    this->mIsLoopPaused = _is_loop_paused;
    if (!this->mIsLoopPaused) {
      this->mResumeLoopCond.notify_all();
    }
  }
  void wait_until_resume_loop(void) {
    std::unique_lock<std::mutex> lck(this->mIsLoopPausedMutex);
    if (this->mIsLoopPaused) {
      this->mResumeLoopCond.wait(lck);
    }
  }

  bool mIsLoopPaused = false;
  std::mutex mIsLoopPausedMutex;
  std::condition_variable mResumeLoopCond;

public:
  /* Sender loop last sequence number */
  uint32_t get_last_seq_no_control() {
    return this->mLastSeqNoControl;
  }
  uint32_t get_last_seq_no_data() {
    return this->mLastSeqNoData;
  }

private:
  /* Sender loop last sequence number */
  void set_last_seq_no_control(uint32_t last_seq_no_control) {
    this->mLastSeqNoControl = last_seq_no_control;
  }

  void set_last_seq_no_data(uint32_t last_seq_no_data) {
    this->mLastSeqNoData = last_seq_no_data;
  }

  uint32_t mLastSeqNoControl;
  uint32_t mLastSeqNoData;

public:
  /* Constructor */
  SenderThread(ServerAdapter *motherAdapter) {
    this->mThread = NULL;
    this->mIsLoopEnabled = false;
    this->mIsLoopPaused = false;

    this->mAdapter = motherAdapter;
  }

private:
  /* Mother adapter */
  ServerAdapter *mAdapter = NULL;

  /* Components */
  std::thread *mThread;

}; /* class SenderThread */
} /* namespace sc */

#endif /* !defined(__SENDER_THREAD_H__) */