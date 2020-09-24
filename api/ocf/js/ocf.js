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
  this.mfg_name = "ANT";
  this.devices = [];

  native.ocf_adapter_constructor(this, "ANT");
}
OCFAdapter.prototype.setPlatform = function (mfg_name) {
  native.ocf_adapter_setPlatform(this, mfg_name);
  this.mfg_name = mfg_name;
}
OCFAdapter.prototype.getPlatform = function () {
  return this.mfg_name;
}
OCFAdapter.prototype.addDevice = function (uri, resource_type, name, spec_version, data_model_version) {
  native.ocf_adapter_addDevice(this, uri, resource_type, name, spec_version, data_model_version);
  this.devices.push(new OCFDevice(uri, resource_type, name, spec_version, data_model_version));
}
OCFAdapter.prototype.getDevices = function () {
  return this.devices;
}
OCFAdapter.prototype.onPrepareClient = function (handler) {
  native.ocf_adapter_onPrepareClient(this, handler);
};
OCFAdapter.prototype.onPrepareServer = function (handler) {
  native.ocf_adapter_onPrepareServer(this, handler);
};

OCFAdapter.prototype.start = function () {
  native.ocf_adapter_start();
};
OCFAdapter.prototype.stop = function () {
  native.ocf_adapter_stop();
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