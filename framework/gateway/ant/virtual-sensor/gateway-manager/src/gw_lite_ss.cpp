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

/// @file gw_lite_ss.cpp

/// @brief  This file contains the implementation of classes and its members
///         related to Gateway lite sensor manager

#include <string>

#include "gw_lite_ss.h"
#include "sql_wrapper.h"
#include "gw_sdm.h"


void ss_initialize(std::string suri)
{
    
	Discomfort_Initialize(suri);
    Sleep_Initialize(suri);
    Inandout_Initialize(suri);
    Blackout_Initialize(suri);
    FireAlarm_Initialize(suri);
    Leakage_Initialize(suri);
    Ventilation_Initialize(suri);
    Sunshine_Initialize(suri);
    PhoneMotion_Initialize(suri);    
}
void ss_check(std::string suri)
{    
    Discomfort_Check(suri);
    Sleep_Check(suri);
    Inandout_Check(suri);
    Blackout_Check(suri);
    FireAlarm_Check(suri);
    Leakage_Check(suri);
    Ventilation_Check(suri);
    Sunshine_Check(suri);
    PhoneMotion_Check(suri);
}

///////////////////////////////Discomfort Sensor implemenatation


void Discomfort_Initialize(std::string suri)
{

    static int oduinoSet = 0;
	static int thSet = 0;
        
    static int disInit = 0;

    if(disInit || (suri != "/a/oduino" && suri != "/a/th"))
        return;
     
	if(suri == "/a/oduino")
	{
		oduinoSet = 1;
	}
	else if(suri == "/a/th")
	{
		thSet = 1;
	}

	if(oduinoSet && thSet && disInit == 0)
	{
		disInit = 1;
	}
    else
        return;
    
    GW_Resource *res = NULL;

    gw_log("/ss/discomfort initialized");
	GW_SensorDeviceManager::getInstance()->GetResource("/ss/discomfort", &res);
	if(NULL == res)
	{
		std::string xml;
		GW_SensorDeviceManager::getInstance()->AddResource("/ss/discomfort", NULL, &res);
		if(res == NULL)
			gw_log("what");

		res->makeXML(xml);
//			gw_log("%s d ", xml.c_str());
		res->setAttribute("level", "All people are comforted");
		sensorDB db;
		db.suri = "/ss/discomfort";
		db.ttl = 1;
		db.index = 0;
		db.attribute = "level";
		db.attributeType = TYPE_INTEGER;
		db.value = "All people are comforted";

//		gw_log("%s, %s, %s", db.suri.c_str(), db.attribute.c_str(), db.value.c_str());


		DBManager::getInstance()->getSensorAttribute(&db);
//			gw_log("11");

		if(db.index <= 0){
//				gw_log("%s Added", db.attribute.c_str());
			DBManager::getInstance()->addSensorAttribute(&db);
		}

		DBManager::getInstance()->addSensorData(&db);

		
		GW_SensorDeviceManager::getInstance()->GetDeviceListXML(xml);
//			gw_log("%s added", xml.c_str());
	}
    else
        gw_log("This should not happen");
}


void Discomfort_Check(std::string sUri)
{
    if(sUri == "/a/oduino" || sUri == "/a/th")
	{
        GW_Resource *gw_res;
        int ret = GW_SensorDeviceManager::getInstance()->GetResource("/ss/discomfort", &gw_res);
        if(ret == VSF_S_OK && gw_res != NULL)
    		Discomfort_Update();
	}
}

void Discomfort_Update()
{
	GW_Resource *ss_res;
	GW_Resource *res, *res1;
    double sumDI = 0.0;
	static std::string prev_level = "All people are comforted";
	std::string curr_level;
	int tmp_od, tmp_th, hum_od, hum_th, ret;
    double f_tmp_od, f_tmp_th;

    const int x_od = 999;
    const int y_od = 999;
    const int x_air = 500;
    const int y_air = 500;
    const int x_th = 0;
    const int y_th = 200;

    const int tmp_air = 24;
    const double f_tmp_air = (9.0 * (double) tmp_air) / 5.0 + 32.0;
    const int hum_air = 40;
	
	ret = GW_SensorDeviceManager::getInstance()->GetResource("/ss/discomfort", &ss_res);
	ret = GW_SensorDeviceManager::getInstance()->GetResource("/a/oduino", &res);
	ret = GW_SensorDeviceManager::getInstance()->GetResource("/a/th", &res1);
    
	if(ss_res == NULL || res == NULL || res1 == NULL)
	{
		gw_log("No softsensor %d, %d, %d", ss_res, res, res1);
	}
	
	std::map<std::string, std::string> attrs;
	sensorDB db;
	db.suri = "/a/oduino";
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	std::map<std::string, std::string>::iterator it = attrs.find("temp");
	if(it == attrs.end()){
		gw_log("End");
		tmp_od = 0;
	}
	else
	{
		std::string temp = it->second;
		tmp_od = atoi(temp.c_str());
	}

	it = attrs.find("humi");
	if(it == attrs.end())
	{
		gw_log("End");	
		hum_od = 0;
	}
	else
	{
		std::string humi = it->second;
		hum_od = atoi(humi.c_str());
	}

    f_tmp_od = (9.0 * (double) tmp_od) / 5.0 + 32.0;
    
	db.suri = "/a/th";
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("temp");
	if(it == attrs.end()){
		gw_log("End");
		tmp_th = 0;
	}
	else
	{
		std::string temp = it->second;
		tmp_th = atoi(temp.c_str());
	}

	it = attrs.find("humi");
	if(it == attrs.end())
	{
		gw_log("End");	
		hum_th = 0;
	}
	else
	{
		std::string humi = it->second;
		hum_th = atoi(humi.c_str());
	}

    f_tmp_th = (9.0 * (double) tmp_th) / 5.0 + 32.0;

    int num = 0;
    for(int i=0; i<1000; i++)
    {
        for (int j=0; j<1000; j++)
        {
            double d1 = sqrt(pow( i-x_od, 2) + pow( j-y_od , 2));
            double d2 = sqrt(pow( i-x_th, 2) + pow( j-y_th , 2));
            double d3 = sqrt(pow( i-x_air, 2) + pow( j-y_air, 2));
            double f_tmp_here = (f_tmp_od * d2 * d3) / (d2*d3 + d1*d3 + d1*d2) + \
                (f_tmp_th *d1*d3) / (d2*d3 + d1*d3 + d1*d2) + \
                (f_tmp_air * d1 * d2) / (d2*d3 + d1*d3 + d1*d2);
            double hum_here = ((double)hum_od * d2 * d3) / (d2*d3 + d1*d3 + d1*d2) + \
                ((double)hum_th *d1*d3) / (d2*d3 + d1*d3 + d1*d2) + \
                ((double)hum_air * d1 * d2) / (d2*d3 + d1*d3 + d1*d2);

            sumDI += f_tmp_here- (f_tmp_here - 58.0) * (double) ((100 - hum_here) * 55) / 10000.0;
            num++;
        }
    }

    sumDI /= (double)num;
    
	if(sumDI >= 80.0)
	{
		curr_level = "All people are discomforted";
	}
	else if(sumDI >= 75.0)
	{
		curr_level = "Half of people are discomforted";
	}
	else if(sumDI >= 68.0)
	{
    	curr_level = "Only few people are discomforted";
	}
	else
	{
		curr_level = "All people are comforted";
    	
	}

    ss_res->setAttribute("level", curr_level);
	db.suri = "/ss/discomfort";
	db.ttl = 1;
	db.index = 0;
	db.attribute = "level";
	db.attributeType = TYPE_INTEGER;
	db.value = curr_level;
	DBManager::getInstance()->addSensorData(&db);
    
	if(prev_level != curr_level)
	{
		ss_res->observationDirty();
		prev_level = curr_level;
	}

	if(ret != VSF_S_OK)
	{
		gw_log("VSF_S_OK is not set on return value");
	}
}
///////////////////////////////~Discomfort Sensor implemenatation

