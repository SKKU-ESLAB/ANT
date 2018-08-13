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

#ifndef INC_API_H_
#define INC_API_H_

#include <Core.h>
#include <ServerAdapter.h>
#include <NetworkSwitcher.h>

namespace cm {

inline void start_sc(void) {
  NetworkSwitcher::get_instance()->start();
  Core::get_instance()->start();
}

inline void stop_sc(void) {
  NetworkSwitcher::get_instance()->stop();
  Core::get_instance()->stop();
}

inline void register_control_adapter(ServerAdapter* adapter) {
  Core::get_instance()->register_control_adapter(adapter);
}

inline void register_data_adapter(ServerAdapter* adapter) {
  Core::get_instance()->register_data_adapter(adapter);
}

/**
 * If data size is big, it is recommanded to use following
 * libraries in a thread
 */
inline int send(const void *dataBuffer, uint32_t dataLength) {
  Core::get_instance()->send(dataBuffer, dataLength);
}

/**
 * @param len: IN buffer length
 * @param buf: OUT buffer read
 * @return: Received bytes(<0 if error)
 */
inline int receive(void **dataBuffer) {
  Core::get_instance()->receive(dataBuffer);
}

} /* namespace cm */
#endif  /* INC_API_H_ */