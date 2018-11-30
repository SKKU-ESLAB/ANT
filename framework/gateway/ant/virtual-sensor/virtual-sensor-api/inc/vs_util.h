//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

/// @file vs_util.h

/// @brief  This file contains the declaration of classes and its members
///         related to virtual sensor framework-utility

#ifndef _vs_util_H_
#define _vs_util_H_

#include <cstdio>
#include <cstdarg>

#define IN
#define OUT

#define vs_log(format, args...) do { _vs_log(format, __FILE__, __func__, __LINE__, ##args); } while(0)


inline void _vs_log(const char *format, const char *fileName, const char *funcName, int lineNo, ...){
	va_list ap;
	
	printf("VSF:%s(%d) > ", funcName, lineNo);
	va_start(ap, lineNo);
	vprintf(format, ap);
	va_end(ap);
	printf("\n");

}

enum VSFRESULT
{
	VSF_S_OK
	, VSF_S_FALSE
	, VSF_E_POINTER
	, VSF_E_OUTOFMEMORY
	, VSF_E_FAIL
	, VSF_E_NOINTERFACE
	, VSF_E_NOTIMPL
	, VSF_E_TYPO
};




#endif
