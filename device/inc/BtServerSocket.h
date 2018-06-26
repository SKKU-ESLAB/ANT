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

#ifndef _BT_SERVER_SOCKET_H_
#define _BT_SERVER_SOCKET_H_

#include <ServerSocket.h>

#include <counter.h>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <stdio.h>

namespace cm {

class BtServerSocket : ServerSocket {
public:
  virtual bool open_impl(void);
  virtual bool close_impl(void);
  virtual int send_impl(const void *buf, size_t len);
  virtual int receive_impl(void *buf, size_t len);

  uuid_t get_service_uuid(void) {
    return this->mServiceUUID;
  }

  BtServerSocket(const char* service_uuid) {
    char buffer[100];
    snprintf(buffer, 100, "%s", svc_uuid);
    str2uuid(buffer, &(this->svc_uuid));
  }

  ~BtServerSocket(void) {
  }

protected:
  uuid_t mServiceUUID;
}; /* class BtServerSocket */

} /* namespace cm */

#endif /* !defined(_BT_SERVER_SOCKET_H_) */
