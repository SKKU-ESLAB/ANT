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

/* OCF Device */
var gDeviceUri = '/vs/d';
var gDeviceType = 'ant.d.virtualsensor';
var gDeviceName = 'VirtualSensorDevice';
var gDeviceSpec = 'ant.1.0.0';
var gDeviceDataModel = 'ant.res.1.0.0';

/* OCF Resource Types of Virtual Sensor Resources */
var gVSUriRoot = 'ant.r.vs';
var gVSUriMask = 'ant.r.vs.';
var gVSInletUri = 'ant.r.vs.inlet';
var gVSOutletUri = 'ant.r.vs.outlet';
var gVSSettingUri = 'ant.r.vs.setting';

/* OCF Resource Types of Gateway Manager Resources */
var gGWUriRoot = 'ant.r.gw';
// var gGWUriMask = 'ant.r.gw.';
var gGWVSMUri = 'ant.r.gw.vsm';

/* URIs of Virtual Sensor Resources */

/**
 * @private
 */
function getVSInletUri(name) {
  return '/vs/' + name + '/inlet';
}

/**
 * @private
 */
function getVSOutletUri(name) {
  return '/vs/' + name + '/outlet';
}

/**
 * @private
 */
function getVSSettingUri(name) {
  return '/vs/' + name + '/setting';
}
var gGWVSMUri = '/gw/vsm';

var gVSAdapter = undefined;

/**
 * @description The constructor of ANT Gateway API object.
 * @class
 * @classdesc ANT Gateway API's main object.
 * 
 * In ANT framework, gateway provides sensor virtualization and
 * DNN partitioning. Sensor virtualization means unifying interfaces to access
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
function ANTGateway() {}

/**
 * Create a ML fragment element that runs a image classificaiton model trained
 * with ImageNet dataset. It is the first way to perform gateway-centric DNN
 * partitioning. It internally uses gstreamer.
 * @param {String} modelPath the path of image classification model
 * @param {Number} numFragments the number of fragments
 * @param {String} targetUri the URI of target device that will be used for
 *                           DNN partitioning
 * @returns {Object} a new ML fragment element for the ML model
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

/**
 * Create a DFE (DNN fragment engine) instance for the given model name and
 * the number of fragments.
 * @param {String} modelName the name of model to be executed by DFE
 * @param {String} numFragments the number of the model's fragments
 * @returns {Object} a new DFE instance
 */
