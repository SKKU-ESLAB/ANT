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
var fs = require('fs');

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

  function onPrepareOCFClient() {
    // TODO: Virtual Sensor Manager
    // TODO: DFE coordinator
    // TODO: get virtual sensor output
  }

  function onPrepareOCFServer() {
    self.mResources = [];
    // Add all the virtual sensors
    for (var i in self.mVirtualSensors) {
      var virtualSensor = self.mVirtualSensors[i];
      if (virtualSensor.getObserverHandler() !== undefined) {
        var rInlet = virtualSensor.setupInlet(self);
        self.addResource(rInlet);
      }
      if (virtualSensor.getGeneratorHandler() !== undefined) {
        var rOutlet = virtualSensor.setupOutlet(self);
        self.addResource(rOutlet);
      }
      if (virtualSensor.getSettingHandler() !== undefined) {
        var rSetting = virtualSensor.setupSetting(self);
        self.addResource(rSetting);
      }
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
  observerHandler,
  generatorHandler,
  settingHandler
) {
  // Check arguments
  if (sensorName === undefined || typeof sensorName !== 'string') {
    throw 'VSA createSensor error: Invalid sensorName ' + sensorName;
  }
  if (observerHandler !== undefined && typeof observerHandler !== 'function') {
    // Observer is optional.
    throw 'VSA observer error: Invalid observerHandler';
  }
  if (
    generatorHandler === undefined ||
    typeof generatorHandler !== 'function'
  ) {
    // Generator is mandatory.
    throw 'VSA observer error: Invalid generatorHandler';
  }
  if (settingHandler !== undefined && typeof settingHandler !== 'function') {
    // Setting is optional.
    throw 'VSA observer error: Invalid settingHandler';
  }

  var virtualSensor = new VirtualSensor(
    sensorName,
    observerHandler,
    generatorHandler,
    settingHandler
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
  // Check arguments
  if (sensorName === undefined || typeof sensorName !== 'string') {
    throw 'VSA createDeepSensor error: Invalid sensorName ' + sensorName;
  }
  if (modelName === undefined || typeof modelName !== 'string') {
    throw 'VSA createDeepSensor error: Invalid modelName ' + modelName;
  }
  if (numFragments === undefined || typeof numFragments !== 'number') {
    throw 'VSA createDeepSensor error: Invalid numFragments ' + numFragments;
  }

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
  var dfe = new DFE(modelName, numFragments);
  dfe.load();
  return dfe;
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

VirtualSensorAdapter.prototype.addResource = function (resource) {
  if (resource !== undefined) {
    this.mResources.push(resource);
  }
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
    var inputData = {};
    if (response.payload !== undefined) {
      var payloadObj = JSON.parse(response.payload);
      inputData.jsObject = payloadObj.jsObject;
    } else {
      inputData.jsObject = JSON.parse(response.payloadString);
      inputData.buffer = response.payloadBuffer;
    }
    if (inputData.jsObject === undefined) {
      console.error('inlet error: jsObject is not defined');
      return;
    }
    if (!(inputData.buffer instanceof Buffer)) {
      console.error('inlet error: invalid buffer');
      return;
    }

    /*
     * Call custom observer handler
     * inputData.jsObject: (mandatory) JavaScript object
     * inputData.buffer: (option) Buffer
     */
    self.mObserverHandler(inputData);
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
VirtualSensor.prototype.getSettingResource = function () {
  return this.mSettingResource;
};

VirtualSensor.prototype.getObserverHandler = function () {
  return this.mObserverHandler;
};
VirtualSensor.prototype.getGeneratorHandler = function () {
  return this.mGeneratorHandler;
};
VirtualSensor.prototype.getSettingHandler = function () {
  return this.mSettingHandler;
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
  if (typeof result !== 'object') {
    console.error('Invalid result: not object');
    isFailed = true;
  }
  if (result.jsObject === undefined) {
    onsole.error('Invalid result.jsObject: not defined');
    isFailed = true;
  } else if (typeof result.jsObject !== 'string') {
    console.error('Invalid result.jsObject: not object');
    isFailed = true;
  }
  if (result.buffer !== undefined && result.buffer instanceof Buffer) {
    console.error('Invalid result.buffer: not Buffer');
    isFailed = true;
  }

  // Send response
  if (!isFailed) {
    var oa = gVSAdapter.mOCFAdapter;
    oa.repStartRootObject();
    if (result.buffer !== undefined) {
      var jsonObject = JSON.stringify(result.jsObject);
      var buffer = result.buffer;
      oa.repSetBufferAndString(jsonObject, buffer);
    } else {
      var jsonObject = JSON.stringify(result.jsObject);
      oa.repSet('jsObject', jsonObject);
    }
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
  // TODO: pre-processing handler
  // TODO: post-processing handler
  this.modelName = modelName;
  this.numFragments = numFragments;
  this.interpreters = undefined;
  this.recentInputData = undefined;
  this.presentFragNum = numFragments - 1;
  this.averageLatencyMS = 0.0;
}

DFE.prototype.updateAverageLatency = function (latency) {
  // Exponential moving average
  var kMomentum = 0.9;
  this.averageLatencyMS =
    this.averageLatencyMS * kMomentum + latency * (1 - kMomentum);
};

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
  var self = this;
  function dfeObserverHandler(inputData) {
    // TODO: custom DFE observer handler
    self.recentInputData = inputData;
  }
  return dfeObserverHandler;
};

DFE.prototype.getGeneratorHandler = function () {
  var self = this;
  function dfeGeneratorHandler() {
    var inputData = self.recentInputData;
    var jsObject = inputData.jsObject;
    var buffer = inputData.buffer;
    if (jsObject === undefined) {
      console.error('DFE generator error: no jsObject');
      return;
    }
    if (buffer === undefined) {
      console.error('DFE generator error: no buffer');
      return;
    }

    var dfeFlag = jsObject.dfeFlag;
    var fragNum = jsObject.jsObject;
    if (dfeFlag === undefined || typeof dfeFlag !== 'boolean') {
      console.error('DFE generator error: invalid dfeFlag');
      return;
    } else if (dfeFlag) {
      if (fragNum === undefined || typeof fragNum !== 'number') {
        console.error('DFE generator error: invalid fragNum');
        return;
      }
    }

    // Execute DNN fragment
    var startTimeMS = Date.now();
    self.execute(buffer, fragNum, self.numFragments);
    var endTimeMS = Date.now();

    // Update average latency
    var latencyMS = endTimeMS - startTimeMS;
    self.updateAverageLatency(latencyMS);
  }
  return dfeGeneratorHandler;
};

DFE.prototype.getLoad = function () {
  var loadavg = fs.readFileSync('/proc/loadavg').toString();
  var avgLoad1min = parseFloat(loadavg.substring(0, loadavg.indexOf(' ')));
  return avgLoad1min;
};

DFE.prototype.getSettingHandler = function (fragNum) {
  var self = this;
  function dfeSettingHandler(setting) {
    // Set fragment number
    self.presentFragNum = setting.fragNum;

    // Return status
    var latencyMS = self.averageLatencyMS;
    var load = self.getLoad();

    var result = {latencyMS: latencyMS, load: load};
    return result;
  }
  return dfeSettingHandler;
};

function DFECoordinator() {
  // TODO: implement
}

function GatewayManager() {
  // TODO: implement
}

function GatewayManager() {
  // TODO: 
}

module.exports = new ANTGateway();
module.exports.ANTGateway = ANTGateway;
