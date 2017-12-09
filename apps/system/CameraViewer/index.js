/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Hayun Lee<lhy920806@gmail.com>
 *              Hyemin Lee<gudbooy@gmail.com>
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
var api = require(ant_api_dir + "ant");
var appApi = api.app();

var cameraApi = require(ant_api_dir + "camera-api");
var recordingObj = new cameraApi.ANTRecording();

var cameraNum = 0;
var ipAddress = "192.168.49.1";
var portNum = 5000;

appApi.onLaunch(function() {
  recordingObj.streamingStart(cameraNum, ipAddress, portNum);

  setInterval(function() { }, 5000);
});

appApi.onTermination(function() {
  recordingObj.streamingStop(cameraNum, ipAddress, portNum);
});

appApi.appReady();
