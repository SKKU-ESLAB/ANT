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

#ifndef OPEL_DEBUG_LOG_H
#define OPEL_DEBUG_LOG_H

#include <stdarg.h>
#include <stdio.h>

//#define NDEBUG
#include <assert.h>

#ifndef likely
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#endif /* likely(x) */

#define FUNCTION_LOG 0

#define FW_NAME "CM"
#define _opel_log(format, fw, color, args...) do { __opel_log(format, fw, \
    __FILE__, __func__, color, __LINE__, ##args); } while(0)
#define _opel_func(format, fw, args...) do { __opel_func(format, fw, \
    __FILE__, __func__, __LINE__, ##args); } while(0)

#if FUNCTION_LOG < 1
#define OPEL_DBG_VERB(fmt, args...) do{ _opel_log(fmt, "[VERB] " FW_NAME " FW", 9, ##args); }while(0)
#else /* FUNCTION_LOG >= 1 */
#define OPEL_DBG_VERB(fmt, args...)
#endif /* FUNCTION_LOG < 1 */

#if FUNCTION_LOG < 2
#define OPEL_DBG_LOG(fmt, args...) do{ _opel_log(fmt, "[LOG] " FW_NAME " FW", 94, ##args); }while(0)
#else /* FUNCTION_LOG >= 2 */
#define OPEL_DBG_LOG(fmt, args...)
#endif /* FUNCTION_LOG < 2 */

#if FUNCTION_LOG < 3
#define OPEL_DBG_WARN(fmt, args...) do{ _opel_log(fmt, "[WARN] " FW_NAME " FW", 91, ##args); }while(0) 
#else /* FUNCTION_LOG >= 3 */
#define OPEL_DBG_WARN(fmt, args...)
#endif /* FUNCTION_LOG < 3 */

#if FUNCTION_LOG < 4
#define OPEL_DBG_ERR(fmt, args...) do{ _opel_log(fmt, "[ERR] " FW_NAME " FW", 101, ##args); }while(0)
#else /* FUNCTION_LOG >= 4 */
#define OPEL_DBG_ERR(fmt, args...)
#endif /* FUNCTION_LOG < 4 */

#define __OPEL_FUNCTION_ENTER__ do{ _opel_func("ENTER", FW_NAME" FW"); }while(0)
#define __OPEL_FUNCTION_EXIT__ do{ _opel_func("EXIT", FW_NAME" FW"); }while(0)

inline void __opel_log(const char *format, const char *fw, const char *fileName,
    const char *funcName, int color, int lineNo, ...){
  va_list ap;
  printf("\033[%dm%s:%s: [%s():%d]:  ", color, fw, fileName, funcName, \
      lineNo);
  va_start(ap, lineNo);
  vprintf(format, ap);
  va_end(ap);
  printf("\033[0m\n");
}

inline void __opel_func(const char *format, const char *fw, const char *fileName, 
    const char *funcName, int lineNo, ...){
  va_list ap;
  printf("\033[2m%s:%s:[%s():%d][", fw, fileName, funcName, lineNo); 
  va_start(ap, lineNo);
  vprintf(format, ap);
  va_end(ap);
  printf("]\033[0m\n");


}
#endif /* OPEL_DEBUG_LOG_H */
