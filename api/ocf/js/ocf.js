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

function OCFAdapter() {
  this._mfg_name = "";
  this._devices = [];

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
  native.ocf_adapter_start(this);
};
OCFAdapter.prototype.stop = function () {
  native.ocf_adapter_stop(this);
};

OCFAdapter.prototype.addResource = function (resource) {
};

function OCFDevice(uri, resource_type, name, spec_version, data_model_version) {
  this.uri = uri;
  this.resource_type = resource_type;
  this.name = name;
  this.spec_version = spec_version;
  this.data_model_version = data_model_version;
}

function OCFResource() { }

OCFResource.prototype.setHandler = function (type, handler) {

};

module.exports = new OCF();
module.exports.OCF = OCF;