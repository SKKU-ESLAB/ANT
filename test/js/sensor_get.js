/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Injung Hwang<sinban04@gmail.com>
 *              Dongig Sin<dongig@skku.edu>
 *              Gyeonghwan Hong<redcarrottt@gmail.com>
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

var ant_api_dir = process.env.ANT_BIN_DIR + "/api/";
var sensorApi = require(ant_api_dir + "sensor-api");


// Get sensor list and size
var sensorList = sensorApi.GetSensorlist();
var sensorNum = sensorList.sensorNum;
var sensorData = {
  "BUTTON": -1,
  "ACC": -1,
  "MOTION": -1,
  "SOUND": -1,
  "LIGHT": -1,
  "VIBRATION": -1,
  "TEMP": -1
};
var data;

// Report sensor data periodically
var repeat = setInterval(function() {
  data = sensorApi.Get("ACC");
  console.log("X Y Z" + "|" + data.X + " " + data.Y + " " + data.Z);

}, 500);
