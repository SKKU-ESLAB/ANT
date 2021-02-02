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

var MLAPI = undefined;
var OCFAPI = undefined;

try {
  MLAPI = require('antml');
} catch (e) {
  throw new Error('Gateway API Dependency Error: not found ML API');
}
try {
  OCFAPI = require('ocf');
} catch (e) {
  throw new Error('Gateway API Dependency Error: not found OCF API');
}

/**
 * ANT Gateway API
 */

var gVirtualSensorAdapter = undefined;
function ANTGateway() {}

/* Gateway API 1: ML Fragment Element */
ANTGateway.prototype.createImgClsImagenetElement = function (
  modelPath,
  numFragments,
  targetUri
) {
  var mlFragmentElement = MLAPI.createMLFragmentElement(
    modelPath,
    [3, 224, 224, 1],
    'uint8',
    'input',
    'gateway_imgcls_imagenet',
    numFragments,
    targetUri
  );
  return mlFragmentElement;
};

/* Gateway API 2: Virtual Sensor Adapter and DFE */
ANTGateway.prototype.getVSAdapter = function () {
  if (gVirtualSensorAdapter === undefined) {
    gVirtualSensorAdapter = new VirtualSensorAdapter();
  }
  return gVirtualSensorAdapter;
};

ANTGateway.prototype.createDFE = function (modelName, numFragments) {
  if (typeof modelName !== 'string') {
    throw 'Invalid modelName: ' + modelName;
  }
  if (
    typeof numFragments !== 'number' ||
    parseInt(numFragments) != numFragments
  ) {
    throw 'Invalid numFragments: ' + numFragments;
  }
  return new DFE(modelName, numFragments);
};

/* Gateway API 2-1: Virtual Sensor Adapter */
function VirtualSensorAdapter() {
  var self = this;
  /* Attributes */
  this.mOCFAdapter = OCFAPI.getAdapter();
  this.mVirtualSensors = [];
  this.mResources = [];

  /* OCF lifecycle handlers */
  function onPrepareOCFEventLoop() {
    self.mOCFAdapter.setPlatform('ant');
    self.mOCFAdapter.addDevice(
      '/gateway',
      'ant.d.gateway',
      'Gateway',
      'ant.1.0.0',
      'ant.res.1.0.0'
    );
  }

  function onPrepareOCFClient() {}

  function onPrepareOCFServer() {
    // Add all the virtual sensors
    for (var i in self.mVirtualSensors) {
      var virtualSensor = self.mVirtualSensors[i];
      var rInlet = virtualSensor.setupInlet(self);
      var rOutlet = virtualSensor.setupOutlet(self);
    }
  }

  /* Set OCF lifecycle handlers */
  this.mOCFAdapter.onPrepareEventLoop(onPrepareOCFEventLoop);
  this.mOCFAdapter.onPrepareClient(onPrepareOCFClient);
  this.mOCFAdapter.onPrepareServer(onPrepareOCFServer);
}

/* Virtual sensor adapter lifecycle handlers */
VirtualSensorAdapter.prototype.start = function () {
  this.mOCFAdapter.start();
};

VirtualSensorAdapter.prototype.stop = function () {
  this.mOCFAdapter.stop();
  this.mOCFAdapter.deinitialize();
};

/* Virtual sensor handling methods */
VirtualSensorAdapter.prototype.addVirtualSensor = function (
  name,
  observerHandler,
  inferenceHandler
) {
  var virtualSensor = new VirtualSensor(
    name,
    observerHandler,
    inferenceHandler
  );
  this.mVirtualSensors.push(virtualSensor);
  this.mResources.push(virtualSensor.getInletResource());
  this.mResources.push(virtualSensor.getOutletResource());
};

VirtualSensorAdapter.prototype.findVirtualSensor = function (name) {
  for (var i in this.mVirtualSensors) {
    var virtualSensor = this.mVirtualSensors[i];
    if (virtualSensor.getName() === name) {
      return virtualSensor;
    }
  }
  return undefined;
};

VirtualSensorAdapter.prototype.findVirtualSensorByUri = function (uri) {
  var nameFrom = uri.indexOf('/', 1) + 1;
  var nameLength = uri.indexOf('/', nameFrom) - nameFrom;
  if (nameFrom < 0 || nameLength < 0) return undefined;

  var name = uri.substring(nameFrom, nameLength);
  if (name === undefined || name.length == 0) return undefined;

  return this.findVirtualSensor(name);
};

