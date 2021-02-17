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

/**
 * ANT Gateway API
 *
 * In ANT framework, gateway provides sensor virtualization and DNN
 * partitioning. Sensor virtualization means unifying interfaces to access
 * physical sensors, soft sensors, local sensors, and remote sensors.
 * DNN partitioning means executing a DNN on multiple heterogeneous devices.
 * In real IoT systems, there are heterogeneous devices including gateway,
 * but the single device among them cannot provide sufficient computational
 * resources to DNNs. Therefore, DNN partitioning is required.
 *
 * There are two ways to implement gateway in ANT framework.
 * 1. ML fragment element (Based on gstreamer)
 * 2. Virtual sensor (Based on OCF standard)
 *
 * ML fragment element is an approach to extend the gstreamer to DNN
 * partitioning. In this way, to associate a DNN with a physical sensor, the
 * app developer must implement their own sensor sampling element that can be
 * compatible to gstreamer.
 *
 * Actually, many IoT devices does not use gstreamer-based approach, but use
 * OCF standard. Therefore, we additionally provide virtual sensor, an approach
 * based on OCF standard.
 *
 * Virtual sensor is an abstract layer that provide unified interface of
 * physical sensors, rule-based soft sensors, and DNN-based soft sensors
 * (a.k.a. deep sensors). It also supports DNN partitioning on the deep
 * sensors. To support that, ANT additionally provides DNN fragment engine
 * and gateway manager.
 */

var gVSAdapter = undefined;
function ANTGateway() {}

/*
 * Gateway API 1. ML fragment element (Based on gstreamer)
 */

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