//////////////////////////////Sleep Sensor Implementation
void Sleep_Initialize(std::string suri)
{
    static int sofaSet = 0;
	static int bedSet = 0;
    static int bedMotionSet = 0;
    static int livingMotionSet = 0;
    static int heartBeatSet = 0;
    static int postureSet = 0;
        
    static int sleepInit = 0;

    if(sleepInit || (suri != SS_SENSOR_4 && suri != SS_SENSOR_10 && suri != SS_SENSOR_11 &&
                    suri != SS_SENSOR_13 && suri != SS_MOTION_1 && suri != SS_MOTION_4))
        return;
     
	if(suri == SS_SENSOR_4)
	{
		sofaSet = 1;
	}
	else if(suri == SS_SENSOR_10)
	{
		bedSet = 1;
	}
    else if(suri == SS_SENSOR_11)
	{
		heartBeatSet = 1;
	}
    else if(suri == SS_SENSOR_13)
	{
		postureSet = 1;
	}
    else if(suri == SS_MOTION_1)
	{
		bedMotionSet = 1;
	}
    else if(suri == SS_MOTION_4)
	{
		livingMotionSet = 1;
	}

	if(sofaSet && bedSet && heartBeatSet && postureSet \
        && bedMotionSet && livingMotionSet && sleepInit == 0)
	{
		sleepInit = 1;
	}
    else
        return;
    
    GW_Resource *res = NULL;

	GW_SensorDeviceManager::getInstance()->GetResource("/ss/sleepSensor", &res);
	if(NULL == res)
	{
		std::string xml;
		GW_SensorDeviceManager::getInstance()->AddResource("/ss/sleepSensor", NULL, &res);
		if(res == NULL)
			gw_log("what");

		res->makeXML(xml);
//			gw_log("%s d ", xml.c_str());
		res->setAttribute("state", "Awaken");
		sensorDB db;
		db.suri = "/ss/sleepSensor";
		db.ttl = 1;
		db.index = 0;
		db.attribute = "state";
		db.attributeType = TYPE_INTEGER;
		db.value = "Awaken";

//		gw_log("%s, %s, %s", db.suri.c_str(), db.attribute.c_str(), db.value.c_str());


		DBManager::getInstance()->getSensorAttribute(&db);
//			gw_log("11");

		if(db.index <= 0){
//				gw_log("%s Added", db.attribute.c_str());
			DBManager::getInstance()->addSensorAttribute(&db);
		}

		DBManager::getInstance()->addSensorData(&db);

		
		GW_SensorDeviceManager::getInstance()->GetDeviceListXML(xml);
//			gw_log("%s added", xml.c_str());
	}
    else
        gw_log("This should not happen"); 

    gw_log("Sleep Sensor initialized");
}
void Sleep_Check(std::string suri)
{
    if(suri == SS_SENSOR_4 || suri == SS_SENSOR_10 || suri == SS_SENSOR_11 ||
                    suri == SS_SENSOR_13 || suri == SS_MOTION_1 || suri == SS_MOTION_4)
	{
        GW_Resource *gw_res;
        int ret = GW_SensorDeviceManager::getInstance()->GetResource("/ss/sleepSensor", &gw_res);
        if(ret == VSF_S_OK && gw_res != NULL)
    		Sleep_Update();
	}
}
void Sleep_Update()
{
    int sofaPressure, bedPressure, bpm, motion1, motion4;
    int posture;
    
    std::map<std::string, std::string> attrs;
	sensorDB db;

    static std::string prev_state;
    std::string curr_state;

    GW_Resource *ss_res = NULL;

    int ret = GW_SensorDeviceManager::getInstance()->GetResource("/ss/sleepSensor", &ss_res);
    
    if(ret != VSF_S_OK || ss_res == NULL)
    {
        gw_log("Sleep Sensor has not been initialized");
        return;
    }
    
    
    db.suri = SS_SENSOR_4;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	std::map<std::string, std::string>::iterator it = attrs.find("pressure");
	if(it == attrs.end()){
		gw_log("End");
		sofaPressure = 0;
	}
	else
	{
		std::string temp = it->second;
		sofaPressure = atoi(temp.c_str());
	}

    db.suri = SS_SENSOR_10;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("pressure");
	if(it == attrs.end()){
		gw_log("End");
		bedPressure = 0;
	}
	else
	{
		std::string temp = it->second;
		bedPressure = atoi(temp.c_str());
	}

    db.suri = SS_SENSOR_11;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("bpm");
	if(it == attrs.end()){
		gw_log("End");
		bpm = 0;
	}
	else
	{
		std::string temp = it->second;
		bpm = atoi(temp.c_str());
	}

    db.suri = SS_SENSOR_13;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("posture");
	if(it == attrs.end()){
		gw_log("End");
		posture = PS_STAND;
	}
	else
	{
        std::string temp = it->second;
		posture = atoi(temp.c_str());
	}

    db.suri = SS_MOTION_1;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("motion");
	if(it == attrs.end()){
		gw_log("End");
		motion1 = 0;
	}
	else
	{
		std::string temp = it->second;
		motion1 = atoi(temp.c_str());
	}

    db.suri = SS_MOTION_4;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("motion");
	if(it == attrs.end()){
		gw_log("End");
		motion4 = 0;
	}
	else
	{
		std::string temp = it->second;
		motion4 = atoi(temp.c_str());
	}
    if(bpm < 70 && posture == PS_LYING)
    {
        if(bedPressure && motion1 == 1)
        {
            curr_state = "Sleep on the bed";
        }
        else if(sofaPressure && motion4 == 1)
        {
            curr_state = "Sleep on the sofa";
        }
        else
        {
            curr_state = "Sleep in somewhere";
        }
    }
    else
    {
        curr_state = "Awaken";
        
    }

    ss_res->setAttribute("state", curr_state);
    db.suri = "/ss/sleepSensor";
	db.ttl = 1;
	db.index = 0;
	db.attribute = "state";
	db.attributeType = TYPE_INTEGER;
	db.value = curr_state;
	DBManager::getInstance()->addSensorData(&db);
    if(prev_state != curr_state)
    {
        gw_log("Sleep : %s -> %s", prev_state.c_str(), curr_state.c_str());
        ss_res->observationDirty();
		prev_state = curr_state;
    }
}
//////////////////////////////~Sleep Sensor Implementataion

