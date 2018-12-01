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

/// @file gw_spm.cpp

/// @brief  This file contains the implementation of classes and its members
///         related to Gateway Sensor Policy Manager

#include "gw_spm.h"


GW_PolicyManager::GW_PolicyManager()
{}		
GW_PolicyManager::~GW_PolicyManager()
{}
int GW_PolicyManager::GetNetPolicy(IN int policyValue, OUT int &dtAcqType, OUT int &level)
{
	dtAcqType = get_dt_acq_net((policyValue >> DT_ACQ_TYPE_SHIFT));
	level = get_dt_acq_net_lev((policyValue >> DT_ACQ_LEVEL_SHIFT));

	return VSF_S_OK;
}
int GW_PolicyManager::GetRawPolicy(IN int policyValue, OUT int &dtAcqType, OUT int &level)
{
	dtAcqType = get_dt_acq_raw((policyValue >> DT_ACQ_TYPE_SHIFT));
	level = get_dt_acq_raw_lev((policyValue >> DT_ACQ_LEVEL_SHIFT));

	return VSF_S_OK;
}
int GW_PolicyManager::SetNetPolicy(IN int dtAcqType, IN int level, OUT int &policyValue)
{
	int result = dtAcqType << DT_ACQ_NET_TYPE_SHIFT;
	result = result << DT_ACQ_TYPE_SHIFT;
	result = result | ((level<<DT_ACQ_NET_VALUE_SHIFT)<<DT_ACQ_LEVEL_SHIFT);
	policyValue = result;
	gw_log("policyValue : %x %x %08x", dtAcqType, level, policyValue);

	return VSF_S_OK;
}
int GW_PolicyManager::SetRawPolicy(IN int dtAcqType, IN int level, OUT int &policyValue)
{
	int result = dtAcqType << DT_ACQ_RAW_TYPE_SHIFT;
	result = result << DT_ACQ_TYPE_SHIFT;
	result = result | ((level<<DT_ACQ_RAW_VALUE_SHIFT)<<DT_ACQ_LEVEL_SHIFT);
	policyValue = result;
	
	gw_log("policyValue : %x %x %08x", dtAcqType, level, policyValue);

	return VSF_S_OK;
}

int GW_PolicyManager::VerifyPolicy(IN int available, IN int policyValue, OUT bool &res)
{
	int dtAcqType, level, ret, retValue;
	int netAvailable, rawAvailable;

	netAvailable = (available >> DT_ACQ_NET_TYPE_SHIFT) & DT_ACQ_MASK;
	rawAvailable = (available >> DT_ACQ_RAW_TYPE_SHIFT) & DT_ACQ_MASK;
	

	ret = GetNetPolicy(policyValue, dtAcqType, level);

	retValue = dtAcqType & netAvailable;

	ret = GetRawPolicy(policyValue, dtAcqType, level);

	retValue |= dtAcqType & rawAvailable;

	if(retValue)
		res = true;
	else
		res = false;

	return ret;
}

