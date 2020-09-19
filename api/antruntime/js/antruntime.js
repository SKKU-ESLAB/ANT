var console = require('console');

var RESULT_SUCCESS = 'Success';
var RESULT_FAILED = 'Failed';

/** Runtime API start **/
function ANTRuntime() { }
ANTRuntime.prototype._mCurrentApp = undefined;

ANTRuntime.prototype._removeCurrentApp = function () {
  this._mCurrentApp = undefined;
};

ANTRuntime.prototype.setCurrentApp = function (onInitialize, onStart, onStop) {
  if (!(onInitialize instanceof Function) || !(onStart instanceof Function) ||
    !(onStop instanceof Function)) {
    return RESULT_FAILED;
  }
  this._mCurrentApp = new App(onInitialize, onStart, onStop);

  return RESULT_SUCCESS;
};

ANTRuntime.prototype.getCurrentApp = function () {
  return this._mCurrentApp;
};

ANTRuntime.prototype.getPSSInKB = ANTRuntime.prototype.getPssInKB = function () {
  return native.ant_runtime_getPssInKB();
};

/* App start */
function App(onInitialize, onStart, onStop) {
  this.state = this.STATE.IDLE;
  this.onInitialize = onInitialize;
  this.onStart = onStart;
  this.onStop = onStop;

  this.onInitialize();
}

App.prototype.STATE = {};
App.prototype.STATE.IDLE = 0;
App.prototype.STATE.RUNNING = 1;

App.prototype.start = function () {
  if (this.state == this.STATE.RUNNING) {
    return RESULT_FAILED;
  }
  this.state = this.STATE.RUNNING;
  this.onStart();
  return RESULT_SUCCESS;
};

App.prototype.stop = function () {
  if (this.state == this.STATE.IDLE) {
    return RESULT_FAILED;
  }
  this.state = this.STATE.IDLE;
  this.onStop();
  return RESULT_SUCCESS;
};

App.prototype.getState = function () {
  if (this.state == this.STATE.IDLE) {
    return 'Idle';
  } else if (this.state == this.STATE.RUNNING) {
    return 'Running';
  } else {
    return 'Unknown';
  }
};

App.prototype.getInfo = function () {
  var appInfo = { 'state': this.getState() };
  return appInfo;
};
/* App end */

module.exports = new ANTRuntime();
module.exports.ANTRuntime = ANTRuntime;