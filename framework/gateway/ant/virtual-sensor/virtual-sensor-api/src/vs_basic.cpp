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

/// @file vs_basic.cpp

/// @brief  This file contains the implementation of classes and its members
///         related to VSF Basic API

#include "vs_basic.h"

VS_Basic *VS_Basic::singleton = NULL; // initialize singleton class


void parseXML(IN std::string &xmlData, OUT rapidxml::xml_document<char>& xmlDoc) {
}

/**
 * @class Resource
 * @brief This class represents sensor information
 **/
 VS_Resource::VS_Resource()
    : mAvailablePolicy(0), mUri(), mName(), mPolicies(), mAttributes(), mResourceXML() 
{}

std::shared_ptr<VS_Resource> VS_Resource::create(IN std::string &inputXML)
{
	std::shared_ptr<VS_Resource> resource(new VS_Resource());
	bool isSuccessful = resource->setFromXML(inputXML);
	if(isSuccessful)
		return resource;
	else
		return NULL;
}

std::shared_ptr<VS_Resource> VS_Resource::create(IN rapidxml::xml_node<char> *xnResource)
{
	std::shared_ptr<VS_Resource> resource(new VS_Resource());
	bool isSuccessful = resource->setFromXML(xnResource);
	if(isSuccessful)
		return resource;
	else
		return NULL;
}

// ResourceXML will be shown except the case of GetDeviceList.
std::string& VS_Resource::getResourceXML()
{
	return this->mResourceXML;
}

std::string& VS_Resource::getUri() {
	return this->mUri;
}

std::string& VS_Resource::getName() {
	return this->mName;
}

std::vector<int>& VS_Resource::getPolicies() {
	return this->mPolicies;
}

std::map<std::string, std::string>& VS_Resource::getAttributes() {
	return this->mAttributes;
}

int VS_Resource::getAvailablePolicy() {
	return this->mAvailablePolicy;
}


bool VS_Resource::setFromXML(IN std::string &xmlData) {
	// Parse XML string to XML data structure
	rapidxml::xml_document<char> xmlDoc;
	vector<char> xml_copy(xmlData.begin(), xmlData.end());
	xml_copy.push_back('\0');
	xmlDoc.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>(&xml_copy[0]);
	//parseXML(xmlData, xmlDoc);

	// XML data structure -> VSF object style
	rapidxml::xml_node<char> *xnResource = xmlDoc.first_node("resource");
	if (!xnResource)
		return false;

	this->mResourceXML = xmlData;
	bool isSuccessful = this->setFromXML(xnResource);
	return isSuccessful;
}

bool VS_Resource::setFromXML(IN rapidxml::xml_node<char> *xnResource) {
	if(xnResource == NULL)
		return false;

	// resource
	// -- uri
	char* uri = xnResource->first_node("uri")->value();
	this->setUri(std::string(uri));

	// -- name
	char *name = xnResource->first_node("name")->value();
	this->setName(std::string(name));

	// -- AvailablePolicy: int
	char *availablePolicyStr = xnResource->first_node("AvailablePolicy")->value();
	int availablePolicy;
	sscanf(availablePolicyStr, "%08x", &availablePolicy);
	this->setAvailablePolicy(availablePolicy);

	// -- attributes
	rapidxml::xml_node<char> *xnAttributes = xnResource->first_node("attributes");
	if(xnAttributes != NULL) {
		// ---- attribute
		for(rapidxml::xml_node<char> *xnAttribute = xnAttributes->first_node(); xnAttribute; xnAttribute = xnAttribute->next_sibling()) {
			// ------ name 
			char* attributeName = xnAttribute->first_node("name")->value();
			// ------ value: std::string
			char* attributeValue = xnAttribute->first_node("value")->value();
			this->addAttribute(attributeName, attributeValue);
		}
	}

	// -- policies
	rapidxml::xml_node<char> *xnPolicies = xnResource->first_node("policies");
	// ---- policy
	if(xnPolicies != NULL) {
		for (rapidxml::xml_node<char> *xnPolicy = xnPolicies->first_node(); xnPolicy; xnPolicy = xnPolicy->next_sibling()) {
			char *policyStr = xnPolicy->value();
			int policy;
			sscanf(policyStr, "%08x", &policy);
			this->addPolicy(policy);
		}
	}
	return true;
}
void VS_Resource::setUri(std::string uri) {
	if(uri.find("root") == 0)
		uri = uri.substr(4, uri.length() - 4);
	this->mUri = uri;
}

