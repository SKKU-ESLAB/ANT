var console = require('console');
var assert = require('assert');

var RESULT_SUCCESS = 'Success';
var RESULT_FAILED = 'Failed';

/** OCF API start **/
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
  device, name, uri, types, interface_masks) {
  return new OCFResource(device, name, uri, types, interface_masks);
};

var gOCFAdapterRequestId = 0;
gObserveRequestList = [];
gGetRequestList = [];
gDeleteRequestList = [];
gPostRequestList = [];
gPutRequestList = [];
gOnewayRequestLists = [gGetRequestList, gDeleteRequestList, gPostRequestList, gPutRequestList];

function OCFAdapter() {
  this._mfg_name = '';
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
OCFAdapter.prototype.setPlatform = function (mfg_name) {
  this._mfg_name = mfg_name;
  native.ocf_adapter_setPlatform(mfg_name);
};
OCFAdapter.prototype.getPlatform = function () {
  return this._mfg_name;
};
OCFAdapter.prototype.addDevice = function (
  uri, resource_type, name, spec_version, data_model_version) {
  var device = new OCFDevice(
    this._nextDeviceId++, uri, resource_type, name, spec_version,
    data_model_version);
  this._devices.push(device);
  native.ocf_adapter_addDevice(
    uri, resource_type, name, spec_version, data_model_version);
};
OCFAdapter.prototype.getDevices = function () {
  return this._devices;
};
OCFAdapter.prototype.getDevice = function (i) {
  return this._devices[i];
};

OCFAdapter.prototype.onPrepareEventLoop = function (handler) {
  // Handler: void function(void)
  native.ocf_adapter_onPrepareEventLoop(handler);
};
OCFAdapter.prototype.onPrepareClient = function (handler) {
  // Handler: void function(void)
  native.ocf_adapter_onPrepareClient(handler);
};
OCFAdapter.prototype.onPrepareServer = function (handler) {
  // Handler: void function(void)
  native.ocf_adapter_onPrepareServer(handler);
};
OCFAdapter.prototype.start = function () {
  oa_start_request_list_cleaner();
  native.ocf_adapter_start();
};
OCFAdapter.prototype.stop = function () {
  native.ocf_adapter_stop();
  oa_stop_request_list_cleaner();
};

OCFAdapter.prototype.addResource = function (resource) {
  this._resources.push(resource);
  native.ocf_adapter_addResource(resource);
};
OCFAdapter.prototype.deleteResource = function (resource) {
  resource.destroyer();
  var index = this._resources.indexOf(resource);
  this._resources.splice(index, 1);
};
OCFAdapter.prototype.getResources = function () {
  return this._resources;
};

OCFAdapter.prototype.repStartRootObject = function () {
  native.ocf_adapter_repStartRootObject();
};
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
  } else {
    console.log('repSet(): Not supported type (' + typeof value + ')');
  }
};
OCFAdapter.prototype.repEndRootObject = function () {
  native.ocf_adapter_repEndRootObject();
};
OCFAdapter.prototype.sendResponse = function (ocf_request, status_code) {
  native.ocf_adapter_sendResponse(ocf_request, status_code);
};

OCFAdapter.prototype.stopDiscovery = function () {
  return native.ocf_adapter_stopDiscovery();
};
OCFAdapter.prototype.isDiscovering = function () {
  return native.ocf_adapter.isDiscovering();
};
OCFAdapter.prototype.discovery = function (resource_type, discovery_handler) {
  return native.ocf_adapter_discovery(resource_type, discovery_handler);
};
OCFAdapter.prototype.discoveryAll = function (discovery_handler) {
  return native.ocf_adapter_discovery(' ', discovery_handler);
};

function make_request(requestId, query, qos, endpoint, uri, user_handler) {
  var request = {};
  request.id = requestId;
  request.query = query;
  request.qos = qos;
  request.endpoint = endpoint;
  request.uri = uri;
  request.user_handler = user_handler;
  request.timestamp = new Date();
  return request;
}

function oa_observe_response_handler(requestId, response) {
  return oa_response_handler(requestId, response, gObserveRequestList, false);
}
function oa_get_response_handler(requestId, response) {
  return oa_response_handler(requestId, response, gGetRequestList, true);
}
function oa_delete_response_handler(requestId, response) {
  return oa_response_handler(requestId, response, gDeleteRequestList, true);
}
function oa_post_response_handler(requestId, response) {
  return oa_response_handler(requestId, response, gPostRequestList, true);
}
function oa_put_response_handler(requestId, response) {
  return oa_response_handler(requestId, response, gPutRequestList, true);
}
function oa_response_handler(requestId, response, requestList, isOneway) {
  var request = undefined;
  for (var i in requestList) {
    var item = requestList[i];
    if (item !== undefined && item.id == requestId) {
      request = item;
      break;
    }
  }
  if (request !== undefined) {
    var user_handler = request.user_handler;
    if (user_handler !== undefined) {
      user_handler(response);
      if (isOneway) {
        requestList.splice(requestId, 1);
      }
    }
  }
}
var response_timeout_ms = 2000;
var request_list_cleaner_frequency_ms = 2000;
var request_list_cleaner = undefined;
function request_list_cleaner_fn() {
  var now = new Date();
  for (var i in gOnewayRequestLists) {
    var requestList = gOnewayRequestLists[i];
    for (var j in requestList) {
      var request = requestList[j];
      if (request === undefined) continue;
      if (now - request.timestamp > response_timeout_ms) {
        requestList.splice(j, 1);
      }
    }
  }
}
function oa_start_request_list_cleaner() {
  request_list_cleaner = setInterval(request_list_cleaner_fn, request_list_cleaner_frequency_ms);
}
function oa_stop_request_list_cleaner() {
  clearInterval(request_list_cleaner);
}

