/* Copyright (c) 2017-2020 SKKU ESLAB, and contributors. All rights reserved.
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

// Resource API Benchmark: resource API mode

var ant = require('ant');
var console = require('console');

var onInitialize = function () {
  console.log('onInitialize');
};

var startTimeValue = new Date().valueOf();
var onStart = function () {
  var func = function () {
    var startTime = '**ResourceBench** ' + startTimeValue;
    ant.resource.requestGet('/resourcebench', startTime, onReceiveMessage);
    if (totalCount < 49) setTimeout(func, 2000);
  };
  setTimeout(func, 2000);
};

var totalCount = 0;
var onReceiveMessage = function (method, targetUri, message) {
  var startTime = parseInt(message);
  var endTime = new Date().valueOf();
  var timeMS = endTime - startTime;
  totalCount++;
  console.log(
    '(' +
      totalCount +
      ')' +
      'Resource API mode elapsed time: ' +
      timeMS +
      'ms'
  );
};

var onStop = function () {
  console.log('onStop');
};

ant.runtime.setCurrentApp(onInitialize, onStart, onStop);
