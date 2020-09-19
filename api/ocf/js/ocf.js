var console = require('console');

var RESULT_SUCCESS = 'Success';
var RESULT_FAILED = 'Failed';

/** Runtime API start **/
function OCF() { }

OCF.prototype.initialize = function () {
  return native.ocf_initialize();
}

module.exports = new OCF();
module.exports.OCF = OCF;