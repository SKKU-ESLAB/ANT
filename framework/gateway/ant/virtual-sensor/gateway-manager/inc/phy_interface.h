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

/// @file phy_interface.h

/// @brief  This file contains the declaration of classes and its members
///         related to physical sensor interface

#ifndef _PHY_INTERFACE_H_
#define _PHY_INTERFACE_H_

#include <string>
#include <cstdarg>


#define DT_ACQ_TYPE_SHIFT 16
#define DT_ACQ_LEVEL_SHIFT 0

#define DT_ACQ_NET_TYPE_SHIFT 4
#define DT_ACQ_RAW_TYPE_SHIFT 0

#define DT_ACQ_NET_VALUE_SHIFT 8
#define DT_ACQ_RAW_VALUE_SHIFT 0

#define DT_ACQ_MASK 0x0F

#define DT_ACQ_POLLING_FLAG 0x01
#define DT_ACQ_BATCHING_FLAG 0x02
#define DT_ACQ_MODEL_FLAG 0x04

#define get_dt_acq_raw(x) (x&DT_ACQ_MASK)
#define get_dt_acq_net(x) ((x>>DT_ACQ_NET_TYPE_SHIFT)&DT_ACQ_MASK)

#define is_dt_acq_polling(x) ((x)&DT_ACQ_POLLING_FLAG)
#define is_dt_acq_batching(x) ((x)&DT_ACQ_BATCHING_FLAG)
#define is_dt_acq_model(x) ((x)&DT_ACQ_MODEL_FLAG)

#define get_dt_acq_net_lev(x) (((x)>>DT_ACQ_NET_VALUE_SHIFT) & 0xFF)
#define get_dt_acq_raw_lev(x) (((x)>>DT_ACQ_RAW_VALUE_SHIFT) & 0xFF)

#define phy_log(format, arg...) do{_phy_log((format), __FILE__, __func__, __LINE__, ##arg);}while(0)

#ifndef OUT
#define OUT
#endif

#ifndef IN
#define IN
#endif

namespace PH = std::placeholders;
using namespace OC;
inline void _phy_log(const char *format, const char *fileName, const char *funcName, const int lineNo ...)
{
	va_list ap;
	
	printf("PHY:%s(%d) > ", funcName, lineNo);
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
enum DT_ACQ_TYPE
{
	DT_ACQ_POLLING = 1,
	DT_ACQ_BATCHING = 2,
	DT_ACQ_MODEL = 4
};
enum POL_TYPE
{
	NET_POL_TYPE,
	RAW_POL_TYPE
};



class PHY_Basic_Callback
{
	public:
		virtual void get(OUT OCRepresentation& rep) = 0;
		virtual void put(IN OCRepresentation& rep) = 0;
		virtual bool pollingAction(int polType, int level) = 0;
		virtual bool modelAction(int polType, int level) = 0;
		virtual bool batchingAction(int polType, int level) = 0;
		virtual bool defaultAction(int polType) = 0;
};

class PHY_Interface
{
	private:
		static PHY_Interface *instance;
		int prevPolicy;
		int policy;
		int availablePolicy;
		unsigned int gObservation;
		OCResourceHandle m_resourceHandle;
		ObservationIds m_interestedObservers;
		std::string m_resourceUri;
		std::shared_ptr<PHY_Basic_Callback> myCallback;

		void *notificationFunc(void *param);
		PHY_Interface(void);
		static void *notificationFuncHelper(void *phyInt);

		int GetNetPolicy(IN int policy, OUT int &dtAcqType, OUT int &level);
		int GetRawPolicy(IN int policy, OUT int &dtAcqType, OUT int &level);
		int GetNetPolicy(OUT int &dtAcqType, OUT int &level);
		int GetRawPolicy(OUT int &dtAcqType, OUT int &level);
		int SetNetPolicy(IN int dtAcqType, IN int level);
		int SetRawPolicy(IN int dtAcqType, IN int level);
		int getNetPolType();
		int getNetPolLevel();
		int getRawPolType();
		int getRawPolLevel();
		bool verifyPolicy(IN int available, IN int policyValue);
		OCEntityHandlerResult entityHandler(std::shared_ptr<OCResourceRequest> request);
		
	public:
		static PHY_Interface *getInstance();
		
		OCStackResult createResource(std::string resourceUri, std::string resourceTypeName, std::string resourceInterface, 
			int availablePolicy, int isSecure, std::shared_ptr<PHY_Basic_Callback> callback);
		static void SetNetPolicy(IN int dtAcqType, IN int level, OUT int& policy);
		static void SetRawPolicy(IN int dtAcqType, IN int level, OUT int& policy);

		static void SetPolicy(IN int rawDtAcqType, IN int rawLevel, IN int netDtAcqType, IN int netLevel, OUT int &policy);
};


#endif
