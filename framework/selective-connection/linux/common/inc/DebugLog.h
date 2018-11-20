/* Copyright 2017-2018 All Rights Reserved.
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *  Eunsoo Park (esevan.park@gmail.com)
 *  Injung Hwang (sinban04@gmail.com)
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

#ifndef __DEBUG_LOG_H__
#define __DEBUG_LOG_H__

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef likely
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif /* likely(x) */

#ifndef LOG_LEVEL
#define LOG_LEVEL 1
#endif

#define _log(format, color, args...)                                           \
  do {                                                                         \
    __log(format, __FILE__, __func__, color, __LINE__, ##args);                \
  } while (0)
#define _func(format, args...)                                                 \
  do {                                                                         \
    __func(format, __FILE__, __func__, __LINE__, ##args);                      \
  } while (0)

#if LOG_LEVEL < 1
#define LOG_DEBUG(fmt, args...)                                                \
  do {                                                                         \
    _log(fmt, 0, 9, ##args);                                                      \
  } while (0)
#else /* LOG_LEVEL >= 1 */
#define LOG_DEBUG(fmt, args...)
#endif /* LOG_LEVEL < 1 */

#if LOG_LEVEL < 2
#define LOG_VERB(fmt, args...)                                                 \
  do {                                                                         \
    _log(fmt, 0, 94, ##args);                                                     \
  } while (0)
#else /* LOG_LEVEL >= 2 */
#define LOG_VERB(fmt, args...)
#endif /* LOG_LEVEL < 2 */

#if LOG_LEVEL < 3
#define LOG_WARN(fmt, args...)                                                 \
  do {                                                                         \
    _log(fmt, 0, 91, ##args);                                                     \
  } while (0)
#else /* LOG_LEVEL >= 3 */
#define LOG_WARN(fmt, args...)
#endif /* LOG_LEVEL < 3 */

#if LOG_LEVEL < 4
#define LOG_ERR(fmt, args...)                                                  \
  do {                                                                         \
    _log(fmt, 101, 0, ##args);                                                    \
  } while (0)
#else /* LOG_LEVEL >= 4 */
#define LOG_ERR(fmt, args...)
#endif /* LOG_LEVEL < 4 */

#define LOG_IMP(fmt, args...)                                                  \
  do {                                                                         \
    _log(fmt, 104, 35, ##args);                                                    \
  } while (0)

#define __FUNCTION_ENTER__                                                     \
  do {                                                                         \
    _func("ENTER");                                                            \
  } while (0)
#define __FUNCTION_EXIT__                                                      \
  do {                                                                         \
    _func("EXIT");                                                             \
  } while (0)

void __log(const char *format, const char *fileName, const char *funcName,
           int bg_color, int font_color, int lineNo, ...);

void __func(const char *format, const char *fileName, const char *funcName,
            int lineNo, ...);

#define LOG_THREAD_LAUNCH(thread_name)                                         \
  do {                                                                         \
    LOG_VERB("Thread(%s; %d) Launch", thread_name, getpid());                  \
  } while (0)

#define LOG_ADAPTER_THREAD_LAUNCH(adapter_name, thread_name)                   \
  do {                                                                         \
    LOG_VERB("Thread(%s-%s; %d) Launch", adapter_name, thread_name, getpid()); \
  } while (0)

#define LOG_THREAD_FINISH(thread_name)                                         \
  do {                                                                         \
    LOG_VERB("Thread(%s; %d) Finish", thread_name, getpid());                  \
  } while (0)

#define LOG_ADAPTER_THREAD_FINISH(adapter_name, thread_name)                   \
  do {                                                                         \
    LOG_VERB("Thread(%s-%s; %d) Finish", adapter_name, thread_name, getpid()); \
  } while (0)

#define LOG_THREAD_FAIL(thread_name)                                           \
  do {                                                                         \
    LOG_ERR("Thread(%s; %d) Failed", thread_name, getpid());                   \
  } while (0)

#define LOG_ADAPTER_THREAD_FAIL(adapter_name, thread_name)                     \
  do {                                                                         \
    LOG_ERR("Thread(%s-%s; %d) Failed", adapter_name, thread_name, getpid());  \
  } while (0)

#endif /* !defined(__DEBUG_LOG_H__) */