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

/// @file gw_collection.cpp

/// @brief  This file contains the implementation of classes and its members
///         related to OIC Resource Model

#include "gw_collection.h"

static int GW_uri_tokenize_back(IN const std::string uri_in, std::string &uri_out){
	size_t tokenized_pos;
	uri_out = uri_in;

	tokenized_pos = uri_in.find_first_of(std::string("/"), 0);
	if(std::string::npos == tokenized_pos){
		gw_log("Can't find / mark");
		return VSF_E_FAIL;
	}

	uri_out = uri_in.substr(tokenized_pos+1, std::string::npos);

	return VSF_S_OK;	
}
static int GW_uri_tokenize_front(IN const std::string uri_in,  std::string &uri_out){
	size_t tokenized_pos;
	uri_out = uri_in;

	tokenized_pos = uri_out.find_first_of(std::string("/"), 0);
	if(std::string::npos == tokenized_pos)
		return VSF_E_FAIL;

	uri_out = uri_out.substr(0, tokenized_pos);

	return VSF_S_OK;
}
static int GW_uri_tokenize_name(IN const std::string uri_in, std::string &uri_out){
	size_t tokenized_pos;
	uri_out = uri_in;

	tokenized_pos = uri_out.find_last_of(std::string("/"), std::string::npos);
	if(std::string::npos == tokenized_pos)
		return VSF_E_FAIL;

	uri_out = uri_out.substr(tokenized_pos+1, std::string::npos);

	return VSF_S_OK;
};
GW_Resource::GW_Resource()
{
	initialized = 0;
	ssm_mark = 0;
	availablePolicy = 0;
	found = 0;
	observated = 0;
	res_mtx = PTHREAD_MUTEX_INITIALIZER;
	put_res_mtx = PTHREAD_MUTEX_INITIALIZER;
	attrMutex = PTHREAD_MUTEX_INITIALIZER;
}


GW_Resource& GW_Resource::operator= (const GW_Resource &resource)
{
	this->initialized = resource.initialized;
	this->resourceUri = resource.resourceUri;
//			this->obsIds = resource.obsIds;
	this->ssm_mark = resource.ssm_mark;
	return *this;
}

std::map<std::string, std::string> GW_Resource::getAttribute()
{
	return attributes;
}

void GW_Resource::lockGetMutex()
{
	pthread_mutex_lock(&res_mtx);
}
void GW_Resource::unlockGetMutex()
{
	pthread_mutex_unlock(&res_mtx);
}
void GW_Resource::lockPutMutex()
{
	pthread_mutex_lock(&put_res_mtx);
}
void GW_Resource::unlockPutMutex()
{
	pthread_mutex_unlock(&put_res_mtx);
}
bool GW_Resource::operator== (const std::string &uri){
//	gw_log("%s??%s = %d", resourceUri.c_str(), uri.c_str(), (resourceUri == uri));
	
	return (resourceUri == uri);
}
bool GW_Resource::operator== (const GW_Resource &resource2){
	return (resourceUri == resource2.resourceUri);
}
void GW_Resource::observing()
{
	observated = 1;
}
bool GW_Resource::is_observated()
{
	return observated;
}
	
void GW_Resource::observationDirty()
{
	obsDirty = 1;
}
void GW_Resource::observationClean()
{
	obsDirty = 0;
}
bool GW_Resource::needNotification()
{
	return  obsDirty;
}
int GW_Resource::numberOfObs()
{
	return obsIds.size();
}

ObservationIds& GW_Resource::getObsIds()
{
	return obsIds;
}
void GW_Resource::addObsId(OCObservationId obsId)
{
	obsIds.push_back(obsId);
}
void GW_Resource::setFound()
{
	found = 1;
}
bool GW_Resource::is_found()
{
	return found;
}
bool GW_Resource::is_initialized(){
	return initialized;
}

