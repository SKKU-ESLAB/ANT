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

/// @brief  This file contains the implementation of classes and its members
///         related to Gateway Sensor Device Manager

#include "gw_sdm.h"

GW_SensorArbiter *GW_SensorArbiter::instance = NULL;
GW_SensorDeviceManager *GW_SensorDeviceManager::instance = NULL;
GW_SensorNotifier *GW_SensorNotifier::instance = NULL;



GW_SensorArbiter::GW_SensorArbiter()
{
	syncGetMutex = PTHREAD_MUTEX_INITIALIZER;
	//req = NULL;
}

GW_SensorArbiter::~GW_SensorArbiter()
{}

void GW_SensorArbiter::getSync(GW_Resource &gw_res, std::string suri, std::shared_ptr<OCResourceRequest> request)
{
	/*
	std::map<std::string, std::vector<std::shared_ptr<OCResourceRequest>>::iterator it;
	if((it = reqs.find(suri)) == reqs.end())
	{
		reqs.insert(std::pair<std::string, \
			std::vector<std::shared_ptr<OCResourceRequest>>(suri, std::vector<std::shared_ptr<OCResourceRequest>());
		if((it = reqs.find(suri)) ==reqs.end())
		{
			gw_log("This should not happened");
		}				
	}

	(it->second).push_back(request);
	
	std::shared_ptr <OCResource> oc_res = gw_res.getOCRes();
	if(oc_res == NULL)
	{
	//	gw_log("OC Resource == NULL");
	}
	else
	{
//				gw_log("GET %s", gw_res.getUri().c_str());
		//gw_res.lockGetMutex();
		oc_res->get(QueryParamsMap(), &onGWGetSync);
	}
	*/
}
void GW_SensorArbiter::responseSyncGet(std::string suri)
{
	/*
	OCRepresentation rep_result;
	std::string resXML;
	std::map<std::string, std::vector<std::shared_ptr<OCResourceRequest>>::iterator it;
	ret = sdm->GetResourceXML(suri, resXML);
	rep_result.setValue("RESULT", resXML);

	if((it = reqs.find(suri)) == reqs.end())
	{
		gw_log("This should not be happened");
	}
	else
	{
		std::shared_ptr<OCResourceRequest> request = (it->second)[(it->second).size()-1];
		auto pResponse = std::make_shared<OC::OCResourceResponse>();
     	pResponse->setRequestHandle(request->getRequestHandle());
        pResponse->setResourceHandle(request->getResourceHandle());
		pResponse->setErrorCode(200);
        pResponse->setResponseResult(OC_EH_OK);
	  
        pResponse->setResourceRepresentation(rep_result);
        if(OC_STACK_OK == OCPlatform::sendResponse(pResponse))
        {
          ehResult = OC_EH_OK;
        }
		(it->second).pop_back();
	}
	if(VSF_S_OK != ret)
		gw_log("Failed");
		*/
}
void GW_SensorArbiter::get(GW_Resource &gw_res)
{
	std::shared_ptr <OCResource> oc_res = gw_res.getOCRes();
	if(oc_res == NULL)
	{
	//	gw_log("OC Resource == NULL");
	}
	else
	{
//				gw_log("GET %s", gw_res.getUri().c_str());
		//gw_res.lockGetMutex();
		oc_res->get(QueryParamsMap(), &onGWGet);
	}
}
void GW_SensorArbiter::firstGet(GW_Resource &gw_res)
{
	std::shared_ptr <OCResource> oc_res = gw_res.getOCRes();
	if(oc_res == NULL)
	{
		gw_log("No OCResource in GWResource");
	}
	else
	{
		QueryParamsMap qpm;
		qpm.insert(std::pair<std::string, std::string>("AV_POL", ""));
		//gw_res.lockGetMutex();
		oc_res->get(qpm, &onGWFirstGet);
	}
}