ANTGateway.prototype.createDFE = function (modelName, numFragments) {
  // TODO(RedCarrottt): add pre-processing handler, post-processing handler
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

/*
 * Virtual sensor (Based on OCF standard)
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

/**
 * Create a virtual sensor adapter. It is the second way to perform
 * gateway-centric DNN partitioning. It internally uses OCF standard.
 * @returns {Object} virtual sensor adapter object
 */
ANTGateway.prototype.getVSAdapter = function () {
  if (gVSAdapter === undefined) {
    gVSAdapter = new VirtualSensorAdapter();
  }
  return gVSAdapter;
};

/* Gateway API 2-1: Virtual Sensor Adapter */

/**
 * @description The constructor of virtual sensor adapter.
 * @class
 * @classdesc Virtual sensor adapter object. It is a module to arbitrate user
 * and OCF thread and to organize virtual sensor system.
 * This object can be created by using GatewayAPI.getVSAdapter().
 */
function VirtualSensorAdapter() {
  var self = this;
  /* Attributes */
  this.mOCFAdapter = OCFAPI.getAdapter();
  this.mVirtualSensors = [];
  this.mResources = [];
  this.mGatewayManager = undefined;
  this.mGatewayClient = undefined;

  /* OCF lifecycle handlers */
  function onPrepareOCFEventLoop() {
    self.mOCFAdapter.setPlatform('ant');
    self.mOCFAdapter.addDevice(
      gDeviceUri,
      gDeviceType,
      gDeviceName,
      gDeviceSpec,
      gDeviceDataModel
    );
  }

  function onPrepareOCFClient() {
    var gwManager = self.getGWManager();
    if (gwManager !== undefined) {
      gwManager.onOCFClientPrepared(self);
    }
    var gwClient = self.getGWClient();
    if (gwClient !== undefined) {
      gwClient.onOCFClientPrepared(self);
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
      var resources = gwManager.setup(self);
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

/**
 * Start the virtual sensor adapter. This function starts also OCF adapter
 * managed by OCF API.
 */
VirtualSensorAdapter.prototype.start = function () {
  this.mOCFAdapter.start();
};

/**
 * Stop the virtual sensor adapter. This function stops also OCF adapter
 * managed by OCF API.
 */
VirtualSensorAdapter.prototype.stop = function () {
  this.mOCFAdapter.stop();
  this.mOCFAdapter.deinitialize();
};

/** 
 * Create a new virtual sensor. Every virtual sensors can be discovered
 * by using one of three keys; sensorName, sensorType and deviceType.
 * @param {String} sensorName the name of virtual sensor
 * @param {String} sensorType the sensor type of virtual sensor
 * @param {String} deviceType the device type of virtual sensor
 * @param {Function} observerHandler (Optional) the observer handler function.
 * It is called when a OCF client sends a request to the inlet of this virtual
 * sensor. If you will not use observerHandler, you can use undefined
 * for observerHandler.
 * @param {Function} generatorHandler the generator handler function.
 * It is called when a OCF client sends a request to the outlet of this
 * virtual sensor. This parameter is mandatory.
 * @param {Function} settingHandler (Optional) the setting handler function.
 * It is called when a OCF client sends a request to the setting of this
 * virtual sensor. If you will not use settingHandler, you can use
 * undefined for settingHandler.
 */
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

/**
 * Create a deep sensor for the given DNN model's name and the number of
 * fragments.
 * @param {String} sensorName the name of virtual sensor
 * @param {String} sensorType the sensor type of virtual sensor
 * @param {String} deviceType the device type of virtual sensor
 * @param {String} modelName the name of DNN model to be executed by this
 * virtual sensor
 * @param {Number} numFragments the number of the DNN model's fragments
 */
VirtualSensorAdapter.prototype.createDeepSensor = function (
  sensorName,
  sensorType,
  deviceType,
  modelName,
  numFragments
) {
  // TODO(RedCarrottt): add pre-processing handler, post-processing handler

  // Check arguments
  if (modelName === undefined || typeof modelName !== 'string') {
    throw 'VSA createDeepSensor error: Invalid modelName ' + modelName;
  }
  if (numFragments === undefined || typeof numFragments !== 'number') {
    throw 'VSA createDeepSensor error: Invalid numFragments ' + numFragments;
  }

  var dfe = this.getANTGateway().createDFE(modelName, numFragments);
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

/**
 * It returns the singleton gateway manager. If the gateway manager is not
 * initialized, the gateway manager will be created.
 * @returns {Object} the singleton gateway manager
 */
VirtualSensorAdapter.prototype.createGWManager = function () {
  if (this.mGatewayManager === undefined) {
    this.mGatewayManager = new GatewayManager();
  }
  return this.getGWManager();
};

/**
 * It returns the singleton gateway client. If the gateway client is not
 * initialized, the gateway client will be created.
 * @returns {Object} the singleton gateway client
 */
VirtualSensorAdapter.prototype.createGWClient = function (onPrepared) {
  if (this.mGatewayClient === undefined) {
    this.mGatewayClient = new GatewayClient();
  }
  var gwClient = this.getGWClient();
  gwClient.setOnPrepared(onPrepared);
  return gwClient;
};

/* Getters */

/**
 * @private
 */
VirtualSensorAdapter.prototype.getANTGateway = function () {
  return this.mANTGateway;
};
/**
 * It returns the OCF adapter. If the OCF adapter is not initialized, it
 * returns undefined.
 * @returns {Object} the OCF adapter managed by the virtual sensor adapter
 */
VirtualSensorAdapter.prototype.getOCFAdapter = function () {
  return this.mOCFAdapter;
};
/**
 * It returns the singleton gateway manager. If the gateway manager is not
 * initialized, it returns undefined.
 * @returns {Object} the singleton gateway manager
 */
VirtualSensorAdapter.prototype.getGWManager = function () {
  return this.mGatewayManager;
};
/**
 * It returns the singleton gateway client. If the gateway client is not
 * initialized, it returns undefined.
 * @returns {Object} the singleton gateway client
 */
VirtualSensorAdapter.prototype.getGWClient = function () {
  return this.mGatewayClient;
};

/* Virtual sensor handling methods */

/**
 * Find the virtual sensor by sensor name.
 * @param {String} name the name of virtual sensor
 * @returns {Object} the found virtual sensor object. If any virtual sensor
 * has not found, this function returns undefined.
 */
VirtualSensorAdapter.prototype.findSensorByName = function (name) {
  for (var i in this.mVirtualSensors) {
    var virtualSensor = this.mVirtualSensors[i];
    if (virtualSensor.getName() === name) {
      return virtualSensor;
    }
  }
  return undefined;
};

/**
 * Find the virtual sensor by URI.
 * @param {String} name the URI of virtual sensor
 * @returns {Object} the found virtual sensor object. If any virtual sensor
 * has not found, this function returns undefined.
 */
VirtualSensorAdapter.prototype.findSensorByUri = function (uri) {
  // offset of the tail of '/vs/'
  var nameFrom = uri.indexOf('/', 1) + 1;
  // offset of the tail of virtual sensor name
  var nameLength = uri.indexOf('/', nameFrom) - nameFrom;
  if (nameFrom < 0 || nameLength < 0) return undefined;

  var name = uri.substring(nameFrom, nameLength);
  if (name === undefined || name.length == 0) return undefined;

  return this.findSensorByName(name);
};

/**
 * Get an OCF resource managed by the virtual sensor adapter. The URI of
 * the target OCF resource is given.
 * @param {String} uri the URI of the target OCF resource
 * @returns {Object} the OCF resource for the given URI. If any resource
 * for the URI is not found, this function returns undefined.
 */
VirtualSensorAdapter.prototype.getResource = function (uri) {
  for (var i in this.mResources) {
    var resource = this.mResources[i];
    if (resource.uri() === uri) {
      return resource;
    }
  }
  return undefined;
};

/**
 * @private
 */
VirtualSensorAdapter.prototype.clearResources = function () {
  this.mResources = [];
};

/**
 * Add an OCF resource to the virtual sensor adapter.
 * @param {Object} resource OCF resource to be added to the resource list
 * of virtual sensor adapter.
 */
VirtualSensorAdapter.prototype.addResource = function (resource) {
  if (resource !== undefined) {
    this.mResources.push(resource);
  }
};

/* Gateway API 2-1: Virtual Sensor */

/**
 * @description The constructor of virtual sensor.
 * @class
 * @classdesc Virtual sensor object. It is a module to service virtual
 * sensor through OCF resources and several JavaScript handlers. 
 * This object can be created by using VirtualSensorAdapter.createSensor().
 * @param {String} sensorName the name of virtual sensor
 * @param {String} sensorType the sensor type of virtual sensor
 * @param {String} deviceType the device type of virtual sensor
 * @param {Function} observerHandler the observer handler function.
 * @param {Function} generatorHandler the generator handler function.
 * @param {Function} settingHandler the setting handler function.
 */
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

/**
 * Configure the handlers of virtual sensor adapter. These handlers must be
 * called after OCF server is prepared. Therefore, calling the handlers are
 * postponed.
 */
/**
 * @private
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

/**
 * @private
 */
VirtualSensor.prototype.setupInlet = function (oa) {
  var device = oa.getDevice(0);
  var uri = getVSInletUri(this.mName);
  this.mInletResource = OCFAPI.createResource(
    device,
    this.mName,
    uri,
    [gVSUriRoot, gVSInletUri, this.mDeviceType, this.mSensorType],
    [OCFAPI.OC_IF_RW]
  );
  this.mInletResource.setDiscoverable(true);
  this.mInletResource.setPeriodicObservable(1);
  this.mInletResource.setHandler(OCFAPI.OC_POST, onPostInlet);
  this.mInletResource.setHandler(OCFAPI.OC_GET, onGetInlet);
  oa.addResource(this.mInletResource);
  return this.mInletResource;
};

/**
 * @private
 */
VirtualSensor.prototype.setupOutlet = function (oa) {
  var device = oa.getDevice(0);
  var uri = getVSOutletUri(this.mName);
  this.mOutletResource = OCFAPI.createResource(
    device,
    this.mName,
    uri,
    [gVSUriRoot, gVSOutletUri, this.mDeviceType, this.mSensorType],
    [OCFAPI.OC_IF_RW]
  );
  this.mOutletResource.setDiscoverable(true);
  this.mOutletResource.setPeriodicObservable(1);
  this.mOutletResource.setHandler(OCFAPI.OC_GET, onGetOutlet);
  oa.addResource(this.mOutletResource);
  return this.mOutletResource;
};

/**
 * @private
 */
VirtualSensor.prototype.setupSetting = function (oa) {
  var device = oa.getDevice(0);
  var uri = getVSSettingUri(this.mName);
  this.mSettingResource = OCFAPI.createResource(
    device,
    this.mName,
    uri,
    [gVSUriRoot, gVSSettingUri, this.mDeviceType, this.mSensorType],
    [OCFAPI.OC_IF_RW]
  );
  this.mSettingResource.setDiscoverable(true);
  this.mSettingResource.setHandler(OCFAPI.OC_POST, onPostSetting);
  oa.addResource(this.mSettingResource);
  return this.mSettingResource;
};

/**
 * @private
 */
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

/**
 * @private
 */
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
/**
 * Get the name of virtual sensor.
 * @returns {String} the name of virtual sensor
 */
VirtualSensor.prototype.getName = function () {
  return this.mName;
};

/**
 * Get the URI of virtual sensor.
 * @returns {String} the URI of virtual sensor
 */
VirtualSensor.prototype.getUri = function () {
  return '/vs/' + this.mName;
};

/**
 * Get the inlet resource of virtual sensor.
 * @returns {Object} the inlet resource of virtual sensor
 */
VirtualSensor.prototype.getInletResource = function () {
  return this.mInletResource;
};

/**
 * Get the outlet resource of virtual sensor.
 * @returns {Object} the outlet resource of virtual sensor
 */
VirtualSensor.prototype.getOutletResource = function () {
  return this.mOutletResource;
};

/**
 * Get the setting resource of virtual sensor.
 * @returns {Object} the setting resource of virtual sensor
 */
VirtualSensor.prototype.getSettingResource = function () {
  return this.mSettingResource;
};

/**
 * Get the observer handler of virtual sensor.
 * @returns {Function} the observer handler of virtual sensor
 */
VirtualSensor.prototype.getObserverHandler = function () {
  return this.mObserverHandler;
};

/**
 * Get the generator handler of virtual sensor.
 * @returns {Function} the generator handler of virtual sensor
 */
VirtualSensor.prototype.getGeneratorHandler = function () {
  return this.mGeneratorHandler;
};

/**
 * Get the setting handler of virtual sensor.
 * @returns {Function} the setting handler of virtual sensor
 */
VirtualSensor.prototype.getSettingHandler = function () {
  return this.mSettingHandler;
};

/**
 * Get the observer list of virtual sensor.
 * @returns {Object[]} the observer list of virtual sensor
 */
VirtualSensor.prototype.getObservers = function () {
  return this.mObservers;
};

/* OCF handlers */
/**
 * @private
 */
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
  var responsePayload = response;
  oa.sendResponse(request, ocf.OC_STATUS_OK, responsePayload);
}

/**
 * @private
 */
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

  oa.discovery(gVSOutletUri, onDiscoveryAfterPostInlet);
  return {result: 'Success', reason: 'None'};
}

/**
 * @private
 */
function onGetInlet(request) {
  // GET inlet: Get observers list
  var virtualSensor = gVSAdapter.findSensorByUri(request.dest_uri);

  // Get observers list
  var observers = virtualSensor.getObservers();

  // Send response
  var oa = gVSAdapter.mOCFAdapter;
  var responsePayload = {
    name: virtualSensor.getName(),
    sensorType: virtualSensor.getSensorType(),
    deviceType: virtualSensor.getDeviceType(),
    observersJSON: JSON.stringify(observers)
  };
  oa.sendResponse(request, ocf.OC_STATUS_OK, responsePayload);
}

/**
 * @private
 */
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
    if (result.buffer === undefined) {
      result.buffer = new Buffer(1);
    }
    var jsonObject = JSON.stringify(result.jsObject);
    var buffer = result.buffer;
    oa.sendResponseBuffer(request, ocf.OC_STATUS_OK, buffer, jsonObject);
  }
}

/**
 * @private
 */
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
    var responsePayload = {
      result: resultJSONString
    };
    oa.sendResponse(request, ocf.OC_STATUS_OK, responsePayload);
  }
}

