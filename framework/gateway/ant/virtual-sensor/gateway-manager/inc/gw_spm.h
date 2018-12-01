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

/// @file gw_spm.h

/// @brief  This file contains the declaration of classes and its members
///         related to Gateway Sensor Policy Manager

#ifndef _GW_SPM_H_
#define _GW_SPM_H_

#include "gw_util.h"
//#include "gw_dbm.h"
//#include "OCPlatform.h"
//#include "OCApi.h"
#include <vector>
#include <string>
#include <map>
//using namespace OC;


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

enum DT_ACQ_TYPE
{
	DT_ACQ_POLLING,
	DT_ACQ_BATCHING,
	DT_ACQ_MODEL
};

class GW_PolicyManager
{
	public:
		GW_PolicyManager();
		~GW_PolicyManager();
		static int GetNetPolicy(IN int policyValue, OUT int &dtAcqType, OUT int &level);
		static int GetRawPolicy(IN int policyValue, OUT int &dtAcqType, OUT int &level);
		static int SetNetPolicy(IN int dtAcqType, IN int level, OUT int &policyValue);
		static int SetRawPolicy(IN int dtAcqType, IN int level, OUT int &policyValue);
		static int VerifyPolicy(IN int available, IN int policyValue, OUT bool &res);
};

	



#endif

