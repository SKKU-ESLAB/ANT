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

var ocf = require('ocf');
console.log('OCF client example app');

var oa = ocf.getAdapter();
oa.onPrepareEventLoop(function () {
  oa.setPlatform('ant');
  oa.addDevice('/oic/d', 'oic.wk.d', 'Client', 'ocf.1.0.0', 'ocf.res.1.0.0');
});

oa.onPrepareClient(function () {
  oa.discovery('oic.r.camera', onDiscovery);
});

var foundUri = undefined;

function onDiscovery(endpoint, uri, types) {
  var isFound = false;
  for (var i in types) {
    if (types[i] == 'oic.r.camera') {
      isFound = true;
    }
  }
  if (isFound) {
    foundUri = uri;
    oa.get(endpoint, uri, onGetCamera, (isPayloadBuffer = true));
    // oa.observe(endpoint, uri, onGetCamera);
  }
}
var interval;

function onGetCamera(response) {
  var payload = response.payload;
  // var endpoint = response.endpoint;
  var uri = foundUri;

  console.log('GET from ' + uri + ': ' + payload);
}

oa.start();
setTimeout(function () {
  console.log('150s elapsed');
  oa.stop();
  oa.deinitialize();
  clearInterval(interval);
}, 150000);
