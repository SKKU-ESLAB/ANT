/* @licence
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

var console = require('console');
var assert = require('assert');
var Buffer = require('buffer');

/**
 * ANT OCF API's main object. OCF only generates OCF adapter, and the OCF adapter takes charge of the main role of OCF API.
 */
function OCF() {
  // Interface constants
  this.OC_IF_BASELINE = 1 << 1;
  this.OC_IF_LL = 1 << 2;
  this.OC_IF_B = 1 << 3;
  this.OC_IF_R = 1 << 4;
  this.OC_IF_RW = 1 << 5;
  this.OC_IF_A = 1 << 6;
  this.OC_IF_S = 1 << 7;
  this.OC_IF_CREATE = 1 << 8;

  // Method constants
  this.OC_GET = 1;
  this.OC_POST = 2;
  this.OC_PUT = 3;
  this.OC_DELETE = 4;

  // QoS constants
  this.OC_HIGH_QOS = 0;
  this.OC_LOW_QOS = 1;

  // Status constants
  this.OC_STATUS_OK = 0;
  this.OC_STATUS_CREATED = 1;
  this.OC_STATUS_CHANGED = 2;
  this.OC_STATUS_DELETED = 3;
  this.OC_STATUS_NOT_MODIFIED = 4;
  this.OC_STATUS_BAD_REQUEST = 5;
  this.OC_STATUS_UNAUTHORIZED = 6;
  this.OC_STATUS_BAD_OPTION = 7;
  this.OC_STATUS_FORBIDDEN = 8;
  this.OC_STATUS_NOT_FOUND = 9;
  this.OC_STATUS_METHOD_NOT_ALLOWED = 10;
  this.OC_STATUS_NOT_ACCEPTABLE = 11;
  this.OC_STATUS_REQUEST_ENTITY_TOO_LARGE = 12;
  this.OC_STATUS_UNSUPPORTED_MEDIA_TYPE = 13;
  this.OC_STATUS_INTERNAL_SERVER_ERROR = 14;
  this.OC_STATUS_NOT_IMPLEMENTED = 15;
  this.OC_STATUS_BAD_GATEWAY = 16;
  this.OC_STATUS_SERVICE_UNAVAILABLE = 17;
  this.OC_STATUS_GATEWAY_TIMEOUT = 18;
  this.OC_STATUS_PROXYING_NOT_SUPPORTED = 19;
}

var sOCFAdapter = undefined;
OCF.prototype.getAdapter = function () {
  if (sOCFAdapter === undefined) {
    sOCFAdapter = new OCFAdapter();
  }
  return sOCFAdapter;
};
OCF.prototype.createResource = function (
  device,
  name,
  uri,
  types,
  interfaceMasks
) {
  return new OCFResource(device, name, uri, types, interfaceMasks);
};

var gOCFAdapterRequestId = 1;
gObserveRequestList = [];
gGetRequestList = [];
gDeleteRequestList = [];
gPostRequestList = [];
gPutRequestList = [];
gOnewayRequestLists = [
  gGetRequestList,
  gDeleteRequestList,
  gPostRequestList,
  gPutRequestList
];

/**
 * OCF Adapter. It manages OCF thread's lifecycle.
 */
function OCFAdapter() {
  this._mfgName = '';
  this._devices = [];
  this._resources = [];
  this._nextDeviceId = 0;

  // Default handler
  var self = this;
  this.initialize();
  this.onPrepareEventLoop(function () {
    self.setPlatform('ant');
    self.addDevice('/oic/d', 'oic.d.light', 'Light', 'ocf.res.1.0.0');
  });
}
OCFAdapter.prototype.initialize = function () {
  native.ocf_adapter_initialize();
};
OCFAdapter.prototype.deinitialize = function () {
  native.ocf_adapter_deinitialize();
};
/**
 * OCFAdapter.setPlatform
 * @param {String} mfgName Manufacturer's name
 */
OCFAdapter.prototype.setPlatform = function (mfgName) {
  this._mfgName = mfgName;
  native.ocf_adapter_setPlatform(mfgName);
};
/**
 * OCFAdapter.getPlatform
 * @returns {string} platformName
 */
OCFAdapter.prototype.getPlatform = function () {
  return this._mfgName;
};
/**
 * OCFAdapter.addDevice
 * @param {String} uri id of device to be serviced
 * @param {String} resourceType resource type of device to be serviced
 * @param {String} name name of device to be serviced
 * @param {String} specVersion specification of device to be serviced
 * @param {String} dataModelVersion data model version of device to be serviced
 * Set the device to be serviced by the OCFAdapter.
 * One OCFAdapter can service multiple devices.
 */
