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
OCF.prototype.getAdapter = function() {
  if (sOCFAdapter === undefined) {
    sOCFAdapter = new OCFAdapter();
  }
  return sOCFAdapter;
};

OCF.prototype.createResource = function(
    device, name, uri, types, interface_masks) {
  return new OCFResource(device, name, uri, types, interface_masks);
};

function OCFAdapter() {
  this._mfg_name = '';
  this._devices = [];
  this._resources = [];
  this._nextDeviceId = 0;

  // Default handler
  var self = this;
  // this.onInitialize(function() {
  //   self.setPlatform('ant');
  //   self.addDevice('/oic/d', 'oic.d.light', 'Light', 'ocf.res.1.0.0');
  // });
}
OCFAdapter.prototype.setPlatform = function(mfg_name) {
  this._mfg_name = mfg_name;
  native.ocf_adapter_setPlatform(mfg_name);
};
OCFAdapter.prototype.getPlatform = function() {
  return this._mfg_name;
};
OCFAdapter.prototype.addDevice = function(
    uri, resource_type, name, spec_version, data_model_version) {
  var device = new OCFDevice(
      this._nextDeviceId++, uri, resource_type, name, spec_version,
      data_model_version);
  this._devices.push(device);
  native.ocf_adapter_addDevice(
      uri, resource_type, name, spec_version, data_model_version);
};
OCFAdapter.prototype.getDevices = function() {
  return this._devices;
};
OCFAdapter.prototype.getDevice = function(i) {
  return this._devices[i];
};

OCFAdapter.prototype.onInitialize = function(handler) {
  // Handler: void function(void)
  native.ocf_adapter_onInitialize(handler);
};
OCFAdapter.prototype.onPrepareClient = function(handler) {
  // Handler: void function(void)
  native.ocf_adapter_onPrepareClient(handler);
};
OCFAdapter.prototype.onPrepareServer = function(handler) {
  // Handler: void function(void)
  native.ocf_adapter_onPrepareServer(handler);
};
OCFAdapter.prototype.start = function() {
  native.ocf_adapter_start();
};
OCFAdapter.prototype.stop = function() {
  native.ocf_adapter_stop();
};

OCFAdapter.prototype.addResource = function(resource) {
  this._resources.push(resource);
  native.ocf_adapter_addResource(resource);
};
OCFAdapter.prototype.getResources = function() {
  return this._resources;
};

OCFAdapter.prototype.repStartRootObject = function() {
  native.ocf_adapter_repStartRootObject();
};
OCFAdapter.prototype.repSet = function(key, value) {
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
OCFAdapter.prototype.repEndRootObject = function() {
  native.ocf_adapter_repEndRootObject();
};
OCFAdapter.prototype.sendResponse = function(ocf_request, status_code) {
  native.ocf_adapter_sendResponse(ocf_request, status_code);
};

OCFAdapter.prototype.discovery = function(resource_type, discovery_handler) {
  native.ocf_adapter_discovery(resource_type, discovery_handler);
};

OCFAdapter.prototype.observe = function(endpoint, uri, response_handler) {
  native.ocf_adapter_observe(endpoint, uri, response_handler);
};

OCFAdapter.prototype.stopObserve = function(endpoint, uri) {
  native.ocf_adapter_stopObserve(endpoint, uri);
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

  native.ocf_resource_constructor(this);
}

OCFResource.prototype.setDiscoverable = function(is_discoverable) {
  this.is_discoverable = is_discoverable;
  native.ocf_resource_setDiscoverable(this, is_discoverable);
};

OCFResource.prototype.setPeriodicObservable = function(period_sec) {
  this.period_sec = period_sec;
  native.ocf_resource_setPeriodicObservable(this, period_sec);
};

OCFResource.prototype.setHandler = function(method, handler) {
  // Handler: void function(OCFRequest request, int method)
  native.ocf_resource_setHandler(this, method, handler);
};

module.exports = new OCF();
module.exports.OCF = OCF;