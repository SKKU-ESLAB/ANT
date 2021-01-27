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

var oa = ocf.getAdapter();
oa.onPrepareEventLoop(function () {
  oa.setPlatform('ant');
  oa.addDevice(
    '/oic/d',
    'oic.d.camera',
    'Camera',
    'ocf.1.0.0',
    'ocf.res.1.0.0'
  );
});

oa.onPrepareServer(function () {
  console.log('onPrepareServer()');
  var device = oa.getDevice(0);
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
  oa.addResource(cameraRes);
});

function getCameraHandler(request) {
  oa.repStartRootObject();
  var buffer = new Buffer(1024);
  oa.repSetByteBuffer(buffer);
  oa.repEndRootObject();
  oa.sendResponse(request, ocf.OC_STATUS_OK);
}

oa.start();
setTimeout(function () {
  console.log('150s elapsed');
  oa.stop();
  oa.deinitialize();
}, 150000);
