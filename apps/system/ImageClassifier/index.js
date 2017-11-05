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

var ant_api_dir = process.env.ANT_BIN_DIR + "/api/";
var api = require(ant_api_dir + "ant");
var cameraApi = require(ant_api_dir + "camera-api");
var appApi = api.app();
var mlApi = api.ml();

var streamObj = new cameraApi.ANTRecording();

//require('date-utils');
var count=0;
var dt;
var d;

//dt = new Date();
//d = dt.toFormat('YYYY-MM-DD HH24:MI:SS');

//two API Set

/*******************************  API MODEL *******************************/
// Start API : count = N; Send Number of N Frames over network than callback callback
// Start API : count = 0; Send Number of Infinte Frames over network unless stop API is invoked
/******************************** API SAMPLE *****************************/
// streamObj.start(int n, function(err, data); n != 0 : finite Number of Frame n == 0 : Infinite Number of Frame
// streamObj.stop(); //aborting

streamObj.streamingStart(0, "192.168.49.1", 5000);


appApi.onTermination(function(){
			console.log('<onTermination Called');
			streamObj.streamingStop(0, "192.168.49.1", 5000);
		});

mlApi.runModel("imageclassifier", function(outputData) {
  // notify to application
  // Since output is drawn in camera frame, output need not to be handled.
});
