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

var ant_api_dir = process.env.ANT_BIN_DIR + '/api/';
var api = require(ant_api_dir + 'ant');
var appApi = api.app();

var countThreshold = 100;
function sensorREST() {
  var resourceApi = api.resource();
  var body = {'commandId': 123, 'text': 'abcd'};
  console.log('Send sensor query');

  var count = 0;

  var startTime = new Date();

  var onGetCallback = function(responseText) {
    console.log('sensor message: ' + responseText);
    count++;
    if (count >= countThreshold) {
      var endTime = new Date();
      var duration = (endTime.getTime() - startTime.getTime());
      console.log(
          'Sampling*' + countThreshold + ' in REST: ' + duration + 'ms');
    }

    resourceApi.get(
        '/thing/apps/4', '/thing/sensors/acc', JSON.stringify(body),
        onGetCallback);
  };

  resourceApi.get(
      '/thing/apps/4', '/thing/sensors/acc', JSON.stringify(body),
      onGetCallback);
  count++;
}

// function sensorLegacy() {
//   var sensorApi = require(ant_api_dir + 'sensor-api');

//   var count = 0;
//   var startTime = new Date();
//   while (count < countThreshold) {
//     var sensorData = {
//       'BUTTON': -1,
//       'ACC': -1,
//       'MOTION': -1,
//       'SOUND': -1,
//       'LIGHT': -1,
//       'VIBRATION': -1,
//       'TEMP': -1
//     };
//     sensorApi.Get('ACC');

//     count++;
//   }
//   var endTime = new Date();
//   var duration = (endTime.getTime() - startTime.getTime());
//   console.log('Sampling*' + countThreshold + ' in Legacy: ' + duration + 'ms');
// }

appApi.onLaunch(function() {
  console.log('App Launched!');

  //sensorLegacy();
  sensorREST();
});

appApi.onTermination(function() {
  console.log('App Terminated!');
});

appApi.appReady();