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

#ifndef __CONTROL_MESSAGE_RECEIVER_H__
#define __CONTROL_MESSAGE_RECEIVER_H__

#include "../inc/ControlMessageProtocol.h"

#include <string>
#include <thread>
#include <vector>

namespace sc {

class ControlPrivMessageListener {
public:
  virtual void on_receive_control_priv_message(int priv_type,
                                               std::string priv_message) = 0;
}; /* class ControlPrivMessageListener */

class ControlMessageReceiver {
public:
  void start_receiving_thread(void);
  void stop_receiving_thread(void);

  /* Handling control message (private data) listener */
  void add_control_message_listener(ControlPrivMessageListener *listener) {
    this->mPrivMessageListeners.push_back(listener);
  }

protected:
  /* Receiving Thread */
  void receiving_thread_loop(void);
  bool receiving_thread_loop_internal(void);
  void on_receive_normal_message(int control_message_code, int adapter_id);
  void on_receive_disconnect_message(int adapter_id,
                                     uint32_t final_seq_no_control,
                                     uint32_t final_seq_no_data);
  void on_receive_priv_message(std::string contents);
  std::thread *mReceivingThread = NULL;
  bool mReceivingThreadOn = false;

  /* Control Message Listeners */
  std::vector<ControlPrivMessageListener *> mPrivMessageListeners;
}; /* class ControlMessageReceiver */
} /* namespace sc */

#endif /* !defined(__CONTROL_MESSAGE_RECEIVER_H__) */