/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Eunsoo Park<esevan.park@gmail.com>
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

#ifndef __CMFW_LOG_H__
#define __CMFW_LOG_H__

#include <stdio.h>
#include <stdarg.h>

#define CommLog(format, args...) do { _CommLog(format, __FILE__, __func__, __LINE__, ##args); } while(0)
#define __ENTER__ CommLog( "Enter=================" )
#define __EXIT__ CommLog( "==================Exit" )

#ifndef IN
#define IN
#define OUT
#endif

#ifndef null
#define null (void *)0
#endif

//#define NO_LOG


#ifndef NO_LOG
inline void _CommLog(const char *format, const char *fileName, const char *funcName, int lineNo, ...){
	va_list ap;

	printf("CMFW(%s):%s(%d)\t> ", funcName, fileName,lineNo);
	va_start(ap, lineNo);
	vprintf(format, ap);
	va_end(ap);
	printf("\n");
}
#else // NO_LOG
inline void _CommLog(const char *format, const char *fileName, const char *funcName, int lineNo, ...){
	va_list ap;
}
#endif // NO_LOG

#endif // __CMFW_LOG_H__ // 2016-5-19 Eunsoo Park