std::shared_ptr <OCResource>& GW_Resource::getOCRes()
{
    if(NULL == oc_res)
        gw_log("This is soft sensor, why are you getting oc_res?");
	return oc_res;
}
void GW_Resource::makeXML(OUT std::string& xmlResult){
	setAttribute();
	xmlResult += "<resource>";
		xmlResult += "<uri>" + resourceUri + "</uri>";
		xmlResult += "<name>"+name+"</name>";
		pthread_mutex_lock(&attrMutex);
		if(attributes.size())
		{
			xmlResult += "<attributes>";
			for(std::map<std::string, std::string>::iterator it=attributes.begin(); it!=attributes.end(); ++it)
			{
				xmlResult += "<attribute>";
				xmlResult += "<name>"+it->first+"</name>";
				xmlResult += "<value>"+it->second+"</value>";
				xmlResult += "</attribute>";
			}
			xmlResult += "</attributes>";
		}
		pthread_mutex_unlock(&attrMutex);

		char avPol[16];
		sprintf(avPol, "%08x", availablePolicy);
		xmlResult += "<AvailablePolicy>";
		xmlResult += std::string(avPol);
		xmlResult += "</AvailablePolicy>";
//				gw_log("%d", policies.size());
		if(policies.size())
		{
			xmlResult += "<policies>";
			for(std::vector<int>::iterator it = policies.begin(); it != policies.end(); ++it)
			{
				char pol[16];
				sprintf(pol, "%08x", *it);
				xmlResult += "<policy>";
				xmlResult += std::string(pol);
				if(*it == currentPolicy)
					xmlResult+= "*";
				xmlResult += "</policy>";
			}
			xmlResult += "</policies>\n";
		}
	xmlResult += "</resource>\n";
}
void GW_Resource::makeLightXML(OUT std::string& xmlResult){
	setAttribute();
	xmlResult += "<resource>";
	xmlResult += resourceUri.substr(4, std::string::npos);		
	xmlResult += "</resource>\n";
}

int GW_Resource::create(std::string uri, std::shared_ptr <OCResource> &oc_res)
{
	int ret;
	initialized = 1;
	this->oc_res = oc_res;
	resourceUri = uri;
	ret = GW_uri_tokenize_name(uri, name);
	return ret;
}
std::string GW_Resource::getUri(void)
{
	return resourceUri;
}

int GW_Resource::getRep(OUT OCRepresentation &oc_rep)
{
	std::string xmlResult;
	this->makeXML(xmlResult);
	oc_rep.setValue("RESULT", xmlResult);
//			gw_log("%s", xmlResult.c_str());

	return VSF_S_OK;
}

void GW_Resource::setAttribute()
{
	std::map<std::string, std::string> out;
	int ret;
	sensorDB db;
	db.suri = resourceUri.substr(5, std::string::npos);
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &out);

//			gw_log("%s, %d", db.suri.c_str(), out.size());
	for(std::map<std::string, std::string>::iterator it = out.begin(); it != out.end(); it++)
	{
//				gw_log("%s, %s", it->first.c_str(), it->second.c_str());
		ret = setAttribute(it->first, it->second);				

		if(ret != VSF_S_OK)
			return;
	}

}

int GW_Resource::setAttribute(IN std::string attr_name, IN std::string attr_value)
{
	pthread_mutex_lock(&attrMutex);

	std::map<std::string, std::string>::iterator it = attributes.find(attr_name);
	if(it == attributes.end())
	{
		attributes.insert( std::pair<std::string, std::string>(attr_name, attr_value));
	}
	else
	{
		it->second = attr_value;
	}
	pthread_mutex_unlock(&attrMutex);

	return VSF_S_OK;
}

int GW_Resource::addPolicy(IN int netType, IN int netLevel, IN int rawType, IN int rawLevel)
{
	int totalNetPolicy = 0;
	int totalRawPolicy = 0;
	int totalPolicy;

	if(netType != 0)
		GW_PolicyManager::SetNetPolicy(netType, netLevel, totalNetPolicy);
	if(rawLevel != 0)
		GW_PolicyManager::SetRawPolicy(rawType, rawLevel, totalRawPolicy);

	if(totalNetPolicy == 0 && totalRawPolicy == 0){
		gw_log("Invalid policy");
		return VSF_E_FAIL;
	}

	totalPolicy = totalNetPolicy | totalRawPolicy;
	bool verified;
	GW_PolicyManager::VerifyPolicy(availablePolicy, totalPolicy, verified);
	
	if(verified == true)
		policies.push_back(totalPolicy);
	else
	{
		gw_log("Verification failed");
		return VSF_E_FAIL;
	}

	return VSF_S_OK;
}
int GW_Resource::verifyPolicy(IN int netType, IN int netLevel, IN int rawType, IN int rawLevel, OUT bool &verified)
{
	int totalNetPolicy = 0;
	int totalRawPolicy = 0;
	int totalPolicy;

	if(netType != 0)
		GW_PolicyManager::SetNetPolicy(netType, netLevel, totalNetPolicy);
	if(rawLevel != 0)
		GW_PolicyManager::SetRawPolicy(rawType, rawLevel, totalRawPolicy);

	if(totalNetPolicy == 0 && totalRawPolicy == 0){
		gw_log("Invalid policy");
		return VSF_E_FAIL;
	}

	totalPolicy = totalNetPolicy | totalRawPolicy;

	GW_PolicyManager::VerifyPolicy(availablePolicy, totalPolicy, verified);
	
	return VSF_S_OK;		
}
bool GW_Resource::verifyPolicy(IN int policyValue)
{
	int dtAcqType, level, retValue, ret;
	int netAvailable, rawAvailable;
	bool res;

	ret = GW_PolicyManager::GetNetPolicy(availablePolicy, netAvailable, level);
	ret = GW_PolicyManager::GetNetPolicy(availablePolicy, rawAvailable, level);
			
	ret = GW_PolicyManager::GetNetPolicy(policyValue, dtAcqType, level);

	retValue = dtAcqType & netAvailable;

	ret = GW_PolicyManager::GetRawPolicy(policyValue, dtAcqType, level);

	if(ret != VSF_S_OK)
		gw_log("NOt happened");
	retValue = retValue && (dtAcqType & rawAvailable);

	if(retValue)
		res = true;
	else
		res = false;

	return res;
}
void GW_Resource::setAvaliablePolicy(int av_pol)
{
	availablePolicy = av_pol;
}
void GW_Resource::setCurPolicy(int cur_pol)
{
	currentPolicy = cur_pol;
	if(verifyPolicy(cur_pol))
	{
		std::vector<int>::iterator it = policies.begin();
		while(it != policies.end())
		{
			if(*it == cur_pol)
				break;
			it++;
		}
		if(it == policies.end())
		{
			policies.push_back(cur_pol);
		}
	}
	else
	{
		gw_log("Not allowed Policy");
	}
}