void VS_Resource::setName(std::string name) {
	this->mName = name;
}

void VS_Resource::addPolicy(IN int policy) {
	this->mPolicies.push_back(policy);
}

void VS_Resource::addAttribute(IN std::string attr_name, IN std::string attr_value) {
	this->mAttributes.insert(std::pair<std::string, std::string>(attr_name, attr_value));
}

void VS_Resource::setAvailablePolicy(IN int availablePolicy) {
	this->mAvailablePolicy = availablePolicy;
}


/**
 * @class Collection
 * @brief This class represents collection struct and provide the methods related it.
 *
 **/
VS_Collection::VS_Collection()
	: mName(), mResources(), mChildren() 
{}

std::shared_ptr<VS_Collection> VS_Collection::create(IN rapidxml::xml_node<char> *xnCollection) {
	std::shared_ptr<VS_Collection> collection(new VS_Collection());
	bool isSuccessful = collection->setFromXML(xnCollection);
	if(isSuccessful)
		return collection;
	else
		return NULL;
}

bool VS_Collection::isResource() {
	return (this->mResources.size() != 0);
}

std::string& VS_Collection::getName() {
	return this->mName;
}

std::vector<std::shared_ptr<VS_Resource>>& VS_Collection::getResources() {
	return this->mResources;
}

std::vector<std::shared_ptr<VS_Collection>>& VS_Collection::getChildren() {
	return this->mChildren;
}

bool VS_Collection::setFromXML(IN rapidxml::xml_node<char> *xnCollection) {
	
	// XML data structure -> VSF object style
	if (!xnCollection) return false;

//			for(rapidxml::xml_node<char> *xnChild = xnCollection->first_node(); xnChild; xnChild = xnChild->next_sibling()) {
////				vs_log("child: %s", xnChild->name());
////				if(!std::string(xnChild->name()).compare("collection"))
////					VS_Collection::create(xnChild);
//
//				{
//				std::string keyStr(xnChild->name());
//				vs_log("child: %s %s", xnChild->name(), keyStr.c_str());
//				if(!keyStr.compare("collection"))
//					VS_Collection::create(xnChild);
//				}
//			}
//			return true;

	for(rapidxml::xml_node<char> *xnChild = xnCollection->first_node(); xnChild; xnChild = xnChild->next_sibling()) {
//				vs_log("key: %s", xnChild->name());
		if(!strcmp(xnChild->name(), "name")) {
			//-- name
			std::string valueStr(xnChild->value());
			this->setName(valueStr);
		} else if(!strcmp(xnChild->name(), "collection")) {
			//-- collection
//					vs_log("collection: %s", xnChild->name());
			std::shared_ptr<VS_Collection> childCollection = VS_Collection::create(xnChild);
			if(childCollection == NULL) { // parse error
				vs_log("Parse error(Collection): cannot parse collection");
				return false;
			}
			// insert collection as a child
			this->addChild(childCollection);
		} else if(!strcmp(xnChild->name(), "resource")) {
			// TEST
			for(rapidxml::xml_node<char> *xnChild2 = xnChild->first_node(); xnChild2; xnChild2 = xnChild2->next_sibling()) {
//						vs_log("resource child: %s", xnChild2->name());
			}

			//-- resource
			std::shared_ptr<VS_Resource> resource = VS_Resource::create(xnChild);
			if(resource == NULL) { // parse error
				vs_log("Parse error(Collection): cannot parse resource");
				return false;
			}
			// set resource
			this->addResource(resource);
		} else {
			vs_log("Parse error(Collection): child should be name, collection or resource (%s)", xnChild->name());
			return false; // child should be name, collection or resource.
		}
	}
	return true;
}

void VS_Collection::setName(std::string& name) {
	this->mName = name;
}

void VS_Collection::addResource(std::shared_ptr<VS_Resource> resource) {
	this->mResources.push_back(resource);
}

void VS_Collection::addChild(std::shared_ptr<VS_Collection> child) {
	this->mChildren.push_back(child);
}


