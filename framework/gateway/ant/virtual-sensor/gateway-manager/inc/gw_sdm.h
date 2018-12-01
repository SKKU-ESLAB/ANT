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

/// @file gw_sdm.h

/// @brief  This file contains the declaration of classes and its members
///         related to Gateway Sensor Device Manager

#ifndef _GW_SDM_H_
#define _GW_SDM_H_

#include <vector>
#include <string>
#include <map>
#include <mutex>

#include "OCPlatform.h"
#include "OCApi.h"

#include "gw_lite_ss.h"
#include "gw_util.h"
#include "gw_collection.h"
#include "sql_wrapper.h"


using namespace OC;

void onGWFoundResource(std::shared_ptr <OCResource> resource);
void onGWFirstGet(const HeaderOptions& opt, const OCRepresentation &rep, const int eCode);
void onGWGet(const HeaderOptions& opt, const OCRepresentation &rep, const int eCode);
void onGWGetSync(const HeaderOptions& opt, const OCRepresentation &rep, const int eCode);
void onGWPut(const HeaderOptions& opt, const OCRepresentation &rep, const int eCode);
void onGWObserve(const HeaderOptions headerOptions, const OCRepresentation& rep,
                    const int& eCode, const int& sequenceNumber);



class GW_SensorArbiter
{
	private:
		GW_SensorArbiter();
		
		static GW_SensorArbiter* instance;
		pthread_mutex_t syncGetMutex;
//		std::map<std::string, std::vector<std::shared_ptr<OCResourceRequest>> reqs;
		
	public:
		~GW_SensorArbiter();
		static GW_SensorArbiter* getInstance();

		void getSync(GW_Resource &gw_res, std::string suri, std::shared_ptr<OCResourceRequest> request);
		void responseSyncGet(std::string suri);
		void get(GW_Resource &gw_res);
		void firstGet(GW_Resource &gw_res);
		void firstGet(std::string suri);
		void put(GW_Resource &gw_res, OCRepresentation &oc_rep);
		void put(GW_Resource &gw_res, OCRepresentation &oc_rep, QueryParamsMap &qpm);

		void observe(GW_Resource &gw_res, QueryParamsMap &queryParamsMap);
		
		
		
};


class GW_SensorNotifier
{
	private:
		GW_SensorNotifier();

		static GW_SensorNotifier *instance;
		std::vector <GW_Resource *> interestingResources;
		std::mutex intResMutex;
		std::map<std::string, OCObservationId> hostKeyStore;
		OCResourceHandle *gw_handle;
	public:
		static GW_SensorNotifier *getInstance();
		~GW_SensorNotifier();
		void setResourceHandle(OCResourceHandle *gwHandle);
		void addInterestingResources(IN GW_Resource *gw_res);

		void setHostKey(std::string key, OCObservationId obsId);
		OCObservationId getHostKey(std::string key);
		void notification();
};

class GW_SensorDeviceManager
{

	private:
		GW_SensorDeviceManager();
		GW_DeviceList deviceList;
		GW_SensorArbiter *sensorArbiter;
		static GW_SensorDeviceManager *instance;
		
	public:

		static GW_SensorDeviceManager *getInstance();
		
		int GetDeviceList(OUT GW_DeviceList* deviceList, OUT int eCode);

		/* ret = err, if resource is not found, resource is null */
		int GetResource (IN const std::string SUri, OUT GW_Resource **resource);
		int GetResourceXML(IN const std::string SUri, OUT std::string &xmlResult);

		int AddResource(IN const std::string SUri, IN std::shared_ptr <OCResource> oc_res, OUT GW_Resource **resource);

		int GetDeviceListXML(OUT std::string &str);
};
#endif
