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
console.log('OCF client example app');

var oa = ocf.getAdapter();
oa.onPrepareEventLoop(function () {
  oa.setPlatform('ant');
  oa.addDevice('/oic/d', 'oic.wk.d', 'Client', 'ocf.1.0.0', 'ocf.res.1.0.0');
});

var gLightState = false;
var gTempState = 15;
oa.onPrepareClient(function () {
  oa.discoveryAll(onDiscovery);
});

var foundLightUri = undefined;
var foundTempUri = undefined;

function onDiscovery(endpoint, uri, types, interfaceMask) {
  console.log('Discovered ' + uri);
  for (var i = 0; i < types.length; i++) {
    console.log('* type ' + i + ': ' + types[i]);
    if (types[i] == 'oic.r.light') {
      console.log('* Light resource => OBSERVE start');
      foundLightUri = uri;
      oa.observe(endpoint, uri, onObserveLight);
      interval = setInterval(function () {
        var res = oa.initPost(endpoint, uri, onPost, '', ocf.OC_LOW_QOS);
        if (res) {
          oa.repStartRootObject();
          oa.repSet('state', gLightState);
          gLightState = !gLightState;
          oa.repEndRootObject();
          oa.post();
        }
      }, 1000);
    } else if (types[i] == 'oic.r.temperature') {
      console.log('* Temperature resource => OBSERVE start');
      foundTempUri = uri;
      oa.observe(endpoint, uri, onObserveTemp);
      interval2 = setInterval(function () {
        var res = oa.initPost(endpoint, uri, onPost, '', ocf.OC_LOW_QOS);
        if (res) {
          oa.repStartRootObject();
          oa.repSet('state', gTempState);
          gTempState = (gTempState + 1) % 30;
          oa.repEndRootObject();
          oa.post();
        }
      }, 2000);
    }
  }
  console.log(' ');
}
var interval;
var interval2;

function onObserveLight(response) {
  // var payload = response.payload;
  // var endpoint = response.endpoint;
  var uri = foundLightUri;

  console.log('GET from ' + uri);
}

function onObserveTemp(response) {
  // var payload = response.payload;
  // var endpoint = response.endpoint;
  var uri = foundTempUri;

  console.log('GET from ' + uri);
}

var i = 0;
function onPost(response) {
  console.log('(' + i++ + ') POST request sent!');
}

oa.start();
setTimeout(function () {
  console.log('150s elapsed');
  oa.stop();
  oa.deinitialize();
  clearInterval(interval);
  clearInterval(interval2);
}, 150000);