//////////////////////////////Inandout Sensor Implementation
void Inandout_Initialize(std::string suri)
{
    static int motion2Set = 0;
    static int doorSet = 0;
        
    static int inandoutInit = 0;

    if(inandoutInit || (suri != SS_SENSOR_1 && suri != SS_MOTION_2))
        return;
     
	if(suri == SS_SENSOR_1)
	{
		doorSet = 1;
	}
	else if(suri == SS_MOTION_2)
	{
		motion2Set = 1;
	}
   
	if(doorSet && motion2Set && inandoutInit == 0)
	{
		inandoutInit = 1;
	}
    else
        return;
    
    GW_Resource *res = NULL;

	GW_SensorDeviceManager::getInstance()->GetResource("/ss/inandoutSensor", &res);
	if(NULL == res)
	{
		std::string xml;
		GW_SensorDeviceManager::getInstance()->AddResource("/ss/inandoutSensor", NULL, &res);
		if(res == NULL)
			gw_log("what");
//			gw_log("%s d ", xml.c_str());
		res->setAttribute("state", "in");
		sensorDB db;
		db.suri = "/ss/inandoutSensor";
		db.ttl = 1;
		db.index = 0;
		db.attribute = "state";
		db.attributeType = TYPE_INTEGER;
		db.value = "in";

//		gw_log("%s, %s, %s", db.suri.c_str(), db.attribute.c_str(), db.value.c_str());


		DBManager::getInstance()->getSensorAttribute(&db);
//			gw_log("11");

		if(db.index <= 0){
//				gw_log("%s Added", db.attribute.c_str());
			DBManager::getInstance()->addSensorAttribute(&db);
		}

		DBManager::getInstance()->addSensorData(&db);

		
		GW_SensorDeviceManager::getInstance()->GetDeviceListXML(xml);
//			gw_log("%s added", xml.c_str());
	}
    else
        gw_log("This should not happen"); 

    gw_log("Inandout Sensor initialized");
}
void Inandout_Check(std::string suri)
{
    if(suri == SS_SENSOR_1 || suri == SS_MOTION_2)
	{
        GW_Resource *gw_res;
        int ret = GW_SensorDeviceManager::getInstance()->GetResource("/ss/inandoutSensor", &gw_res);
        if(ret == VSF_S_OK && gw_res != NULL)
    		Inandout_Update();
	}
}
void Inandout_Update()
{
    int doorSensor, motion2;
    static int prevDoorSensor = 0;
    
    std::map<std::string, std::string> attrs;
	sensorDB db;

    static std::string prev_state = "in";
    std::string curr_state;

    GW_Resource *ss_res = NULL;

    int ret = GW_SensorDeviceManager::getInstance()->GetResource("/ss/inandoutSensor", &ss_res);
    
    if(ret != VSF_S_OK || ss_res == NULL)
    {
        gw_log("Inandout Sensor has not been initialized");
        return;
    }
    
    
    db.suri = SS_SENSOR_1;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	std::map<std::string, std::string>::iterator it = attrs.find("state");
	if(it == attrs.end()){
		gw_log("End");
		doorSensor = 0;
	}
	else
	{
		std::string temp = it->second;
		doorSensor = atoi(temp.c_str());
	}

    db.suri = SS_MOTION_2;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("motion");
	if(it == attrs.end()){
		gw_log("End");
		motion2 = 0;
	}
	else
	{
		std::string temp = it->second;
		motion2 = atoi(temp.c_str());
	}

    if(doorSensor != prevDoorSensor)
    {
        prevDoorSensor = doorSensor;
        if(motion2 && doorSensor == 0)
        {
            curr_state = "in";
        }
        else if(motion2 == 0 && doorSensor == 0)
        {
            curr_state = "out";
        }
        else
            curr_state = prev_state;
    }
    else
    {
        curr_state = prev_state;
    }


    ss_res->setAttribute("state", curr_state);
	db.suri = "/ss/inandoutSensor";
	db.ttl = 1;
	db.index = 0;
	db.attribute = "state";
	db.attributeType = TYPE_INTEGER;
	db.value = curr_state;
	DBManager::getInstance()->addSensorData(&db);
    if(prev_state != curr_state)
    {
        gw_log("InandOut : %s -> %s", prev_state.c_str(), curr_state.c_str());
        ss_res->observationDirty();
		prev_state = curr_state;
    }
}
//////////////////////////////~InandOut Implementataion