/**
 * @class DeviceList
 * @brief This class represents devices list struct and provide the methods related it.
 *
 **/
VS_DeviceList::VS_DeviceList()
 : mDeviceListXML(), mResources() {
}

std::shared_ptr<VS_DeviceList> VS_DeviceList::create(IN std::string &inputXML)
{
	
	std::shared_ptr<VS_DeviceList> deviceList(new VS_DeviceList());
	
	bool isSuccessful = deviceList->setFromXML(inputXML);
	
	if(isSuccessful)
		return deviceList;
	else
		return NULL;
}

std::string& VS_DeviceList::getDeviceListXML() {
	printf("getDeviceListXML(): %s\n", this->mDeviceListXML.c_str());
	return this->mDeviceListXML;
}

std::shared_ptr<std::vector<std::string>>& VS_DeviceList::getResourcesVector()
{
	return this->mResources;
}

bool VS_DeviceList::setFromXML(IN std::string &xmlData) {
	this->mDeviceListXML = xmlData;
	
	// Parse XML string to XML data structure
	rapidxml::xml_document<char> xmlDoc;
	vector<char> xml_copy(xmlData.begin(), xmlData.end());
	xml_copy.push_back('\0');
	xmlDoc.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>(&xml_copy[0]);
	//parseXML(xmlData, xmlDoc);

	// XML data structure -> VSF object style

//#ifdef BIG_NETWORK_SIZE_SUPPORTED
//	rapidxml::xml_node<char> *xnCollection = xmlDoc.first_node("collection");
//	if (!xnCollection) return false;
//
//
//	this->mRoot = VS_Collection::create(xnCollection);
//	if(this->mRoot == NULL)
//		return false;
//	else
//		return true;
//#else
	for(rapidxml::xml_node<char> *xnResource = xmlDoc.first_node(); xnResource; xnResource = xnResource->next_sibling())
	{
		if(!strcmp(xnResource->name(), "resource"))
		{
			if(mResources == NULL)
				mResources = std::shared_ptr<std::vector<std::string>>(new std::vector<std::string>());
			mResources->push_back(std::string(xnResource->value()));
		}
	}
	if(this->mResources == NULL)
		return false;
	else
		return true;
//#endif
}

VS_Basic::VS_Basic() {
}
VS_Basic::~VS_Basic() {
}
		
VS_Basic* VS_Basic::get() {
	if(singleton == NULL)
		singleton = new VS_Basic();
	return singleton;
}

void VS_Basic::free() {
	if(singleton != NULL) {
		delete singleton;
		singleton = NULL;
	}
}

void VS_Basic::setCallbacks(std::shared_ptr<VS_Basic_Callbacks> callbacks) {
	this->m_callbacks = callbacks;
}

/*	void setVSF(VS_Basic **vs)
{
	*vs = this;
}*/
int VS_Basic::ConnectGW()
{
	srand(time(NULL));
	host_key = rand();
	PlatformConfig cfg {
		OC::ServiceType::InProc,
		OC::ModeType::Client,
		"0.0.0.0",
		0,
		OC::QualityOfService::LowQos
	};

	OCPlatform::Configure(cfg);
	try
	{
		// makes it so that all boolean values are printed as 'true/false' in this stream
		std::cout.setf(std::ios::boolalpha);
		// Find all resources
		OCPlatform::findResource("", "coap://224.0.1.187/oc/core?rt=core.gateway", onConnected_priv);
		vs_log("Finding Gateway...");

		// A condition variable will free the mutex it is given, then do a non-
		// intensive block until 'notify' is called on it.  In this case, since we
		// don't ever call cv.notify, this should be a non-processor intensive version
		// of while(true);
		std::mutex blocker;
		condition_variable cv;
		std::unique_lock<std::mutex> lock(blocker);
		cv.wait(lock);
	}catch(OCException& e)
	{
		//log(e.what());
		return VSF_E_FAIL;
	}

	return VSF_S_OK;
}

void VS_Basic::setResource(std::shared_ptr<OCResource> res)
{
	m_resource = res;
}

