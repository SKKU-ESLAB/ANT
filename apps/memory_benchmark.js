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

// Memory Benchmark

var ant = require('ant');
var console = require('console');

function onInitialize() {
  console.log('onInitialize');
}

function onStart() {
  console.log('onStart');
  var totalPss = 0.0;
  var sampleCount = 0;
  console.log('Start memory benchmarking...');
  if (ant.stream !== undefined) {
    ant.stream.initialize();
  }
  var getSample = function () {
    totalPss += ant.runtime.getPssInKB();
    sampleCount++;
    if (sampleCount == 10) {
      console.log((totalPss / sampleCount).toFixed(2) + ' KB');
    } else {
      setTimeout(getSample, 1000);
    }
  };
  setTimeout(getSample, 1000);
}

function onStop() {
  console.log('onStop');
}

ant.runtime.setCurrentApp(onInitialize, onStart, onStop);
