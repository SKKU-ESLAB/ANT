/* Copyright 2016 Eunsoo Park (esevan.park@gmail.com). All rights reserved
 * 
 * Contact: Eunsoo Park (esevan.park@gmail.com)
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

#ifndef INC_PROTOCOL_MANAGER_H_
#define INC_PROTOCOL_MANAGER_H_
#include <stdint.h>

// #define COMMUNICATOR_UNIT_TEST

/*  This is determined by the structure ProtocolData.
 *  In the ProtocolData, the size except the data pointer is the kProHeaderSize.
 */
#define kProtHeaderSize 6


namespace cm {

/**
 * Do not use architecture dependent sized types in this file
 * e.g. such as size_t
 */
typedef struct {
  uint16_t id;
  uint32_t len;
  const uint8_t *data;
} ProtocolData;

class ProtocolManager {
 public:
  static void data_to_protocol_data(const uint8_t *data,
                                     uint32_t len,
                                     ProtocolData *ret_pd);
  static uint32_t serialize(ProtocolData *pd,
                             const uint8_t *buf,
                             uint32_t offset,
                             uint32_t payload_size,
                             uint8_t **ret_vector);
  static int send_packet(uint8_t *serialized, uint32_t packet_size);

  static uint32_t recv_packet(uint8_t **seralized);

  static uint32_t parse_header(uint8_t *serialized, ProtocolData *ret_pd);
 private:
  static uint16_t packet_id;
  static void serialize_header(ProtocolData *pd, uint8_t *vec_ptr);
  static void serialize_data(const uint8_t *dat_buf,
                              uint32_t len,
                              uint8_t *vec_ptr);
};

} /* namespace cm */

#endif  // INC_PROTOCOL_MANAGER_H_
