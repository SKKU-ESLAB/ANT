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

#ifndef DEVICE_INC_WIFI_CONTROL_H_
#define DEVICE_INC_WIFI_CONTROL_H_

#include <stddef.h>

namespace wifi {
int wifi_direct_server_up(char *device_name);
int wifi_direct_server_down(void);
int wifi_direct_server_reset(char *pin, size_t len);

int wifi_get_p2p_device_addr(char *dev_addr, size_t len);
int wifi_get_device_name(char *dev_name, size_t len);
int wifi_set_device_name(char *dev_name, size_t len);

int wifi_direct_ip_addr(char *ip_str, size_t len);

/* Not allow to modify device information */
// int wifi_set_manufacturer(char *manufacturer);
// int wifi_set_model_name(char *model_name);
// int wifi_set_model_number(char *model_number);
// int wifi_set_serial_number(char *serial_number);

}  /* namespace wifi */

#endif  // DEVICE_INC_WIFI_CONTROL_H_
