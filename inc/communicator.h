/* Copyright 2015-2016 CISS, and contributors. All rights reserved
 * 
 * Contact: Eunsoo Park <esevan.park@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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

#ifndef INC_COMMUNICATOR_H_
#define INC_COMMUNICATOR_H_
#include <stdint.h>
namespace cm {

enum CommErr {
  kProtOk = 0,
  kProtErr = -1,
};

class Communicator {
 public:
    static Communicator *get_instance(void);

    /**
     * If data size is big, it is recommanded to use following
     * libraries in a thread
     */
    int send_data(const void *buf, uint32_t len);

    /**
     * @param len: IN buffer length
     * @param buf: OUT buffer read
     * @return: Received bytes(<0 if error)
     */
    int recv_data(void **buf);

    void finalize(void);

 private:
    Communicator(void);
};

} /* namespace cm */
#endif  /* INC_COMMUNICATOR_H_ */
