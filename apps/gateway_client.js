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

// Gateway Virtual Sensor API: gateway client

var ant = require('ant');
var console = require('console');
var gateway = ant.gateway;
var ocf = require('ocf');

// VSA: virtual sensor adapter
var gVSA = undefined;
var gGWC = undefined;

// var gInlets = undefined;
// var gOutlets = undefined;
// var gSettings = undefined;

var kDeviceType = 'ant.d.userdev';
var kSensorType = 'ant.s.imgcls';

// Utility function
function vsResourcesToString(vsResources) {
  var str = '';
  for (var i in vsResources) {
    if (vsResources.hasOwnProperty(i)) {
      var vsResource = vsResources[i];
      if (i > 0) str += '\n';
      str += '  - ' + vsResource.uri + ': ';
      for (var j = 0; j < vsResource.types.length; j++) {
        if (j > 0) str += ', ';
        str += vsResource.types[j];
      }
    }
  }
  return str;
}

/* Gateway client lifecycle */

/* Step 1. Prepare gateway client */
function onPrepareGatewayClient() {
  console.log('* Prepared gateway client');
  var result = gGWC.getVirtualSensors(onGetVirtualSensors);
  if (!result) {
    console.error('Cannot get virtual sensors');
  }
}

/* Step 2. Get virtual sensor list */
function onGetVirtualSensors(inlets, outlets, settings) {
  // gInlets = inlets;
  // gOutlets = outlets;
  // gSettings = settings;

  console.log('* Received virtual sensor list');
  console.log(' - Inlets: ' + JSON.stringify(inlets));
  console.log(vsResourcesToString(inlets));
  console.log(' - Outlets: ' + JSON.stringify(outlets));
  console.log(vsResourcesToString(outlets));
  console.log(' - Settings: ' + JSON.stringify(settings));
  console.log(vsResourcesToString(settings));

  associateVirtualSensors();
}

var kSTCamera = 'ant.s.camera';
var kSTImgCls = 'ant.s.imgcls';
var kDTSensorDev = 'ant.d.sensordev';
var kDTGateway = 'ant.d.gateway';
var kDTUserDev = 'ant.d.userdev';

/* Step 3. Associate virtual sensors */
function associateVirtualSensors() {
  console.log('* Send association commands to virtual sensors');
  var intervalMS = 100;
  gGWC.associateVirtualSensors(
    {sensorType: kSTCamera, deviceType: kDTSensorDev},
    {sensorType: kSTImgCls, deviceType: kDTGateway},
    intervalMS,
    undefined
  );
  gGWC.associateVirtualSensors(
    {sensorType: kSTImgCls, deviceType: kDTGateway},
    {sensorType: kSTImgCls, deviceType: kDTUserDev},
    intervalMS,
    undefined
  );

  discoverLocalDeepSensor();
}

/* Step 4. Discover local deep sensor */
function discoverLocalDeepSensor() {
  setTimeout(function () {
    var oa = gVSA.getOCFAdapter();
    var kORTypeVSOutlet = 'ant.r.vs.outlet';
    oa.discovery(kORTypeVSOutlet, onDiscoverOutlet);
  }, 2000);
}

function onDiscoverOutlet(endpoint, uri, types, interfaceMask) {
  var isDeviceTypeFound = false;
  var isSensorTypeFound = false;
  for (var i = 0; i < types.length; i++) {
    var type = types[i];
    if (type === kDeviceType) isDeviceTypeFound = true;
    if (type === kSensorType) isSensorTypeFound = true;
  }
  if (isDeviceTypeFound && isSensorTypeFound) {
    console.log(
      '* Found outlet of target deep sensor: ' + uri + '(' + kDeviceType + ')'
    );
    observeDeepSensor(endpoint, uri);
  }
}

/* Step 5. Observe the local deep sensor */
function observeDeepSensor(endpoint, uri) {
  var oa = gVSA.getOCFAdapter();
  setInterval(function () {
    oa.observe(endpoint, uri, onGetDeepSensor, '', ocf.OC_LOW_QOS, true);
  }, 100);
}

function onGetDeepSensor(response) {
  var inputObject = JSON.stringify(response.payloadString);
  var inputBuffer = response.payloadBuffer;
  console.log(
    '* Get deep sensor\'s output: ' + inputObject + ', ' + inputBuffer.length
  );
}

/* ANT lifecycle handlers */
function onInitialize() {
  // Empty function
}

function onStart() {
  gVSA = gateway.getVSAdapter();
  gGWC = gVSA.createGWClient(onPrepareGatewayClient); // gateway client
  gVSA.start();
}

function onStop() {
  gVSA.stop();
}

ant.runtime.setCurrentApp(onInitialize, onStart, onStop);
