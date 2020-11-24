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
oa.onPrepareClient(function () {
  oa.discovery('oic.r.light', onDiscovery);
});

var foundLightUri = undefined;

function onDiscovery(endpoint, uri, types, interfaceMask) {
  var isFound = false;
  for (var i in types) {
    if (types[i] == 'oic.r.light') {
      isFound = true;
    }
  }
  if (isFound) {
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
  }
}
var interval;

function onObserveLight(response) {
  var payload = response.payload;
  // var endpoint = response.endpoint;
  var uri = foundLightUri;

  console.log('GET from ' + uri + ': ' + payload);
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
}, 150000);
