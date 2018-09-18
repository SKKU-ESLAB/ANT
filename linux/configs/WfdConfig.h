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

#ifndef __WFD_CONFIG_H__
#define __WFD_CONFIG_H__

/* WLAN device configuration */
#define DEFAULT_WFD_DEVICE_NAME "wlan0"

/* Server IP configuration */
#define DEFAULT_WFD_IP_ADDRESS "192.168.49.1"

/* DHCPD configuration */
#define WFD_DHCPD_LEASES_START_ADDRESS "192.168.49.20"
#define WFD_DHCPD_LEASES_END_ADDRESS "192.168.49.40"
#define WFD_DHCPD_MAX_LEASES 20
#define WFD_DHCPD_LEASE 864000
#define WFD_DHCPD_SUBNET_MASK "255.255.255.0"
#define WFD_DHCPD_BROADCAST_ADDRESS "192.168.49.255"
#define UDHCPD_CONFIG_PATH "dhcpd.conf"

#endif /* defined(__WFD_CONFIG_H__) */