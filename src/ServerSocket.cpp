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

#include <ServerAdapter.h>

#include <dbug_log.h>

using namespace cm;

bool ServerSocket::open() {
  if(this->get_state != ServerSocketState::kClosed) {
    LOG_ERR("It is already opened or opening/closing is in progress.");
    return false;
  }

  this->set_state(ServerSocketState::kOpening);

  bool res = this->open_impl();

  if(!res) {
    this->set_state(ServerSocketState::kClosed);
  } else {
    this->set_state(ServerSocketState::kOpened);
  }
  return res;
}

bool ServerSocket::close() {
  if(this->get_state != ServerSocketState::kOpened) {
    LOG_ERR("It is already closed or opening/closing is in progress.");
    return false;
  }

  this->set_state(ServerSocketState::kClosing);

  bool res = this->close_impl();

  this->set_state(ServerSocketState::kClosed);
  return res;
}

int ServerSocket::send(const void *buf, size_t len) {
  if(this->get_state() != ServerSocketState::kOpened) {
    LOG_ERR("Socket is not opened");
    return -1;
  }
  
  int res = this->send_impl(buf, len);
  if(!res) {
    this->set_state(ServerSocketState::kClosed);
  }
  return res;
}

int ServerSocket::receive(void *buf, size_t len) {
  if(this->get_state() != ServerSocketState::kOpened) {
    LOG_ERR("Socket is not opened");
    return -1;
  }
  
  int res = this->receive_impl(buf, len);
  if(!res) {
    this->set_state(ServerSocketState::kClosed);
  }
  return res;
}
