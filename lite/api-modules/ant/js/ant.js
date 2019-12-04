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
function StreamAPI() {
  this._mIsInitialized = false;
}
StreamAPI.callDbusMethod = function (message) {
  if (!this._mIsInitialized) {
    console.error("ERROR: Stream API is not initialized");
    return false;
  }
  return native.stream_callDbusMethod(message);
};

StreamAPI.isInitialized = function () {
  return this._mIsInitialized;
};

StreamAPI.initialize = function () {
  this._mIsInitialized = true;
  native.stream_initializeStream();
};
StreamAPI.createPipeline = function (pipeline_name) {
  if (!this._mIsInitialized) {
    console.error("ERROR: Stream API is not initialized");
    return undefined;
  }
  var element_index = Number(this.callDbusMethod(
    "createPipeline\n" + pipeline_name));
  return new Pipeline(this, pipeline_name, element_index);
};
StreamAPI.createElement = function (element_name) {
  if (!this._mIsInitialized) {
    console.error("ERROR: Stream API is not initialized");
    return undefined;
  }
  var element_index = Number(this.callDbusMethod(
    "createElement\n" + element_name));
  return new Element(this, element_name, element_index);
};

function Pipeline(streamapi, name, element_index) {
  this._streamapi = streamapi;
  this._element_index = element_index;

  this.name = name;
  this.elements = [];

  this.GST_STATE_VOID_PENDING = 0;
  this.GST_STATE_NULL = 1;
  this.GST_STATE_READY = 2;
  this.GST_STATE_PAUSED = 3;
  this.GST_STATE_PLAYING = 4;
}
Pipeline.prototype.bin_add = function (element) {
  if (!this._streamapi.isInitialized()) {
    console.error("ERROR: Stream API is not initialized");
    return false;
  } else if (element.constructor.name !== "Element") {
    console.error("ERROR: Invalid element");
    return false;
  }
  this.elements.push(element);
  var result = Boolean(this._streamapi.callDbusMethod(
    "pipelineBinAdd\n" + element._element_index));
  return result;
};
Pipeline.prototype.setState = function (state) {
  if (!this._streamapi.isInitialized()) {
    console.error("ERROR: Stream API is not initialized");
    return false;
  }
  if (typeof state !== "number") {
    console.error("ERROR: Invalid state! " + state);
    return false;
  }
  var result = Boolean(this._streamapi.callDbusMethod(
    "pipelineSetState\n" + state));
  return result;
};
Pipeline.prototype.link_many = function (elements) {
  if (!this._streamapi.isInitialized()) {
    console.error("ERROR: Stream API is not initialized");
    return false;
  }
  console.error("ERROR: not yet implemented");
  return false;
};

function Element(streamapi, name, element_index) {
  this._streamapi = streamapi;
  this._element_index = element_index;
  this.name = name;  // copy of native
  this.properties = {}; // copy of native
  this.isSinkElement = false; // copy of native
  this.handlers = {};
  this.srcElement = undefined;
  this.sinkElement = undefined;
}
Element.prototype.setProperty = function (key, value) {
  if (!this._streamapi.isInitialized()) {
    console.error("ERROR: Stream API is not initialized");
    return false;
  }
  if (typeof key !== "string") {
    console.error("ERROR: Invalid key: " + key);
    return false;
  } else if (typeof value !== "string" && typeof value !== "number") {
    console.error("ERROR: Invalid value: " + value);
    return false;
  }

  var type = (typeof value === "number") ? 1 : 0;
  var result = Boolean(this._streamapi.callDbusMethod(
    "elementSetProperty\n" + key + "\n" + type + "\n" + value));
  this.properties[key] = value;
  return result;
}
Element.prototype.setSinkElement = function (isSinkElement) {
  if (!this._streamapi.isInitialized()) {
    console.error("ERROR: Stream API is not initialized");
    return false;
  }
  this.isSinkElement = isSinkElement;
  console.error("ERROR: not yet implemented");
  return false;
}
Element.prototype.connect = function (detailedSignal, handler) {
  if (!this._streamapi.isInitialized()) {
    console.error("ERROR: Stream API is not initialized");
    return;
  }
  // TODO: type check for detailedSignal and handler is required
  // TODO: check for existing handler is required
  // TODO: success/failed result is required
  this.handlers[detailedSignal] = handler;
  // TODO: add native function call
}
Element.prototype.link = function (sinkElement) {
  if (sinkElement === undefined || sinkElement.constructor.name !== "Element") {
    console.error("ERROR: Invalid sink element");
    return false;
  }
  this.sinkElement = sinkElement;
  sinkElement.srcElement = this;

  var result = Boolean(this._streamapi.callDbusMethod(
    "elementLink\n" + this._element_index + "\n" + sinkElement._element_index));
  return result;
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