void GW_SensorArbiter::firstGet(std::string suri)
{
    GW_Resource *gw_res = NULL;
    GW_SensorDeviceManager::getInstance()->GetResource(suri, &gw_res);
    if(gw_res == NULL)
    {
        gw_log("No GW_Resource found %s", suri.c_str());
        return;
    }
	std::shared_ptr <OCResource> oc_res = gw_res->getOCRes();
	if(oc_res == NULL)
	{
		gw_log("No OCResource in GWResource");
	}
	else
	{
		QueryParamsMap qpm;
		qpm.insert(std::pair<std::string, std::string>("AV_POL", ""));
		//gw_res.lockGetMutex();
		oc_res->get(qpm, &onGWFirstGet);
	}
}

void GW_SensorArbiter::put(GW_Resource &gw_res, OCRepresentation &oc_rep)
{
	std::shared_ptr <OCResource> oc_res = gw_res.getOCRes();
	if(oc_res == NULL)
		gw_log("OC Resource == NULL");
	else{
		gw_log("put:%s", gw_res.getUri().c_str());
		//gw_res.lockPutMutex();
		oc_res->put(oc_rep, QueryParamsMap(), &onGWPut);
	}
}

void GW_SensorArbiter::put(GW_Resource &gw_res, OCRepresentation &oc_rep, QueryParamsMap &qpm)
{
	std::shared_ptr <OCResource> oc_res = gw_res.getOCRes();
	if(oc_res == NULL)
		gw_log("OC Resource == NULL");
	else{
		//gw_res.lockPutMutex();
		oc_res->put(oc_rep, qpm, &onGWPut);
	}
}

void GW_SensorArbiter::observe(GW_Resource &gw_res, QueryParamsMap &queryParamsMap)
{
	std::shared_ptr <OCResource> oc_res = gw_res.getOCRes();
	if(oc_res == NULL)
		gw_log("OC Resource == NULL");
	else
		oc_res->observe(ObserveType::Observe, queryParamsMap, &onGWObserve);

}

GW_SensorArbiter *GW_SensorArbiter::getInstance()
{
	if(NULL == instance)
		instance = new GW_SensorArbiter();

	return instance;
}

GW_SensorNotifier::GW_SensorNotifier()
	:interestingResources(), gw_handle(NULL)
{}
GW_SensorNotifier::~GW_SensorNotifier(){}
void GW_SensorNotifier::setResourceHandle(OCResourceHandle *gwHandle)
{
	gw_handle = gwHandle;
}
void GW_SensorNotifier::addInterestingResources(IN GW_Resource *gw_res)
{
	intResMutex.lock();
	interestingResources.push_back(gw_res);
	intResMutex.unlock();
}

void GW_SensorNotifier::setHostKey(std::string key, OCObservationId obsId)
{
//			gw_log("Host %s ObsId %d", key.c_str(), obsId);
	hostKeyStore.insert(std::pair<std::string, OCObservationId>(key, obsId));
}
OCObservationId GW_SensorNotifier::getHostKey(std::string key)
{
	std::map<std::string, OCObservationId>::iterator it = hostKeyStore.find(key);
	OCObservationId ret= 0;
	
	if(it == hostKeyStore.end())
		return 0;

	ret = it->second;
//			gw_log("Host %s %d", key.c_str(), ret);
	return ret;
}
void GW_SensorNotifier::notification()
{
	//for test
//		static int i=0;
//			gw_log("%d", interestingResources.size());
	intResMutex.lock();
	for(std::vector<GW_Resource *>::iterator it = interestingResources.begin(); it != interestingResources.end(); it++)
	{
		if((*it)->needNotification())
		{
			gw_log("Notify %s", (*it)->getUri().c_str());
			OCStackResult result;
			std::shared_ptr<OCResourceResponse> resourceResponse(new OCResourceResponse());
			OCRepresentation oc_rep;

			(*it)->getRep(oc_rep);

		    resourceResponse->setErrorCode(200);
		    resourceResponse->setResourceRepresentation(oc_rep, DEFAULT_INTERFACE);
			
			if(gw_handle == NULL)
			{
				gw_log("gw_handle NULL");
			}
			
		    result = OCPlatform::notifyListOfObservers(*gw_handle, (*it)->getObsIds(), resourceResponse);

			if(result == OC_STACK_OK)
			{
				(*it)->observationClean();
			}
			usleep(50 * 1000);
		}	
	}


	intResMutex.unlock();

}
GW_SensorNotifier *GW_SensorNotifier::getInstance()
{
	if(NULL == instance)
		instance = new GW_SensorNotifier();

	return instance;
}