OCFAdapter.prototype.observe = function (
  endpoint, uri, user_handler, query, qos) {
  if (query === undefined) {
    query = '';
  }
  if (qos == undefined) {
    qos = 0;  // HIGH_QOS
  }

  var requestId = gOCFAdapterRequestId++;
  var request = make_request(requestId, query, qos, endpoint, uri, user_handler);

  var result = native.ocf_adapter_observe(request, oa_observe_response_handler);
  if (result) {
    gObserveRequestList.push(request);
  }
  return result;
};

OCFAdapter.prototype.stopObserve = function (endpoint, uri) {
  var requestId = undefined;
  for (var i in gObserveRequestList) {
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

OCFAdapter.prototype.get = function (
  endpoint, uri, user_handler, query, qos) {
  if (query === undefined) {
    query = '';
  }
  if (qos == undefined) {
    qos = 0;  // HIGH_QOS
  }

  var requestId = gOCFAdapterRequestId++;
  var request = make_request(requestId, query, qos, endpoint, uri, user_handler);
  var result = native.ocf_adapter_get(request, oa_get_response_handler);
  if (result) {
    gGetRequestList.push(request);
  }
  return result;
};

OCFAdapter.prototype.delete = function (
  endpoint, uri, user_handler, query, qos) {
  if (query === undefined) {
    query = '';
  }
  if (qos == undefined) {
    qos = 0;  // HIGH_QOS
  }

  var requestId = gOCFAdapterRequestId++;
  var request = make_request(requestId, query, qos, endpoint, uri, user_handler);
  var result = native.ocf_adapter_delete(request, oa_delete_response_handler);
  if (result) {
    gDeleteRequestList.push(request);
  }
  return result;
};

OCFAdapter.prototype.initPost = function (
  endpoint, uri, user_handler, query, qos) {
  if (query === undefined) {
    query = '';
  }
  if (qos == undefined) {
    qos = 0;  // HIGH_QOS
  }

  var requestId = gOCFAdapterRequestId++;
  var request = make_request(requestId, query, qos, endpoint, uri, user_handler);
  var result = native.ocf_adapter_initPost(request, oa_post_response_handler);
  if (result) {
    gPostRequestList.push(request);
  }
  return result;
};

OCFAdapter.prototype.initPut = function (
  endpoint, uri, user_handler, query, qos) {
  if (query === undefined) {
    query = '';
  }
  if (qos == undefined) {
    qos = 0;  // HIGH_QOS
  }

  var requestId = gOCFAdapterRequestId++;
  var request = make_request(requestId, query, qos, endpoint, uri, user_handler);
  var result = native.ocf_adapter_initPut(request, oa_put_response_handler);
  if (result) {
    gPutRequestList.push(request);
  }
  return result;
};

OCFAdapter.prototype.post = function () {
  var result = native.ocf_adapter_post();
  return result;
};

OCFAdapter.prototype.put = function () {
  var result = native.ocf_adapter_put();
  return result;
};


function OCFDevice(
  id, uri, resource_type, name, spec_version, data_model_version) {
  this.id = id;
  this.uri = uri;
  this.resource_type = resource_type;
  this.name = name;
  this.spec_version = spec_version;
  this.data_model_version = data_model_version;
}

function OCFResource(device, name, uri, types, interface_masks) {
  assert(device !== undefined && device.id !== undefined);

  this.device = device;
  this.name = name;
  this.uri = uri;
  this.types = types;
  this.interface_mask = 0;
  for (var i in interface_masks) {
    this.interface_mask |= interface_masks[i];
  }
  this.default_interface_mask = interface_masks[0];
  this.is_discoverable = undefined;
  this.period_sec = undefined;
  this.handler_id_map = {};

  native.ocf_resource_constructor(this);
}

OCFResource.prototype.destroyer = function () {
  var handler_ids = [];
  for (var i in this.handler_id_map) {
    handler_ids.push(this.handler_id_map[i]);
  }
  native.ocf_resource_destroyer(this, handler_ids);
};

OCFResource.prototype.setDiscoverable = function (is_discoverable) {
  this.is_discoverable = is_discoverable;
  native.ocf_resource_setDiscoverable(this, is_discoverable);
};

OCFResource.prototype.setPeriodicObservable = function (period_sec) {
  this.period_sec = period_sec;
  native.ocf_resource_setPeriodicObservable(this, period_sec);
};

var gOCFResourceHandlerId = 0;
OCFResource.prototype.setHandler = function (method, handler) {
  // Handler: void function(OCFRequest request, int method)
  var handlerId = gOCFResourceHandlerId;
  this.handler_id_map[method] = handlerId;
  native.ocf_resource_setHandler(this, handlerId, method, handler);
  gOCFResourceHandlerId++;
};

module.exports = new OCF();
module.exports.OCF = OCF;