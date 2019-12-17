var console = require('console');

var CompanionAPI = require('antcompanion');

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

/** Runtime API end **/

/** Remote UI API start **/
function RemoteUIAPI() {
}
RemoteUIAPI.setStreamingViewPipeline = function (pipeline, handler) {
  ResourceAPI.requestPost("/remoteui/streamingview/pipeline", pipeline, handler);
};
RemoteUIAPI.setStreamingViewLabelText = function (labelText, handler) {
  ResourceAPI.requestPost("/remoteui/streamingview/labelText", labelText, handler);
};
/** Remote UI API end **/

/** Resource API start **/
function ResourceAPI() { }
ResourceAPI._mIsInitialized = false;
ResourceAPI._mRequestId = 0;
ResourceAPI._mOnResourceResponseDict = {};
// TODO: implement hosting resource

ResourceAPI._initialize = function () {
  ResourceAPI._mIsInitialized = true;
  CompanionAPI.registerOnReceiveMessage(ResourceAPI._onReceiveRawMessage);
};
ResourceAPI._onReceiveRawMessage = function (rawMessage) {
  var firstLineEnd = rawMessage.indexOf("\n");
  var firstLine = rawMessage.substring(0, firstLineEnd);
  if (firstLine != "ResourceResponse") {
    return;
  }
  var secondLineEnd = rawMessage.indexOf("\n", firstLineEnd + 1);
  var secondLine = rawMessage.substring(firstLineEnd + 1, secondLineEnd);
  var thirdLineEnd = rawMessage.indexOf("\n", secondLineEnd + 1);
  var thirdLine = rawMessage.substring(secondLineEnd + 1, thirdLineEnd);
  var fourthLineEnd = rawMessage.indexOf("\n", thirdLineEnd + 1);
  var fourthLine = rawMessage.substring(thirdLineEnd + 1, fourthLineEnd);
  var otherLines = rawMessage.substring(fourthLineEnd + 1);

  var requestId = Number(secondLine);
  var method = thirdLine;
  var targetUri = fourthLine;
  var message = otherLines;

  var onResourceResponse = ResourceAPI._mOnResourceResponseDict[requestId];
  if (onResourceResponse !== undefined) {
    onResourceResponse(method, targetUri, message);
    delete ResourceAPI._mOnResourceResponseDict[requestId];
  }
};

// ResourceHandler arguments: (String method, String targetUri, String message)
ResourceAPI._sendRequest = function (method, targetUri, message, onResourceResponse) {
  if (!ResourceAPI._mIsInitialized) {
    ResourceAPI._initialize();
  }
  var rawMessage = "" + "ResourceRequest\n" + ResourceAPI._mRequestId + "\n" + method + "\n" + targetUri + "\n" + message;
  ResourceAPI._mRequestId++;

  CompanionAPI.sendMessage(rawMessage);
  if (onResourceResponse !== undefined) {
    ResourceAPI._mOnResourceResponseDict[ResourceAPI._mRequestId] = onResourceResponse;
  }
};

ResourceAPI.reqeustGet = function (targetUri, message, onResourceResponse) {
  ResourceAPI._sendRequest("GET", targetUri, message, onResourceResponse);
};
ResourceAPI.requestPost = function (targetUri, message, onResourceResponse) {
  ResourceAPI._sendRequest("POST", targetUri, message, onResourceResponse);
};
ResourceAPI.requestPut = function (targetUri, message, onResourceResponse) {
  ResourceAPI._sendRequest("PUT", targetUri, message, onResourceResponse);
};
ResourceAPI.requestDelete = function (targetUri, message, onResourceResponse) {
  ResourceAPI._sendRequest("DELETE", targetUri, message, onResourceResponse);
};

/** Resource API end */


/** ANT main object start **/
function ANT() { }

ANT.prototype.runtime = RuntimeAPI;
ANT.prototype.resource = ResourceAPI;
ANT.prototype.remoteui = RemoteUIAPI;
ANT.prototype.stream = require('antstream');
ANT.prototype.ml = require('antml');
ANT.prototype.companion = require('antcompanion');

/** ANT main object end **/

module.exports = new ANT();
module.exports.ANT = ANT;