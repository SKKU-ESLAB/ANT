var console = require('console');
var http = require('http');

var RESULT_SUCCESS = 'Success';
var RESULT_FAILED = 'Failed';

/** Runtime API start **/
var RuntimeAPI = { _mCurrentApp: undefined };

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

/** Runtime API end **/

/** Stream API start **/
// TODO: native-side Stream API implementation is required.
function StreamAPI() {
  this._mIsInitialized = false;
  this._mIsTestInitialized = false;
}
StreamAPI.initialize = function () {
  this._mIsInitialized = true;
  // TODO: add native function call
};

StreamAPI.createPipeline = function (pipeline_name) {
  if (!this._mIsInitialized) {
    console.error("ERROR: Stream API is not initialized");
    return undefined;
  }
  // TODO: add native function call
  return new Pipeline(pipeline_name);
};
StreamAPI.createElement = function (element_name) {
  if (!this._mIsInitialized) {
    console.error("ERROR: Stream API is not initialized");
    return undefined;
  }
  // TODO: add native function call
  return new Element(element_name);
};
StreamAPI.sendDbusSignal = function (message) {
  if (!this._mIsTestInitialized) {
    console.error("ERROR: Stream API is not initialized");
    return false;
  }
  return native.stream_sendDbusSignal(message);
};
StreamAPI.testPipeline = function (ip_address) {
  if (ip_address === undefined || typeof ip_address !== "string") {
    console.error("ERROR: invalid arguments: " + ip_address);
    return false;
  }
  var result = native.stream_testPipeline(ip_address);
  if (result) {
    this._mIsTestInitialized = true;
  }
  return result;
};

function Pipeline(name) {
  this.name = name;
  this.elements = [];
}
Pipeline.prototype.add = function (element) {
  // TODO: type check for element is required
  this.elements.push(element);
  // TODO: add native function call
};
Pipeline.prototype.link_many = function (elements) {
  // TODO: not yet implemented (low priority)
};
Pipeline.prototype.setState = function (state) {
  // TODO: add native function call
};

function Element(name) {
  this.name = name;
  this.properties = {};
  this.isSinkElement = false;
  this.handlers = {};
  this.srcElement = undefined;
  this.sinkElement = undefined;
}

Element.prototype.setProperty = function (key, value) {
  // TODO: type check for key, value is required
  // TODO: check for existing property is required
  // TODO: success/failed result is required
  this.properties[key] = value;
  // TODO: add native function call
}
Element.prototype.setSinkElement = function (isSinkElement) {
  this.isSinkElement = isSinkElement;
}
Element.prototype.connect = function (detailedSignal, handler) {
  // TODO: type check for detailedSignal and handler is required
  // TODO: check for existing handler is required
  // TODO: success/failed result is required
  this.handlers[detailedSignal] = handler;
  // TODO: add native function call
}
Element.prototype.link = function (sinkElement) {
  if (sinkElement !== undefined) {
    this.sinkElement = sinkElement;
    sinkElement.srcElement = this;
    // TODO: add native function call
  }
}

/** Stream API end **/

/** Companion API start **/
function CompanionAPI() {
  this._mCompanionHost = undefined;
  this._mCompanionPort = undefined;
  this._mCompanionPath = undefined;
  this._mHandler = undefined;
}

CompanionAPI._setCompanionAddress = function (
  companionHost, companionPort, companionPath) {
  this._mCompanionHost = companionHost;
  this._mCompanionPort = companionPort;
  this._mCompanionPath = companionPath;
  return true;
};

CompanionAPI._onReceiveMessageFromCompanion = function (message) {
  this._mHandler(message);
};

CompanionAPI.sendMessage = function (message) {
  if (this._mCompanionPath === undefined) {
    console.log('Error: failed to send message due to no companion address');
    return false;
  }

  // IoT.js ----
  var options = {
    method: 'POST',
    host: this._mCompanionHost,
    port: this._mCompanionPort,
    path: this._mCompanionPath,
    headers: { 'Content-Length': message.length },
  };
  var client_request = http.request(options);
  client_request.write(message);
  client_request.end();
  // node.js ----
  // var options = {
  //   method: 'POST',
  //   uri: 'http://' + this._mCompanionHost + ':' + this._mCompanionPort + '/' + this._mCompanionPath,
  //   body: message
  // };
  // request.post(options, function (err, httpResponse, body) {
  // });
  return true;
};

CompanionAPI.setOnReceiveMessage = function (handler) {
  this._mHandler = handler;
};
/** Companion API end **/


/** Compression Server API start **/
function CompressionServerAPI() {
  this.ipAddress = undefined;
}
CompressionServerAPI.connect = function (ipAddress) {
  this.ipAddress = ipAddress;
};
CompressionServerAPI.downloadModel = function (modelId) {
  if (this.ipAddress === undefined)
    return undefined;
  // TODO: implement details
};
CompressionServerAPI.downloadKernel = function (kernelId) {
  if (this.ipAddress === undefined)
    return undefined;
  // TODO: implement details
};
/** Compression Server API end **/


/** ANT main object start **/
function ANT() { }

ANT.prototype.runtime = RuntimeAPI;
ANT.prototype.stream = StreamAPI;
ANT.prototype.companion = CompanionAPI;
ANT.prototype.compressionServer = CompressionServerAPI;

/** ANT main object end **/

module.exports = new ANT();
module.exports.ANT = ANT;