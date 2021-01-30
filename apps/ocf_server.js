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
var gLightState = false;

/* OCF response handlers */
function onGetLight(request) {
  gOA.repStartRootObject();
  gOA.repSet('state', gLightState);
  gOA.repEndRootObject();
  gOA.sendResponse(request, ocf.OC_STATUS_OK);
}

function onPostLight(request) {
  var requestPayloadString = request.request_payload_string;
  var requestPayload = JSON.parse(requestPayloadString);
  console.log('POST Request: state=' + requestPayload.state);

  gLightState = requestPayload.state;
  gOA.sendResponse(request, ocf.OC_STATUS_OK);
}

/* OCF lifecycle handlers */
function onPrepareOCFEventLoop() {
  gOA.setPlatform('ant');
  gOA.addDevice('/oic/d', 'oic.d.light', 'Light', 'ocf.1.0.0', 'ocf.res.1.0.0');
}

function onPrepareOCFServer() {
  console.log('onPrepareServer()');
  var device = gOA.getDevice(0);
  var rLight = ocf.createResource(
    device,
    'lightbulb',
    '/light/1',
    ['oic.r.light'],
    [ocf.OC_IF_RW]
  );
  rLight.setDiscoverable(true);
  rLight.setPeriodicObservable(1);
  rLight.setHandler(ocf.OC_GET, onGetLight);
  rLight.setHandler(ocf.OC_POST, onPostLight);
  gOA.addResource(rLight);
}

/* ANT lifecycle handlers */
function onInitialize() {
  console.log('OCF server example app');
  gOA = ocf.getAdapter();
  gOA.onPrepareEventLoop(onPrepareOCFEventLoop);
  gOA.onPrepareServer(onPrepareOCFServer);
}

function onStart() {
  gOA.start();
}

function onStop() {
  gOA.stop();
  gOA.deinitialize();
}

ant.runtime.setCurrentApp(onInitialize, onStart, onStop);