/*
 * Gateway API 3: DFE
 * DFE: DNN Fragment Engine for the deep sensors running on gateway
 */
/**
 * @class
 * @classdesc DFE (DNN Fragment Engine) for the deep sensors running on gateway
 * @param {String} modelName the name of DNN model to be executed by DFE.
 * @param {Number} numFragments the number of the DNN model's fragments.
 * @private
 */
function DFE(modelName, numFragments) {
  // TODO(RedCarrottt): pre-processing handler
  // TODO(RedCarrottt): post-processing handler
  this.modelName = modelName;
  this.numFragments = numFragments;
  this.interpreters = undefined;
  this.recentInputData = undefined;
  this.presentFragNum = numFragments - 1;
  this.averageLatencyMS = 0.0;
}

/**
 * @private
 */
DFE.prototype.updateAverageLatency = function (latency) {
  // Exponential moving average
  var kMomentum = 0.9;
  this.averageLatencyMS =
    this.averageLatencyMS * kMomentum + latency * (1 - kMomentum);
};

/**
 * Load and pre-process an input image file. It loads the image to a buffer.
 * @param {String} imgPath the path of input image file
 * @returns {Object} the buffer that contains pre-processed image
 */
DFE.prototype.loadAndPreprocessImage = function (imgPath) {
  // TODO(RedCarrottt): hard-coding input
  return native.ant_gateway_dfeLoadAndPreprocessImage(imgPath);
};

