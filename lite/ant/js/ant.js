var console = require('console');

var RESULT_SUCCESS = "Success";
var RESULT_FAILED = "Failed";

/** Runtime API start **/
var RuntimeAPI = { _current_app: undefined };

RuntimeAPI.setCurrentApp = function (on_initialize, on_start, on_stop) {
  if (!(on_initialize instanceof Function)
    || !(on_start instanceof Function)
    || !(on_stop instanceof Function)) {
    return RESULT_FAILED;
  }
  _current_app = new App(on_initialize, on_start, on_stop);

  return RESULT_SUCCESS;
};

RuntimeAPI.getCurrentApp = function () {
  return _current_app;
};

/* App start */
function App(on_initialize, on_start, on_stop) {
  this.state = this.STATE.IDLE;
  this.on_initialize = on_initialize;
  this.on_start = on_start;
  this.on_stop = on_stop;

  this.on_initialize();
}

App.prototype.STATE = {};
App.prototype.STATE.IDLE = 0;
App.prototype.STATE.RUNNING = 1;

App.prototype.start = function () {
  if (this.state == this.STATE.RUNNING) {
    return RESULT_FAILED;
  }
  this.on_start();
  return RESULT_SUCCESS;
};

App.prototype.stop = function () {
  if (this.state == this.STATE.IDLE) {
    return RESULT_FAILED;
  }
  this.on_stop();
  return RESULT_SUCCESS;
};

App.prototype.getState = function () {
  if (this.state == this.STATE.IDLE) {
    return "Idle";
  } else if (this.state == this.STATE.RUNNING) {
    return "Running";
  } else {
    return "Unknown";
  }
};
/* App end */

/** Runtime API end **/


/** Stream API start **/
function StreamAPI() {

}
/** Stream API end **/

/** Companion API start **/
function CompanionAPI() {

}
/** Companion API end **/


/** Compression Server API start **/
function CompressionServerAPI() {
}
/** Compression Server API end **/


/** ANT main object start **/
function ANT() {
}

ANT.prototype.runtime = RuntimeAPI;
ANT.prototype.stream = StreamAPI;
ANT.prototype.companion = CompanionAPI;
ANT.prototype.compressionServer = CompressionServerAPI;

/** ANT main object end **/

module.exports = new ANT();
module.exports.ANT = ANT;