/*
 * Gateway API 2. Virtual sensor (Based on OCF standard)
 *
 * There are many modules related to virtual sensor.
 *
 * - VirtualSensorAdapter : module to arbitrate user and OCF thread and to
 *                          organize virtual sensor system.
 *   - VirtualSensor : module to service virtual sensor through OCF resources
 *                     and several JavaScript handlers.
 *   - DFE (DNN Fragment Engine) : module to perform partitioned DNN inference
 *                                 (as now, it uses Tensorflow Lite.)
 *   - GatewayManager : module to manage virtual sensors and DNN partitioning.
 *                      This module must run on gateway device.
 *     - DFEScheduler : module to manage DNN partitioning. It monitors
 *                        the current status of DFEs running on several
 *                        devices and make commands to control DFE's
 *                        partitioning points.
 *     - VirtualSensorManager : module to manage virtual sensors. It discovers
 *                              virtual sensors via OCF discovery message and
 *                              associate the virtual sensors to form a sensor
 *                              data pipeline for the IoT applications.
 */
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
  this.mGatewayManager = undefined;

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
    var gwManager = self.getGWManager();
    if (gwManager !== undefined) {
      gwManager.onOCFClientPrepared(self);
    }
  }

  function onPrepareOCFServer() {
    self.clearResources();
    // Add OCF resources of all the virtual sensors
    for (var i in self.mVirtualSensors) {
      var virtualSensor = self.mVirtualSensors[i];
      var resources = virtualSensor.setup(self);
      for (var j in resources) {
        self.addResource(resources[j]);
      }
    }

    // Add OCF resources of gateway manager
    var gwManager = self.getGWManager();
    if (gwManager !== undefined) {
      var resources = gwManager.setup();
      for (var i in resources) {
        self.addResource(resources[j]);
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
  sensorType,
  deviceType,
  observerHandler,
  generatorHandler,
  settingHandler
) {
  // Check arguments
  if (sensorName === undefined || typeof sensorName !== 'string') {
    throw 'VSA createSensor error: Invalid sensorName ' + sensorName;
  }
  if (sensorType === undefined || typeof sensorType !== 'string') {
    throw 'VSA createSensor error: Invalid sensorType ' + sensorType;
  }
  if (deviceType === undefined || typeof deviceType !== 'string') {
    throw 'VSA createSensor error: Invalid deviceType ' + deviceType;
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
    sensorType,
    deviceType,
    observerHandler,
    generatorHandler,
    settingHandler
  );
  this.mVirtualSensors.push(virtualSensor);
  return virtualSensor;
};

/* Create a new deep sensor */
// TODO: add pre-processing handler, post-processing handler
VirtualSensorAdapter.prototype.createDeepSensor = function (
  sensorName,
  sensorType,
  deviceType,
  modelName,
  numFragments
) {
  // Check arguments
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
  return this.createSensor(
    sensorName,
    sensorType,
    deviceType,
    hObserver,
    hGenerator,
    hSetting
  );
};

// TODO: add pre-processing handler, post-processing handler
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

/* Gateway manager */
VirtualSensorAdapter.prototype.createGWManager = function () {
  if (this.mGatewayManager === undefined) {
    this.mGatewayManager = new GatewayManager();
  }
  return this.getGWManager();
};

/* Getters */
VirtualSensorAdapter.prototype.getOCFAdapter = function () {
  return this.mOCFAdapter;
};
VirtualSensorAdapter.prototype.getGWManager = function () {
  return this.mGatewayManager;
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

VirtualSensorAdapter.prototype.clearResources = function () {
  this.mResources = [];
};

VirtualSensorAdapter.prototype.addResource = function (resource) {
  if (resource !== undefined) {
    this.mResources.push(resource);
  }
};

/* OCF Resource Types of Virtual Sensor Resources */
var ORType_VSRoot = 'ant.r.vs';
var ORType_VSMask = 'ant.r.vs.';
var ORType_VSInlet = 'ant.r.vs.inlet';
var ORType_VSOutlet = 'ant.r.vs.outlet';
var ORType_VSSetting = 'ant.r.vs.setting';

/* OCF Resource Types of Gateway Manager Resources */
var ORType_GWRoot = 'ant.r.gw';
var ORType_GWMask = 'ant.r.gw.';
var ORType_GWVSM = 'ant.r.gw.vsm';

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

/* Gateway API 2-1: Virtual Sensor */
function VirtualSensor(
  name,
  sensorType,
  deviceType,
  observerHandler,
  generatorHandler,
  settingHandler
) {
  this.mName = name;
  this.mSensorType = sensorType;
  this.mDeviceType = deviceType;
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
VirtualSensor.prototype.setup = function (vsAdapter) {
  var oa = vsAdapter.getOCFAdapter();
  var resources = [];
  if (this.getObserverHandler() !== undefined) {
    var rInlet = this.setupInlet(oa);
    resources.push(rInlet);
  }
  if (this.getGeneratorHandler() !== undefined) {
    var rOutlet = this.setupOutlet(oa);
    resources.push(rOutlet);
  }
  if (this.getSettingHandler() !== undefined) {
    var rSetting = this.setupSetting(oa);
    resources.push(rSetting);
  }
  return resources;
};

VirtualSensor.prototype.setupInlet = function (oa) {
  var device = oa.getDevice(0);
  var uri = ORURI_VSInlet(this.mName);
  this.mInletResource = OCFAPI.createResource(
    device,
    this.mName,
    uri,
    [ORType_VSRoot, ORType_VSInlet, this.mDeviceType, this.mSensorType],
    [OCFAPI.OC_IF_RW]
  );
  this.mInletResource.setDiscoverable(true);
  this.mInletResource.setPeriodicObservable(1);
  this.mInletResource.setHandler(OCFAPI.OC_POST, onPostInlet);
  this.mInletResource.setHandler(OCFAPI.OC_GET, onGetInlet);
  oa.addResource(this.mInletResource);
  return this.mInletResource;
};

VirtualSensor.prototype.setupOutlet = function (oa) {
  var device = oa.getDevice(0);
  var uri = ORURI_VSOutlet(this.mName);
  this.mOutletResource = OCFAPI.createResource(
    device,
    this.mName,
    uri,
    [ORType_VSRoot, ORType_VSOutlet, this.mDeviceType, this.mSensorType],
    [OCFAPI.OC_IF_RW]
  );
  this.mOutletResource.setDiscoverable(true);
  this.mOutletResource.setPeriodicObservable(1);
  this.mOutletResource.setHandler(OCFAPI.OC_GET, onGetOutlet);
  oa.addResource(this.mOutletResource);
  return this.mOutletResource;
};

VirtualSensor.prototype.setupSetting = function (oa) {
  var device = oa.getDevice(0);
  var uri = ORURI_VSSetting(this.mName);
  this.mSettingResource = OCFAPI.createResource(
    device,
    this.mName,
    uri,
    [ORType_VSRoot, ORType_VSSetting, this.mDeviceType, this.mSensorType],
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

  // Parse OCF request
  var requestPayloadString = request.payload_string;
  var requestPayload = JSON.parse(requestPayloadString);
  var commandType = requestPayload.commandType; // command's type (add or remove)
  var sensorType = requestPayload.sensorType; // sensor's type (OCF resource type)
  var deviceType = requestPayload.deviceType; // device's type (OCF resource type)
  var intervalMS = requestPayload.intervalMS; // observation interval (milliseconds)
  var defaultIntervalMS = 1000; // default observation interval: 1 sec

  // Check OCF request
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
}

/*
 * Gateway API 3: DFE
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

// TODO: hard-coding input
DFE.prototype.loadAndPreprocessImage = function (imgPath) {
  return native.ant_gateway_dfeLoadAndPreprocessImage(imgPath);
};

DFE.prototype.load = function () {
  this.interpreters = native.ant_gateway_dfeLoad(
    this.modelName,
    this.numFragments
  );
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
  return native.ant_gateway_dfeExecute(
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

/* Gateway Manager */
function GatewayManager() {
  this.mVirtualSensorManager = new VirtualSensorManager();
  this.mDFEScheduler = new DFEScheduler();
}

GatewayManager.prototype.getVSManager = function () {
  return this.mVirtualSensorManager;
};

GatewayManager.prototype.getDFEScheduler = function () {
  return this.mDFEScheduler;
};

GatewayManager.prototype.setup = function (vsManager) {
  var resources = [];
  var oa = vsManager.getOCFAdapter();

  var vsManager = gwManager.getVSManager();
  var rVSManager = vsManager.setup(oa);
  resources.push(rVSManager);

  return resources;
};

GatewayManager.prototype.onOCFClientPrepared = function (vsAdapter) {
  // Virtual sensor manager: start discovery
  var vsManager = this.getVSManager();
  if (vsManager !== undefined) {
    vsManager.startDiscovery(vsAdapter);
  }

  // DFE scheduler: start
  var dfeScheduler = this.getDFEScheduler();
  if (dfeScheduler !== undefined) {
    dfeScheduler.start();
  }
};

/* Virtual Sensor Manager */
function VirtualSensorManager() {
  this.mResource = undefined;
  this.mFoundInlets = [];
  this.mFoundOutlets = [];
  this.mFoundSettings = [];
}

VirtualSensorManager.prototype.setup = function (oa) {
  var device = oa.getDevice(0);
  var uri = ORURI_VSInlet(this.mName);
  this.mInletResource = OCFAPI.createResource(
    device,
    this.mName,
    uri,
    [ORType_GWRoot, ORType_GWVSM],
    [OCFAPI.OC_IF_RW]
  );
  this.mInletResource.setDiscoverable(true);
  this.mInletResource.setHandler(OCFAPI.OC_GET, onGetVirtualSensorManager);
  this.mInletResource.setHandler(OCFAPI.OC_POST, onPostVirtualSensorManager);
  oa.addResource(this.mInletResource);
  return this.mInletResource;
};

// Virtual sensor manager (OCF-client-side): background discovery
VirtualSensorManager.prototype.startDiscovery = function (vsAdapter) {
  var self = this;
  function onDiscovery(endpoint, uri, types, interfaceMask) {
    var isFound = false;
    for (var i in types) {
      if (types[i] == ORType_VSRoot) {
        isFound = true;
      }
    }
    if (isFound) {
      self.addResource(endpoint, uri, types);
    }
  }

  var oa = vsAdapter.getOCFAdapter();
  oa.discoveryAll(onDiscovery);
};

// Virtual sensor manager (OCF-server-side): POST association command
function onPostVirtualSensorManager(request) {
  // POST association command
  var response = {result: 'None', reason: 'None'};

  // Parse OCF request
  var requestPayload = JSON.parse(request.payload_string);
  var sinkInlet = requestPayload.sinkInlet;
  var sourceOutlet = requestPayload.sourceOutlet;
  var intervalMS = requestPayload.intervalMS;

  // Check OCF request
  if (
    sinkInlet === undefined ||
    sinkInlet.sensorType === undefined ||
    sinkInlet.deviceType === undefined ||
    typeof sinkInlet.sensorType !== 'string' ||
    typeof sinkInlet.deviceType !== 'string'
  ) {
    response = {
      result: 'Failure',
      reason: 'Invalid inlet: ' + JSON.stringify(sinkInlet)
    };
  }
  if (
    sourceOutlet === undefined ||
    sourceOutlet.sensorType === undefined ||
    sourceOutlet.deviceType === undefined ||
    typeof sourceOutlet.sensorType !== 'string' ||
    typeof sourceOutlet.deviceType !== 'string'
  ) {
    response = {
      result: 'Failure',
      reason: 'Invalid outlet: ' + JSON.stringify(sourceOutlet)
    };
  }
  if (intervalMS !== undefined && typeof intervalMS !== 'number') {
    response = {
      result: 'Failure',
      reason: 'Invalid intervalMS: ' + JSON.stringify(intervalMS)
    };
  }

  // Find sink inlet
  var foundSinkInlet = undefined;
  if (response.result !== 'Failure') {
    var vsManager = gVSAdapter.getGWManager().getVSManager();
    var foundInlets = vsManager.mFoundInlets();
    for (var i in foundInlets) {
      var foundInlet = foundInlets[i];
      var types = foundInlet.types;
      var isFoundSensorType = false;
      var isFoundDeviceType = false;
      for (var j in types) {
        var type = types[j];
        if (type == sinkInlet.sensorType) isFoundSensorType = true;
        if (type == sinkInlet.deviceType) isFoundDeviceType = true;
      }

      if (isFoundSensorType && isFoundDeviceType) {
        foundSinkInlet = foundInlet;
        break;
      }
    }
  }
  if (foundSinkInlet === undefined) {
    response = {
      result: 'Failure',
      reason: 'Cannot find inlet resource ' + JSON.stringify(sinkInlet)
    };
  }

  // Send response
  var oa = gVSAdapter.mOCFAdapter;
  oa.repStartRootObject();
  oa.repSet('result', response.result);
  oa.repSet('reason', response.reason);
  oa.repEndRootObject();
  oa.sendResponse(request, ocf.OC_STATUS_OK);

  // Send association command to sink inlet
  if (response.result !== 'Failure') {
    setTimeout(function () {
      var res = oa.initPost(
        foundSinkInlet.endpoint,
        foundSinkInlet.uri,
        undefined,
        '',
        ocf.OC_LOW_QOS
      );
      if (res) {
        oa.repStartRootObject();
        oa.repSet('commandType', 'add');
        oa.repSet('sensorType', sourceOutlet.sensorType);
        oa.repSet('deviceType', sourceOutlet.deviceType);
        if (intervalMS !== undefined) {
          oa.repSet('intervalMS', intervalMS);
        }
        oa.repEndRootObject();
        oa.post();
      }
    }, 100);
  }
}

// Virtual sensor manager (OCF-server-side): GET found virtual sensor resources
function onGetVirtualSensorManager(request) {
  // GET found virtual sensor resources
  var inlets = [];
  var foundInlets = this.mFoundInlets;
  for (var i in foundInlets) {
    var entry = {
      types: foundInlets[i].types,
      uri: foundInlets[i].uri
    };
    inlets.push(entry);
  }

  var outlets = [];
  var foundOutlets = this.mFoundOutlets;
  for (var i in foundOutlets) {
    var entry = {
      types: foundOutlets[i].types,
      uri: foundOutlets[i].uri
    };
    outlets.push(entry);
  }

  var settings = [];
  var foundSettings = this.mFoundSettings;
  for (var i in foundSettings) {
    var entry = {
      types: foundSettings[i].types,
      uri: foundSettings[i].uri
    };
    settings.push(entry);
  }

  // Send response
  var oa = gVSAdapter.mOCFAdapter;
  oa.repStartRootObject();
  oa.repSet('inletsJSON', JSON.stringify(inlets));
  oa.repSet('outletsJSON', JSON.stringify(outlets));
  oa.repSet('settingsJSON', JSON.stringify(settings));
  oa.repEndRootObject();
  oa.sendResponse(request, ocf.OC_STATUS_OK);
}

VirtualSensorManager.prototype.addResource = function (endpoint, uri, types) {
  var vsTypes = [];
  var letType = undefined;
  for (var i in types) {
    var type = types[i];
    if (type.indexOf(ORType_VSMask) >= 0) {
      letType = type;
      continue;
    } else if (type == ORType_VSRoot) {
      continue;
    }
    vsTypes.push(type);
  }
  var foundEntry = {endpoint: endpoint, uri: uri, types: vsTypes};
  if (letType == ORType_VSInlet) {
    this.mFoundInlets.push(foundEntry);
  } else if (letType == ORType_VSOutlet) {
    this.mFoundOutlets.push(foundEntry);
  } else if (letType == ORType_VSSetting) {
    this.mFoundSettings.push(foundEntry);
  } else {
    console.error('Cannot identify resource: ' + uri);
    console.error('\tTypes: ' + JSON.stringify(types));
  }
};

/* DFE Scheduler */
function DFEScheduler() {
  this.mResource = undefined;
}

DFEScheduler.prototype.start = function (vsAdapter) {
  // TODO: DFEScheduler: client - GET setting resource
};

module.exports = new ANTGateway();
module.exports.ANTGateway = ANTGateway;
