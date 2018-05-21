/* Copyright 2017-2018 All Rights Reserved.
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *  Eunsoo Park (esevan.park@gmail.com)
 *  Injung Hwang (sinban04@gmail.com)
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

#include <network_adapter.h>

#include <dbug_log.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace cm;

class TestAdapter : public NetworkAdapter {
 public:
  TestAdapter() {
    snprintf(dev_name, sizeof(dev_name), "Test");
    LOG_VERB("Test Adapter created");
    copied = false;
    set_controllable();
  }

  uint16_t get_id(void) {
    return 0x8383;
  }

 protected:
  size_t send_impl(const void *buf, size_t len) {
    if (sch) {
      LOG_VERB("Sent:%x (%u)", buf, len);
      memcpy(buff, buf, len);
      lenn = len;
      copied = true;
      usleep(100000);
    } else {
      LOG_VERB("Failed sending", buf, len);
    }

    return lenn;
  }
  size_t recv_impl(void *buf, size_t len) {
    if (sch) {
      while (true) {
        if (copied) {
          memcpy(buf, buff, (lenn < len)? lenn:len);
          copied = false;
          LOG_VERB("Recv:%s (%u)",
                       (unsigned char*)buf + 4,
                       (lenn < len)? lenn : len);
          return (lenn < len)? lenn : len;
        } else {
          usleep(50000);
        }
      }
    } else {
      LOG_VERB("Failed recving", buf, len);
    }
    return sch;
  }

 private:
  bool sch;
  char buff[4096];
  unsigned lenn;
  bool copied;
  bool device_on() {
    LOG_VERB("Devince ON");
    return true;
  }
  bool device_off() {
    LOG_VERB("Device OFF");
    return true;
  }
  bool make_connection(void) {
    LOG_VERB("Connected");
    sch = true;
    return true;
  }
  bool close_connection(void) {
    LOG_VERB("Closed");
    sch = false;
    return true;
  }

  virtual void on_control_recv(const void *buf, size_t len){
  }
};