int GW_SensorDeviceManager::GetDeviceList(OUT GW_DeviceList* deviceList, OUT int eCode){
	*deviceList = this->deviceList;

	return VSF_S_OK;
}

/* ret = err, if resource is not found, resource is null */
int GW_SensorDeviceManager::GetResource (IN const std::string SUri, OUT GW_Resource **resource){
	*resource = NULL;
	return deviceList.getSensor(SUri, resource);
}
int GW_SensorDeviceManager::GetResourceXML(IN const std::string SUri, OUT std::string &xmlResult)
{
	GW_Resource *resource;
	int ret;
	if(VSF_S_OK == (ret = GetResource(SUri, &resource)) && resource != NULL)
	{
		resource->makeXML(xmlResult);
		return ret;
	}
	
	return ret;
}

int GW_SensorDeviceManager::AddResource(IN const std::string SUri, IN std::shared_ptr <OCResource> oc_res, OUT GW_Resource **resource){
	*resource = NULL;

	int ret = deviceList.addSensor(SUri, oc_res, resource);

//			gw_log("%s, %d ", SUri.c_str(), *resource);
	return  ret;
}
// int RemoveResource(IN const std::string SUri, OUT GW_Resource **resource)
//{
//}

int GW_SensorDeviceManager::GetDeviceListXML(OUT std::string &str)
{
	deviceList.makeXML(str);
	return VSF_S_OK;
}

GW_SensorDeviceManager *GW_SensorDeviceManager::getInstance()
{
	if(NULL == instance)
	{
		instance = new GW_SensorDeviceManager();	
	}
	
	return instance;
}

GW_SensorDeviceManager::GW_SensorDeviceManager()
{
	sensorArbiter = GW_SensorArbiter::getInstance();
}

static pthread_mutex_t fndMtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t obsMtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void onGWFoundResource(std::shared_ptr <OCResource> resource)
{
    static int n = 0;


    pthread_mutex_lock(&fndMtx);
    

	GW_Resource *res = NULL;
	GW_SensorDeviceManager *sdm = GW_SensorDeviceManager::getInstance();
    GW_SensorDeviceManager::getInstance()->GetResource(resource->uri(), &res);
	if(res == NULL)
	{
        
		int ret;
		QueryParamsMap tmpParamsMap;
		ret = sdm->AddResource(resource->uri(), resource, &res);
		if(ret != VSF_S_OK && res == NULL)
		{
			gw_log("Warning:Add REsource failed");
			return;
		}

		//GW_SensorArbiter::getInstance()->firstGet(resource->uri());
//		res->lockGetMutex();
//		res->unlockGetMutex();
		if(res == NULL)
		{
			gw_log("why null?");
			return;
		}
		//res->setFound();		 		
		//res->observing();
		 		
		//GW_SensorArbiter::getInstance()->observe(*res, tmpParamsMap);
        
        gw_log("Found Resource(%d): %s done ", ++n, resource->uri().c_str());
	}
    else
    {
        
        if(res->is_found() == 0)
        {
            GW_SensorArbiter::getInstance()->firstGet(resource->uri());
    //		res->lockGetMutex();
    //		res->unlockGetMutex();
    		if(res == NULL)
    		{
    			gw_log("why null?");
    			return;
    		}
    		res->setFound();
            static int n1=0;
            gw_log("First Get Resource(%d): %s done ", ++n1, resource->uri().c_str());
        }
        else if(res->is_observated() == 0)
        {
            QueryParamsMap qpm;
            GW_SensorArbiter::getInstance()->observe(*res, qpm);
    //		res->lockGetMutex();
    //		res->unlockGetMutex();
    		if(res == NULL)
    		{
    			gw_log("why null?");
    			return;
    		}
    		//res->setFound();		 		
    		res->observing();
            static int n2=0;
            gw_log("Observe Resource(%d): %s done ", ++n2, resource->uri().c_str());
            ss_initialize(resource->uri());
        }
		 		
		//GW_SensorArbiter::getInstance()->observe(*res, tmpParamsMap);
    }
	
    pthread_mutex_unlock(&fndMtx);
}
void onGWGetSync(const HeaderOptions& opt, const OCRepresentation &rep, const int eCode)
{
	onGWGet(opt, rep, eCode);
	GW_SensorArbiter::getInstance()->responseSyncGet(rep.getUri());
}

