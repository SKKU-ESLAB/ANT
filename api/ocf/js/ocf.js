var console = require('console');

var RESULT_SUCCESS = 'Success';
var RESULT_FAILED = 'Failed';

/** Runtime API start **/
function OCF() { }
OCF.prototype._mCurrentApp = undefined;

OCF.prototype._removeCurrentApp = function () {
  this._mCurrentApp = undefined;
};

OCF.prototype.setCurrentApp = function (onInitialize, onStart, onStop) {
  if (!(onInitialize instanceof Function) || !(onStart instanceof Function) ||
    !(onStop instanceof Function)) {
    return RESULT_FAILED;
  }
  this._mCurrentApp = new App(onInitialize, onStart, onStop);

  return RESULT_SUCCESS;
};

OCF.prototype.getCurrentApp = function () {
  return this._mCurrentApp;
};

OCF.prototype.getPSSInKB = OCF.prototype.getPssInKB = function () {
  return native.runtime_getPssInKB();
};

module.exports = new OCF();
module.exports.OCF = OCF;
