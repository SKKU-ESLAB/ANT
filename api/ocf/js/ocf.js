var console = require('console');

var RESULT_SUCCESS = 'Success';
var RESULT_FAILED = 'Failed';

/** OCF API start **/
function OCF() { }

var sOCFAdapter = undefined;
OCF.prototype.getAdapter = function () {
  if (sOCFAdapter === undefined) {
    sOCFAdapter = new OCFAdapter();
  }
  return sOCFAdapter;
};

OCF.prototype.createResource = function(name, uri, types, interface_mask) {
  return new OCFResource(name, uri, types, interface_mask);
};

function OCFAdapter() {
  this._mfg_name = "";
  this._devices = [];
  this._resources = [];

  // Default handler
  var self = this;
  this.onInitialize(function () {
    self.setPlatform("ant");
    self.addDevice("/oic/d", "oic.d.light", "Light", "ocf.res.1.0.0");
  });
}
OCFAdapter.prototype.setPlatform = function (mfg_name) {
  this._mfg_name = mfg_name;
  native.ocf_adapter_setPlatform(mfg_name);
}
OCFAdapter.prototype.getPlatform = function () {
  return this._mfg_name;
}
OCFAdapter.prototype.addDevice = function (uri, resource_type, name, spec_version, data_model_version) {
  this._devices.push(new OCFDevice(uri, resource_type, name, spec_version, data_model_version));
  native.ocf_adapter_addDevice(uri, resource_type, name, spec_version, data_model_version);
}
OCFAdapter.prototype.getDevices = function () {
  return this._devices;
}

OCFAdapter.prototype.onInitialize = function (handler) {
  native.ocf_adapter_onInitialize(handler);
};
OCFAdapter.prototype.onPrepareClient = function (handler) {
  native.ocf_adapter_onPrepareClient(handler);
};
OCFAdapter.prototype.onPrepareServer = function (handler) {
  native.ocf_adapter_onPrepareServer(handler);
};
OCFAdapter.prototype.start = function () {
  native.ocf_adapter_start();
};
OCFAdapter.prototype.stop = function () {
  native.ocf_adapter_stop();
};

OCFAdapter.prototype.addResource = function (resource) {
  this._resources.push(resource);
  // TODO:
  native.ocf_adapter_addResource(resource.name, resource.uri, resource.types, resource.interface_mask);
};
OCFAdapter.prototype.getResources = function () {
  return this._resources;
};

function OCFDevice(uri, resource_type, name, spec_version, data_model_version) {
  this.uri = uri;
  this.resource_type = resource_type;
  this.name = name;
  this.spec_version = spec_version;
  this.data_model_version = data_model_version;
}

function OCFResource(name, uri, types, interface_mask) {
  this.name = name;
  this.uri = uri;
  this.types = types;
  this.interface_mask = interface_mask;
  this.is_discoverable = false;
  this.periodic_observable = 

  // TODO: binding with native ocf_resource_t
  native.ocf_resource_constructor(name, uri, types, interface_mask);
}

OCFResource.prototype.setHandler = function (type, handler) {
  // TODO:
  native.ocf_resource_setHandler(this, type, handler);
};

OCFResource.prototype.setDiscoverable = function (is_discoverable) {
  this.is_discoverable = is_discoverable;
  // TODO:
  native.ocf_resource_setDiscoverable(this, is_discoverable);
};

OCFResource.prototype.setPeriodicObservable = function (periodic_b) {
  this.is_discoverable = is_discoverable;
  // TODO:
  native.ocf_resource_setPeriodicObservable(this, is_periodicObservable);
};

module.exports = new OCF();
module.exports.OCF = OCF;