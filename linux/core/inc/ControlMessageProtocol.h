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

#ifndef __CONTROL_MESSAGE_PROTOCOL_H__
#define __CONTROL_MESSAGE_PROTOCOL_H__

namespace sc {
/**
 *  <Control Message Protocol>
 *  Line 1:  Control Message Code
 *  Line 2:  Target Adapter ID (Normal Type)
 *           Priv Message Type (Priv Type)
 *  Line 3+: Priv Message Contents (Priv Type)
 */

/**
 * Control Message Code
 * It is used to classify "control request message" that is transferred to the
 * peer.
 *  - Commands: "Connect", "Sleep", "WakeUp", "Disconnect"
 *  - Acks: "Disconnect"
 *  - Private Data: "Priv"
 */
typedef enum {
  kCMCodeConnect = 1,
  kCMCodeSleep = 2,
  kCMCodeWakeUp = 3,
  kCMCodeDisconnect = 4,
  kCMCodePriv = 10,
  kCMCodeDisconnectAck = 24
} CMCode; /* enum CMCode */

/**
 * Control Priv Message Type
 *  - WFDInfo: Wi-fi Direct Information
 */
typedef enum {
  kPrivTypeWFDInfo = 1,
  kPrivTypeUnknown = 999
} PrivType; /* enum PrivType */

} /* namespace sc */

#endif /* !defined(__CONTROL_MESSAGE_PROTOCOL_H__) */