//////////////////////////////Blackout Sensor Implementation
void Blackout_Initialize(std::string suri)
{        
    static int blackoutInit = 0;

    if(blackoutInit || ((suri.find("/outlet") == std::string::npos) && suri.find("/light") == std::string::npos))
        return;
     
	   
	if(blackoutInit == 0)
	{
		blackoutInit = 1;
	}
    else
        return;
    
    GW_Resource *res = NULL;

	GW_SensorDeviceManager::getInstance()->GetResource("/ss/blackoutSensor", &res);
	if(NULL == res)
	{
		std::string xml;
		GW_SensorDeviceManager::getInstance()->AddResource("/ss/blackoutSensor", NULL, &res);
		if(res == NULL)
			gw_log("what");

		res->makeXML(xml);
//			gw_log("%s d ", xml.c_str());
		res->setAttribute("state", "Ok");
		sensorDB db;
		db.suri = "/ss/blackoutSensor";
		db.ttl = 1;
		db.index = 0;
		db.attribute = "state";
		db.attributeType = TYPE_INTEGER;
		db.value = "Ok";

//		gw_log("%s, %s, %s", db.suri.c_str(), db.attribute.c_str(), db.value.c_str());


		DBManager::getInstance()->getSensorAttribute(&db);
//			gw_log("11");

		if(db.index <= 0){
//				gw_log("%s Added", db.attribute.c_str());
			DBManager::getInstance()->addSensorAttribute(&db);
		}

		DBManager::getInstance()->addSensorData(&db);

		
		GW_SensorDeviceManager::getInstance()->GetDeviceListXML(xml);
//			gw_log("%s added", xml.c_str());
	}
    else
        gw_log("This should not happen"); 

    gw_log("Black-out Sensor initialized");
}
void Blackout_Check(std::string suri)
{
    if(suri.find("/outlet") != std::string::npos || suri.find("/light") != std::string::npos)
	{
        GW_Resource *gw_res;
        int ret = GW_SensorDeviceManager::getInstance()->GetResource("/ss/blackoutSensor", &gw_res);
        if(ret == VSF_S_OK && gw_res != NULL)
    		Blackout_Update();
	}
}
void Blackout_Update()
{
    int numOutlet, numLight;

    numOutlet =0;
    numLight =0;
    
    std::map<std::string, std::string> attrs;
	sensorDB db;

    static std::string prev_state = "Ok";
    std::string curr_state;

    GW_Resource *ss_res = NULL;

    int ret = GW_SensorDeviceManager::getInstance()->GetResource("/ss/blackoutSensor", &ss_res);
    
    if(ret != VSF_S_OK || ss_res == NULL)
    {
        gw_log("Black-out Sensor has not been initialized");
        return;
    }
    
    
    db.suri = SS_ELEC_1;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	std::map<std::string, std::string>::iterator it = attrs.find("power");
	if(it == attrs.end()){
		gw_log("End");
	}
	else
	{
		std::string temp = it->second;
		numOutlet += atoi(temp.c_str());
	}

     db.suri = SS_ELEC_2;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("power");
	if(it == attrs.end()){
		gw_log("End");
	}
	else
	{
		std::string temp = it->second;
		numOutlet += atoi(temp.c_str());
	}

     db.suri = SS_ELEC_3;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("power");
	if(it == attrs.end()){
		gw_log("End");
	}
	else
	{
		std::string temp = it->second;
		numOutlet += atoi(temp.c_str());
	}

     db.suri = SS_ELEC_4;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("power");
	if(it == attrs.end()){
		gw_log("End");
	}
	else
	{
		std::string temp = it->second;
		numOutlet += atoi(temp.c_str());
	}

     db.suri = SS_ELEC_5;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("power");
	if(it == attrs.end()){
		gw_log("End");
	}
	else
	{
		std::string temp = it->second;
		numOutlet += atoi(temp.c_str());
	}

     db.suri = SS_ELEC_6;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("power");
	if(it == attrs.end()){
		gw_log("End");
	}
	else
	{
		std::string temp = it->second;
		numOutlet += atoi(temp.c_str());
	}

     db.suri = SS_ELEC_7;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("power");
	if(it == attrs.end()){
		gw_log("End");
	}
	else
	{
		std::string temp = it->second;
		numOutlet += atoi(temp.c_str());
	}

    db.suri = SS_ELEC_8;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("power");
	if(it == attrs.end()){
		gw_log("End");
	}
	else
	{
		std::string temp = it->second;
		numOutlet += atoi(temp.c_str());
	}
    
    db.suri = SS_LIGHT_1;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("power");
	if(it == attrs.end()){
		gw_log("End");
	}
	else
	{
		std::string temp = it->second;
		numLight += atoi(temp.c_str());
	}

    db.suri = SS_LIGHT_2;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("power");
	if(it == attrs.end()){
		gw_log("End");
	}
	else
	{
		std::string temp = it->second;
		numLight += atoi(temp.c_str());
	}

    db.suri = SS_LIGHT_3;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("power");
	if(it == attrs.end()){
		gw_log("End");
	}
	else
	{
		std::string temp = it->second;
		numLight += atoi(temp.c_str());
	}

    db.suri = SS_LIGHT_4;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("power");
	if(it == attrs.end()){
		gw_log("End");
	}
	else
	{
		std::string temp = it->second;
		numLight += atoi(temp.c_str());
	}

    db.suri = SS_LIGHT_5;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("power");
	if(it == attrs.end()){
		gw_log("End");
	}
	else
	{
		std::string temp = it->second;
		numLight += atoi(temp.c_str());
	}

    db.suri = SS_LIGHT_6;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("power");
	if(it == attrs.end()){
		gw_log("End");
	}
	else
	{
		std::string temp = it->second;
		numLight += atoi(temp.c_str());
	}

    if(numOutlet + numLight > 12)
    {
        curr_state = "Warning: Using too much electricity";
    }
    else if(numOutlet > 6)
    {
        curr_state = "Too many outlets on";
    }
    else if(numLight > 4)
    {
        curr_state = "Too many light bulbs on";
    }
    else
    {
        curr_state = "Ok";
    }

    ss_res->setAttribute("state", curr_state);
	db.suri = "/ss/blackoutSensor";
	db.ttl = 1;
	db.index = 0;
	db.attribute = "state";
	db.attributeType = TYPE_INTEGER;
	db.value = curr_state;
	DBManager::getInstance()->addSensorData(&db);
    if(prev_state != curr_state)
    {
        gw_log("BlackOut : %s -> %s", prev_state.c_str(), curr_state.c_str());
        ss_res->observationDirty();
		prev_state = curr_state;
    }
}
//////////////////////////////~BlackOut Implementataion

