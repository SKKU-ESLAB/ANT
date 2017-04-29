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

#include <protocol_manager.h>
#include <segment_manager.h>
#include <dbug_log.h>

#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>


namespace cm {
uint16_t ProtocolManager::packet_id = 1;

void ProtocolManager::data_to_protocol_data(const uint8_t *dat,
                                            uint32_t size,
                                            ProtocolData *ret_pd) {
  assert(ret_pd != NULL);
  assert(size <= UINT16_MAX);
  ret_pd -> id = packet_id++;
  ret_pd -> len = size;
  ret_pd -> data = dat;

#ifdef COMMUNICATOR_UNIT_TEST
  OPEL_DBG_LOG("%d\t%u\t[%s]",
               ret_pd -> id,
               ret_pd -> len,
               ret_pd -> data);
#endif /* COMMUNICATOR_UNIT_TEST */
}

inline void ProtocolManager::serialize_header(ProtocolData *pd,
                                              uint8_t *vec_ptr) {
  uint32_t vec_offset = 0;
  uint16_t net_id = htons(pd -> id);
  uint16_t net_len = htons(pd -> len);

  memcpy(vec_ptr + vec_offset, &net_id, sizeof(uint16_t));
  vec_offset += sizeof(uint16_t);
  memcpy(vec_ptr + vec_offset, &net_len, sizeof(uint16_t));
  vec_offset += sizeof(uint16_t);
}

inline void ProtocolManager::serialize_data(const uint8_t *dat_buf,
                                            uint32_t len,
                                            uint8_t *vec_ptr) {
  memcpy(vec_ptr, dat_buf, (size_t) len);
}

uint32_t ProtocolManager::serialize(ProtocolData *pd,
                                    const uint8_t *buf,
                                    uint32_t offset,
                                    uint32_t payload_size,
                                    uint8_t **ret_vector) {
  uint8_t *serialized_vector;
  uint32_t vector_size;
  uint32_t data_offset;

  data_offset = kProtHeaderSize;
  vector_size = data_offset + payload_size;

  serialized_vector = reinterpret_cast<uint8_t *>(calloc((size_t) vector_size,
                                                         sizeof(uint8_t)));

  serialize_header(pd, serialized_vector);
  serialize_data(buf + offset, payload_size, serialized_vector + data_offset);

#ifdef COMMUNICATOR_UNIT_TEST
  OPEL_DBG_LOG("%p\t(%u)\tFrom %p~%p(%u)",
               serialized_vector,
               vector_size,
               buf + offset,
               buf + offset + payload_size - 1,
               payload_size);

  uint8_t buf_test = *(serialized_vector);
  buf_test = *(serialized_vector + vector_size - 1);
  if (buf != NULL) {
    buf_test = *(buf);
    buf_test = *(buf + offset + payload_size -1);
  }
  OPEL_DBG_LOG("Done");
#endif /* COMMUNICATOR_UNIT_TEST */

  *ret_vector = serialized_vector;

  return vector_size;
}

uint32_t ProtocolManager::parse_header(uint8_t *serialized,
                                       ProtocolData *ret_pd) {
  uint32_t vec_offset = 0;
  uint16_t net_id;
  uint32_t net_len;

  memcpy(&net_id, serialized + vec_offset, sizeof(uint16_t));
  vec_offset += sizeof(uint16_t);
  ret_pd -> id = ntohs(net_id);

  memcpy(&net_len, serialized + vec_offset, sizeof(uint16_t));
  vec_offset += sizeof(uint16_t);
  ret_pd -> len = ntohs(net_len);

  return vec_offset;
}

int ProtocolManager::send_packet(uint8_t *serialized, uint32_t packet_size) {
  SegmentManager *sm = SegmentManager::get_instance();
#ifdef COMMUNICATOR_UNIT_TEST
  uint8_t buf_test = *serialized;
  buf_test = *(serialized + packet_size - 1);
  free(serialized);
#endif
  // Hand over the data to the segment manager
  return sm -> send_to_segment_manager(serialized, packet_size);
}

uint32_t ProtocolManager::recv_packet(uint8_t **buf) {
  ProtocolData pd = {0, 0, NULL};
  SegmentManager *sm = SegmentManager::get_instance();
  uint8_t *p_data_buf =
      sm -> recv_from_segment_manager(reinterpret_cast<void *>(&pd));
  assert(pd.len == 0 || p_data_buf != NULL);

  *buf = p_data_buf;

  return pd . len;
}
} /* namespace cm */
