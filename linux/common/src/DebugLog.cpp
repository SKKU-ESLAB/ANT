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

#include "../inc/DebugLog.h"

#include "../../configs/ExpConfig.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void __log(const char *format, const char *fileName, const char *funcName,
           int bg_color, int font_color, int lineNo, ...) {
  va_list ap;
#if defined(LOG_FILE_NAME) && defined(LOG_FUNC_NAME)
  printf("\033[%d;%dm%s:%d (%s())  ", bg_color, font_color, fileName, lineNo,
         funcName);
#elif defined(LOG_FILE_NAME)
  printf("\033[%d;%dm%s:%d  ", bg_color, font_color, fileName, lineNo);
#elif defined(LOG_FUNC_NAME)
  printf("\033[%d;%dm(%s())  ", bg_color, font_color, funcName);
#else
  printf("\033[%d;%dm", bg_color, font_color);
#endif
  va_start(ap, lineNo);
  vprintf(format, ap);
  va_end(ap);
  printf("\033[0;0m\n");
}

void __func(const char *format, const char *fileName, const char *funcName,
            int lineNo, ...) {
  va_list ap;
#if defined(LOG_FILE_NAME) && defined(LOG_FUNC_NAME)
  printf("\033[%dm%s:%d (%s())  ", 2, fileName, lineNo, funcName);
#elif defined(LOG_FILE_NAME)
  printf("\033[%dm%s:%d  ", 2, fileName, lineNo);
#elif defined(LOG_FUNC_NAME)
  printf("\033[%dm(%s())  ", 2, funcName);
#else
  printf("\033[%dm", 2);
#endif
  va_start(ap, lineNo);
  vprintf(format, ap);
  va_end(ap);
  printf("]\033[0m\n");
}