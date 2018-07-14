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

#include <NetworkSwitcher.h>

#include <Core.h>
#include <ServerAdapter.h>
#include <SegmentManager.h>
#include <DebugLog.h>

#include <thread>
#include <unistd.h>

#define PRINT_DETAILS 0

#define SLEEP_USECS 250 * 1000

namespace cm {
NetworkSwitcher* NetworkSwitcher::singleton = NULL;

bool SwitchAdapterTransaction::sIsOngoing = false;
int SwitchAdapterTransaction::sPrevIndex = 0;
int SwitchAdapterTransaction::sNextIndex = 0;

bool ConnectRequestTransaction::sIsOngoing = false;
int ConnectRequestTransaction::sAdapterId = 0;

bool ReconnectControlAdapterTransaction::sIsOngoing = false;

void NetworkSwitcher::start(void) {
  this->mSwitcherThreadOn = true;
  this->mThread = new std::thread(std::bind(&NetworkSwitcher::switcher_thread, this));
  this->mThread->detach();
}

void NetworkSwitcher::stop(void) {
  this->mSwitcherThreadOn = false;
}

void NetworkSwitcher::switcher_thread(void) {
  this->set_state(NSState::kNSStateRunning);

  while(this->mSwitcherThreadOn) {
    int avg_send_request_speed;
    int avg_send_queue_data_size;
    int avg_total_bandwidth_now;
    this->monitor(avg_send_request_speed, avg_send_queue_data_size, avg_total_bandwidth_now);

    switch(this->get_state()) {
      case NSState::kNSStateInitialized:
        LOG_VERB("%s %d %lu %d\n",
            "Initialized",
            avg_send_request_speed, avg_send_queue_data_size, avg_total_bandwidth_now);
        break;
      case NSState::kNSStateRunning:
        LOG_VERB("%s %d %lu %d\n",
            "Ready",
            avg_send_request_speed, avg_send_queue_data_size, avg_total_bandwidth_now);
        this->check_and_handover(avg_send_request_speed, avg_send_queue_data_size, avg_total_bandwidth_now);
        break;
      case NSState::kNSStateSwitching:
        LOG_VERB("%s %d %lu %d\n",
            "Switching",
            avg_send_request_speed, avg_send_queue_data_size, avg_total_bandwidth_now);
        /* Network switcher do not work during increasing or decreasing adapter */
        break;
    }

    usleep(SLEEP_USECS);
  }

  this->set_state(NSState::kNSStateInitialized);
}

void NetworkSwitcher::connect_adapter(int adapter_id) {
  NSState state = this->get_state();
  if(state != NSState::kNSStateSwitching) {
    LOG_VERB("It's now switching. Cannot connect to adapter %d.", adapter_id);
    return;
  } else if(state != NSState::kNSStateRunning) {
    LOG_VERB("Switcher is not running. Cannot connect to adapter %d.", adapter_id);
    return;
  }
  this->set_state(NSState::kNSStateSwitching);
  ConnectRequestTransaction::start(adapter_id);
}

void NetworkSwitcher::reconnect_control_adapter(void) {
  NSState state = this->get_state();
  if(state != NSState::kNSStateSwitching) {
    LOG_VERB("It's now switching. Cannot reconnect control adapter.");
    this->reconnect_control_adapter();
    return;
  } else if(state != NSState::kNSStateRunning) {
    LOG_VERB("Switcher is not running. Cannot reconnect control adapter.");
    this->reconnect_control_adapter();
    return;
  }
  this->set_state(NSState::kNSStateSwitching);
  ReconnectControlAdapterTransaction::start();
}

void NetworkSwitcher::monitor(int &avg_send_request_speed,
    int& avg_send_queue_data_size, int& avg_total_bandwidth_now) {
  /* Monitor metrics */
  int send_request_speed;
  int send_queue_data_size;
  int total_bandwidth_now = 0;

  SegmentManager *segment_manager = SegmentManager::get_instance();
  send_request_speed = segment_manager->get_send_request_per_sec();
  send_queue_data_size = segment_manager->get_queue_data_size(kSegSend);
  send_queue_data_size += segment_manager->get_failed_sending_queue_data_size();

  Core *core = Core::get_instance();
  int i = 0;
  {
    ServerAdapter* adapter = core->get_control_adapter();
    int bandwidth_up = adapter->get_bandwidth_up();
    int bandwidth_down = adapter->get_bandwidth_down();
    total_bandwidth_now += (bandwidth_up + bandwidth_down);
#if PRINT_DETAILS == 1
    LOG_VERB("- A%d (Ctrl: %s): Up=%d B/s Down=%d B/s",
        i, adapter->get_dev_name(), bandwidth_up, bandwidth_down);
#endif
    i++;
  }

  int data_adapter_count = core->get_data_adapter_count();
  for(int i = 0; i < data_adapter_count; i++) {
    ServerAdapter *adapter = core->get_data_adapter(i);
    if(adapter == NULL) continue;
    int bandwidth_up = adapter->get_bandwidth_up();
    int bandwidth_down = adapter->get_bandwidth_down();
    total_bandwidth_now += (bandwidth_up + bandwidth_down);
#if PRINT_DETAILS == 1
    LOG_VERB("- A%d (Data: %s): Up=%d B/s Down=%d B/s",
        i, adapter->get_dev_name(), bandwidth_up, bandwidth_down);
#endif
    i++;
  }

  /* Get average */
  put_values(send_request_speed, send_queue_data_size, total_bandwidth_now);

  avg_send_request_speed = get_average_send_request_speed();
  avg_send_queue_data_size = get_average_send_queue_data_size();
  avg_total_bandwidth_now = get_average_total_bandwidth_now();
}

void NetworkSwitcher::check_and_handover(int avg_send_request_speed,
    int avg_send_queue_data_size, int avg_total_bandwidth_now) {
    /* Determine Increasing/Decreasing adapter */
    if(this->check_increase_adapter(avg_send_request_speed, avg_send_queue_data_size)) {
      /* Maintain bandwidth when increasing */
      this->mBandwidthWhenIncreasing = avg_total_bandwidth_now;

      /* Increase Adapter */
      this->set_state(NSState::kNSStateSwitching);
      bool res = this->increase_adapter();
      if(!res) {
        this->done_switch();
      }
    } else if(this->check_decrease_adapter(avg_total_bandwidth_now, this->mBandwidthWhenIncreasing)) {
      /* Decrease Adapter */
      this->set_state(NSState::kNSStateSwitching);
      bool res = this->decrease_adapter();
      if(!res) {
        this->done_switch();
      }
    }
}

#define AVERAGE_INCREASE_LATENCY_SEC 8.04f /* 8.04 sec */
#define MAX_BANDWIDTH 90000 /* 90000B/s */
bool NetworkSwitcher::check_increase_adapter(int send_request_speed, int send_queue_data_size) {
  /*
   * Increase condition: LHS > RHS
   * LHS: (average increase latency) * (r(t): send request speed) + (|SQ(t)|: send queue data size)
   * RHS: (average increase latency) * (maximum bluetooth bandwidth)
   */
  if(!this->is_increaseable()) {
    return false;
  } else if(Core::get_instance()->get_state() != kCMStateReady) {
    return false;
  } else if(((float)AVERAGE_INCREASE_LATENCY_SEC * send_request_speed + send_queue_data_size) >
      ((float)AVERAGE_INCREASE_LATENCY_SEC * MAX_BANDWIDTH)) {
    return true;
  } else {
    return false;
  }
}

#define CHECK_DECREASING_OK_COUNT 5
bool NetworkSwitcher::check_decrease_adapter(int bandwidth_now, int bandwidth_when_increasing) {
  /*
   * Decrease condition: LHS < RHS (6 times)
   * LHS: (b(t): total bandwidth)
   * RHS: (b(t_wfdon): total bandwidth when increasing)
   */
  if(!this->is_decreaseable()) {
    return false;
  } else if(Core::get_instance()->get_state() != kCMStateReady) {
    return false;
  } else if(bandwidth_when_increasing == 0) {
    return false;
  } else if(bandwidth_now < bandwidth_when_increasing) {
    this->mDecreasingCheckCount++;
    if(this->mDecreasingCheckCount >= CHECK_DECREASING_OK_COUNT) {
      this->mDecreasingCheckCount = 0;
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

bool NetworkSwitcher::increase_adapter(void) {
  NSState state = this->get_state();
  if(state != NSState::kNSStateRunning) {
    if(state == NSState::kNSStateSwitching) {
      LOG_ERR("It's already connecting or disconnecting an adapter!");
      return false;
    } else {
      LOG_ERR("Core is not started.");
      return false;
    }
  } else if(Core::get_instance()->get_data_adapter_count() == 0) {
    LOG_ERR("No data adapter is registered!");
    return false;
  } else if(!this->is_increaseable()) {
    LOG_WARN("Cannot increase adapter!");
    return false;
  }

  int prev_index = this->mActiveDataAdapterIndex;
  int next_index = this->mActiveDataAdapterIndex + 1;

  return this->switch_adapters(prev_index, next_index);
}

bool NetworkSwitcher::decrease_adapter(void) {
  NSState state = this->get_state();
  if(state != NSState::kNSStateRunning) {
    if(state == NSState::kNSStateSwitching) {
      LOG_ERR("It's already connecting or disconnecting an adapter!");
      return false;
    } else {
      LOG_ERR("Core is not started.");
      return false;
    }
  } else if(Core::get_instance()->get_data_adapter_count() == 0) {
    LOG_ERR("No data adapter is registered!");
    return false;
  } else if(!this->is_decreaseable()) {
    LOG_WARN("Cannot deccrease adapter!");
    return false;
  }

  int prev_index = this->mActiveDataAdapterIndex;
  int next_index = this->mActiveDataAdapterIndex - 1;

  return this->switch_adapters(prev_index, next_index);
}

bool NetworkSwitcher::switch_adapters(int prev_index, int next_index) {
  NSState state = this->get_state();
  if(state != NSState::kNSStateRunning) {
    if(state == NSState::kNSStateSwitching) {
      LOG_ERR("It's already connecting or disconnecting an adapter!");
      return false;
    } else {
      LOG_ERR("Core is not started.");
      return false;
    }
  }
  // Switch Step 1/4
  // Increase/decrease active data adapter index
  this->mActiveDataAdapterIndex = next_index;

  return SwitchAdapterTransaction::start(prev_index, next_index);
}

bool NetworkSwitcher::is_increaseable(void) {
  int data_adapter_count = Core::get_instance()->get_data_adapter_count();
  int active_data_adapter_index = this->get_active_data_adapter_index();
  return ((data_adapter_count > 1) && (active_data_adapter_index < (data_adapter_count - 1)));
}
bool NetworkSwitcher::is_decreaseable(void) {
  int data_adapter_count = Core::get_instance()->get_data_adapter_count();
  int active_data_adapter_index = this->get_active_data_adapter_index();
  return ((data_adapter_count > 1) && (active_data_adapter_index > 0));
}

bool SwitchAdapterTransaction::start(int prev_index, int next_index) {
  // Switch Step 2/4
  Core* core = Core::get_instance();
  NetworkSwitcher* switcher = NetworkSwitcher::get_instance();
  if(SwitchAdapterTransaction::sIsOngoing) {
    LOG_ERR("Only one transaction can run at the same time.");
    switcher->done_switch();
    return false;
  }
  SwitchAdapterTransaction::sIsOngoing = true;
  SwitchAdapterTransaction::sPrevIndex = prev_index;
  SwitchAdapterTransaction::sNextIndex = next_index;

  // Connect next active adapter
  ServerAdapter* next_adapter = core->get_data_adapter(SwitchAdapterTransaction::sNextIndex);
  if(next_adapter == NULL) {
    LOG_ERR("Connecting next data adapter is failed 1");
    switcher->done_switch();
    SwitchAdapterTransaction::sIsOngoing = false;
    return false;
  }
  bool res = next_adapter->connect(SwitchAdapterTransaction::connect_callback, true);
  if(!res) {
    LOG_ERR("Connecting next data adapter is failed 2");
    switcher->done_switch();
    SwitchAdapterTransaction::sIsOngoing = false;
    return false;
  }
  return true;
}

void SwitchAdapterTransaction::connect_callback(bool is_success) {
  // Switch Step 3/4
  Core* core = Core::get_instance();
  NetworkSwitcher* switcher = NetworkSwitcher::get_instance();
  if(!is_success) {
    LOG_ERR("Connecting next data adapter is failed 3");
    switcher->done_switch();
    SwitchAdapterTransaction::sIsOngoing = false;
    return;
  }
  // Disconnect previous active adapter
  ServerAdapter* prev_adapter = core->get_data_adapter(SwitchAdapterTransaction::sPrevIndex);
  if(prev_adapter == NULL) {
    LOG_ERR("Disconnecting previous data adapter is failed 1");
    switcher->done_switch();
    SwitchAdapterTransaction::sIsOngoing = false;
    return;
  }
  bool res = prev_adapter->disconnect(SwitchAdapterTransaction::disconnect_callback);
  if(!res) {
    LOG_ERR("Disconnecting previous data adapter is failed 2");
    switcher->done_switch();
    SwitchAdapterTransaction::sIsOngoing = false;
    return;
  }
}

void SwitchAdapterTransaction::disconnect_callback(bool is_success) {
  // Switch Step 4/4
  Core* core = Core::get_instance();
  NetworkSwitcher* switcher = NetworkSwitcher::get_instance();
  if(!is_success) {
    LOG_ERR("Disconnecting previous data adapter is failed 3");
    switcher->done_switch();
    SwitchAdapterTransaction::sIsOngoing = false;
    return;
  }

  LOG_DEBUG("Switch from %d to %d done.",
      SwitchAdapterTransaction::sPrevIndex, SwitchAdapterTransaction::sNextIndex);
  switcher->done_switch();
  switcher->set_active_data_adapter_index(SwitchAdapterTransaction::sNextIndex);
  SwitchAdapterTransaction::sIsOngoing = false;
  return;
}

// Connect Request
bool ConnectRequestTransaction::start(int adapter_id) {
  if(ConnectRequestTransaction::sIsOngoing) {
    LOG_ERR("Only one transaction can run at the same time.");
    ConnectRequestTransaction::on_fail();
    return false;
  }
  ConnectRequestTransaction::sIsOngoing = true;
  ConnectRequestTransaction::sAdapterId = adapter_id;

  // Connect requested adapter
  ServerAdapter* adapter = Core::get_instance()->find_data_adapter_by_id(ConnectRequestTransaction::sAdapterId);
  if(adapter == NULL) {
    LOG_ERR("Connecting requested data adapter is failed 1");
    ConnectRequestTransaction::on_fail();
    return false;
  }
  bool res = adapter->connect(ConnectRequestTransaction::connect_callback, false);
  if(!res) {
    LOG_ERR("Connecting requested data adapter is failed 2");
    ConnectRequestTransaction::on_fail();
    return false;
  }
  return true;
}

void ConnectRequestTransaction::connect_callback(bool is_success) {
  if(!is_success) {
    LOG_ERR("Connecting requested data adapter is failed 3");
    ConnectRequestTransaction::on_fail();
    return;
  }
  LOG_VERB("Connecting requested data adapter is done");
  NetworkSwitcher::get_instance()->done_switch();
}

void ConnectRequestTransaction::on_fail(void) {
  ConnectRequestTransaction::sIsOngoing = false;
  NetworkSwitcher::get_instance()->done_switch();
}

// Reconnect Control Adapter
bool ReconnectControlAdapterTransaction::start() {
  if(ReconnectControlAdapterTransaction::sIsOngoing) {
    LOG_ERR("Only one transaction can run at the same time.");
    ReconnectControlAdapterTransaction::on_fail(false);
    return false;
  }
  ReconnectControlAdapterTransaction::sIsOngoing = true;

  // disconnect control adapter
  ServerAdapter* control_adapter = Core::get_instance()->get_control_adapter();
  if(control_adapter == NULL) {
    LOG_ERR("Reconnecting control adapter is failed 1: retry");
    ReconnectControlAdapterTransaction::on_fail(true);
    return false;
  }
  bool res = control_adapter->disconnect(ReconnectControlAdapterTransaction::disconnect_callback);
  if(!res) {
    LOG_ERR("Reconnecting control adapter is failed 2: retry");
    ReconnectControlAdapterTransaction::on_fail(true);
    return false;
  }
  return true;
}

void ReconnectControlAdapterTransaction::disconnect_callback(bool is_success) {
  if(!is_success) {
    LOG_ERR("Reconnecting control adapter is failed 3: retry");
    ReconnectControlAdapterTransaction::on_fail(true);
    return;
  }
  // connect control adapter
  ServerAdapter* control_adapter = Core::get_instance()->get_control_adapter();
  if(control_adapter == NULL) {
    LOG_ERR("Reconnecting control adapter is failed 4: retry");
    ReconnectControlAdapterTransaction::sIsOngoing = false;
    ReconnectControlAdapterTransaction::start();
    return;
  }
  bool res = control_adapter->connect(ReconnectControlAdapterTransaction::disconnect_callback, false);
  if(!res) {
    LOG_ERR("Disconnecting control adapter is failed 5: retry");
    ReconnectControlAdapterTransaction::on_fail(true);
    return;
  }
}

void ReconnectControlAdapterTransaction::connect_callback(bool is_success) {
  if(!is_success) {
    LOG_ERR("Reconnecting control adapter is failed 6: retry");
    ReconnectControlAdapterTransaction::on_fail(true);
    return;
  }
  LOG_VERB("Reconnecting control adapter is done");
  NetworkSwitcher::get_instance()->done_switch();
}

void ReconnectControlAdapterTransaction::on_fail(bool is_restart) {
  ReconnectControlAdapterTransaction::sIsOngoing = false;

  if(is_restart) {
    ReconnectControlAdapterTransaction::start();
  } else {
    NetworkSwitcher::get_instance()->done_switch();
  }
}

} /* namespace cm */