/**
 * @private
 */
DFE.prototype.load = function () {
  this.interpreters = native.ant_gateway_dfeLoad(
    this.modelName,
    this.numFragments
  );
};

/**
 * @private
 */
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

/**
 * @private
 */
DFE.prototype.getObserverHandler = function () {
  var self = this;
  function dfeObserverHandler(inputData) {
    // TODO: custom DFE observer handler
    self.recentInputData = inputData;
  }
  return dfeObserverHandler;
};

/**
 * @private
 */
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

/**
 * @private
 */
DFE.prototype.getLoad = function () {
  var loadavg = fs.readFileSync('/proc/loadavg').toString();
  var avgLoad1min = parseFloat(loadavg.substring(0, loadavg.indexOf(' ')));
  return avgLoad1min;
};

/**
 * @private
 */
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

/**
 * @class
 * @classdesc Module to manage virtual sensors and DNN partitioning.
 * This module must run on gateway device.
 */
function GatewayManager() {
  this.mVirtualSensorManager = new VirtualSensorManager(this);
  this.mDFEScheduler = new DFEScheduler();
}

/**
 * @private
 */
GatewayManager.prototype.getVSManager = function () {
  return this.mVirtualSensorManager;
};

/**
 * @private
 */
GatewayManager.prototype.getDFEScheduler = function () {
  return this.mDFEScheduler;
};

