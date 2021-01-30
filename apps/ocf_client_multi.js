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

var ocf = require('ocf');

var gOA = undefined;
var gFoundLightUri = undefined;
var gFoundTempUri = undefined;
var gLightState = false;
var gTempState = 15;
var gPeriodicLightSampler;
var gPeriodicTempSampler;

/* OCF response handlers */
function onDiscovery(endpoint, uri, types, interfaceMask) {
  console.log('Discovered ' + uri);
  for (var i = 0; i < types.length; i++) {
    console.log('* type ' + i + ': ' + types[i]);
    if (types[i] == 'oic.r.light') {
      console.log('* Light resource => OBSERVE start');
      gFoundLightUri = uri;
      gOA.observe(endpoint, uri, onObserveLight);
      gPeriodicLightSampler = setInterval(periodicPostLight, 1000);
    } else if (types[i] == 'oic.r.temperature') {
      console.log('* Temperature resource => OBSERVE start');
      gFoundTempUri = uri;
      gOA.observe(endpoint, uri, onObserveTemp);
      gPeriodicTempSampler = setInterval(periodicPostTemp, 2000);
    }
  }
  console.log(' ');
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

function periodicPostTemp() {
  var res = gOA.initPost(endpoint, uri, onPost, '', ocf.OC_LOW_QOS);
  if (res) {
    gOA.repStartRootObject();
    gOA.repSet('state', gTempState);
    gTempState = (gTempState + 1) % 30;
    gOA.repEndRootObject();
    gOA.post();
  }
}

function onObserveLight(response) {
  var uri = gFoundLightUri;
  console.log('GET from ' + uri);
}

function onObserveTemp(response) {
  var uri = gFoundTempUri;
  console.log('GET from ' + uri);
}

function onPost(response) {
  console.log('POST request sent!');
}

/* OCF Lifecycle handlers */
function onPrepareOCFEventLoop() {
  gOA.setPlatform('ant');
  gOA.addDevice('/oic/d', 'oic.wk.d', 'Client', 'ocf.1.0.0', 'ocf.res.1.0.0');
}

function onPrepareOCFClient() {
  gOA.discoveryAll(onDiscovery);
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
  clearInterval(gPeriodicLightSampler);
  clearInterval(gPeriodicTempSampler);
}

ant.runtime.setCurrentApp(onInitialize, onStart, onStop);
