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

#ifndef __CONTROL_MESSAGE_SENDER_H__
#define __CONTROL_MESSAGE_SENDER_H__

#include "ControlMessageProtocol.h"

#include <stdint.h>

namespace sc {
class ControlMessageSender {
public:
  // TODO:
  /* Control message handling (External) */
  void send_request_connect(int adapter_id);
  void send_request_disconnect(int adapter_id);
  void send_request_disconnect_ack(int adapter_id);
  void send_request_sleep(int adapter_id);
  void send_request_wake_up(int adapter_id);
  void send_noti_private_data(PrivType priv_type, char *private_data_buf,
                              uint32_t private_data_len);

private:
  /* Control message handling (Internal) */
  void send_control_message(std::string& message);
  void send_request(CMCode request_code, uint16_t adapter_id);
}; /* class ControlMessageSender */
} /* namespace sc */

#endif /* !defined(__CONTROL_MESSAGE_SENDER_H__) */