/**
 * @private
 */
GatewayManager.prototype.setup = function (vsManager) {
  var resources = [];
  var oa = vsManager.getOCFAdapter();

  var vsManager = this.getVSManager();
  var rVSManager = vsManager.setup(oa);
  resources.push(rVSManager);

  return resources;
};

/**
 * @private
 */
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

/**
 * @class
 * @classdesc module to manage virtual sensors. It discovers virtual sensors
 * via OCF discovery message and associate the virtual sensors to form a sensor
 * data pipeline for the IoT applications.
 */
function VirtualSensorManager(antGateway) {
  this.mANTGateway = antGateway;

  this.mResource = undefined;
  this.mInletList = [];
  this.mOutletList = [];
  this.mSettingList = [];
}

/**
 * @private
 */
VirtualSensorManager.prototype.findInlet = function (sensorType, deviceType) {
  this.findVSResource(this.mInletList, sensorType, deviceType);
};

/**
 * @private
 */
VirtualSensorManager.prototype.findOutlet = function (sensorType, deviceType) {
  this.findVSResource(this.mOutletList, sensorType, deviceType);
};

/**
 * @private
 */
VirtualSensorManager.prototype.findSetting = function (sensorType, deviceType) {
  this.findVSResource(this.mSettingList, sensorType, deviceType);
};

/**
 * @private
 */
VirtualSensorManager.prototype.findVSResource = function (
  list,
  sensorType,
  deviceType
) {
  for (var i in list) {
    var entry = list[i];
    var isSensorTypeFound = false;
    var isDeviceTypeFound = false;
    for (var j in entry.types) {
      var type = entry.types[j];
      if (type === sensorType) isSensorTypeFound = true;
      if (type === deviceType) isDeviceTypeFound = true;
    }
    if (isSensorTypeFound && isDeviceTypeFound) {
      return entry;
    }
  }
  return undefined;
};

