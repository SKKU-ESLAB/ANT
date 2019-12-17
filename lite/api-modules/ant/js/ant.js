var console = require('console');

var RESULT_SUCCESS = 'Success';
var RESULT_FAILED = 'Failed';

/** Runtime API start **/
function RuntimeAPI() { }
RuntimeAPI._mCurrentApp = undefined;

RuntimeAPI._removeCurrentApp = function () {
  this._mCurrentApp = undefined;
};

RuntimeAPI.setCurrentApp = function (onInitialize, onStart, onStop) {
  if (!(onInitialize instanceof Function) || !(onStart instanceof Function) ||
    !(onStop instanceof Function)) {
    return RESULT_FAILED;
  }
  this._mCurrentApp = new App(onInitialize, onStart, onStop);

  return RESULT_SUCCESS;
};

RuntimeAPI.getCurrentApp = function () {
  return this._mCurrentApp;
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

/** ANT main object start **/
function ANT() { }

ANT.prototype.runtime = RuntimeAPI;

try {
  ANT.prototype.resource = require('antresource');
} catch (e) {
  console.log("Not found Resource API");
}
try {
  ANT.prototype.remoteui = require('antremoteui');
} catch (e) {
  console.log("Not found Remote UI API");
}
try {
  ANT.prototype.stream = require('antstream');
} catch (e) {
  console.log("Not found Stream API");
}
try {
  ANT.prototype.ml = require('antml');
} catch (e) {
  console.log("Not found ML API");
}
try {
  ANT.prototype.companion = require('antcompanion');
} catch (e) {
  console.log("Not found Companion API");
}

/** ANT main object end **/

module.exports = new ANT();
module.exports.ANT = ANT;