GW_Collection::GW_Collection()
{
	isResource = 0;
	isCollection = 0;
}
GW_Collection& GW_Collection::operator=(const GW_Collection& operand)
{
	this->isResource = operand.isResource;
	this->isCollection = operand.isCollection;
	name = operand.name;
	resource = operand.resource;
	children = operand.children;

	return *this;
}
void GW_Collection::makeXML(OUT std::string& xmlResult){
#ifdef BIG_NETWORK_SIZE_SUPPROTED
	if(isCollection)
	{
//				gw_log("Collection Make XML %s %d", name.c_str(), children.size());
		if(children.size())
		{
			if(isResource){
				resource.makeXML(xmlResult);
			
			}else{ // Otherwise, this collection should have the children.
				for(std::vector<GW_Collection>::iterator it = children.begin(); it != children.end(); ++it){
					it->makeXML(xmlResult);
				}
			}
			xmlResult +="</collection>\n";
		}
	}
	else if(isResource)
	{
		resource.makeXML(xmlResult);
	}
#else
	if(isCollection)
	{
//				gw_log("Collection Make XML %s %d", name.c_str(), children.size());
		if(children.size())
		{
			if(isResource){
				resource.makeLightXML(xmlResult);
			}else{
				for(std::vector<GW_Collection>::iterator it = children.begin(); it != children.end(); ++it){
						it->makeXML(xmlResult);
					}
			}
		}
	}
	else if(isResource)
	{
		resource.makeLightXML(xmlResult);
	}
#endif
}

int GW_Collection::createResource(IN std::string SUri, std::shared_ptr < OCResource > &oc_res){
	isResource = 1;
	resource.create(SUri, oc_res);

	return VSF_S_OK;
}
int GW_Collection::createCollection(IN std::string name){
	isCollection = 1;
	this->name = name;

	return VSF_S_OK;
}
void GW_Collection::setName(IN std::string name){
	this->name = name;
}

void GW_Collection::setResource(IN GW_Resource &gwResource){
	resource = gwResource;
}

void GW_Collection::addCollection(IN GW_Collection &gwCollection){
	children.push_back(gwCollection);
//			gw_log("%s %d size", name.c_str(), children.size());
}