//////////////////////////////FirAlarm Sensor Implementation
void FireAlarm_Initialize(std::string suri)
{
    static int infraredSet = 0;
    static int valveSet = 0;
        
    static int firealarmInit = 0;

    if(firealarmInit || (suri != SS_SENSOR_2 && suri != SS_SENSOR_3 && suri.find("/motion") == std::string::npos))
        return;
     
	if(suri == SS_SENSOR_2)
	{
		infraredSet = 1;
	}
	else if(suri == SS_SENSOR_3)
	{
		valveSet = 1;
	}
   
	if(infraredSet && valveSet && firealarmInit == 0)
	{
		firealarmInit = 1;
	}
    else
        return;
    
    GW_Resource *res = NULL;

	GW_SensorDeviceManager::getInstance()->GetResource("/ss/firealarmSensor", &res);
	if(NULL == res)
	{
		std::string xml;
		GW_SensorDeviceManager::getInstance()->AddResource("/ss/firealarmSensor", NULL, &res);
		if(res == NULL)
			gw_log("what");

		res->makeXML(xml);
//			gw_log("%s d ", xml.c_str());
		res->setAttribute("state", "Ok");
		sensorDB db;
		db.suri = "/ss/firealarmSensor";
		db.ttl = 1;
		db.index = 0;
		db.attribute = "state";
		db.attributeType = TYPE_INTEGER;
		db.value = "Ok";

//		gw_log("%s, %s, %s", db.suri.c_str(), db.attribute.c_str(), db.value.c_str());


		DBManager::getInstance()->getSensorAttribute(&db);
//			gw_log("11");

		if(db.index <= 0){
//				gw_log("%s Added", db.attribute.c_str());
			DBManager::getInstance()->addSensorAttribute(&db);
		}

		DBManager::getInstance()->addSensorData(&db);

		
		GW_SensorDeviceManager::getInstance()->GetDeviceListXML(xml);
//			gw_log("%s added", xml.c_str());
	}
    else
        gw_log("This should not happen"); 

    gw_log("FireAlarm Sensor initialized");
}
void FireAlarm_Check(std::string suri)
{
    if(suri == SS_SENSOR_2 || suri == SS_SENSOR_3 || suri.find("/motion") != std::string::npos)
	{
        GW_Resource *gw_res;
        int ret = GW_SensorDeviceManager::getInstance()->GetResource("/ss/firealarmSensor", &gw_res);
        if(ret == VSF_S_OK && gw_res != NULL)
    		FireAlarm_Update();
	}
}
void FireAlarm_Update()
{
    int sensor2, sensor3, motion5;
    
    std::map<std::string, std::string> attrs;
	sensorDB db;

    static std::string prev_state = "Ok";
    std::string curr_state;

    GW_Resource *ss_res = NULL;

    int ret = GW_SensorDeviceManager::getInstance()->GetResource("/ss/firealarmSensor", &ss_res);
    
    if(ret != VSF_S_OK || ss_res == NULL)
    {
        gw_log("FireAlarm Sensor has not been initialized");
        return;
    }
    
    
    db.suri = SS_SENSOR_2;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	std::map<std::string, std::string>::iterator it = attrs.find("lux");
	if(it == attrs.end()){
		gw_log("End");
		sensor2 = 0;
	}
	else
	{
		std::string temp = it->second;
		sensor2 = atoi(temp.c_str());
	}

    db.suri = SS_SENSOR_3;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("state");
	if(it == attrs.end()){
		gw_log("End");
		sensor3 = 0;
	}
	else
	{
		std::string temp = it->second;
		sensor3 = atoi(temp.c_str());
	}

    db.suri = SS_MOTION_5;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);

	it = attrs.find("motion");
	if(it == attrs.end()){
		gw_log("End");
		motion5 = 0;
	}
	else
	{
		std::string temp = it->second;
		motion5 = atoi(temp.c_str());
	}
    if(sensor3 == 1 && motion5 == 0)
    {
        if(sensor2 > 80)
        {
            curr_state = "Warning: Very dangerous";
        }
        else
        {
            curr_state = "Warning: Dangerous";
        }
    }
    else
        curr_state = "Ok";
    
	ss_res->setAttribute("state", curr_state);
	db.suri = "/ss/firealarmSensor";
	db.ttl = 1;
	db.index = 0;
	db.attribute = "state";
	db.attributeType = TYPE_INTEGER;
	db.value = curr_state;
	DBManager::getInstance()->addSensorData(&db);
    if(prev_state != curr_state)
    {
        gw_log("BlackOut : %s -> %s", prev_state.c_str(), curr_state.c_str());
        ss_res->observationDirty();
		prev_state = curr_state;
    }
}
//////////////////////////////~FireAlarm Implementataion

