/* Copyright (c) 2017-2018 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "SensorManager.h"
#include "ANTdbugLog.h"
#include "NativeResourceAPI.h"

extern "C" {
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <math.h>
}

#define ACC_X 0
#define ACC_Y 1
#define ACC_Z 2
#define Clock 27
#define Address 28
#define DataOut 29

using namespace std;

#define APPS_URI "/thing/apps"
#define SENSOR_MANAGER_URI "/thing/sensors"
#define ACC_SENSOR_URI "/thing/sensors/acc"

SensorManager *SensorManager::sSingleton = NULL;

unsigned int ADC_Read(unsigned char channel);

// Main loop
void SensorManager::run() {
  // Initialize MessageRouter and Channels
  this->mMessageRouter = new MessageRouter();
  this->mDbusChannel = new DbusChannel(this->mMessageRouter);
  this->mLocalChannel =
      new LocalChannel(this->mMessageRouter, SENSOR_MANAGER_URI, false);

  // Run DbusChannel and add it to MessageRouter's routing table
  this->mDbusChannel->run();
  this->mMessageRouter->addRoutingEntry(APPS_URI, this->mDbusChannel);

  // Initialize Resource API
  NativeResourceAPI::initialize(this->mMessageRouter, this->mLocalChannel);

  // Initialize and register main resource
  std::string sensorManagerUri(SENSOR_MANAGER_URI);
  this->mMainResource = new Resource(sensorManagerUri);
  this->mMainResource->setOnGet(SensorManager::onGetMainResource);
  NativeResourceAPI::registerResource(this->mMainResource);

  // Initialize and register sensor resource
  std::string sensorUri(ACC_SENSOR_URI);
  this->mSensorResource = new Resource(sensorUri);
  this->mSensorResource->setOnGet(SensorManager::onGetAccSensorResource);
  NativeResourceAPI::registerResource(this->mSensorResource);

  // LocalChannel: run on main thread
  // Main loop starts to run in LocalChannel::run()
  this->mLocalChannel->run();
}

void SensorManager::onGetMainResource(BaseMessage *requestMessage,
                                      BaseMessage *responseMessage) {
  ResourceResponse *response =
      (ResourceResponse *)responseMessage->getPayload();
  SensorManager::singleton()->onGetSensorList(response);
}

// TODO: hard-coded
void SensorManager::onGetAccSensorResource(BaseMessage *requestMessage,
                                           BaseMessage *responseMessage) {
  ResourceResponse *response =
      (ResourceResponse *)responseMessage->getPayload();
  std::string sensorName("acc");
  SensorManager::singleton()->onGet(response, sensorName);
}

// Sensor Commands
void SensorManager::onGetSensorList(ResourceResponse *response) {
  // TODO: hard-coded
  std::string sensorList("\
[{\
  \"name\":\"acc\"\
  \"values\": [\
    {\"name\": \"x\", \"type\": \"int\"},\
    {\"name\": \"y\", \"type\": \"int\"},\
    {\"name\": \"z\", \"type\": \"int\"}\
  ]\
},\n\
{\
  \"name\":\"temp\"\
  \"values\": [\
    {\"name\": \"temp\", \"type\": \"float\"}\
  ]\
},\n\
{\
  \"name\":\"sound\"\
  \"values\": [\
    {\"name\": \"sound\", \"type\": \"int\"}\
  ]\
},\n\
{\
  \"name\":\"light\"\
  \"values\": [\
    {\"name\": \"light\", \"type\": \"int\"}\
  ]\
},\n\
{\
  \"name\":\"motion\"\
  \"values\": [\
    {\"name\": \"motion\", \"type\": \"int\"}\
  ]\
},\n\
{\
  \"name\":\"vibration\"\
  \"values\": [\
    {\"name\": \"vibration\", \"type\": \"int\"}\
  ]\
},\n\
{\
  \"name\":\"touch\"\
  \"values\": [\
    {\"name\": \"touch\", \"type\": \"int\"}\
  ]\
}]");
  response->setBody(sensorList);
}

int ADC_INIT = 0;
int ADC_LOCK = 0;
int wiring_pi_setup;

int setupWiringPi() {
  int res = 0;
  if (!wiring_pi_setup) {
    res = wiringPiSetup();
    wiring_pi_setup = 1;
  }
  return res;
}

int ADC_init() {
  if (setupWiringPi() < 0) {
    printf("Error : Fail to init WiringPi\n");
    return 1;
  }

  pinMode(DataOut, INPUT);
  pullUpDnControl(DataOut, PUD_UP);

  pinMode(Clock, OUTPUT);
  pinMode(Address, OUTPUT);
  ADC_INIT = 1;
}

int isWiringPiSetup() { return wiring_pi_setup; }



unsigned int ADC_Read(unsigned char channel) {
  unsigned int value;
  unsigned char i;
  unsigned char LSB = 0, MSB = 0;

  if (!ADC_INIT)
    ADC_init();

  if (ADC_LOCK) {
    while (ADC_LOCK) {
    }
  }

  ADC_LOCK = 1;

  channel = channel << 4;
  for (i = 0; i < 4; i++) {
    if (channel & 0x80)
      digitalWrite(Address, 1);
    else
      digitalWrite(Address, 0);
    digitalWrite(Clock, 1);
    digitalWrite(Clock, 0);
    channel = channel << 1;
  }

  for (i = 0; i < 6; i++) {
    digitalWrite(Clock, 1);
    digitalWrite(Clock, 0);
  }

  delayMicroseconds(15);

  for (i = 0; i < 2; i++) {
    digitalWrite(Clock, 1);
    MSB <<= 1;
    if (digitalRead(DataOut))
      MSB |= 0x1;
    digitalWrite(Clock, 0);
  }
  for (i = 0; i < 8; i++) {
    digitalWrite(Clock, 1);
    LSB <<= 1;
    if (digitalRead(DataOut))
      LSB |= 0x1;
    digitalWrite(Clock, 0);
  }

  value = MSB;
  value <<= 8;
  value |= LSB;

  ADC_LOCK = 0;
  return value;
}

char *ACC_get() {
  int i;
  int val_x, val_y, val_z;
  double b;
  float a;
  static char value[20];

  val_x = 0;
  val_y = 0;
  val_z = 0;

  for (i = 0; i < 10; i++) {
    val_x += ADC_Read(ACC_X);
    val_y += ADC_Read(ACC_Y);
    val_z += ADC_Read(ACC_Z);
  }

  val_x = val_x / 10;
  val_y = val_y / 10;
  val_z = val_z / 10;

  delay(300);
  b = (double)(abs(val_x - 320)) / (abs(val_z - 320));

  a = atan(b);
  a = a / 3.14 * 180;

  sprintf(value, "%d %d %d\n", val_x, val_y, val_z);

  return value;
}

void SensorManager::onGet(ResourceResponse *response, std::string sensorName) {
  // TODO: hard-coded
  std::string sensorData("\
{\n\
  \"x\": \"123\",\n\
  \"y\": \"456\",\n\
  \"z\": \"789\"\n\
}");
  ACC_get();
  response->setBody(sensorData);
}