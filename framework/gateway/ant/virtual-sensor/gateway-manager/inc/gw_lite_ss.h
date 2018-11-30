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

/// @file gw_lite_ss.h

/// @brief  This file contains the declaration of classes and its members
///         related to Gateway lite sensor manager

#ifndef __GW_LITE_SS_H__
#define __GW_LITE_SS_H__

#include <string>

#define SS_SENSOR_1 "/door/door"
#define SS_SENSOR_2 "/kitchen/infrared"
#define SS_SENSOR_3 "/kitchen/valve"
#define SS_SENSOR_4 "/living/pressure"
#define SS_SENSOR_5 "/living/phone"
#define SS_SENSOR_6 "/living/damp1"
#define SS_SENSOR_7 "/living/damp2"
#define SS_SENSOR_8 "/living/illum"
#define SS_SENSOR_9 "/outside/th"
#define SS_SENSOR_10 "/bed/pressure"
#define SS_SENSOR_11 "/gear/heart"
#define SS_SENSOR_12 "/gear/motion"
#define SS_SENSOR_13 "/lumo/posture"

#define SS_ELEC_1 "/living/outlet1"
#define SS_ELEC_2 "/living/outlet2"
#define SS_ELEC_3 "/living/outlet3"
#define SS_ELEC_4 "/bath/outlet1"
#define SS_ELEC_5 "/bath/outlet2"
#define SS_ELEC_6 "/kitchen/outlet1"
#define SS_ELEC_7 "/bed/outlet1"
#define SS_ELEC_8 "/bed/outlet2"

#define SS_LIGHT_1 "/bed/light1"
#define SS_LIGHT_2 "/door/light1"
#define SS_LIGHT_3 "/bath/light1"
#define SS_LIGHT_4 "/kitchen/light1"
#define SS_LIGHT_5 "/living/light1"
#define SS_LIGHT_6 "/living/light2"

#define SS_MOTION_1 "/bed/motion1"
#define SS_MOTION_2 "/door/motion1"
#define SS_MOTION_3 "/bath/motion1"
#define SS_MOTION_4 "/living/motion1"
#define SS_MOTION_5 "/kitchen/motion1"

enum POSTURE_STATE
{
	PS_STAND = 0,
	PS_WALKING,
	PS_RUNNING,
	PS_LYING,
	PS_LEANING
};

void ss_initialize(std::string suri);
void ss_check(std::string suri);

void Discomfort_Initialize(std::string);
void Discomfort_Check(std::string suri);
void Discomfort_Update();

void Sleep_Initialize(std::string);
void Sleep_Check(std::string suri);
void Sleep_Update();

void Inandout_Initialize(std::string);
void Inandout_Check(std::string suri);
void Inandout_Update();

void Blackout_Initialize(std::string);
void Blackout_Check(std::string suri);
void Blackout_Update();

void FireAlarm_Initialize(std::string);
void FireAlarm_Check(std::string suri);
void FireAlarm_Update();

void Leakage_Initialize(std::string);
void Leakage_Check(std::string suri);
void Leakage_Update();

void Ventilation_Initialize(std::string);
void Ventilation_Check(std::string suri);
void Ventilation_Update();

void Sunshine_Initialize(std::string);
void Sunshine_Check(std::string suri);
void Sunshine_Update();

void PhoneMotion_Initialize(std::string);
void PhoneMotion_Check(std::string suri);
void PhoneMotion_Update();


#endif

