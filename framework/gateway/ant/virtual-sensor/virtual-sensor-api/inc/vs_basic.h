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

/// @file vs_basic.h

/// @brief  This file contains the declaration of classes and its members
///         related to VSF Basic API


#ifndef _VS_BASIC_H_
#define _VS_BASIC_H_

#include <vector>
#include <pthread.h>
#include <mutex>
#include <map>
#include <condition_variable>
#include <typeinfo>
#include <sstream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "vs_util.h"
#include "OCPlatform.h"
#include "OCApi.h"

#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

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


using namespace OC;

enum VS_GET_TYPE
{
	VS_GET_DIRECT
	, VS_GET_INDIRECT
};

enum VS_VAR_TYPE
{	
	VS_BOOL,
	VS_CHAR,
	VS_INT,
	VS_DOUBLE,
	VS_STRING
};

enum
{
	DT_ACQ_POLLING = 1,
	DT_ACQ_BATCHING = 2,
	DT_ACQ_MODEL = 4
};

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

class VS_Collection;
class VS_Resource;
class VS_DeviceList;

void parseXML(IN std::string &xmlData, OUT rapidxml::xml_document<char>& xmlDoc);
/**
 * @class Resource
 * @brief This class represents sensor information
 **/
class VS_Resource
{
	private:
		int mAvailablePolicy;
		std::string mUri;
		std::string mName;
		std::vector<int> mPolicies;
		std::map<std::string, std::string> mAttributes;
		std::string mResourceXML;

		VS_Resource();

	public:
		static std::shared_ptr<VS_Resource> create(IN std::string &inputXML);

		static std::shared_ptr<VS_Resource> create(IN rapidxml::xml_node<char> *xnResource);

		// ResourceXML will be shown except the case of GetDeviceList.
		std::string& getResourceXML();

		std::string& getUri();

		std::string& getName();

		std::vector<int>& getPolicies();

		std::map<std::string, std::string>& getAttributes();

		int getAvailablePolicy();

	private:
		bool setFromXML(IN std::string &xmlData);

		bool setFromXML(IN rapidxml::xml_node<char> *xnResource);
		void setUri(std::string uri);

		void setName(std::string name);

		void addPolicy(IN int policy);
		void addAttribute(IN std::string attr_name, IN std::string attr_value);
		void setAvailablePolicy(IN int availablePolicy);
};


/**
 * @class Collection
 * @brief This class represents collection struct and provide the methods related it.
 *
 **/
class VS_Collection
{
	private:
		std::string mName;
		std::vector<std::shared_ptr<VS_Resource>> mResources;
		std::vector<std::shared_ptr<VS_Collection>> mChildren;

		VS_Collection();

	public:
		static std::shared_ptr<VS_Collection> create(IN rapidxml::xml_node<char> *xnCollection);

		bool isResource();
		std::string& getName();

		std::vector<std::shared_ptr<VS_Resource>>& getResources();

		std::vector<std::shared_ptr<VS_Collection>>& getChildren();
	private:
		bool setFromXML(IN rapidxml::xml_node<char> *xnCollection);

		void setName(std::string& name);

		void addResource(std::shared_ptr<VS_Resource> resource);

		void addChild(std::shared_ptr<VS_Collection> child);
};

/**
 * @class DeviceList
 * @brief This class represents devices list struct and provide the methods related it.
 *
 **/
class VS_DeviceList
{
	private:
		std::string mDeviceListXML;
		std::shared_ptr<std::vector<std::string>>mResources;

		VS_DeviceList();

	public:
		static std::shared_ptr<VS_DeviceList> create(IN std::string &inputXML);

		std::string& getDeviceListXML();

		std::shared_ptr<std::vector<std::string>>& getResourcesVector();

	private:
		bool setFromXML(IN std::string &xmlData);
};
class VS_Attributes
{
	private:
		std::string suri;
		std::shared_ptr<std::map<std::string, std::map<int, std::string>>> attributes;

