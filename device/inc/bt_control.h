/* Copyright 2017 Eunsoo Park (esevan.park@gmail.com). All rights reserved
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

#ifndef DEVICE_INC_BT_CONTROL_H_
#define DEVICE_INC_BT_CONTROL_H_

namespace bt {
void bt_server_init();
int bt_open();
int bt_close();
int bt_accept();
} /* namespace bt */
// Now bt handling code is workin under bluez 4.X.
// Not support bluez 5.X version

