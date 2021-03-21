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

var Buffer = require('buffer');

var gOA = undefined;

/* OCF response handlers */
function onGetCamera(request) {
  var buffer = new Buffer(1024);
  var dataObject = {data: 'OCF API test'};
  var dataString = JSON.stringify(dataObject);
  gOA.sendResponseBuffer(request, ocf.OC_STATUS_OK, buffer, dataString);
}

/* OCF lifecycle handlers */
function onPrepareOCFEventLoop() {
  gOA.setPlatform('ant');
  gOA.addDevice(
    '/oic/d',
    'ant.d.camera',
    'Camera',
    'ocf.1.0.0',
    'ocf.res.1.0.0'
  );
}

function onPrepareOCFServer() {
  console.log('onPrepareServer()');
  var device = gOA.getDevice(0);
  var cameraRes = ocf.createResource(
    device,
    'camera',
    '/camera/1',
    ['ant.r.camera'],
    [ocf.OC_IF_RW]
  );
  cameraRes.setDiscoverable(true);
  cameraRes.setPeriodicObservable(1);
  cameraRes.setHandler(ocf.OC_GET, onGetCamera);
  gOA.addResource(cameraRes);
}

/* ANT lifecycle handlers */
function onInitialize() {
  gOA = ocf.getAdapter();
  gOA.onPrepareEventLoop(onPrepareOCFEventLoop);
  gOA.onPrepareServer(onPrepareOCFServer);
  console.log('OCF server example app initialized');
}

function onStart() {
  gOA.start();
}

function onStop() {
  gOA.stop();
  gOA.deinitialize();
}

ant.runtime.setCurrentApp(onInitialize, onStart, onStop);
