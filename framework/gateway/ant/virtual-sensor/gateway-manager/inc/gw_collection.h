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

/// @file gw_collection.h

/// @brief  This file contains the declaration of classes and its members
///         related to OIC Resource Model

#ifndef _GW_COLLECTION_H_
#define _GW_COLLECTION_H_

#include <string>
#include <vector>
#include <pthread.h>

//#include <boost/thread.hpp>
#include "OCApi.h"
#include "OCPlatform.h"

#include "gw_util.h"
#include "gw_spm.h"
#include "sql_wrapper.h"

using namespace OC;


/*
NullType, // Note: this handles the null-type and must match the above static const
        int,
        double,
        bool,
        std::string,
        OC::OCRepresentation,

        // Sequences:
        std::vector<int>,
        std::vector<double>,
        std::vector<bool>,
        std::vector<std::string>,
        std::vector<OC::OCRepresentation>,

        // Nested sequences:
        std::vector<std::vector<int>>,
        std::vector<std::vector<std::vector<int>>>,

        std::vector<std::vector<double>>,
        std::vector<std::vector<std::vector<double>>>,

        std::vector<std::vector<bool>>,
        std::vector<std::vector<std::vector<bool>>>,

        std::vector<std::vector<std::string>>,
        std::vector<std::vector<std::vector<std::string>>>,

        std::vector<std::vector<OC::OCRepresentation>>,
        std::vector<std::vector<std::vector<OC::OCRepresentation>>>


class gw_visitor : public */

class GW_Resource//: public OCResource
{
	private:
		bool initialized;
		std::string resourceUri;
		//OCResourceHandle m_resourceHandle;
		//OCResourceProperty props;
		//std::vector<std::string> resTypes;
		ObservationIds obsIds;		
		bool observated;
		bool obsDirty;
		bool ssm_mark;
		bool found;
		pthread_mutex_t res_mtx;
		pthread_mutex_t put_res_mtx;
		
		std::string name;

		int availablePolicy;
		std::vector<int> policies;
		int currentPolicy;
		std::map<std::string, std::string> attributes;
		pthread_mutex_t attrMutex;
		std::shared_ptr< OCResource > oc_res;
		
	public:
		GW_Resource();

		GW_Resource& operator= (const GW_Resource &resource);

		std::map<std::string, std::string> getAttribute();
		void lockGetMutex();
		void unlockGetMutex();
		void lockPutMutex();
		void unlockPutMutex();
		bool operator== (const std::string &uri);
		bool operator== (const GW_Resource &resource2);
		void observing();
		bool is_observated();
			
		void observationDirty();
		void observationClean();

		bool needNotification();
		int numberOfObs();

		ObservationIds& getObsIds();
		void addObsId(OCObservationId obsId);
		void setFound();
		bool is_found();
		bool is_initialized();

		std::shared_ptr <OCResource>& getOCRes();
		void makeXML(OUT std::string& xmlResult);
		void makeLightXML(OUT std::string& xmlResult);
		
		int create(std::string uri, std::shared_ptr <OCResource> &oc_res);
		std::string getUri(void);
		int getRep(OUT OCRepresentation &oc_rep);

		void setAttribute();
		
		int setAttribute(IN std::string attr_name, IN std::string attr_value);

		int addPolicy(IN int netType, IN int netLevel, IN int rawType, IN int rawLevel);
		int verifyPolicy(IN int netType, IN int netLevel, IN int rawType, IN int rawLevel, OUT bool &verified);
		bool verifyPolicy(IN int policyValue);
		void setAvaliablePolicy(int av_pol);
		void setCurPolicy(int cur_pol);
};

class GW_Collection
{
	private:
		bool isResource;
		bool isCollection;
		std::string name;
		GW_Resource resource;
		std::vector<GW_Collection> children;
		
	public:
		GW_Collection();

		GW_Collection& operator=(const GW_Collection& operand);
		void makeXML(OUT std::string& xmlResult);

		int createResource(IN std::string SUri, std::shared_ptr < OCResource > &oc_res);
		int createCollection(IN std::string name);
		void setName(IN std::string name);
		void setResource(IN GW_Resource &gwResource);
		
		void addCollection(IN GW_Collection &gwCollection);
		
		int getSensor(IN const std::string SUri, IN std::string tokenizedString, GW_Resource **resource);
		int addSensor(IN const std::string SUri, IN std::string tokenizedString, IN std::shared_ptr <OCResource> &oc_res, OUT GW_Resource **resource);
};

class GW_DeviceList
{
	public:
		GW_DeviceList();
		void makeXML(OUT std::string& xmlResult);
		int getSensor(IN const std::string SUri, OUT GW_Resource **resource);
		int addSensor(IN const std::string SUri, IN std::shared_ptr <OCResource> &oc_res, OUT GW_Resource **resource);
		//GW_Sensor 
	private:
		bool is_initialized;
		pthread_mutex_t deviceTreeMtx;
		GW_Collection root;
};

#endif