void onGWFirstGet(const HeaderOptions& opt, const OCRepresentation &rep, const int eCode)
{
    
    static int n = 0;
    std::string suri = rep.getUri();
    GW_Resource *res = NULL;
    GW_SensorDeviceManager *sdm = GW_SensorDeviceManager::getInstance();
    if(VSF_S_OK != sdm->GetResource(suri, &res))
		gw_log("No Resource correspongding to suri:%s", suri.c_str());
	else if(NULL == res)
		gw_log("res null error %s", suri.c_str());
	else
	{
        onGWGet(opt, rep, eCode);
        res->getUri();
        QueryParamsMap qpm;
       // GW_SensorArbiter::getInstance()->observe(*res, qpm);
	}
}


void onGWGet(const HeaderOptions& opt, const OCRepresentation &rep, const int eCode)
{
	/* dbm addition code should be implemented*/
    pthread_mutex_lock(&mtx);
	std::string suri = rep.getUri();
	GW_Resource *res = NULL;
	GW_SensorDeviceManager *sdm = GW_SensorDeviceManager::getInstance();
//	gw_log("onGWGET");
	if(VSF_S_OK != sdm->GetResource(suri, &res))
		gw_log("No Resource correspongding to suri:%s", suri.c_str());
	else if(NULL == res)
		gw_log("res null error %s", suri.c_str());
	else
	{
        res->setAttribute();


		std::map <std::string, AttributeValue> repValue;
		rep.getAttributes(repValue);
		
		std::vector<sensorDB> db;
		int i=0;
        
		for(std::map <std::string, AttributeValue>::iterator it = repValue.begin(); it != repValue.end(); it++)
		{
			//TODO Type inference should be solved by visitor class
			//std::cout << it->first << ":" << boost::get<std::string>(it->second) << std::endl;
			
			if(it->first == "AV_POL" || it->first == "POLICY")
			{
				if(it->first == "AV_POL")
				{
					res->setAvaliablePolicy(boost::get<int>(it->second));
				}
				else
				{
					res->setCurPolicy(boost::get<int>(it->second));
				}
				continue;
			}    
			db.push_back(sensorDB());
			db[i].suri = suri;
			db[i].ttl = 1;
			db[i].index = 0;
			db[i].attribute = it->first;
			db[i].attributeType = TYPE_INTEGER;
			db[i].value = boost::get<std::string>(it->second);
            db[i].epoch_offset = 0;
			res->setAttribute(it->first, boost::get<std::string>(it->second));

			DBManager::getInstance()->getSensorAttribute(&db[i]);

			if(db[i].index <= 0){
//				gw_log("%s Added", db[i].attribute.c_str());
				DBManager::getInstance()->addSensorAttribute(&db[i]);
			}
			DBManager::getInstance()->addSensorData(&db[i]);
            
			i++;
			
		}
		
		
//		gw_log("RES-> %s", xml.c_str());
//		res->unlockGetMutex();

	}
    pthread_mutex_unlock(&mtx);
}
void onGWPut(const HeaderOptions& opt, const OCRepresentation &rep, const int eCode)
{
    pthread_mutex_lock(&fndMtx);
	std::string suri = rep.getUri();
	GW_Resource *res = NULL;
	GW_SensorDeviceManager *sdm = GW_SensorDeviceManager::getInstance();

//	gw_log("onGWPUT");
	if(VSF_S_OK != sdm->GetResource(suri, &res))
		gw_log("No Resource correspongding to suri:%s", suri.c_str());
	else if(NULL == res)
		gw_log("res null error %s", suri.c_str());
	else
	{
//		gw_log("onGWPut:%s", suri.c_str());
		std::map <std::string, AttributeValue> repValue;
		rep.getAttributes(repValue);
		
		std::vector<sensorDB> db;

		int i=0;
		for(std::map <std::string, AttributeValue>::iterator it = repValue.begin(); it != repValue.end(); it++)
		{
			//TODO Type inference should be solved by visitor class
			//std::cout << it->first << ":" << boost::get<std::string>(it->second) << std::endl;
			if(it->first == "AV_POL" || it->first == "POLICY")
			{
				if(it->first == "AV_POL")
				{
					res->setAvaliablePolicy(boost::get<int>(it->second));
				}
				else
				{
//					gw_log("POLICY:%d", boost::get<int>(it->second));
					res->setCurPolicy(boost::get<int>(it->second));
				}
				continue;
			}
			res->setAttribute(it->first, boost::get<std::string>(it->second));

			db.push_back(sensorDB());
			db[i].suri = suri;
			db[i].ttl = 1;
			db[i].index = 0;
			db[i].attribute = it->first;
			db[i].attributeType = TYPE_INTEGER;
            db[i].epoch_offset = 0;
			db[i].value = boost::get<std::string>(it->second);
			
			res->setAttribute(it->first, boost::get<std::string>(it->second));


			DBManager::getInstance()->getSensorAttribute(&db[i]);

			if(db[i].index <= 0){
//				gw_log("%s Added", db[i].attribute.c_str());
				DBManager::getInstance()->addSensorAttribute(&db[i]);
			}

			DBManager::getInstance()->addSensorData(&db[i]);
			i++;
		}

		std::string xml;
		res->makeXML(xml);
		
//		gw_log("RES-> %s", xml.c_str());
		
	}

	//res->unlockPutMutex();
	pthread_mutex_unlock(&fndMtx);
}

