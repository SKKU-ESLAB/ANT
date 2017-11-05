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

MLTensor* SensorInputReader::read(std::string sourceUri) {
  /* TODO: implement it
   * Now sensor data is given as dummy data.
   */

  printf("aaa\n");
  MLTensor* inputTensor = new MLTensor(this->getLayout());
  DBusConnection *dbus_conn = get_dbus_connection();
  DBusMessage *msg;
  DBusMessage *reply;
  DBusError error;
  char *sensor_name="ACC";
  char *sensor_value, *sensor_type, *value_name;

  int randData = rand() % 1000;
  float data[3];

  srand(time(NULL));

  printf("send method call\n");
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

  sscanf(sensor_value, "%d %d %d", &data[0], &data[1], &data[2]); 
  //data[0] = randData*0.001f; data[1] = randData*0.0001f; data[2] = randData*0.001f;
  inputTensor->assignData(data);

  return inputTensor;
}

MLTensorLayout SensorInputReader::getLayout() {
  // Input layout : float[3] input
  int inputShape[] = {3};
  MLTensorLayout inputTensorLayout(1, inputShape, MLDataType::Float);
  return inputTensorLayout;
}
