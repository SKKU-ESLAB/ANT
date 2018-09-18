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

#include "../inc/ControlMessageSender.h"

#include "../inc/API.h"

#include <string>

using namespace sc;

void ControlMessageSender::send_control_message(std::string& message) {
    int message_buffer_size = message.size() + 1;
    char* message_buffer = new char[message_buffer_size];
    int res = Core::get_instance()->send(message_buffer, message_buffer_size);
}

void ControlMessageSender::send_request(CMCode request_code,
                                        uint16_t adapter_id) {
  bool retry_check = true;
  while (retry_check) {
    ServerAdapter *control_adapter = this->get_active_control_adapter();
    ServerAdapterState controlAdapterState = control_adapter->get_state();
    if (controlAdapterState != ServerAdapterState::kActive) {
      LOG_WARN("Now switching control adapter... adapter_id=%d state=%d",
               control_adapter->get_id(), controlAdapterState);
    } else {
      retry_check = false;
    }
  }

  uint8_t net_request_code = (uint8_t)request_code;
  uint16_t net_adapter_id = htons(adapter_id);

  this->send_control_message(&net_request_code, 1);
  this->send_control_message(&net_adapter_id, 2);
}

void ControlMessageSender::send_request_connect(uint16_t adapter_id) {
  this->send_request(kCMCodeConnect, adapter_id);
  LOG_VERB("Send(Control Msg): Request(Connect %d)", adapter_id);
}

void ControlMessageSender::send_request_disconnect(uint16_t adapter_id) {
  this->send_request(kCMCodeDisconnect, adapter_id);
  LOG_VERB("Send(Control Msg): Request(Disconnect %d)", adapter_id);
}

void ControlMessageSender::send_request_disconnect_ack(uint16_t adapter_id) {
  this->send_request(kCMCodeDisconnectAck, adapter_id);
  LOG_VERB("Send(Control Msg): Request(DisconnectAck %d)", adapter_id);
}

void ControlMessageSender::send_request_sleep(uint16_t adapter_id) {
  this->send_request(kCMCodeSleep, adapter_id);
  LOG_VERB("Send(Control Msg): Request(Sleep %d)", adapter_id);
}

void ControlMessageSender::send_request_wake_up(uint16_t adapter_id) {
  this->send_request(kCMCodeWakeUp, adapter_id);
  LOG_VERB("Send(Control Msg): Request(WakeUp %d)", adapter_id);
}

void ControlMessageSender::send_noti_private_data(PrivType priv_type,
                                                  char *private_data_buf,
                                                  uint32_t private_data_len) {
  bool retry_check = true;
  while (retry_check) {
    ServerAdapter *control_adapter = this->get_active_control_adapter();
    ServerAdapterState controlAdapterState = control_adapter->get_state();
    if (controlAdapterState != ServerAdapterState::kActive) {
      LOG_WARN("Now switching control adapter...");
    } else {
      retry_check = false;
    }
  }

  uint8_t request_code = kCMCodePriv;
  uint32_t net_priv_type = htonl((unsigned long)priv_type);
  uint32_t net_private_data_len = htonl(private_data_len);

  LOG_VERB("Send(Control Msg): Request(Priv Noti--Start)");
  this->send_control_message(&request_code, 1);
  this->send_control_message(&net_priv_type, 4);
  this->send_control_message(&net_private_data_len, 4);
  this->send_control_message(private_data_buf, private_data_len);
  LOG_VERB("Send(Control Msg): Request(Priv Noti--End; type=%d)\n%s", priv_type,
           private_data_buf);
}