/**
 * @private
 */
VirtualSensorManager.prototype.setup = function (oa) {
  var device = oa.getDevice(0);
  this.mInletResource = OCFAPI.createResource(
    device,
    'vsm',
    gGWVSMUri,
    [gGWUriRoot, gGWVSMUri],
    [OCFAPI.OC_IF_RW]
  );
  this.mInletResource.setDiscoverable(true);
  this.mInletResource.setHandler(OCFAPI.OC_GET, onGetVirtualSensorManager);
  this.mInletResource.setHandler(OCFAPI.OC_POST, onPostVirtualSensorManager);
  oa.addResource(this.mInletResource);
  return this.mInletResource;
};

/**
 * @private
 */
VirtualSensorManager.prototype.startDiscovery = function (vsAdapter) {
  // Virtual sensor manager (OCF-client-side): background discovery
  var self = this;
  function onDiscovery(endpoint, uri, types, interfaceMask) {
    var isFound = false;
    for (var i in types) {
      if (types[i] == gVSUriRoot) {
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

/**
 * @private
 */
function onPostVirtualSensorManager(request) {
  // Virtual sensor manager (OCF-server-side): POST association command
  // TODO(RedCarrottt): delete association

  // POST association command
  var response = {result: 'None', reason: 'None'};

  // Parse and check OCF request
  var requestPayload = JSON.parse(request.payload_string);
  var intervalMS = requestPayload.intervalMS;
  var sourceOutletJSON = requestPayload.sourceOutletJSON;
  var sinkInletJSON = requestPayload.sinkInletJSON;
  if (sourceOutletJSON === undefined) {
    response = {
      result: 'Failure',
      reason: 'Not found sourceOutlet'
    };
  }
  if (sinkInletJSON === undefined) {
    response = {
      result: 'Failure',
      reason: 'Not found sinkInlet'
    };
  }
  var sourceOutlet = JSON.parse(sourceOutletJSON);
  var sinkInlet = JSON.parse(sinkInletJSON);
  if (
    typeof sinkInlet !== 'object' ||
    sinkInlet.sensorType === undefined ||
    sinkInlet.deviceType === undefined ||
    typeof sinkInlet.sensorType !== 'string' ||
    typeof sinkInlet.deviceType !== 'string'
  ) {
    response = {
      result: 'Failure',
      reason: 'Invalid sinkInlet: ' + JSON.stringify(sinkInlet)
    };
  }
  if (
    typeof sourceOutlet !== 'object' ||
    sourceOutlet.sensorType === undefined ||
    sourceOutlet.deviceType === undefined ||
    typeof sourceOutlet.sensorType !== 'string' ||
    typeof sourceOutlet.deviceType !== 'string'
  ) {
    response = {
      result: 'Failure',
      reason: 'Invalid sourceOutlet: ' + JSON.stringify(sourceOutlet)
    };
  }
  if (intervalMS === undefined || typeof intervalMS !== 'number') {
    response = {
      result: 'Failure',
      reason: 'Invalid intervalMS: ' + JSON.stringify(intervalMS)
    };
  }

  // Find sink inlet
  var foundSinkInlet = undefined;
  if (response.result !== 'Failure') {
    var vsManager = gVSAdapter.getGWManager().getVSManager();
    var foundInlets = vsManager.mInletList();
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
  if (response.result !== 'Failure') {
    response = {
      result: 'Success',
      reason: 'None',
      intervalMS: intervalMS,
      sourceOutletJSON: sourceOutletJSON,
      sinkInletJSON: sinkInletJSON
    };
  }

  // Send response
  var oa = gVSAdapter.mOCFAdapter;
  var responsePayload = response;
  oa.sendResponse(request, ocf.OC_STATUS_OK, responsePayload);

  // Send association command to sink inlet
  if (response.result !== 'Failure') {
    setTimeout(function () {
      var requestPayload = {
        sensorType: sourceOutlet.sensorType,
        deviceType: sourceOutlet.deviceType,
        intervalMS: intervalMS
      };
      var res = oa.post(
        foundSinkInlet.endpoint,
        foundSinkInlet.uri,
        undefined,
        '',
        ocf.OC_LOW_QOS,
        false,
        requestPayload
      );
      if (!res) {
        console.error(
          'Failed to post association message: ' +
            JSON.stringify(requestPayload)
        );
      }
    }, 100);
  }
}

/**
 * @private
 */
function onGetVirtualSensorManager(request) {
  // Virtual sensor manager (OCF-server-side): GET found virtual sensor resources
  
  // GET found virtual sensor resources
  var inlets = [];
  var foundInlets = this.mInletList;
  for (var i in foundInlets) {
    var entry = {
      types: foundInlets[i].types,
      uri: foundInlets[i].uri
    };
    if (foundInlets[i].observers !== undefined) {
      entry.observers = foundInlets[i].observers;
    }
    inlets.push(entry);
  }

  var outlets = [];
  var foundOutlets = this.mOutletList;
  for (var i in foundOutlets) {
    var entry = {
      types: foundOutlets[i].types,
      uri: foundOutlets[i].uri
    };
    outlets.push(entry);
  }

  var settings = [];
  var foundSettings = this.mSettingList;
  for (var i in foundSettings) {
    var entry = {
      types: foundSettings[i].types,
      uri: foundSettings[i].uri
    };
    settings.push(entry);
  }

  // Send response
  var oa = gVSAdapter.mOCFAdapter;
  var responsePayload = {
    inletsJSON: JSON.stringify(inlets),
    outletsJSON: JSON.stringify(outlets),
    settingsJSON: JSON.stringify(settings)
  };
  oa.sendResponse(request, ocf.OC_STATUS_OK, responsePayload);
}

/**
 * @private
 */
VirtualSensorManager.prototype.addResource = function (endpoint, uri, types) {
  var vsTypes = [];
  var letType = undefined;
  for (var i in types) {
    var type = types[i];
    if (type.indexOf(gVSUriMask) >= 0) {
      letType = type;
      continue;
    } else if (type == gVSUriRoot) {
      continue;
    }
    vsTypes.push(type);
  }
  var foundEntry = {endpoint: endpoint, uri: uri, types: vsTypes};
  if (letType == gVSInletUri) {
    this.mInletList.push(foundEntry);
  } else if (letType == gVSOutletUri) {
    this.mOutletList.push(foundEntry);
  } else if (letType == gVSSettingUri) {
    this.mSettingList.push(foundEntry);
  } else {
    console.error('Cannot identify resource: ' + uri);
    console.error('\tTypes: ' + JSON.stringify(types));
  }
};

/**
 * @private
 */
VirtualSensorManager.prototype.observeInlet = function (inletEntry) {
  var self = this;
  function _onObserve(response) {
    var payload = response.payload;
    if (payload === undefined) {
      console.error('Undefined response from observing inlet');
      return;
    }
    var sensorType = payload.sensorType;
    var deviceType = payload.deviceType;
    var observers = JSON.parse(payload.observersJSON);

    var inlet = self.findInlet(sensorType, deviceType);
    inlet.observers = observers;
  }

  // Send observe request to inlet
  var oa = gVSAdapter.mOCFAdapter;
  oa.observe(
    inletEntry.endpoint,
    inletEntry.uri,
    _onObserve,
    '',
    ocf.OC_LOW_QOS,
    false
  );
};

/**
 * @class
 * @classdesc DFE Scheduler object. It is a module to manage DNN partitioning.
 * It monitors the current status of DFEs running on several devices and make
 * commands to control DFE's partitioning points. (Not yet implemented)
 */
function DFEScheduler() {
  this.mResource = undefined;
}

/**
 * @private
 */
DFEScheduler.prototype.start = function (vsAdapter) {
  // TODO(RedCarrottt): DFEScheduler: client - GET setting resource
  // Not yet implemented
};

/**
 * @class
 * @classdesc Gateway client.
 */
function GatewayClient() {
  this.mOnPrepared = undefined;
  this.mFoundVSM = undefined;

  this.mVSAdapter = undefined;
}

/**
 * @private
 */
GatewayClient.prototype.setOnPrepared = function (onPrepared) {
  this.mOnPrepared = onPrepared;
};

/**
 * @private
 */
GatewayClient.prototype.onOCFClientPrepared = function (vsAdapter) {
  var self = this;
  this.mVSAdapter = vsAdapter;
  function _onDiscovery(endpoint, uri, types, interfaceMask) {
    // Store the endpoint and resource
    // It allows only one gateway.
    for (var i in types) {
      var type = types[i];
      if (type == gGWVSMUri) {
        self.mFoundVSM = {endpoint: endpoint, uri: uri};
      }
    }
  }

  // 1. start discovery
  // 2. If gateway resource is found, store the endpoint and resource
  // 3. Then any request can be sent to gateway.
  var oa = vsAdapter.getOCFAdapter();
  oa.discovery(gGWUriRoot, _onDiscovery);

  // Call custom onPrepared callback
  if (this.mOnPrepared !== undefined) {
    this.mOnPrepared();
  }
};

/**
 * @private
 */
GatewayClient.prototype.isPrepared = function () {
  return this.mOnPrepared !== undefined && this.mVSAdapter !== undefined;
};

// GatewayClient.prototype.isVSMAvailable = function () {
//   return this.mFoundVSM !== undefined;
// };

/**
 * Send a request to gateway to retreive virtual sensor list
 * @param {Function} onResult callback function when the response came
 * @returns {Boolean} the result of sending the request
 */
GatewayClient.prototype.getVirtualSensors = function (onResult) {
  function _onGet(response) {
    var payloadObj = JSON.parse(response.payload);
    var inlets = JSON.parse(payloadObj.inlets);
    var outlets = JSON.parse(payloadObj.outlets);
    var settings = JSON.parse(payloadObj.settings);

    if (onResult !== undefined) onResult(inlets, outlets, settings);
  }

  if (!this.isPrepared()) {
    throw 'getVirtualSensors(): Gateway client not prepared yet';
  }

  var oa = this.mVSAdapter.getOCFAdapter();
  if (oa === undefined) {
    throw 'getVirtualSensors(): Cannot get OCF Adapter';
  }
  if (this.mFoundVSM !== undefined) {
    oa.get(
      this.mVSAdapter.endpoint,
      this.mVSAdapter.uri,
      _onGet,
      undefined,
      undefined,
      false
    );
    return true;
  } else {
    // Virtual sensor manager is not found yet
    return false;
  }
};

// Default sampling interval of virtual sensors
var kVSDefaultIntervalMS = 100;

/**
 * Send a request to gateway to associate two virtual sensors
 * @param {Object} sourceOutlet the information about source outlet resource
 * @param {Object} sinkInlet the information about sink inlet resource
 * @param {Number} intervalMS the interval of sending GET request to the outlet
 * Its unit is milliseconds.
 * @param {Function} onResult callback function when the response came
 * @returns {Boolean} the result of sending the request
 */
GatewayClient.prototype.associateVirtualSensors = function (
  sourceOutlet,
  sinkInlet,
  intervalMS,
  onResult
) {
  function _onPost(response) {
    var payloadObj = JSON.parse(response.payload);
    var result = payloadObj.result;
    var reason = payloadObj.reason;

    if (result === undefined) {
      onResult('Failure', 'Result from virtual sensor is undefined');
    } else if (result === 'Failure') {
      onResult(result, reason);
    } else if (result === 'Success') {
      var sourceOutlet = JSON.parse(payloadObj.sourceOutletJSON);
      var sinkInlet = JSON.parse(payloadObj.sinkInletJSON);
      onResult(result, reason, sourceOutlet, sinkInlet);
    } else {
      onResult(
        'Failure',
        'Result from virtual sensor is invalid: ' + response.payload
      );
    }
  }

  if (intervalMS === undefined) {
    intervalMS = kVSDefaultIntervalMS;
  }

  if (!this.isPrepared()) {
    throw 'getVirtualSensors(): Gateway client not prepared yet';
  }

  var oa = this.mVSAdapter.getOCFAdapter();
  if (oa === undefined) {
    throw 'getVirtualSensors(): Cannot get OCF Adapter';
  }
  if (this.mFoundVSM !== undefined) {
    var requestPayload = {
      intervalMS: intervalMS,
      sourceOutletJSON: JSON.stringify(sourceOutlet),
      sinkInletJSON: JSON.stringify(sinkInlet)
    };
    oa.post(
      this.mVSAdapter.endpoint,
      this.mVSAdapter.uri,
      _onPost,
      undefined,
      undefined,
      false,
      requestPayload
    );
    return true;
  } else {
    // Virtual sensor manager is not found yet
    return false;
  }
};

module.exports = new ANTGateway();
module.exports.ANTGateway = ANTGateway;
