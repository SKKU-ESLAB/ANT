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

/// @file gw_interface.h

/// @brief  This file contains the declaration of classes and its members
///         related to Gatewayinterface 

#ifndef _GW_INTERFACE_H_
#define _GW_INTERFACE_H_

#include <pthread.h>
#include <mutex>
#include <condition_variable>

#include "gw_util.h"
#include "gw_sdm.h"
#include "OCPlatform.h"
#include "OCApi.h"

using namespace OC;

enum GW_CMD
{
	GW_CMD_GET_DEVICE_LIST,
	GW_CMD_GET_SENSOR_INFO,
	GW_CMD_GET_SENSOR_DATA_EPOCH,
	GW_CMD_START_OBSERVING,
	GW_CMD_STOP_OBSERVING,
	GW_CMD_PUT_ACTUATOR_DATA,
	GW_CMD_VERIFY_SS_LOGIC,
	GW_CMD_REG_SS,
	GW_CMD_UNREG_SS,
	GW_CMD_CONST_POLICY,
	GW_CMD_VERIFY_POLICY,
	GW_CMD_REG_POLICY,
	GW_CMD_UNREG_POLICY,
	GW_CMD_CONNECT_POLICY,
	GW_CMD_DISCON_POLICY,
	GW_CMD_APPLY_POLICY,
	GW_CMD_ADD_POLICY
};

void* notifierWrapparFunc(void *);

class GW_Interface
{
	private:
		std::string gw_uri;
		std::string gw_name;
		std::string gw_cmd;
		GW_SensorDeviceManager* sdm;
		GW_SensorNotifier* snf;
		static GW_Interface *instance;

		
		OCResourceHandle gw_resourceHandle;
		OCRepresentation gw_rep;
		GW_Interface();
		
	public:
		static GW_Interface *getInstance();
		~GW_Interface();

		void addType(const std::string& type) const;

	    void addInterface(const std::string& interface) const;
		void createResource();
		OCEntityHandlerResult gw_entityHandler(std::shared_ptr<OCResourceRequest> request);
};

void* notifierWrapparFunc(void *);





#endif

