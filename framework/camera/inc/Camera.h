/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Hayun Lee<lhy920806@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <vector>
#include <list>
#include <gst/gst.h>
#include <dbus/dbus.h>
#include <glib.h>
#include <dbus/dbus-protocol.h>
#include <dbus/dbus-glib-lowlevel.h>
#include <dbus/dbus-glib.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <stdarg.h>
#include <stdio.h>

#define CAM "CAMERA"

// Debug Option
#define CONFIG_LOG_DBG      1
#define CONFIG_LOG_WARN     1
#define CONFIG_LOG_ERR      1

#define _ant_log(format, fw, color, args...) do { __ant_log(format, fw, \
    __FILE__, __func__, color, __LINE__, ##args); } while(0)

#if CONFIG_LOG_DBG
#define ANT_LOG_DBG(fw, fmt, args...) do{ _ant_log(fmt, "[LOG] CAMERA FW", 94, ##args); }while(0)
#else
#define ANT_LOG_DBG(fw, fmt, args...)
#endif

#if CONFIG_LOG_WARN
#define ANT_LOG_WARN(fw, fmt, args...) do{ _ant_log(fmt, "[WARN] CAMERA FW", 91, ##args); }while(0)
#else
#define ANT_LOG_WARN(fw, fmt, args...)
#endif

#if CONFIG_LOG_ERR
#define ANT_LOG_ERR(fw, fmt, args...) do{ _ant_log(fmt, "[ERR] CAMERA FW", 101, ##args); }while(0)
#else
#define ANT_LOG_ERR(fw, fmt, args...)
#endif

inline void __ant_log(const char *format, const char *fw, const char *fileName,
    const char *funcName, int color, int lineNo, ...)
{
  va_list ap;
  printf("\033[%dm%s:%s: [%s():%d]:  ", color, fw, fileName, funcName, lineNo);
  va_start(ap, lineNo);
  vprintf(format, ap);
  va_end(ap);
  printf("\033[0m\n");
}

#endif /* CAMERA_H */
