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
var console = require('console');

var gOA = undefined;
var gIntervalLight;
var gIntervalTemp;
var gFoundLightUri = undefined;
var gFoundTempUri = undefined;
var gLightState = false;
var gTempState = 15;

/* OCF response handlers */
function onDiscovery(endpoint, uri, types, interfaceMask) {
  console.log('Discovered ' + uri);
  for (var i = 0; i < types.length; i++) {
    console.log('* type ' + i + ': ' + types[i]);
    if (types[i] == 'oic.r.light') {
      console.log('* Light resource => OBSERVE start');
      gFoundLightUri = uri;
      gOA.observe(endpoint, uri, onObserveLight);
      gIntervalLight = setInterval(function () {
        var res = gOA.initPost(endpoint, uri, onPost, '', ocf.OC_LOW_QOS);
        if (res) {
          gOA.repStartRootObject();
          gOA.repSet('state', gLightState);
          gLightState = !gLightState;
          gOA.repEndRootObject();
          gOA.post();
        }
      }, 1000);
    } else if (types[i] == 'oic.r.temperature') {
      console.log('* Temperature resource => OBSERVE start');
      gFoundTempUri = uri;
      gOA.observe(endpoint, uri, onObserveTemp);
      gIntervalTemp = setInterval(function () {
        var res = gOA.initPost(endpoint, uri, onPost, '', ocf.OC_LOW_QOS);
        if (res) {
          gOA.repStartRootObject();
          gOA.repSet('state', gTempState);
          gTempState = (gTempState + 1) % 30;
          gOA.repEndRootObject();
          gOA.post();
        }
      }, 2000);
    }
  }
  console.log(' ');
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

/* Lifecycle handlers */
function onInitialize() {
  console.log('OCF client example app initialized');

  gOA = ocf.getAdapter();
  gOA.onPrepareEventLoop(function () {
    gOA.setPlatform('ant');
    gOA.addDevice('/oic/d', 'oic.wk.d', 'Client', 'ocf.1.0.0', 'ocf.res.1.0.0');
  });

  gOA.onPrepareClient(function () {
    gOA.discoveryAll(onDiscovery);
  });
}

function onStart() {
  gOA.start();
}

function onStop() {
  console.log('150s elapsed');
  gOA.stop();
  gOA.deinitialize();
  clearInterval(gIntervalLight);
  clearInterval(gIntervalTemp);
}

/* Setting lifecycle handlers */
ant.runtime.setCurrentApp(onInitialize, onStart, onStop);
