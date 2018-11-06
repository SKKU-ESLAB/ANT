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

#ifndef __SEGMENT_H__
#define __SEGMENT_H__

#include <stdint.h>

#define kSegSize 512
#define kSegHeaderSize 28

/**
 * < Data Structure of Segment > - Handled by Segment Manager
 * Segment is the minimum unit of sending data through the network.
 * The partial of the segment cannot be sent.
 *
 * (*c.f.) Segment Header (seq_no + flag_len) is delicate to memroy alignment.
 *    You need to be careful with the segment header.
 */
#define SEGMENT_DATA_SIZE (kSegSize + kSegHeaderSize)
typedef struct {
  uint32_t seq_no;
  uint32_t len; // To present the size of the segment(consider the flag)
  uint32_t flag;
  int send_start_ts_sec;
  int send_start_ts_usec;
  int media_start_ts_sec; // TODO: hard-coded: actually, not used
  int media_start_ts_usec; // TODO: hard-coded: actually, not used
  uint8_t data[SEGMENT_DATA_SIZE];
} Segment; /* struct Segment */

#endif /* !defined(__SEGMENT_H__) */