void onGWObserve(const HeaderOptions headerOptions, const OCRepresentation& rep,
                    const int& eCode, const int& sequenceNumber)
{
    pthread_mutex_lock(&obsMtx);
	GW_Resource *gw_res = NULL;
    int ret =  GW_SensorDeviceManager::getInstance()->GetResource(rep.getUri(), &gw_res);

    if(VSF_S_OK != ret|| gw_res == NULL)
    {
        gw_log("GW resource has not been found");
        return;
    }

	std::map <std::string, AttributeValue> repValue;
	rep.getAttributes(repValue);

	std::vector<sensorDB> db;

	int i=0;

	for(std::map <std::string, AttributeValue>::iterator it = repValue.begin(); it != repValue.end(); it++)
	{
		//TODO Type inference should be solved by visitor class
		//std::cout << it->first << ":" << boost::get<std::string>(it->second) << std::endl;
		if(it->first == "AV_POL" || it->first == "POLICY")
		{
			if(it->first == "AV_POL")
			{
				gw_res->setAvaliablePolicy(boost::get<int>(it->second));
			}
			else
			{
				gw_res->setCurPolicy(boost::get<int>(it->second));
			}
			continue;
		}
		gw_res->setAttribute(it->first, boost::get<std::string>(it->second));
		db.push_back(sensorDB());
		db[i].suri = rep.getUri();
		db[i].ttl = 1;
		db[i].index = 0;
		db[i].attribute = it->first;
		db[i].attributeType = TYPE_INTEGER;
		db[i].value = boost::get<std::string>(it->second);
        db[i].epoch_offset = 0;
		DBManager::getInstance()->getSensorAttribute(&db[i]);
		if(db[i].index <= 0){
			gw_log("%s Added", db[i].attribute.c_str());
			DBManager::getInstance()->addSensorAttribute(&db[i]);
		}
		DBManager::getInstance()->addSensorData(&db[i]);
		i++;
	}


	gw_res->setAttribute();
	
	
	/* dbm addtion code should be implemented */
//	gw_log(rep.getJSONRepresentation().c_str());
	gw_res->observationDirty(); //This observation result should be notified by the consumer (VS).

    ss_check(rep.getUri());
    pthread_mutex_unlock(&obsMtx);
	
}