	public:
		void setFromXML(std::string &xmlData)
		{
//			printf("VS_Attributes::setFromXML(%s)\n", xmlData.c_str());
//			rapidxml::xml_document<char> xmlDoc;
//			std::vector<char> xml_copy(xmlData.begin(), xmlData.end());
//			xml_copy.push_back('\0');
//			xmlDoc.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>(&xml_copy[0]);
//			// XML data structure -> VSF object style
//			rapidxml::xml_node<char> *xnAttr = xmlDoc.first_node();
//			if(!strcmp(xnAttr->name(), "attribute"))
//			{
//				rapidxml::xml_attribute<rapidxml::Ch> attrName = xnAttr->first_attribute("name");
//				//rapidxml::xml_attribute<rapidxml::Ch> attrName = xnAttr->first_attribute("name");
//				std::string attrNameValue = attrName->value();
//			}
//			else
//				//BUG
//			{
//			}
		}
};
void onConnected_priv(std::shared_ptr<OCResource> resource);
void onGetDeviceList_priv(const HeaderOptions& headerOptions, const OCRepresentation& rep, const int eCode);
void onGetSensorInfo_priv(const HeaderOptions& headerOptions, const OCRepresentation& rep, const int eCode);
void onGetSensorDataEpoch_priv(const HeaderOptions& headerOptions, const OCRepresentation& rep, const int eCode);
void onPutActuatorData_priv(const HeaderOptions& headerOptions, const OCRepresentation& rep, const int eCode);


void onObserveSensorData_priv(const HeaderOptions headerOptions, const OCRepresentation& rep,
                    const int& eCode, const int& sequenceNumber);
void onObserveSensorDataVirt_priv(const HeaderOptions& headerOptions, const OCRepresentation& rep, const int eCode);


class VS_Basic_Callbacks
{
	public:
		virtual void onConnected() = 0;
		virtual void onGetDeviceList(IN std::shared_ptr<VS_DeviceList> deviceList, const int eCode) = 0;
		virtual void onGetSensorInfo(IN std::shared_ptr<VS_Resource> resource, const int eCode) = 0;
		virtual void onGetSensorData(IN std::shared_ptr<VS_Resource> resource, const int eCode) = 0;
		virtual void onObservingSensorData(IN std::shared_ptr<VS_Resource> resource, const int& eCode, const int& sequenceNumber) = 0;
		virtual void onPutActuatorData(IN std::shared_ptr<VS_Resource> resource, const int& eCode) = 0;
};

class VS_Basic
{

	private:
		int host_key;
		static VS_Basic* singleton;
		std::shared_ptr<OCResource> m_resource;
		std::shared_ptr<VS_Basic_Callbacks> m_callbacks;
//		void parseRep2SensorData_priv(const OCRepresentation& rep, OUT VS_Resource& sensorData);
		VS_Basic();
		~VS_Basic();
		
	public:
		static VS_Basic* get();

		static void free();
		
		void setCallbacks(std::shared_ptr<VS_Basic_Callbacks> callbacks);
		int ConnectGW();
		void setResource(std::shared_ptr<OCResource> res);
		int GetDeviceList();


		int GetSensorInfo(IN const std::string SUri);

		int GetSensorData(IN const std::string SUri, const int epoch);
		int PutActuatorData(IN const std::string SUri, IN const std::string attrName, IN const std::string attrValue);
		int PutActuatorData(IN const std::string SUri, IN OCRepresentation &rep);
		int SetPolicy(IN const std::string SUri, IN int rawDtAcqType, IN int rawLevel, IN int netDtAcqType, IN int netLevel);
		int StartObservingSensorData(IN const std::string SUri);

		

		//Following methods can be overrided.
		void onConnected();
		void onGetDeviceList(IN std::shared_ptr<VS_DeviceList> deviceList, const int eCode);
		void onGetSensorInfo(IN std::shared_ptr<VS_Resource> resource, const int eCode);
		void onGetSensorData(IN std::shared_ptr<VS_Resource> resource, const int eCode);
		void onObservingSensorData(IN std::shared_ptr<VS_Resource> resource, const int& eCode, const int& sequenceNumber);
		void onPutActuatorData(IN std::shared_ptr<VS_Resource> resource, const int eCode);
}; // VS_Basic

#endif