OCFAdapter.prototype.addDevice = function (
  uri,
  resourceType,
  name,
  specVersion,
  dataModelVersion
) {
  var device = new OCFDevice(
    this._nextDeviceId++,
    uri,
    resourceType,
    name,
    specVersion,
    dataModelVersion
  );
  this._devices.push(device);
  native.ocf_adapter_addDevice(
    uri,
    resourceType,
    name,
    specVersion,
    dataModelVersion
  );
};
/**
 * OCFAdapter.getDevices
 * @returns {Array<OCFDevice>} devices
 */
OCFAdapter.prototype.getDevices = function () {
  return this._devices;
};
/**
 * OCFAdapter.getDevice
 * @param {Integer} i
 * @returns {OCFDevice} device
 */
OCFAdapter.prototype.getDevice = function (i) {
  return this._devices[i];
};

/**
 * OCFAdapter.prototype.onPrepareEventLoop
 * @param {Function} handler
 */
OCFAdapter.prototype.onPrepareEventLoop = function (handler) {
  // Handler: void function(void)
  native.ocf_adapter_onPrepareEventLoop(handler);
};
/**
 * OCFAdapter.onPrepareClient
 * @param {Function} handler
 * called when all preparations for executing client functions in the OCF thread are complete
 */
OCFAdapter.prototype.onPrepareClient = function (handler) {
  // Handler: void function(void)
  native.ocf_adapter_onPrepareClient(handler);
};
/**
 * OCFAdapter.onPrepareServer
 * @param {Function} handler
 * called when all preparations for executing server functions in OCF thread are complete
 */
OCFAdapter.prototype.onPrepareServer = function (handler) {
  // Handler: void function(void)
  native.ocf_adapter_onPrepareServer(handler);
};
/**
 * OCFAdapter.start
 * Run the OCF thread. This function must be called after OCFAdapter.initialize() is called.
 * You can use the OCF Server API and OCF Client API only while the OCF thread is running.
 * If you need to know exactly when you can use OCF Server API and OCF Client API,
 * you can use the handlers of OCFAdapter.onPrepareServer() and OCFAdapter.onPrepareClient().
 */
OCFAdapter.prototype.start = function () {
  oaStartRequestListCleaner();
  native.ocf_adapter_start();
};
/**
 * OCFAdapter.stop
 * Stop the OCF thread.
 */
OCFAdapter.prototype.stop = function () {
  native.ocf_adapter_stop();
  oaStopRequestListCleaner();
};
/**
 * OCFAdapter.addResource
 * @param {OCFResource} resource
 */
OCFAdapter.prototype.addResource = function (resource) {
  this._resources.push(resource);
  native.ocf_adapter_addResource(resource);
};
/**
 * OCFAdapter.deleteResource
 * @param {OCFResource} resource
 */
OCFAdapter.prototype.deleteResource = function (resource) {
  resource.destroyer();
  var index = this._resources.indexOf(resource);
  this._resources.splice(index, 1);
};
/**
 * OCFAdapter.getResources
 * @returns {OCFResource} resource
 */
OCFAdapter.prototype.getResources = function () {
  return this._resources;
};
/**
 * OCFAdapter.repStartRootObject
 * Let the OCF thread start writing the OCRepresentation.
 */
OCFAdapter.prototype.repStartRootObject = function () {
  native.ocf_adapter_repStartRootObject();
};
/**
 * OCFAdapter.repSet
 * @param {String} key
 * @param {Boolean|Number|String} value
 * The value is stored in a specific key among OCRepresentations being created by OCF thread.
 * In this function, various types of data including Boolean, Number, and String can be used as value.
 */
OCFAdapter.prototype.repSet = function (key, value) {
  if (typeof value === 'boolean') {
    native.ocf_adapter_repSetBoolean(key, value);
  } else if (typeof value === 'number') {
    if (value == parseInt(value, 10)) {
      native.ocf_adapter_repSetInt(key, value);
    } else {
      native.ocf_adapter_repSetDouble(key, value);
    }
  } else if (typeof value === 'string') {
    native.ocf_adapter_repSetString(key, value);
  } else if (typeof value === 'object' && value instanceof Buffer) {
    native.ocf_adapter_repSetByteArray(key, value);
  } else {
    console.log('repSet(): Not supported type (' + typeof value + ')');
  }
};

/**
 * OCFAdapter.repSetBufferAndString
 * @param {Object} value
 * The special case of repSet() method.
 * The data can include both byte buffer and string.
 */
