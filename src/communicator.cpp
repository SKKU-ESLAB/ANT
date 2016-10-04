/* Copyright 2015-2016 CISS, and contributors. All rights reserved
 * 
 * Contact: Eunsoo Park <esevan.park@gmail.com>
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

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <dbug_log.h>
#include <communicator.h>
#include <protocol_manager.h>
#include <segment_manager.h>
#include <network_manager.h>

/**
 * Memory Free Timing
 * When ProtocolManager::serialize() method is invoked, memory will be allocated.
 * ProtocolManager is in charge of allocating this memory.
 * SegmentManager is in charge of freeing this memory.
 * 
 * SegmentManager invokes ProtocolManager::parse() with allocated serialized vector.
 * SegmentManager is in charge of allocating this memory.
 * After copying to application memory, serialized vector should be freed.
 * ProtocolManager is in charge of freeing this memory.
 */
namespace cm {
static Communicator *communicator_instance = NULL;

Communicator * Communicator::get_instance(void) {
  if (communicator_instance == NULL) {
    communicator_instance = new Communicator();
  }
  return communicator_instance;
}

Communicator::Communicator(void) {
  SegmentManager *sm = SegmentManager::get_instance();
  NetworkManager *nm = NetworkManager::get_instance();
}

void Communicator::finalize(void) {
  SegmentManager::get_instance() -> free_segment_all();
}

int Communicator::send_data(const void *buf, uint32_t len) {
  uint32_t curr_offset = 0;
  int sent_bytes;

  ProtocolData pd;

  uint8_t *serialized_vector;
  uint32_t packet_size;

  ProtocolManager::data_to_protocol_data((const uint8_t *) buf, len, &pd);

  packet_size = ProtocolManager::serialize(&pd,
                                           (const uint8_t *)buf,
                                           curr_offset,
                                           len,
                                           &serialized_vector);
  assert(serialized_vector != NULL && packet_size > 0);

  sent_bytes = ProtocolManager::send_packet(serialized_vector, packet_size);
  if (unlikely(sent_bytes < 0)) {
    OPEL_DBG_ERR("Sending stopped(%u/%u) by %d",
                 curr_offset,
                 len,
                 sent_bytes);
  }

  return sent_bytes;
}

int Communicator::recv_data(void **buf) {
  int ret;
  uint32_t packet_size;
  uint8_t *packet;

  packet_size = ProtocolManager::recv_packet(&packet);

  if (packet_size > UINT16_MAX)
    return kProtErr;

  *buf = packet;

  return packet_size;
}

} /* namespace cm */
