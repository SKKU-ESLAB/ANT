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

#ifndef __TMP_CONTROL_H__
#define __TMP_CONTROL_H__

#include <stdio.h>
#include <stdarg.h>
#ifndef IN
#define IN
#define OUT
#endif //IN

#define tmpc_log(format, args...) do { _tmpc_log(format, __FILE__, __func__, __LINE__, ##args); } while(0)
#define __TMP_ENTER__ tmpc_log( "Enter=====" )
#define __TMP_EXIT__ tmpc_log( "=====Exit" )


#ifndef null
#define null (void *)0
#endif

#define RES_NAME_LEN 256

#define __TMPC_NO_LOG

#ifndef __TMPC_NO_LOG
inline void _tmpc_log(const char *format, const char *fileName, const char *funcName, int lineNo, ...){
	va_list ap;

	printf("tmp-Control(%s):%s(%d)\t> ", funcName, fileName,lineNo);
	va_start(ap, lineNo);
	vprintf(format, ap);
	va_end(ap);
	printf("\n");
}
#else //NO_LOG

inline void _tmpc_log(const char *format, const char *fileName, const char *funcName, int lineNo, ...){
	
}

#endif // NO_LOG

int tmpc_post(char *res_name, char *res_val, size_t count);
int tmpc_get(char *res_name, OUT char *val, size_t count);
int tmpc_put(char *res_name, char *res_val, size_t count);
int tmpc_del(char *res_name);
int tmpc_subscribe(char *res_name, void (*callback)(void *));

#endif //__TMP_CONTROL_H__