OCFAdapter.prototype.repSetBufferAndString = function (bufferValue, stringValue) {
  if (typeof bufferValue !== 'object' || !(bufferValue instanceof Buffer)) {
    console.log('repSet(): Not supported type (' + typeof bufferValue + ')');
  }

  var KEY_BUFFER_VALUE = 'bufferValue';
  var KEY_STRING_VALUE = 'stringValue';
  native.ocf_adapter_repSetByteArray(KEY_BUFFER_VALUE, bufferValue);
  native.ocf_adapter_repSet(KEY_STRING_VALUE, stringValue);
};

/**
 * OCFAdapter.repEndRootObject
 * Finish writing OCRepresentation of OCF thread.
 */
OCFAdapter.prototype.repEndRootObject = function () {
  native.ocf_adapter_repEndRootObject();
};
/**
 * OCFAdapter.sendResponse
 * @param {OCFRequest} ocfRequest target of response
 * @param {Number} statusCode Response status code value
 * Sends a response with a specific status code to a request from another device.
 */
OCFAdapter.prototype.sendResponse = function (ocfRequest, statusCode) {
  native.ocf_adapter_sendResponse(ocfRequest, statusCode);
};
/**
 * OCFAdapter.stopDiscovery
 * @returns {Boolean} isSuccess
 */
OCFAdapter.prototype.stopDiscovery = function () {
  return native.ocf_adapter_stopDiscovery();
};
/**
 * OCFAdapter.isDiscovering
 * @returns {Boolean} isDiscovering
 */
OCFAdapter.prototype.isDiscovering = function () {
  return native.ocf_adapter.isDiscovering();
};
/**
 * OCFAdapter.discovery
 * @param {String} resourceType Type of resource to find on the network
 * @param {Function} discoveryHandler called whenever one OCFResource is discovered
 * @returns {Boolean} isSuccess
 */
OCFAdapter.prototype.discovery = function (resourceType, discoveryHandler) {
  return native.ocf_adapter_discovery(resourceType, discoveryHandler);
};
/**
 * OCFAdapter.discoveryAll
 * @param {Function} discoveryHandler Handler function for discovery response
 * @returns {Boolean} isSuccess
 * Search all resources regardless of any type on the network.
 */
OCFAdapter.prototype.discoveryAll = function (discoveryHandler) {
  return native.ocf_adapter_discovery(' ', discoveryHandler);
};

var makeRequest = function (requestId, query, qos, endpoint, uri, userHandler) {
  var request = {};
  request.id = requestId;
  request.query = query;
  request.qos = qos;
  request.endpoint = endpoint;
  request.uri = uri;
  request.userHandler = userHandler;
  request.timestamp = new Date();
  return request;
};

var oaObserveResponseHandler = function (requestId, response) {
  return oaResponseHandler(requestId, response, gObserveRequestList, false);
};
var oaGetResponseHandler = function (requestId, response) {
  return oaResponseHandler(requestId, response, gGetRequestList, true);
};
var oaDeleteResponseHandler = function (requestId, response) {
  return oaResponseHandler(requestId, response, gDeleteRequestList, true);
};
var oaPostResponseHandler = function (requestId, response) {
  return oaResponseHandler(requestId, response, gPostRequestList, true);
};
var oaPutResponseHandler = function (requestId, response) {
  return oaResponseHandler(requestId, response, gPutRequestList, true);
};
var oaResponseHandler = function (requestId, response, requestList, isOneway) {
  var request = undefined;
  for (var i = 0; i < requestList.length; i++) {
    var item = requestList[i];
    if (item !== undefined && item.id == requestId) {
      request = item;
      break;
    }
  }
  if (request !== undefined) {
    var userHandler = request.userHandler;
    if (userHandler !== undefined) {
      userHandler(response);
      if (isOneway) {
        requestList.splice(requestId, 1);
      }
    }
  }
};
var responseTimeoutMs = 2000;
var requestListCleanerFrequencyMS = 2000;
var requestListCleaner = undefined;
var requestListCleanerFn = function () {
  var now = new Date();
  for (var i = 0; i < gOnewayRequestLists.length; i++) {
    var requestList = gOnewayRequestLists[i];
    for (var j = 0; j < requestList.length; j++) {
      var request = requestList[j];
      if (request === undefined) continue;
      if (now - request.timestamp > responseTimeoutMs) {
        requestList.splice(j, 1);
      }
    }
  }
};
var oaStartRequestListCleaner = function () {
  requestListCleaner = setInterval(
    requestListCleanerFn,
    requestListCleanerFrequencyMS
  );
};
var oaStopRequestListCleaner = function () {
  clearInterval(requestListCleaner);
};
/**
 * OCFAdapter.observe
 * @param {OCFEndpoint} endpoint
 * @param {String} uri
 * @param {Function} userHandler
 * @param {String} query
 * @param {Integer} qos
 * @param {Boolean} isPayloadBuffer
 * @return {Boolean} isSuccess
 */
