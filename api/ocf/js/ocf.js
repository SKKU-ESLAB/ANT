var console = require('console');
var assert = require('assert');

var RESULT_SUCCESS = 'Success';
var RESULT_FAILED = 'Failed';

/** OCF API start **/
function OCF() {}

var sOCFAdapter = undefined;
OCF.prototype.getAdapter = function() {
  if (sOCFAdapter === undefined) {
    sOCFAdapter = new OCFAdapter();
  }
  return sOCFAdapter;
};

OCF.prototype.createResource = function(name, uri, types, interface_mask) {
  var ocf_adapter = this.getAdapter();
  return new OCFResource(ocf_adapter, name, uri, types, interface_mask);
};

function OCFAdapter() {
  this._mfg_name = '';
  this._devices = [];
  this._resources = [];
  this._nextDeviceId = 0;

  // Default handler
  var self = this;
  this.onInitialize(function() {
    self.setPlatform('ant');
    self.addDevice('/oic/d', 'oic.d.light', 'Light', 'ocf.res.1.0.0');
  });
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

OCFAdapter.prototype.onInitialize = function(handler) {
  native.ocf_adapter_onInitialize(handler);
};
OCFAdapter.prototype.onPrepareClient = function(handler) {
  native.ocf_adapter_onPrepareClient(handler);
};
OCFAdapter.prototype.onPrepareServer = function(handler) {
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
    interface_mask |= interface_masks[i];
  }
  this.default_interface_mask = interface_mask[0];
  this.is_discoverable = undefined;
  this.period_sec = undefined;

  native.ocf_resource_constructor(
      this, name, uri, types, interface_mask, this.default_interface_mask,
      device.id);
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
  // TODO:
  native.ocf_resource_setHandler(this, method, handler);
};

module.exports = new OCF();
module.exports.OCF = OCF;