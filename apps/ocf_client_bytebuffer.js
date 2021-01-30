/* Copyright (c) 2017-2021 SKKU ESLAB, and contributors. All rights reserved.
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

var ant = require('ant');
var ocf = require('ocf');

var gOA = undefined;
var gPeriodicCameraSampler = undefined;
var gFoundURI = undefined;

/* OCF response handlers */
function onDiscovery(endpoint, uri, types) {
  var isFound = false;
  for (var i in types) {
    if (types[i] == 'ant.r.camera') {
      isFound = true;
    }
  }
  if (isFound) {
    gFoundURI = uri;
    gOA.get(endpoint, uri, onGetCamera, (isPayloadBuffer = true));
  }
}

function onGetCamera(response) {
  var payload = response.payload;
  var uri = gFoundURI;

  console.log('GET from ' + uri + ': ' + payload);
}

/* OCF lifecycle handlers */
function onPrepareOCFEventLoop() {
  gOA.setPlatform('ant');
  gOA.addDevice('/oic/d', 'oic.wk.d', 'Client', 'ocf.1.0.0', 'ocf.res.1.0.0');
}

function onPrepareOCFClient() {
  gOA.discovery('ant.r.camera', onDiscovery);
}

/* ANT Lifecycle handlers */
function onInitialize() {
  gOA = ocf.getAdapter();
  gOA.onPrepareEventLoop(onPrepareOCFEventLoop);
  gOA.onPrepareClient(onPrepareOCFClient);
  console.log('OCF client example app initialized');
}

function onStart() {
  gOA.start();
}

function onStop() {
  gOA.stop();
  gOA.deinitialize();
  clearInterval(gPeriodicCameraSampler);
}

ant.runtime.setCurrentApp(onInitialize, onStart, onStop);