OCFAdapter.prototype.observe = function (
  endpoint,
  uri,
  userHandler,
  query,
  qos,
  isPayloadBuffer
) {
  if (query === undefined) {
    query = '';
  }
  if (qos == undefined) {
    qos = 0; // HIGH_QOS
  }
  if (isPayloadBuffer == undefined) {
    isPayloadBuffer = false; // payload string
  }

  var requestId = gOCFAdapterRequestId++;
  var request = makeRequest(requestId, query, qos, endpoint, uri, userHandler);

  var result = native.ocf_adapter_observe(
    request,
    oaObserveResponseHandler,
    isPayloadBuffer
  );
  if (result) {
    gObserveRequestList.push(request);
  }
  return result;
};
/**
 * OCFAdapter.stopObserve
 * @param {OCFEndpoint} endpoint
 * @param {String} uri
 * @returns {Boolean} isSuccess
 */
OCFAdapter.prototype.stopObserve = function (endpoint, uri) {
  var requestId = undefined;
  for (var i = 0; i < gObserveRequestList.length; i++) {
    var request = gObserveRequestList[i];
    if (request.endpoint == endpoint && request.uri == request.uri) {
      requestId = request.id;
      gObserveRequestList.splice(i, 1);
      break;
    }
  }

  if (requestId !== undefined) {
    var result = native.ocf_adapter_stopObserve(requestId, endpoint, uri);
    return result;
  } else {
    console.log('Error: cannot find observe request for ' + uri);
    return false;
  }
};
/**
 * OCFAdapter.get
 * @param {OCFEndpoint} endpoint
 * @param {String} uri
 * @param {Function} userHandler
 * @param {String} query
 * @param {Integer} qos
 * @param {Boolean} isPayloadBuffer
 * @returns {Boolean} isSuccess
 */
OCFAdapter.prototype.get = function (
  endpoint,
  uri,
  userHandler,
  query,
  qos,
  isPayloadBuffer
) {
  if (query === undefined) {
    query = '';
  }
  if (qos == undefined) {
    qos = 0; // HIGH_QOS
  }
  if (isPayloadBuffer == undefined) {
    isPayloadBuffer = false; // payload string
  }

  var requestId = gOCFAdapterRequestId++;
  var request = makeRequest(requestId, query, qos, endpoint, uri, userHandler);
  var result = native.ocf_adapter_get(
    request,
    oaGetResponseHandler,
    isPayloadBuffer
  );
  if (result) {
    gGetRequestList.push(request);
  }
  return result;
};
/**
 * OCFAdapter.delete
 * @param {OCFEndpoint} endpoint
 * @param {String} uri
 * @param {Function} userHandler
 * @param {String} query
 * @param {Integer} qos
 * @param {Boolean} isPayloadBuffer
 * @returns {Boolean} isSuccess
 */
OCFAdapter.prototype.delete = function (
  endpoint,
  uri,
  userHandler,
  query,
  qos,
  isPayloadBuffer
) {
  if (query === undefined) {
    query = '';
  }
  if (qos == undefined) {
    qos = 0; // HIGH_QOS
  }
  if (isPayloadBuffer == undefined) {
    isPayloadBuffer = false; // payload string
  }

  var requestId = gOCFAdapterRequestId++;
  var request = makeRequest(requestId, query, qos, endpoint, uri, userHandler);
  var result = native.ocf_adapter_delete(
    request,
    oaDeleteResponseHandler,
    isPayloadBuffer
  );
  if (result) {
    gDeleteRequestList.push(request);
  }
  return result;
};
/**
 * OCFAdapter.initPost
 * @param {OCFEndpoint} endpoint
 * @param {String} uri
 * @param {Function} userHandler
 * @param {String} query
 * @param {Integer} qos
 * @param {Boolean} isPayloadBuffer
 * @returns {Boolean} isSuccess
 */
