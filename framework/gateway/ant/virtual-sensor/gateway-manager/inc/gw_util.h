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

#ifndef _GW_UTIL_H_
#define _GW_UTIL_H_

#include <cstdio>
#include <cstdarg>
#include <queue>
#include <list>

#define IN
#define OUT

#define gw_log(format, arg...) do{_gw_log((format), __FILE__, __func__, __LINE__, ##arg);}while(0)
#define itoa10(x, str) do{ sprintf(str, "%d", x); }while(0)

inline void _gw_log(const char *format, const char *fileName, const char *funcName, const int lineNo ...)
{
	va_list ap;
	
	printf("GW:%s(%d) > ", funcName, lineNo);
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


enum { 
	ASYNC_GET = 0,
	SYNC_GET = 1
};

typedef void* BasicPointer;

class Message
{
	public:
	private:
};
#if 0
/*
class Message_Queue
{
	typedef queue<Message *, list<Message *>> MsgQueType;
	private:
	MsgQueType m_msgQueue;
	public:
	void add(Message *pMsg)
	{
		m_msgQueue.push(pMsg);
	}

	Message *Remove()
	{
		Message *pMsg = NULL;

		if(!m_msgQueue.empty())
		{
			pMsg = m_msgQueue.front();

			m_msgQueue.pop();
		}

		return pMsg;
	}

	int GetLength() const
	{
		return m_msgQueue.size();
	}
};
*/
#endif

#endif
