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

// TODO: singleton adapter
function OCFAdapter() { }

OCFAdapter.prototype.onPrepareClient = function (handler) {

};

OCFAdapter.prototype.onPrepareServer = function (handler) {

};

OCFAdapter.prototype.addResource = function (resource) {
};

OCFAdapter.prototype.start = function () {
  native.ocf_adapter_start();
};

OCFAdapter.prototype.stop = function () {

};

function OCFResource() { }

OCFResource.prototype.setHandler = function (type, handler) {

};

module.exports = new OCF();
module.exports.OCF = OCF;