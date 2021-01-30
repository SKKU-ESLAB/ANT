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
var Buffer = require('buffer');
console.log('OCF server example app');

var gOA = undefined;

/* OCF response handlers */
function getCameraHandler(request) {
  gOA.repStartRootObject();
  var buffer = new Buffer(1024);
  gOA.repSetByteBuffer(buffer);
  gOA.repEndRootObject();
  gOA.sendResponse(request, ocf.OC_STATUS_OK);
}

/* ANT Lifecycle Handlers */
function onInitialize() {
  gOA = ocf.getAdapter();
  gOA.onPrepareEventLoop(function () {
    gOA.setPlatform('ant');
    gOA.addDevice(
      '/oic/d',
      'oic.d.camera',
      'Camera',
      'ocf.1.0.0',
      'ocf.res.1.0.0'
    );
  });

  gOA.onPrepareServer(function () {
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
    cameraRes.setHandler(ocf.OC_GET, getCameraHandler);
    gOA.addResource(cameraRes);
  });
}

function onStart() {
  gOA.start();
}

function onStop() {
  console.log('150s elapsed');
  gOA.stop();
  gOA.deinitialize();
}

ant.runtime.setCurrentApp(onInitialize, onStart, onStop);
