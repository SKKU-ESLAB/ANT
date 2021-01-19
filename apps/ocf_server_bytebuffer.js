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
console.log('OCF server example app');

var oa = ocf.getAdapter();
oa.onPrepareEventLoop(function () {
  oa.setPlatform('ant');
  oa.addDevice('/oic/d', 'oic.d.light', 'Light', 'ocf.1.0.0', 'ocf.res.1.0.0');
});

var gLightState = false;
oa.onPrepareServer(function () {
  console.log('onPrepareServer()');
  var device = oa.getDevice(0);
  var lightRes = ocf.createResource(
    device,
    'lightbulb',
    '/light/1',
    ['oic.r.light'],
    [ocf.OC_IF_RW]
  );
  lightRes.setDiscoverable(true);
  lightRes.setPeriodicObservable(1);
  lightRes.setHandler(ocf.OC_GET, getLightHandler);
  lightRes.setHandler(ocf.OC_POST, postLightHandler);
  oa.addResource(lightRes);
});

function getLightHandler(request) {
  oa.repStartRootObject();
  oa.repSet('state', gLightState);
  oa.repEndRootObject();
  oa.sendResponse(request, ocf.OC_STATUS_OK);
}

var i = 0;
function postLightHandler(request) {
  var requestPayloadString = request.request_payload_string;
  var requestPayload = JSON.parse(requestPayloadString);
  console.log(
    '(' +
      i++ +
      ') POST Request: state=' +
      requestPayload.state +
      ' (present:' +
      gLightState +
      ')'
  );

  gLightState = requestPayload.state;
  oa.sendResponse(request, ocf.OC_STATUS_OK);
}

oa.start();
setTimeout(function () {
  console.log('150s elapsed');
  oa.stop();
  oa.deinitialize();
}, 150000);
