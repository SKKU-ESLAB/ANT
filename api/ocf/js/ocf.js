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

function OCFAdapter() { }
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
};

function OCFResource() { }

OCFResource.prototype.setHandler = function (type, handler) {

};

module.exports = new OCF();
module.exports.OCF = OCF;