int VS_Basic::GetDeviceList()
{
	if(m_resource)
    {
        OCRepresentation rep;
		
        rep.setValue("GW_CMD", GW_CMD_GET_DEVICE_LIST);

        // Invoke resource's put API with rep, query map and the callback parameter
        m_resource->put(rep, QueryParamsMap(), onGetDeviceList_priv);
	sleep(1);

		return VSF_S_OK;
    }
	
	vs_log("No connection with GW");
	
	return VSF_E_FAIL;
}



int VS_Basic::GetSensorInfo(IN const std::string SUri)
{
	if(m_resource)
    {
        OCRepresentation rep;
		

        rep.setValue("GW_CMD", GW_CMD_GET_SENSOR_INFO);
		rep.setValue("SURI", SUri);

        // Invoke resource's put API with rep, query map and the callback parameter
        m_resource->put(rep, QueryParamsMap(), onGetSensorInfo_priv);
		//sleep(1);

		return VSF_S_OK;
    }

	vs_log("No connection with GW");

	return VSF_E_FAIL;
}

int VS_Basic::GetSensorData(IN const std::string SUri, const int epoch)
{
	if(m_resource)
	{
		OCRepresentation rep;
		std::stringstream ss;
		ss<<epoch;
		rep.setValue("GW_CMD", GW_CMD_GET_SENSOR_DATA_EPOCH);
		rep.setValue("SURI", SUri);
		rep.setValue("EPOCH", ss.str());

		m_resource->put(rep, QueryParamsMap(), onGetSensorDataEpoch_priv);
		return VSF_S_OK;
	}

	return VSF_E_FAIL;

}
int VS_Basic::PutActuatorData(IN const std::string SUri, IN const std::string attrName, IN const std::string attrValue)
{
	if(m_resource)
	{
		OCRepresentation rep;
		std::stringstream ss;
		rep.setValue("GW_CMD", GW_CMD_PUT_ACTUATOR_DATA);
		rep.setValue("SURI", SUri);
		rep.setValue(attrName.c_str(), attrValue.c_str());

		m_resource->put(rep, QueryParamsMap(), onPutActuatorData_priv);
	}
	return VSF_S_OK;
}
int VS_Basic::PutActuatorData(IN const std::string SUri, IN OCRepresentation &rep)
{
	if(m_resource)
	{
		std::stringstream ss;
		rep.setValue("GW_CMD", GW_CMD_PUT_ACTUATOR_DATA);
		rep.setValue("SURI", SUri);

		m_resource->put(rep, QueryParamsMap(), onPutActuatorData_priv);
	}
	return VSF_S_OK;
}
int VS_Basic::SetPolicy(IN const std::string SUri, IN int rawDtAcqType, IN int rawLevel, IN int netDtAcqType, IN int netLevel)
{
	if(m_resource)
	{
		OCRepresentation rep;
		int totalPolicy;
			
		int netResult = netDtAcqType << DT_ACQ_NET_TYPE_SHIFT;
		netResult = netResult << DT_ACQ_TYPE_SHIFT;
		netResult = netResult | ((netLevel<<DT_ACQ_NET_VALUE_SHIFT)<<DT_ACQ_LEVEL_SHIFT);

		int rawResult = rawDtAcqType << DT_ACQ_RAW_TYPE_SHIFT;
		rawResult = rawResult << DT_ACQ_TYPE_SHIFT;
		rawResult = rawResult | ((rawLevel<<DT_ACQ_RAW_VALUE_SHIFT)<<DT_ACQ_LEVEL_SHIFT);
		totalPolicy = netResult | rawResult;
		
		rep.setValue("GW_CMD", GW_CMD_PUT_ACTUATOR_DATA);
		rep.setValue("SURI", SUri);
		rep.setValue("POLICY", totalPolicy);

		m_resource->put(rep, QueryParamsMap(), onPutActuatorData_priv);
	}

	return VSF_S_OK;
}
int VS_Basic::StartObservingSensorData(IN const std::string SUri)
{
	static int observing = 0;
	QueryParamsMap queryParamsMap;
	std::stringstream ss;

	ss<<host_key;

	queryParamsMap.insert(std::pair <std::string, std::string> ("SURI", SUri));
	queryParamsMap.insert(std::pair <std::string, std::string> ("HOST", ss.str()));

	if(!observing)
	{				
		m_resource->observe(ObserveType::Observe, queryParamsMap, onObserveSensorData_priv);
		observing = 1;
	}
	else
	{
		OCRepresentation rep;
		

        rep.setValue("GW_CMD", GW_CMD_START_OBSERVING);

        // Invoke resource's put API with rep, query map and the callback parameter
        m_resource->put(rep, queryParamsMap, onObserveSensorDataVirt_priv);
	sleep(1);

		return VSF_S_OK;
	}

	return VSF_S_OK;
}



