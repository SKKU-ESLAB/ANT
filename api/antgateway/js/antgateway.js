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

/* OCF Resource Types of Virtual Sensor Resources */
var ORType_VSInlet = 'ant.r.vs.inlet';
var ORType_VSOutlet = 'ant.r.vs.outlet';
var ORType_VSSetting = 'ant.r.vs.setting';

/* URIs of Virtual Sensor Resources */
function ORURI_VSInlet(name) {
  return '/vs/' + name + '/inlet';
}
function ORURI_VSOutlet(name) {
  return '/vs/' + name + '/outlet';
}
function ORURI_VSSetting(name) {
  return '/vs/' + name + '/setting';
}

/**
 * ANT Gateway API
 */

var gVSAdapter = undefined;
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

/* Gateway API 2: Virtual Sensor Adapter */
ANTGateway.prototype.getVSAdapter = function () {
  if (gVSAdapter === undefined) {
    gVSAdapter = new VirtualSensorAdapter();
  }
  return gVSAdapter;
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
      '/vs',
      'ant.d.virtualsensor',
      'VirtualSensor',
      'ant.1.0.0',
      'ant.res.1.0.0'
    );
  }

  function onPrepareOCFClient() {}

  function onPrepareOCFServer() {
    self.mResources = [];
    // Add all the virtual sensors
    for (var i in self.mVirtualSensors) {
      var virtualSensor = self.mVirtualSensors[i];
      var rInlet = virtualSensor.setupInlet(self);
      self.mResources.push(rInlet);
      var rOutlet = virtualSensor.setupOutlet(self);
      self.mResources.push(rOutlet);
      var rSetting = virtualSensor.setupSetting(self);
      self.mResources.push(rSetting);
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

/* Create a new virtual sensor */
VirtualSensorAdapter.prototype.createSensor = function (
  sensorName,
  hObserver,
  hGenerator,
  hSetting
) {
  var virtualSensor = new VirtualSensor(
    sensorName,
    hObserver,
    hGenerator,
    hSetting
  );
  this.mVirtualSensors.push(virtualSensor);
  return virtualSensor;
};

/* Create a new deep sensor */
VirtualSensorAdapter.prototype.createDeepSensor = function (
  sensorName,
  modelName,
  numFragments
) {
  var dfe = this.createDFE(modelName, numFragments);
  var hObserver = dfe.getObserverHandler();
  var hGenerator = dfe.getGeneratorHandler();
  var hSetting = dfe.getSettingHandler();
  return this.createSensor(sensorName, hObserver, hGenerator, hSetting);
};

VirtualSensorAdapter.prototype.createDFE = function (modelName, numFragments) {
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

/* Virtual sensor handling methods */
VirtualSensorAdapter.prototype.findSensorByName = function (name) {
  for (var i in this.mVirtualSensors) {
    var virtualSensor = this.mVirtualSensors[i];
    if (virtualSensor.getName() === name) {
      return virtualSensor;
    }
  }
  return undefined;
};

VirtualSensorAdapter.prototype.findSensorByUri = function (uri) {
  var nameFrom = uri.indexOf('/', 1) + 1; // offset of the tail of '/vs/'
  var nameLength = uri.indexOf('/', nameFrom) - nameFrom; // offset of the tail of virtual sensor name
  if (nameFrom < 0 || nameLength < 0) return undefined;

  var name = uri.substring(nameFrom, nameLength);
  if (name === undefined || name.length == 0) return undefined;

  return this.findSensorByName(name);
};

VirtualSensorAdapter.prototype.getResource = function (uri) {
  for (var i in this.mResources) {
    var resource = this.mResources[i];
    if (resource.uri() === uri) {
      return resource;
    }
  }
  return undefined;
};

/* Gateway API 2-1-1: Virtual Sensor */
function VirtualSensor(
  name,
  observerHandler,
  generatorHandler,
  settingHandler
) {
  this.mName = name;
  this.mObserverHandler = observerHandler;
  this.mGeneratorHandler = generatorHandler;
  this.mSettingHandler = settingHandler;
  this.mInletResource = undefined;
  this.mOutletResource = undefined;
  this.mSettingResource = undefined;
  this.mObservers = [];
  this.mDFE = undefined;
}

/*
 * OCF resource setting handlers
 * These handlers must be called after OCF server is prepared.
 * Therefore, calling the handlers are postponed.
 */
VirtualSensor.prototype.setupInlet = function (gatewayAdapter) {
  var oa = gatewayAdapter.mOCFAdapter;
  var device = oa.getDevice(0);
  var uri = ORURI_VSInlet(this.mName);
  this.mInletResource = OCFAPI.createResource(
    device,
    this.mName,
    uri,
    [ORType_VSInlet],
    [OCFAPI.OC_IF_RW]
  );
  this.mInletResource.setDiscoverable(true);
  this.mInletResource.setHandler(OCFAPI.OC_POST, onPostInlet);
  this.mInletResource.setHandler(OCFAPI.OC_GET, onGetInlet);
  oa.addResource(this.mInletResource);
  return this.mInletResource;
};

VirtualSensor.prototype.setupOutlet = function (gatewayAdapter) {
  var oa = gatewayAdapter.mOCFAdapter;
  var device = oa.getDevice(0);
  var uri = ORURI_VSOutlet(this.mName);
  this.mOutletResource = OCFAPI.createResource(
    device,
    this.mName,
    uri,
    [ORType_VSOutlet],
    [OCFAPI.OC_IF_RW]
  );
  this.mOutletResource.setDiscoverable(true);
  this.mOutletResource.setPeriodicObservable(1);
  this.mOutletResource.setHandler(OCFAPI.OC_GET, onGetOutlet);
  oa.addResource(this.mOutletResource);
  return this.mOutletResource;
};

VirtualSensor.prototype.setupSetting = function (gatewayAdapter) {
  var oa = gatewayAdapter.mOCFAdapter;
  var device = oa.getDevice(0);
  var uri = ORURI_VSSetting(this.mName);
  this.mSettingResource = OCFAPI.createResource(
    device,
    this.mName,
    uri,
    [ORType_VSSetting],
    [OCFAPI.OC_IF_RW]
  );
  this.mSettingResource.setDiscoverable(true);
  this.mSettingResource.setHandler(OCFAPI.OC_POST, onPostSetting);
  oa.addResource(this.mSettingResource);
  return this.mSettingResource;
};

VirtualSensor.prototype.addObserver = function (
  sensorType,
  deviceType,
  endpoint,
  uri,
  intervalMS
) {
  var self = this;
  var oa = gVSAdapter.mOCFAdapter;
  function onIncomingInletData(response) {
    var payload = response.payload;

    // Call custom observer handler
    self.mObserverHandler(payload);
  }

  // Start observer
  var intervalDesc = setInterval(function () {
    oa.get(endpoint, uri, onIncomingInletData, undefined, undefined, true);
  }, intervalMS);

  // Add observer to the virtual sensor's observer list
  var observer = {
    sensorType: sensorType,
    deviceType: deviceType,
    intervalDesc: intervalDesc
  };
  this.mObservers.push(observer);
};

VirtualSensor.prototype.removeObserver = function (sensorType, deviceType) {
  // Find observer for the given sensor/device type
  var foundObserver = undefined;
  var foundObserverIndex = -1;
  for (var i in this.mObservers) {
    var observer = this.mObservers[i];
    if (
      observer.sensorType == sensorType &&
      observer.deviceType == deviceType
    ) {
      foundObserver = observer;
      foundObserverIndex = i;
      break;
    }
  }

  // Stop observer
  if (foundObserver !== undefined && foundObserverIndex >= 0) {
    clearInterval(foundObserver.intervalDesc);
    this.mObservers.splice(foundObserverIndex, 1);
  }
};

/* Getters/setters */
VirtualSensor.prototype.getName = function () {
  return this.mName;
};

VirtualSensor.prototype.getUri = function () {
  return '/vs/' + this.mName;
};

VirtualSensor.prototype.getInletResource = function () {
  return this.mInletResource;
};
VirtualSensor.prototype.getOutletResource = function () {
  return this.mOutletResource;
};

VirtualSensor.prototype.getObservers = function () {
  return this.mObservers;
};

/* OCF handlers */
function onPostInlet(request) {
  // POST inlet: Add observer
  var virtualSensor = gVSAdapter.findSensorByUri(request.dest_uri);

  // Parse OCF request
  var requestPayloadString = request.payload_string;
  var requestPayload = JSON.parse(requestPayloadString);
  var commandType = requestPayload.commandType; // command's type (add or remove)
  var sensorType = requestPayload.sensorType; // sensor's type (OCF resource type)
  var deviceType = requestPayload.deviceType; // device's type (OCF resource type)
  var intervalMS = requestPayload.intervalMS; // observation interval (milliseconds)
  var defaultIntervalMS = 1000; // default observation interval: 1 sec

  var response = {result: 'None', reason: 'None'};
  if (commandType === undefined || typeof commandType !== 'string')
    response = {
      result: 'Failure',
      reason: 'Invalid commandType (' + commandType + ')'
    };
  if (sensorType === undefined || typeof sensorType !== 'string')
    response = {
      result: 'Failure',
      reason: 'Invalid sensorType (' + sensorType + ')'
    };
  if (deviceType === undefined || typeof deviceType !== 'string')
    response = {
      result: 'Failure',
      reason: 'Invalid deviceType (' + deviceType + ')'
    };
  if (sensorType == deviceType)
    response = {
      result: 'Failure',
      reason: 'sensorType == deviceType (' + sensorType + ')'
    };
  if (intervalMS === undefined) intervalMS = defaultIntervalMS;
  else if (typeof intervalMS !== 'number')
    response = {
      result: 'Failure',
      reason: 'Invalid intervalMS (' + intervalMS + ')'
    };

  if (commandType !== 'add' && commandType !== 'remove')
    response = {
      result: 'Failure',
      reason: 'Invalid commandType (' + commandType + ')'
    };

  if (response.result !== 'Failure')
    response = _onPostInlet_internal(
      commandType,
      sensorType,
      deviceType,
      intervalMS
    );

  // Send response
  var oa = gVSAdapter.mOCFAdapter;
  oa.repStartRootObject();
  oa.repSet('result', response.result);
  oa.repSet('reason', response.reason);
  oa.repEndRootObject();
  oa.sendResponse(request, ocf.OC_STATUS_OK);
}

function _onPostInlet_internal(
  commandType,
  sensorType,
  deviceType,
  intervalMS
) {
  // Step 1. Discover outlet resource
  var oa = gVSAdapter.mOCFAdapter;
  function onDiscoveryAfterPostInlet(endpoint, uri, types, interfaceMask) {
    // Step 2. On discover outlet resource
    var isFoundSensorType = false;
    var isFoundDeviceType = false;
    for (var i in types) {
      if (types[i] == sensorType) {
        isFoundSensorType = true;
      } else if (types[i] == deviceType) {
        isFoundDeviceType = true;
      }
    }
    if (isFoundSensorType && isFoundDeviceType) {
      if (commandType == 'add') {
        // Add observer for the discovered outlet resource
        virtualSensor.addObserver(
          sensorType,
          deviceType,
          endpoint,
          uri,
          intervalMS
        );
      } else if (commandType == 'remove') {
        // Remove observer
        virtualSensor.removeObserver(sensorType, deviceType);
      }
    }
  }

  oa.discovery(ORType_VSOutlet, onDiscoveryAfterPostInlet);
  return {result: 'Success', reason: 'None'};
}

function onGetInlet(request) {
  // GET inlet: Get observers list
  var virtualSensor = gVSAdapter.findSensorByUri(request.dest_uri);

  // Get observers list
  var observers = virtualSensor.getObservers();

  // Send response
  var oa = gVSAdapter.mOCFAdapter;
  oa.repStartRootObject();
  oa.repSet('observersJSON', JSON.stringify(observers));
  oa.repEndRootObject();
  oa.sendResponse(request, ocf.OC_STATUS_OK);
}

function onGetOutlet(request) {
  // GET outlet: Get DFE message
  var virtualSensor = gVSAdapter.findSensorByUri(request.dest_uri);

  // Call custom generator handler
  var result = virtualSensor.mGeneratorHandler();

  // Check result
  var isFailed = false;
  if (typeof result !== 'object' || result instanceof Buffer) {
    console.error('Invalid result: not buffer');
    isFailed = true;
  }

  // Send response
  if (!isFailed) {
    var oa = gVSAdapter.mOCFAdapter;
    oa.repStartRootObject();
    oa.repSetBufferAndString(result.byteValue, result.stringValue);
    oa.repEndRootObject();
    oa.sendResponse(request, ocf.OC_STATUS_OK);
  }
}

function onPostSetting(request) {
  // POST dfe: control DFE's fragment number and get it's computing status
  var virtualSensor = gVSAdapter.findSensorByUri(request.dest_uri);

  // Parse OCF request
  var requestPayloadString = request.payload_string;
  var setting = JSON.parse(requestPayloadString);

  // Call custom setting handler
  var result = virtualSensor.mSettingHandler(setting);
  var resultJSONString = JSON.stringify(result);

  // Send response
  if (!isFailed) {
    var oa = gVSAdapter.mOCFAdapter;
    oa.repStartRootObject();
    oa.repSet('result', resultJSONString);
    oa.repEndRootObject();
    oa.sendResponse(request, ocf.OC_STATUS_OK);
  }

  // TODO: setting command API
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

DFE.prototype.getObserverHandler = function () {
  // TODO:
};

DFE.prototype.getGeneratorHandler = function () {
  // TODO:
  function dfeGeneratorHandler() {}
  return dfeGeneratorHandler;
};

DFE.prototype.getSettingHandler = function () {
  // TODO:
  function dfeSettingHandler(setting) {
    var fragNum = setting.fragNum;
  }
  return dfeSettingHandler;
};

function DFECoordinator() {
  // TODO: implement DFE coordinator
}

module.exports = new ANTGateway();
module.exports.ANTGateway = ANTGateway;