//////////////////////////////Leakage Sensor Implementation
void Leakage_Initialize(std::string suri)
{
    static int damp1Set = 0;
    static int damp2Set = 0;
        
    static int leakageInit = 0;

    if(leakageInit || (suri != SS_SENSOR_6 && suri != SS_SENSOR_7))
        return;
     
	if(suri == SS_SENSOR_6)
	{
		damp1Set = 1;
	}
	else if(suri == SS_SENSOR_7)
	{
		damp2Set = 1;
	}
   
	if(damp1Set && damp2Set && leakageInit == 0)
	{
		leakageInit = 1;
	}
    else
        return;
    
    GW_Resource *res = NULL;

	GW_SensorDeviceManager::getInstance()->GetResource("/ss/leakageSensor", &res);
	if(NULL == res)
	{
		std::string xml;
		GW_SensorDeviceManager::getInstance()->AddResource("/ss/leakageSensor", NULL, &res);
		if(res == NULL)
			gw_log("what");

		res->makeXML(xml);
//			gw_log("%s d ", xml.c_str());
		res->setAttribute("state", "Ok");
		sensorDB db;
		db.suri = "/ss/leakageSensor";
		db.ttl = 1;
		db.index = 0;
		db.attribute = "state";
		db.attributeType = TYPE_INTEGER;
		db.value = "Ok";

//		gw_log("%s, %s, %s", db.suri.c_str(), db.attribute.c_str(), db.value.c_str());


		DBManager::getInstance()->getSensorAttribute(&db);
//			gw_log("11");

		if(db.index <= 0){
//				gw_log("%s Added", db.attribute.c_str());
			DBManager::getInstance()->addSensorAttribute(&db);
		}

		DBManager::getInstance()->addSensorData(&db);

		
		GW_SensorDeviceManager::getInstance()->GetDeviceListXML(xml);
//			gw_log("%s added", xml.c_str());
	}
    else
        gw_log("This should not happen"); 

    gw_log("Leakage Sensor initialized");
}
void Leakage_Check(std::string suri)
{
    if(suri == SS_SENSOR_6 || suri == SS_SENSOR_7)
	{
        GW_Resource *gw_res;
        int ret = GW_SensorDeviceManager::getInstance()->GetResource("/ss/leakageSensor", &gw_res);
        if(ret == VSF_S_OK && gw_res != NULL)
    		Leakage_Update();
	}
}
void Leakage_Update()
{
    int sensor6, sensor7;
    
    std::map<std::string, std::string> attrs;
	sensorDB db;

    static std::string prev_state = "Ok";
    std::string curr_state;

    GW_Resource *ss_res = NULL;

    int ret = GW_SensorDeviceManager::getInstance()->GetResource("/ss/leakageSensor", &ss_res);
    
    if(ret != VSF_S_OK || ss_res == NULL)
    {
        gw_log("Leakage Sensor has not been initialized");
        return;
    }
    
    db.suri = SS_SENSOR_6;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);
	std::map<std::string, std::string>::iterator it = attrs.find("damp");
	if(it == attrs.end()){
		gw_log("End");
		sensor6 = 0;
	}
	else
	{
		std::string temp = it->second;
		sensor6 = atoi(temp.c_str());
	}

    db.suri = SS_SENSOR_7;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);
	it = attrs.find("damp");
	if(it == attrs.end()){
		gw_log("End");
		sensor7 = 0;
	}
	else
	{
		std::string temp = it->second;
		sensor7 = atoi(temp.c_str());
	}

    int avrDamp = (sensor6 + sensor7) / 2;
    if(avrDamp > 80)
    {
         curr_state = "It's leaking";
    }
    else if(avrDamp > 60)
    {
        curr_state = "It's leaking soon";
    }
    else
    {
        curr_state = "Ok";
    }
    
	ss_res->setAttribute("state", curr_state);
	db.suri = "/ss/leakageSensor";
	db.ttl = 1;
	db.index = 0;
	db.attribute = "state";
	db.attributeType = TYPE_INTEGER;
	db.value = curr_state;
	DBManager::getInstance()->addSensorData(&db);
    if(prev_state != curr_state)
    {
        gw_log("Leakage : %s -> %s", prev_state.c_str(), curr_state.c_str());
        ss_res->observationDirty();
		prev_state = curr_state;
    }
}
//////////////////////////////~Leakage Implementataion

