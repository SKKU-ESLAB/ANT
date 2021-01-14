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

var gGatewayAdapter = undefined;
function ANTGateway() {}

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

ANTGateway.prototype.getAdapter = function () {
  if (gGatewayAdapter === undefined) {
    gGatewayAdapter = new GatewayAdapter();
  }
  return gGatewayAdapter;
};

function GatewayAdapter() {
  var self = this;
  this.mOCFAdapter = OCFAPI.getAdapter();
  this.mOCFAdapter.onPrepareEventLoop(function () {
    self.setPlatform('ant');
    self.addDevice(
      '/gateway',
      'ant.d.gateway',
      'Gateway',
      'ant.1.0.0',
      'ant.res.1.0.0'
    );
  });
  this.mVirtualSensors = [];
  this.mResources = [];
}

GatewayAdapter.prototype.addVirtualSensor = function (
  name,
  inletHandler,
  outletHandler
) {
  var virtualSensor = new VirtualSensor(name, inletHandler, outletHandler);
  this.mVirtualSensors.push(virtualSensor);
  this.mResources.push(virtualSensor.getInletResource());
  this.mResources.push(virtualSensor.getOutletResource());
};

GatewayAdapter.prototype.findVirtualSensor = function (name) {
  for (var i in this.mVirtualSensors) {
    var virtualSensor = this.mVirtualSensors[i];
    if (virtualSensor.getName() === name) {
      return virtualSensor;
    }
  }
  return undefined;
};

GatewayAdapter.prototype.findVirtualSensorByUri = function (uri) {
  var nameFrom = uri.indexOf('/', 1) + 1;
  var nameLength = uri.indexOf('/', nameFrom) - nameFrom;
  if (nameFrom < 0 || nameLength < 0) return undefined;

  var name = uri.substring(nameFrom, nameLength);
  if (name === undefined || name.length == 0) return undefined;

  return this.findVirtualSensor(name);
};

GatewayAdapter.prototype.findResource = function (uri) {
  for (var i in this.mResources) {
    var resource = this.mResources[i];
    if (resource.uri() === uri) {
      return resource;
    }
  }
  return undefined;
};

GatewayAdapter.prototype.start = function () {
  var self = this;
  this.mOCFAdapter.onPrepareServer(function () {
    // Add all the virtual sensors
    for (var i in self.mVirtualSensors) {
      var virtualSensor = self.mVirtualSensors[i];
      virtualSensor.setupInlet(self);
      virtualSensor.setupOutlet(self);
    }
  });
  this.mOCFAdapter.start();
};

GatewayAdapter.prototype.stop = function () {
  this.mOCFAdapter.stop();
  this.mOCFAdapter.deinitialize();
};

function VirtualSensor(name, inletHandler, outletHandler) {
  this.mName = name;
  this.mInletHandler = inletHandler;
  this.mOutletHandler = outletHandler;
  this.mInletResource = undefined;
  this.mOutletResource = undefined;
}
VirtualSensor.prototype.getName = function () {
  return this.mName;
};

VirtualSensor.prototype.getInletResource = function () {
  return this.mInletResource;
};
VirtualSensor.prototype.getOutletResource = function () {
  return this.mOutletResource;
};

VirtualSensor.prototype._sample = function () {
  // TODO: implement sampling handler
  // TODO: call mInletHandler in sampling handlers
};

VirtualSensor.prototype.associateInlet = function (outletUri) {
  // TODO: associate inlet
};

function onPostInlet(request) {
  // Associate inlet with an outlet
  var virtualSensor = gGatewayAdapter.findVirtualSensorByUri(request.dest_uri);
  // TODO: parse inlet POST request
  virtualSensor.associateInlet(); // TODO: associate with target outlet
  // TODO: make OCF response
}
function onGetOutlet(request) {
  // Get output sensor data
  var virtualSensor = gGatewayAdapter.findVirtualSensorByUri(request.dest_uri);
  virtualSensor.mOutletHandler(); // TODO: process virtual sensor and get output
  // TODO: make OCF response
}

VirtualSensor.prototype.setupInlet = function (gatewayAdapter) {
  var oa = gatewayAdapter.mOCFAdapter;
  var device = oa.getDevice(0);
  var uri = '/gateway/' + this.mName + '/inlet';
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

module.exports = new ANTGateway();
module.exports.ANTGateway = ANTGateway;
