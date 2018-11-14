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
#include "../inc/WfdDevice.h"

#include "../../common/inc/ChildProcess.h"
#include "../../common/inc/Counter.h"

#include "../../configs/WfdConfig.h"

#include <mutex>
#include <thread>

#include <stdio.h>

using namespace sc;

bool WfdDevice::turn_on_impl(void)
{
  // char buf[512];
  // char const *const params[] = {"ifconfig", DEFAULT_WFD_DEVICE_NAME, "up",
  // NULL};

  // int res = ChildProcess::run(IFCONFIG_PATH, params, buf, 512, true);
  // return (res >= 0);
  return true;
}

bool WfdDevice::turn_off_impl(void)
{
  // char buf[512];
  // char const *const params[] = {"ifconfig", DEFAULT_WFD_DEVICE_NAME, "down",
  // NULL};

  // int res = ChildProcess::run(IFCONFIG_PATH, params, buf, 512, true);
  // return (res >= 0);
  return true;
}