//////////////////////////////Ventilation Sensor Implementation
void Ventilation_Initialize(std::string suri)
{
    static int thSet = 0;
        
    static int ventilationInit = 0;

    if(ventilationInit || (suri != SS_SENSOR_9))
        return;
     
	if(suri == SS_SENSOR_9)
	{
		thSet = 1;
	}
   
	if(thSet && ventilationInit == 0)
	{
		ventilationInit = 1;
	}
    else
        return;
    
    GW_Resource *res = NULL;

	GW_SensorDeviceManager::getInstance()->GetResource("/ss/ventilationSensor", &res);
	if(NULL == res)
	{
		std::string xml;
		GW_SensorDeviceManager::getInstance()->AddResource("/ss/ventilationSensor", NULL, &res);
		if(res == NULL)
			gw_log("what");

		res->makeXML(xml);
		res->setAttribute("state", "Ok");
		sensorDB db;
		db.suri = "/ss/ventilationSensor";
		db.ttl = 1;
		db.index = 0;
		db.attribute = "state";
		db.attributeType = TYPE_INTEGER;
		db.value = "Ok";

		DBManager::getInstance()->getSensorAttribute(&db);
//			gw_log("11");

		if(db.index <= 0){
//				gw_log("%s Added", db.attribute.c_str());
			DBManager::getInstance()->addSensorAttribute(&db);
		}

		DBManager::getInstance()->addSensorData(&db);

		
//		GW_SensorDeviceManager::getInstance()->GetDeviceListXML(xml);
//			gw_log("%s added", xml.c_str());
	}
    else
        gw_log("This should not happen"); 

    gw_log("Ventilation Sensor initialized");
}
void Ventilation_Check(std::string suri)
{
    if(suri == SS_SENSOR_9)
	{
        GW_Resource *gw_res;
        int ret = GW_SensorDeviceManager::getInstance()->GetResource("/ss/ventilationSensor", &gw_res);
        if(ret == VSF_S_OK && gw_res != NULL)
    		Ventilation_Update();
	}
}
void Ventilation_Update()
{
    int sensor9_temp, sensor9_humi;
    
    std::map<std::string, std::string> attrs;
	sensorDB db;

    static std::string prev_state = "Ok";
    std::string curr_state;

    GW_Resource *ss_res = NULL;

    int ret = GW_SensorDeviceManager::getInstance()->GetResource("/ss/ventilationSensor", &ss_res);
    
    if(ret != VSF_S_OK || ss_res == NULL)
    {
        gw_log("Ventilation Sensor has not been initialized");
        return;
    }
    
    db.suri = SS_SENSOR_9;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);
	std::map<std::string, std::string>::iterator it = attrs.find("temp");
	if(it == attrs.end()){
		gw_log("End");
		sensor9_temp = 0;
	}
	else
	{
		std::string temp = it->second;
		sensor9_temp = atoi(temp.c_str());
	}

    it = attrs.find("humi");
	if(it == attrs.end()){
		gw_log("End");
		sensor9_humi = 0;
	}
	else
	{
		std::string temp = it->second;
		sensor9_humi = atoi(temp.c_str());
	}

    double f_tmp = (9.0 * (double) sensor9_temp) / 5.0 + 32.0;
    double dI = f_tmp- (f_tmp - 58.0) * (double) ((100 - sensor9_humi) * 55) / 10000.0;
    
    if(dI >= 80.0)
	{
		curr_state = "No ventilation";
	}
	else if(dI >= 75.0)
	{
		curr_state = "Recommand not to ventilate";
	}
	else if(dI >= 68.0)
	{
    	curr_state = "Recommand to ventilate";
	}
	else
	{
		curr_state = "Ok";
	}
    
	ss_res->setAttribute("state", curr_state);
	db.suri = "/ss/ventilationSensor";
	db.ttl = 1;
	db.index = 0;
	db.attribute = "state";
	db.attributeType = TYPE_INTEGER;
	db.value = curr_state;
	DBManager::getInstance()->addSensorData(&db);
    if(prev_state != curr_state)
    {
        gw_log("Ventilation : %s -> %s", prev_state.c_str(), curr_state.c_str());
        ss_res->observationDirty();
		prev_state = curr_state;
    }
}
//////////////////////////////~Ventilation Implementataion

//////////////////////////////Sunshine Sensor Implementation
void Sunshine_Initialize(std::string suri)
{
    static int thSet = 0;
    static int sunSet = 0;
        
    static int sunshineInit = 0;
  
    if(sunshineInit || ((suri != SS_SENSOR_9) && (suri != SS_SENSOR_8)))
        return;
     
	if(suri == SS_SENSOR_9)
	{
		thSet = 1;
	}
    else if(suri == SS_SENSOR_8)
    {
        sunSet = 1;
    }
	if(thSet && sunSet && sunshineInit == 0)
	{
		sunshineInit = 1;
	}
    else
        return;
    
    GW_Resource *res = NULL;
    
	GW_SensorDeviceManager::getInstance()->GetResource("/ss/sunshineSensor", &res);
	if(NULL == res)
	{
		std::string xml;
		GW_SensorDeviceManager::getInstance()->AddResource("/ss/sunshineSensor", NULL, &res);
		if(res == NULL)
			gw_log("what");

		res->makeXML(xml);
		res->setAttribute("state", "Ok");
		sensorDB db;
		db.suri = "/ss/sunshineSensor";
		db.ttl = 1;
		db.index = 0;
		db.attribute = "state";
		db.attributeType = TYPE_INTEGER;
		db.value = "Ok";

		DBManager::getInstance()->getSensorAttribute(&db);
//			gw_log("11");

		if(db.index <= 0){
//				gw_log("%s Added", db.attribute.c_str());
			DBManager::getInstance()->addSensorAttribute(&db);
		}

		DBManager::getInstance()->addSensorData(&db);

		
//		GW_SensorDeviceManager::getInstance()->GetDeviceListXML(xml);
//			gw_log("%s added", xml.c_str());
	}
    else
        gw_log("This should not happen"); 

    gw_log("Sunshine Sensor initialized");
}
void Sunshine_Check(std::string suri)
{
    if(suri == SS_SENSOR_9 || suri == SS_SENSOR_8)
	{
        GW_Resource *gw_res;
        int ret = GW_SensorDeviceManager::getInstance()->GetResource("/ss/sunshineSensor", &gw_res);
        if(ret == VSF_S_OK && gw_res != NULL)
    		Sunshine_Update();
	}
}
void Sunshine_Update()
{
    int sensor9_temp;
    int sensor8_violet, sensor8_visible;
    
    std::map<std::string, std::string> attrs;
	sensorDB db;

    static std::string prev_state = "Ok";
    std::string curr_state;

    GW_Resource *ss_res = NULL;

    int ret = GW_SensorDeviceManager::getInstance()->GetResource("/ss/sunshineSensor", &ss_res);
    
    if(ret != VSF_S_OK || ss_res == NULL)
    {
        gw_log("Sunshine Sensor has not been initialized");
        return;
    }
    
    db.suri = SS_SENSOR_9;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);
	std::map<std::string, std::string>::iterator it = attrs.find("temp");
	if(it == attrs.end()){
		gw_log("End");
		sensor9_temp = 0;
	}
	else
	{
		std::string temp = it->second;
		sensor9_temp = atoi(temp.c_str());
	}

    db.suri = SS_SENSOR_8;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);
	it = attrs.find("uv");
	if(it == attrs.end()){
		gw_log("End");
		sensor8_violet = 0;
	}
	else
	{
		std::string temp = it->second;
		sensor8_violet = atoi(temp.c_str());
	}

    it = attrs.find("visible");
    if(it == attrs.end()){
		gw_log("End");
		sensor8_visible = 0;
	}
	else
	{
		std::string temp = it->second;
		sensor8_visible = atoi(temp.c_str());
	}

    if(sensor8_visible > 50)
    {
        if(sensor8_violet < 20)
        {
            if(sensor9_temp < 30)
                curr_state = "Ok";
            else
                curr_state = "Low UV but Hot";
        }
        else if(sensor9_temp < 30)
        {
            curr_state = "High UV";
        }
        else
        {
            curr_state = "High UV and Hot";
        }
    }
    else
    {
        curr_state = "No sunshine";
    }
    
	ss_res->setAttribute("state", curr_state);
	db.suri = "/ss/sunshineSensor";
	db.ttl = 1;
	db.index = 0;
	db.attribute = "state";
	db.attributeType = TYPE_INTEGER;
	db.value = curr_state;
	DBManager::getInstance()->addSensorData(&db);
    if(prev_state != curr_state)
    {
        gw_log("Sunshine : %s -> %s", prev_state.c_str(), curr_state.c_str());
        ss_res->observationDirty();
		prev_state = curr_state;
    }
}
//////////////////////////////~Sunshine Implementataion