OCFAdapter.prototype.initPost = function (
  endpoint,
  uri,
  userHandler,
  query,
  qos,
  isPayloadBuffer
) {
  if (query === undefined) {
    query = '';
  }
  if (qos == undefined) {
    qos = 0; // HIGH_QOS
  }
  if (isPayloadBuffer == undefined) {
    isPayloadBuffer = false; // payload string
  }

  var requestId = gOCFAdapterRequestId++;
  var request = makeRequest(requestId, query, qos, endpoint, uri, userHandler);
  var result = native.ocf_adapter_initPost(
    request,
    oaPostResponseHandler,
    isPayloadBuffer
  );
  if (result) {
    gPostRequestList.push(request);
  }
  return result;
};
/**
 * OCFAdapter.initPut
 * @param {OCFEndpoint} endpoint
 * @param {String} uri
 * @param {Function} userHandler
 * @param {String} query
 * @param {Integer} qos
 * @param {Boolean} isPayloadBuffer
 * @returns {Boolean} isSuccess
 */
OCFAdapter.prototype.initPut = function (
  endpoint,
  uri,
  userHandler,
  query,
  qos,
  isPayloadBuffer
) {
  if (query === undefined) {
    query = '';
  }
  if (qos == undefined) {
    qos = 0; // HIGH_QOS
  }
  if (isPayloadBuffer == undefined) {
    isPayloadBuffer = false; // payload string
  }

  var requestId = gOCFAdapterRequestId++;
  var request = makeRequest(requestId, query, qos, endpoint, uri, userHandler);
  var result = native.ocf_adapter_initPut(
    request,
    oaPutResponseHandler,
    isPayloadBuffer
  );
  if (result) {
    gPutRequestList.push(request);
  }
  return result;
};
/**
 * OCFAdapter.post
 * @returns {Boolean} isSuccess
 */
OCFAdapter.prototype.post = function () {
  var result = native.ocf_adapter_post();
  return result;
};
/**
 * OCFAdapter.put
 * @returns {Boolean} isSuccess
 */
OCFAdapter.prototype.put = function () {
  var result = native.ocf_adapter_put();
  return result;
};

/**
 * OCF Device
 * @param {int} id OCF device's ID that is internally managed.
 * @param {string} uri URI of the OCF device
 * @param {string} resourceType OCF device's resource type
 * @param {string} name the OCF device's name
 * @param {string} specVersion version of the device's spec
 * @param {string} dataModelVersion version of the device's data model
 */
function OCFDevice(id, uri, resourceType, name, specVersion, dataModelVersion) {
  this.id = id;
  this.uri = uri;
  this.resourceType = resourceType;
  this.name = name;
  this.specVersion = specVersion;
  this.dataModelVersion = dataModelVersion;
}

/**
 * OCF Resource.
 * @param {object} device OCF device that serves this resource
 * @param {string} name the resource's name
 * @param {string} uri URI of the resource
 * @param {array} types the array of resource's types.
 * each element's type is string.
 * @param {interfaceMasks} interfaceMasks the array of interface masks.
 * each element's type is string.
 */
function OCFResource(device, name, uri, types, interfaceMasks) {
  assert(device !== undefined && device.id !== undefined);

  this.device = device;
  this.name = name;
  this.uri = uri;
  this.types = types;
  this.interfaceMask = 0;
  for (var i = 0; i < interfaceMasks.length; i++) {
    this.interfaceMask |= interfaceMasks[i];
  }
  this.defaultInterfaceMask = interfaceMasks[0];
  this.isDiscoverable = undefined;
  this.periodSec = undefined;
  this.handlerIDMap = {};

  native.ocf_resource_constructor(this);
}
/**
 * OCFResource.destroyer
 */
OCFResource.prototype.destroyer = function () {
  var handlerIDs = [];
  for (var i = 0; i < this.handlerIDMap.length; i++) {
    handlerIDs.push(this.handlerIDMap[i]);
  }
  native.ocf_resource_destroyer(this, handlerIDs);
};
/**
 * OCFResource.setDiscoverable
 * @param {Boolean} isDiscoverable
 */
OCFResource.prototype.setDiscoverable = function (isDiscoverable) {
  this.isDiscoverable = isDiscoverable;
  native.ocf_resource_setDiscoverable(this, isDiscoverable);
};
/**
 * OCFResource.setPeriodicObservable
 * @param {Integer} periodSec
 */
OCFResource.prototype.setPeriodicObservable = function (periodSec) {
  this.periodSec = periodSec;
  native.ocf_resource_setPeriodicObservable(this, periodSec);
};

var gOCFResourceHandlerId = 0;

/**
 * OCFResource.setHandler
 * @param {Number} method
 * @param {Function} handler
 */
OCFResource.prototype.setHandler = function (method, handler) {
  // Handler: void function(OCFRequest request, int method)
  var handlerId = gOCFResourceHandlerId;
  this.handlerIDMap[method] = handlerId;
  native.ocf_resource_setHandler(this, handlerId, method, handler);
  gOCFResourceHandlerId++;
};

module.exports = new OCF();
module.exports.OCF = OCF;