VirtualSensorAdapter.prototype.findResource = function (uri) {
  for (var i in this.mResources) {
    var resource = this.mResources[i];
    if (resource.uri() === uri) {
      return resource;
    }
  }
  return undefined;
};

/* Gateway API 2-1-1: Virtual Sensor */
function VirtualSensor(name, observerHandler, inferenceHandler) {
  this.mName = name;
  this.mObserverHandler = observerHandler;
  this.mInferenceHandler = inferenceHandler;
  this.mInletResource = undefined;
  this.mOutletResource = undefined;
  this.mControletResource = undefined;
}

/* OCF resource setting handlers */
VirtualSensor.prototype.setupInlet = function (gatewayAdapter) {
  var oa = gatewayAdapter.mOCFAdapter;
  var device = oa.getDevice(0);
  var uri = this.getUri() + '/inlet';
  this.mInletResource = OCFAPI.createResource(
    device,
    this.mName,
    uri,
    ['ant.r.inlet'],
    [OCFAPI.OC_IF_RW]
  );
  this.mInletResource.setDiscoverable(true);
  this.mInletResource.setHandler(OCFAPI.OC_POST, onPostInlet);
  oa.addResource(this.mInletResource);
  return this.mInletResource;
};

VirtualSensor.prototype.setupOutlet = function (gatewayAdapter) {
  var oa = gatewayAdapter.mOCFAdapter;
  var device = oa.getDevice(0);
  var uri = '/gateway/' + this.mName + '/outlet';
  this.mOutletResource = OCFAPI.createResource(
    device,
    this.mName,
    uri,
    ['ant.r.outlet'],
    [OCFAPI.OC_IF_RW]
  );
  this.mOutletResource.setDiscoverable(true);
  this.mOutletResource.setPeriodicObservable(1);
  this.mOutletResource.setHandler(OCFAPI.OC_GET, onGetOutlet);
  oa.addResource(this.mOutletResource);
  return this.mOutletResource;
};

VirtualSensor.prototype._sample = function () {
  // TODO: implement sampling handler
  // TODO: call mObserverHandler in sampling handlers
};

VirtualSensor.prototype.associateInlet = function (outletUri) {
  // TODO: associate inlet
};

/* Getters/setters */
VirtualSensor.prototype.getName = function () {
  return this.mName;
};

VirtualSensor.prototype.getUri = function () {
  return '/gateway/' + this.mName;
};

VirtualSensor.prototype.getInletResource = function () {
  return this.mInletResource;
};
VirtualSensor.prototype.getOutletResource = function () {
  return this.mOutletResource;
};

/* OCF handlers */
function onPostInlet(request) {
  // Associate inlet with an outlet
  var virtualSensor = gVirtualSensorAdapter.findVirtualSensorByUri(
    request.dest_uri
  );
  // TODO: parse inlet POST request
  virtualSensor.associateInlet(); // TODO: associate with target outlet
  // TODO: make OCF response
}

function onGetOutlet(request) {
  // Get output sensor data
  var virtualSensor = gVirtualSensorAdapter.findVirtualSensorByUri(
    request.dest_uri
  );
  virtualSensor.mInferenceHandler(); // TODO: process virtual sensor and get output
  // TODO: make OCF response
}

/*
 * Gateway API 2-2: DFE
 * DFE: DNN Fragment Engine for the deep sensors running on gateway
 */
function DFE(modelName, numFragments) {
  this.modelName = modelName;
  this.numFragments = numFragments;
  this.interpreters = undefined;
}

DFE.prototype.load = function () {
  this.interpreters = native.ant_ml_dfeLoad(this.modelName, this.numFragments);
};

DFE.prototype.execute = function (inputBuffer, startLayerNum, endLayerNum) {
  if (!inputBuffer instanceof Buffer) {
    throw 'Invalid inputBuffer';
  }
  if (
    typeof startLayerNum !== 'number' ||
    parseInt(startLayerNum) != startLayerNum
  ) {
    throw 'Invalid startLayerNum ' + startLayerNum;
  }
  if (typeof endLayerNum !== 'number' || parseInt(endLayerNum) != endLayerNum) {
    throw 'Invalid endLayerNum ' + endLayerNum;
  }
  return native.ant_ml_dfeExecute(
    this.interpreters,
    inputBuffer,
    startLayerNum,
    endLayerNum
  );
};

module.exports = new ANTGateway();
module.exports.ANTGateway = ANTGateway;
