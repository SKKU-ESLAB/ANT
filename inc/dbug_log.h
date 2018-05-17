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

#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H

#include <stdarg.h>
#include <stdio.h>

//#define NDEBUG
#include <assert.h>

#ifndef likely
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#endif /* likely(x) */

#ifndef LOG_LEVEL
#define LOG_LEVEL 0
#endif

#define FW_NAME "CM"
#define _log(format, fw, color, args...) do { __log(format, fw, \
    __FILE__, __func__, color, __LINE__, ##args); } while(0)
#define _func(format, fw, args...) do { __func(format, fw, \
    __FILE__, __func__, __LINE__, ##args); } while(0)

#if LOG_LEVEL < 1
#define LOG_DEBUG(fmt, args...) do{ _log(fmt, "[VERB] " FW_NAME " FW", 9, ##args); }while(0)
#else /* LOG_LEVEL >= 1 */
#define LOG_DEBUG(fmt, args...)
#endif /* LOG_LEVEL < 1 */

#if LOG_LEVEL < 2
#define LOG_VERB(fmt, args...) do{ _log(fmt, "[LOG] " FW_NAME " FW", 94, ##args); }while(0)
#else /* LOG_LEVEL >= 2 */
#define LOG_VERB(fmt, args...)
#endif /* LOG_LEVEL < 2 */

#if LOG_LEVEL < 3
#define LOG_WARN(fmt, args...) do{ _log(fmt, "[WARN] " FW_NAME " FW", 91, ##args); }while(0) 
#else /* LOG_LEVEL >= 3 */
#define LOG_WARN(fmt, args...)
#endif /* LOG_LEVEL < 3 */

#if LOG_LEVEL < 4
#define LOG_ERR(fmt, args...) do{ _log(fmt, "[ERR] " FW_NAME " FW", 101, ##args); }while(0)
#else /* LOG_LEVEL >= 4 */
#define LOG_ERR(fmt, args...)
#endif /* LOG_LEVEL < 4 */

#define __FUNCTION_ENTER__ do{ _func("ENTER", FW_NAME" FW"); }while(0)
#define __FUNCTION_EXIT__ do{ _func("EXIT", FW_NAME" FW"); }while(0)

inline void __log(const char *format, const char *fw, const char *fileName,
    const char *funcName, int color, int lineNo, ...){
  va_list ap;
  printf("\033[%dm%s:%s: [%s():%d]:  ", color, fw, fileName, funcName, \
      lineNo);
  va_start(ap, lineNo);
  vprintf(format, ap);
  va_end(ap);
  printf("\033[0m\n");
}

inline void __func(const char *format, const char *fw, const char *fileName, 
    const char *funcName, int lineNo, ...){
  va_list ap;
  printf("\033[2m%s:%s:[%s():%d][", fw, fileName, funcName, lineNo); 
  va_start(ap, lineNo);
  vprintf(format, ap);
  va_end(ap);
  printf("]\033[0m\n");


}
#endif /* DEBUG_LOG_H */