//Following methods can be overrided.
void VS_Basic::onConnected()
{
	// Call user-side callback
	this->m_callbacks->onConnected();
}
void VS_Basic::onGetDeviceList(IN std::shared_ptr<VS_DeviceList> deviceList, const int eCode)
{

	// Call user-side callback
	this->m_callbacks->onGetDeviceList(deviceList, eCode);
}
void VS_Basic::onGetSensorInfo(IN std::shared_ptr<VS_Resource> resource, const int eCode)
{

	// Call user-side callback
	this->m_callbacks->onGetSensorInfo(resource, eCode);
}
void VS_Basic::onGetSensorData(IN std::shared_ptr<VS_Resource> resource, const int eCode)
	//TODO: Desgin for proper parameters
{

	// Call user-side callback
	this->m_callbacks->onGetSensorData(resource, eCode);
}
void VS_Basic::onObservingSensorData(IN std::shared_ptr<VS_Resource> resource, const int& eCode, const int& sequenceNumber)
{
	this->m_callbacks->onObservingSensorData(resource, eCode, sequenceNumber);
}
void VS_Basic::onPutActuatorData(IN std::shared_ptr<VS_Resource> resource, const int eCode)
{
	this->m_callbacks->onPutActuatorData(resource, eCode);
}

void onConnected_priv(std::shared_ptr<OCResource> resource)
{
	VS_Basic::get()->setResource(resource);
	VS_Basic::get()->onConnected();
//	onConnected();
}

void onGetDeviceList_priv(const HeaderOptions& headerOptions, const OCRepresentation& rep, const int eCode)
{
	std::string deviceListXML;
	rep.getValue("RESULT", deviceListXML);
	vs_log("%s", deviceListXML.c_str());
	std::shared_ptr<VS_DeviceList> deviceList = VS_DeviceList::create(deviceListXML);
	//std::shared_ptr<VS_DeviceList> deviceList;
	VS_Basic::get()->onGetDeviceList(deviceList, eCode);
	
}
void onGetSensorInfo_priv(const HeaderOptions& headerOptions, const OCRepresentation& rep, const int eCode)
{
	std::string resourceXML;
	rep.getValue("RESULT", resourceXML);
	shared_ptr<VS_Resource> resource = VS_Resource::create(resourceXML);
	
	VS_Basic::get()->onGetSensorInfo(resource, eCode);
}


void onGetSensorDataEpoch_priv(const HeaderOptions& headerOptions, const OCRepresentation& rep, const int eCode)
{
	std::string resourceXML;
	std::string suri;
	rep.getValue("RESULT", resourceXML);
	rep.getValue("SURI", suri);
	
	shared_ptr<VS_Resource> resource = VS_Resource::create(resourceXML);
	
	VS_Basic::get()->onGetSensorData(resource, eCode);
}

void onPutActuatorData_priv(const HeaderOptions & headerOptions, const OCRepresentation & rep, const int eCode)
{
	std::string resourceXML;
	rep.getValue("RESULT", resourceXML);
	shared_ptr<VS_Resource> resource = VS_Resource::create(resourceXML);

	VS_Basic::get()->onPutActuatorData(resource, eCode);
}

void onObserveSensorDataVirt_priv(const HeaderOptions& headerOptions, const OCRepresentation& rep, const int eCode)
{
	
}
void onObserveSensorData_priv(const HeaderOptions headerOptions, const OCRepresentation& rep,
                    const int& eCode, const int& sequenceNumber)
{
	
    if(eCode == OC_STACK_OK)
    {
		std::string resourceXML;
		resourceXML = rep.getValue<std::string>("RESULT");
		shared_ptr<VS_Resource> resource = VS_Resource::create(resourceXML);

		VS_Basic::get()->onObservingSensorData(resource, eCode, sequenceNumber);
    }
    else
    {
        std::cout << "onObserve Response error: " << eCode << std::endl;
        std::exit(-1);
    }
}
