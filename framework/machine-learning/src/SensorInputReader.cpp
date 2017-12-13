/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
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

#include <time.h>
#include <stdlib.h>

#include "SensorInputReader.h"

SensorInputReader::SensorInputReader(DBusConnection* dbusConn)
{
  this->mDBusConnection = dbusConn;
}

DBusConnection* SensorInputReader::get_dbus_connection(){
  return this->mDBusConnection;
}

/*
 * This function is also defined in api/sensor, nil.cc
 */
int SensorInputReader::parsing_string(char* string, char *value){
  //devicde string into word
  int i,j;
	int length = strlen(string);
	
	//printf("input : %s\n", string);

	for (i = 0; i < 1024; i++){
		if (string[i] != ' ')
			break;
	}
	
	if (i  >= length)
		return 1;

	for (j = i; j <= length; j++){
		if (string[j] == ' '){
			j--;
			break;
		}
	}
	
	strncpy(value, &(string[i]), j - i + 1);
	value[j - i + 1] = '\0'; // NULL;

	for (i = 0; i <= j; i++)
		string[i] = ' ';

	string[length] = '\0'; // NULL;

	return 0;

}

MLTensor* SensorInputReader::read(std::string sourceUri) {
  /* TODO: implement it
   * Now sensor data is given as dummy data.
   */
  // Input: /thing/sensor/***
  // *** (sensor name) : parsing -> sensor_name


  /*
   * ************Warning ************
   * This SensorInputReader::read() is not tested yet
   * Need to debug this function
   */

  DBusConnection *dbus_conn = get_dbus_connection();
  DBusMessage *msg;
  DBusMessage *reply;
  DBusError error;
  char *sensor_name;
  char *sensor_value, *sensor_type, *value_name;

  int randData = rand() % 1000;
  float data[3];
  int str_len;

  sensor_name = (char*) malloc(30 * sizeof(char));
  sscanf("/thing/sensor/%s", sensor_name); 
  str_len = strlen(sensor_name); 
  sensor_name[str_len-1] = '\0';
  
  srand(time(NULL));


  msg = dbus_message_new_method_call("org.ant.sensorManager",
      "/", "org.ant.sensorManager", "Get");
  if(msg == NULL){
    fprintf(stderr, "Message Null\n");
  }

  dbus_message_append_args(msg,
      DBUS_TYPE_STRING, &sensor_name,
      DBUS_TYPE_INVALID
      );
  dbus_error_init(&error);
  if(dbus_conn == NULL){
    fprintf(stderr, "dbus connection is NULL\n");
  } 

  /* 
   * Timeout: 500 ms
   * It blocked
   */
  reply = dbus_connection_send_with_reply_and_block(dbus_conn, msg, 500, &error);

  if(reply == NULL){
    fprintf(stderr, "Error: %s", error.message);
  }

  dbus_error_free(&error);

  dbus_message_unref(msg);
  dbus_message_get_args(reply, NULL,
      DBUS_TYPE_STRING, &sensor_value,
      DBUS_TYPE_STRING, &sensor_type,
      DBUS_TYPE_STRING, &value_name,
      DBUS_TYPE_INVALID);

  dbus_message_unref(reply);


  int element_num = 0;
  int data_type = 0;

  char value_tmp[1024];
  char type_tmp[1024];
  char name_tmp[1024];

  // Parsing
  while (1) {
    if( this->parsing_string(sensor_value, value_tmp))
      break;

    element_num++;
    this->parsing_string(sensor_type, type_tmp);
    this->parsing_string(value_name, name_tmp);

    if (!strcmp(type_tmp, "FLOAT")){
      data_type = 1;
      data[element_num-1] = atof(value_tmp);
    } else if (!strcmp(type_tmp, "INT")){
      data_type = 2;
      data[element_num-1] = atoi(value_tmp);
    } else if (!strcmp(type_tmp, "STRING")){
      data_type = 3;
    } else {
      printf("Error: not supported data type %s \n", type_tmp);

    }
  } 
 
  int inputShape[] = {0};
  inputShape[0] = element_num;

  MLTensorLayout* inputTensorLayout;
  if(data_type == 1){
    inputTensorLayout = new MLTensorLayout(1, inputShape, MLDataType::Float); 
  } else if (data_type == 2){
    inputTensorLayout = new MLTensorLayout(1, inputShape, MLDataType::Int32); 
  } else if (data_type == 3){

  } else {
  
  }
 
  MLTensor* inputTensor = new MLTensor(*inputTensorLayout);

  inputTensor->assignData(data);

  return inputTensor;
}

MLTensorLayout SensorInputReader::getLayout() {
  // Input layout : float[3] input
  int inputShape[] = {3};
  MLTensorLayout inputTensorLayout(1, inputShape, MLDataType::Float);
  return inputTensorLayout;
}