//////////////////////////////PhoneMotion Sensor Implementation
void PhoneMotion_Initialize(std::string suri)
{
    static int sensor5Set = 0;
    static int sensor12Set = 0;
        
    static int phonemotionInit = 0;

    if(phonemotionInit || ((suri != SS_SENSOR_5) && (suri != SS_SENSOR_12)))
        return;
     
	if(suri == SS_SENSOR_5)
	{
		sensor5Set = 1;
	}
    else if(suri == SS_SENSOR_12)
    {
        sensor12Set = 1;
    }
   
	if(sensor5Set && sensor12Set && phonemotionInit == 0)
	{
		phonemotionInit = 1;
	}
    else
        return;
    
    GW_Resource *res = NULL;

	GW_SensorDeviceManager::getInstance()->GetResource("/ss/phonemotionSensor", &res);
	if(NULL == res)
	{
		std::string xml;
		GW_SensorDeviceManager::getInstance()->AddResource("/ss/phonemotionSensor", NULL, &res);
		if(res == NULL)
			gw_log("what");

		res->makeXML(xml);
		res->setAttribute("state", "Ok");
		sensorDB db;
		db.suri = "/ss/phonemotionSensor";
		db.ttl = 1;
		db.index = 0;
		db.attribute = "state";
		db.attributeType = TYPE_INTEGER;
		db.value = "Normal";

		DBManager::getInstance()->getSensorAttribute(&db);
//			gw_log("11");

		if(db.index <= 0){
//				gw_log("%s Added", db.attribute.c_str());
			DBManager::getInstance()->addSensorAttribute(&db);
		}

		DBManager::getInstance()->addSensorData(&db);

		
//		GW_SensorDeviceManager::getInstance()->GetDeviceListXML(xml);
//			gw_log("%s added", xml.c_str());
	}
    else
        gw_log("This should not happen"); 

    gw_log("Phone Motion Sensor initialized");
}
void PhoneMotion_Check(std::string suri)
{
    if(suri == SS_SENSOR_5 || suri == SS_SENSOR_12)
	{
        GW_Resource *gw_res;
        int ret = GW_SensorDeviceManager::getInstance()->GetResource("/ss/phonemotionSensor", &gw_res);
        if(ret == VSF_S_OK && gw_res != NULL)
    		PhoneMotion_Update();
	}
}
void PhoneMotion_Update()
{
    int sensor5, sensor12;
    
    std::map<std::string, std::string> attrs;
	sensorDB db;

    static std::string prev_state = "Normal";
    std::string curr_state;

    GW_Resource *ss_res = NULL;

    int ret = GW_SensorDeviceManager::getInstance()->GetResource("/ss/phonemotionSensor", &ss_res);
    
    if(ret != VSF_S_OK || ss_res == NULL)
    {
        gw_log("Sunshine Sensor has not been initialized");
        return;
    }
    
    db.suri = SS_SENSOR_5;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);
	std::map<std::string, std::string>::iterator it = attrs.find("state");
	if(it == attrs.end()){
		gw_log("End");
		sensor5 = 0;
	}
	else
	{
		std::string temp = it->second;
		sensor5 = atoi(temp.c_str());
	}

    db.suri = SS_SENSOR_12;
	db.attributeType = TYPE_INTEGER;
	db.index = 0;
	DBManager::getInstance()->getSensorAttrValueMap(&db, &attrs);
	it = attrs.find("motion");
	if(it == attrs.end()){
		gw_log("End");
		sensor12 = 0;
	}
	else
	{
		std::string temp = it->second;
		sensor12 = atoi(temp.c_str());
	}

    if(sensor5 || sensor12)
    {
       curr_state = "Phone";
    }
    else
        curr_state = "Normal";
    
	ss_res->setAttribute("state", curr_state);
	db.suri = "/ss/phonemotionSensor";
	db.ttl = 1;
	db.index = 0;
	db.attribute = "state";
	db.attributeType = TYPE_INTEGER;
	db.value = curr_state;
	DBManager::getInstance()->addSensorData(&db);
    if(prev_state != curr_state)
    {
        gw_log("PhoneMotion : %s -> %s", prev_state.c_str(), curr_state.c_str());
        ss_res->observationDirty();
		prev_state = curr_state;
    }
}
//////////////////////////////~PhoneMotion Implementataion