int GW_Collection::getSensor(IN const std::string SUri, IN std::string tokenizedString, GW_Resource **resource){
	if(isResource)
	{
		if(this->resource.getUri() == SUri){
			*resource = &(this->resource);
			return VSF_S_OK;
		}
		else
			return VSF_S_OK;
	}
	else if(isCollection)
	{
		std::string collectionName;
		if(VSF_S_OK != GW_uri_tokenize_front(tokenizedString, collectionName))
		{
			gw_log("This should not be a collection");
			return VSF_E_FAIL;
		}
		else
		{
		
			if(collectionName == name)
			{
				std::string token;
				if(VSF_S_OK != GW_uri_tokenize_back(tokenizedString, token))
				{
					gw_log("This should not be happened");
					return VSF_E_FAIL;
				}

				for(std::vector<GW_Collection>::iterator it = children.begin(); it != children.end(); ++it)
				{
					if(VSF_S_OK == it->getSensor(SUri, token, resource))
					{
						if(NULL != *resource)
							return VSF_S_OK;
					}
					else
						return VSF_E_FAIL;
				}
			}
			
			return VSF_S_OK;					
		}
	}
	else
	{
		gw_log("This collection has not be initialized");
		return VSF_E_FAIL;
	}
}
int GW_Collection::addSensor(IN const std::string SUri, IN std::string tokenizedString, IN std::shared_ptr <OCResource> &oc_res, OUT GW_Resource **resource){
//			int i=0;
//			gw_log("suri:%s, %s", SUri.c_str(), tokenizedString.c_str());
	if(isResource)
	{
//				gw_log("%d", i++);
		gw_log("suri:%s, %s", SUri.c_str(), tokenizedString.c_str());
		
		if(this->resource == SUri){
			*resource = &(this->resource);
			//gw_log("This is already added resource");
			return VSF_S_OK;
		}
		else
			return VSF_S_OK;
	}
	else if(isCollection)
	{
//				gw_log("%d", i++);				
		std::string collectionName;
		if(VSF_S_OK != GW_uri_tokenize_front(tokenizedString, collectionName))
		{
			return VSF_E_FAIL;
		}
		else
		{
			if(collectionName == name)
			{
                std::string token;
				std::string token_collection;
				int res;
				if(VSF_S_OK != GW_uri_tokenize_back(tokenizedString, token))
				{
					gw_log("This should not be happened");
					return VSF_E_FAIL;
				}
				
				res = GW_uri_tokenize_front(token, collectionName);				
				for(std::vector<GW_Collection>::iterator it = children.begin(); it != children.end(); ++it)
				{
					if(collectionName == it->name)
					{
						if(VSF_S_OK == it->addSensor(SUri, token, oc_res, resource))
						{
							if(NULL != *resource)
								return VSF_S_OK;
						}
						else{
							
							return VSF_E_FAIL;
						}
					}
				}


				if(VSF_S_OK != res)
				{

					GW_Collection newCollection;
					newCollection.createResource(SUri, oc_res);
					this->addCollection(newCollection);
					*resource = &(((this->children).rbegin())->resource);
//							gw_log("Resource added");
//							gw_log("suri:%s, %s, %d, %d", SUri.c_str(), tokenizedString.c_str(), &(newCollection.resource), *resource);
					return VSF_S_OK;
				}
				else
				{
//							gw_log("suri:%s, %s", SUri.c_str(), tokenizedString.c_str());
					GW_Collection newCollection;
					newCollection.createCollection(collectionName);
					this->addCollection(newCollection);
//							gw_log("Collection added");
					return (children.rbegin())->addSensor(SUri, token, oc_res, resource);
				}
			}					
            
		}
	}
	
	return VSF_S_OK;
}


GW_DeviceList::GW_DeviceList()
: is_initialized(1), root()
{
	root.createCollection("root");
	deviceTreeMtx = PTHREAD_MUTEX_INITIALIZER;
}
void GW_DeviceList::makeXML(OUT std::string& xmlResult){
	if(is_initialized){
//				gw_log("DeviceList Make XML");
		pthread_mutex_lock(&deviceTreeMtx);
		root.makeXML(xmlResult);
		pthread_mutex_unlock(&deviceTreeMtx);
//				gw_log("DeviceList Make XML Done");
	}
}

int GW_DeviceList::getSensor(IN const std::string SUri, OUT GW_Resource **resource)
{
	std::string suri = "root"+SUri;
	std::string tokenizing = suri;
	pthread_mutex_lock(&deviceTreeMtx);
	int ret = root.getSensor(suri, tokenizing, resource);
	pthread_mutex_unlock(&deviceTreeMtx);

    if(*resource == NULL)
    {
        return ret;
    }
    
	return ret;
}
int GW_DeviceList::addSensor(IN const std::string SUri, IN std::shared_ptr <OCResource> &oc_res, OUT GW_Resource **resource)
{
	std::string suri;
	suri = "root"+SUri;
	std::string tokenizing = suri;
	GW_Resource *res = NULL;
    /*
	int ret = root.getSensor(suri, tokenizing, &res);
	if(res != NULL)
	{
		gw_log("suri:%s, %d", suri.c_str(), res);
		*resource = res;
		return ret;
	}
	*/
	pthread_mutex_lock(&deviceTreeMtx);
	int ret = root.addSensor(suri, tokenizing, oc_res, resource);
    static int n;
 //   if(*resource != NULL)
 //       gw_log("(%d)%s added", ++n, suri.c_str());
	pthread_mutex_unlock(&deviceTreeMtx);			
	return ret;
}

