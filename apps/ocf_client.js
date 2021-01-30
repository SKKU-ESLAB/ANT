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

var ant = require('ant');
var ocf = require('ocf');

var gOA = undefined;
var gFoundLightURI = undefined;
var gLightState = false;
var gPeriodicLightSampler = undefined;

/* OCF response handlers */
function onDiscovery(endpoint, uri, types, interfaceMask) {
  var isFound = false;
  for (var i in types) {
    if (types[i] == 'oic.r.light') {
      isFound = true;
    }
  }
  if (isFound) {
    gFoundLightURI = uri;

    gOA.observe(endpoint, uri, onObserveLight);
    gPeriodicLightSampler = setInterval(periodicPostLight, 1000);
  }
}

function periodicPostLight() {
  var res = gOA.initPost(endpoint, uri, onPost, '', ocf.OC_LOW_QOS);
  if (res) {
    gOA.repStartRootObject();
    gOA.repSet('state', gLightState);
    gLightState = !gLightState;
    gOA.repEndRootObject();
    gOA.post();
  }
}

function onObserveLight(response) {
  var payload = response.payload;
  var uri = gFoundLightURI;

  console.log('GET from ' + uri + ': ' + payload);
}

function onPost(response) {
  console.log('POST request sent!');
}

/* OCF lifecycle handlers */
function onPrepareOCFEventLoop() {
  gOA.setPlatform('ant');
  gOA.addDevice('/oic/d', 'oic.wk.d', 'Client', 'ocf.1.0.0', 'ocf.res.1.0.0');
}

function onPrepareOCFClient() {
  gOA.discovery('oic.r.light', onDiscovery);
}

/* ANT lifecycle handlers */
function onInitialize() {
  console.log('OCF client example app');
  gOA = ocf.getAdapter();
  gOA.onPrepareEventLoop(onPrepareOCFEventLoop);
  gOA.onPrepareClient(onPrepareOCFClient);
}

function onStart() {
  gOA.start();
}

function onStop() {
  gOA.stop();
  gOA.deinitialize();
  clearInterval(gPeriodicLightSampler);
}

ant.runtime.setCurrentApp(onInitialize, onStart, onStop);
