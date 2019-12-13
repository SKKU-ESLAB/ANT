var console = require('console');
var http = require('http');
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

/** Stream API start **/
function StreamAPI() { }
StreamAPI._mIsInitialized = false;
StreamAPI.pipelines = [];

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
StreamAPI.finalize = function () {
  var streamapi = this;
  var quitMainLoop = function () {
    var result = Boolean(streamapi.callDbusMethod("streamapi_quitMainLoop"));
    if (result) {
      this._mIsInitialized = false;
    }
    return result;
  };
  for (var i in this.pipelines) {
    this.pipelines[i].setState(this.pipelines[i].STATE_NULL);
    this.pipelines[i].unref();
    this.pipelines.splice(i, 1);
  }
  console.log("quitMainLoop()");
  quitMainLoop();
  console.log("end");
};
StreamAPI.createPipeline = function (pipeline_name) {
  if (!this._mIsInitialized) {
    console.error("ERROR: Stream API is not initialized");
    return undefined;
  }
  var element_index = Number(this.callDbusMethod(
    "streamapi_createPipeline\n" + pipeline_name));
  var pipeline = new Pipeline(pipeline_name, element_index);
  this.pipelines.push(pipeline);
  return pipeline;
};
StreamAPI.createElement = function (element_name) {
  if (!this._mIsInitialized) {
    console.error("ERROR: Stream API is not initialized");
    return undefined;
  }
  var element_index = Number(this.callDbusMethod(
    "streamapi_createElement\n" + element_name));
  // TODO: embedding Pipeline.bin_add()
  // console.log("Element: " + element_name + " / " + element_index);
  return new Element(element_name, element_index);
};

function Pipeline(name, element_index) {
  this._element_index = element_index;

  this.name = name;
  this.elements = [];

  this.STATE_VOID_PENDING = 0;
  this.STATE_NULL = 1;
  this.STATE_READY = 2;
  this.STATE_PAUSED = 3;
  this.STATE_PLAYING = 4;
}
Pipeline.prototype.binAdd = function (element_or_elements) {
  if (!StreamAPI.isInitialized()) {
    console.error("ERROR: Stream API is not initialized");
    return false;
  }
  if (Array.isArray(element_or_elements)) {
    var elements = element_or_elements;
    for (var i in elements) {
      var result = this.binAdd(elements[i]);
      if (!result) {
        console.error("ERROR: Failed to add to bin at " + i);
        return false;
      }
    }
    return true;
  } else if (element_or_elements instanceof Element) {
    var element = element_or_elements;
    this.elements.push(element);
    var result = Boolean(StreamAPI.callDbusMethod(
      "pipeline_binAdd\n" + this._element_index + "\n" + element._element_index));
    return result;
  } else {
    console.error("ERROR: Invalid element");
    return false;
  }
};
Pipeline.prototype.setState = function (state) {
  if (!StreamAPI.isInitialized()) {
    console.error("ERROR: Stream API is not initialized");
    return false;
  }
  if (typeof state !== "number") {
    console.error("ERROR: Invalid state! " + state);
    return false;
  }
  var result = Number(StreamAPI.callDbusMethod(
    "pipeline_setState\n" + this._element_index + "\n" + state));
  return result;
};
Pipeline.prototype.linkMany = function (elements) {
  if (!StreamAPI.isInitialized()) {
    console.error("ERROR: Stream API is not initialized");
    return false;
  }
  var prevElement = undefined;
  for (var i in elements) {
    if (prevElement !== undefined) {
      var result = prevElement.link(elements[i]);
      if (!result) {
        console.error("ERROR: Failed to link element at " + (i - 1) + " with one at " + (i));
        return false;
      }
    }
    prevElement = elements[i];
  }
  return true;
};
Pipeline.prototype.unref = function () {
  var result = Boolean(StreamAPI.callDbusMethod(
    "pipeline_unref\n" + this._element_index));
  return result;
};

function Element(name, element_index) {
  this._element_index = element_index;
  this.name = name;  // copy of native
  this.properties = {}; // copy of native
  this.handlers = {};
  this.srcElement = undefined;
  this.sinkElement = undefined;
}
Element.prototype.setProperty = function (key, value) {
  if (!StreamAPI.isInitialized()) {
    console.error("ERROR: Stream API is not initialized");
    return false;
  } else if (typeof key !== "string") {
    console.error("ERROR: Invalid key: " + key);
    return false;
  } else if (typeof value !== "string" && typeof value !== "number" && typeof value !== "boolean") {
    console.error("ERROR: Invalid value: " + value);
    return false;
  }
  var type;
  if (typeof value === "boolean") {
    type = 0;
  } else if (typeof value === "string") {
    type = 1;
  } else if (value === parseInt(value, 10)) {
    type = 2;
  } else {
    type = 3;
  }
  var result = Boolean(StreamAPI.callDbusMethod(
    "element_setProperty\n" + this._element_index + "\n" + key + "\n" + type + "\n" + value));
  this.properties[key] = value;
  return result;
};
Element.prototype.setCapsProperty = function (key, value) {
  if (!StreamAPI.isInitialized()) {
    console.error("ERROR: Stream API is not initialized");
    return false;
  } else if (typeof key !== "string") {
    console.error("ERROR: Invalid key: " + key);
    return false;
  } else if (typeof value !== "string") {
    console.error("ERROR: Invalid value: " + value);
    return false;
  }
  var result = Boolean(StreamAPI.callDbusMethod(
    "element_setCapsProperty\n" + this._element_index + "\n" + key + "\n" + value));
  this.properties[key] = value;
  return result;
};
Element.prototype.connectSignal = function (detailedSignal, handler) {
  if (!StreamAPI.isInitialized()) {
    console.error("ERROR: Stream API is not initialized");
    return;
  } else if (typeof detailedSignal !== "string") {
    console.error("ERROR: Invalid detailedSignal: " + detailedSignal);
    return false;
  } else if (typeof handler !== "function") {
    console.error("ERROR: Invalid handler for " + detailedSignal);
    return false;
  } else if (this.handlers[detailedSignal] !== undefined) {
    console.error("ERROR: Handler already exists for " + detailedSignal);
    return false;
  }
  var result = native.stream_elementConnectSignal(this._element_index, detailedSignal, handler);
  if (result) {
    this.handlers[detailedSignal] = handler;
  }
  return result;
};
Element.prototype.link = function (destElement) {
  if (destElement === undefined || !(destElement instanceof Element)) {
    console.error("ERROR: Invalid sink element");
    return false;
  }
  this.sinkElement = destElement;
  destElement.srcElement = this;

  var result = Boolean(StreamAPI.callDbusMethod(
    "element_link\n" + this._element_index + "\n" + destElement._element_index));
  return result;
};

/** Stream API end **/

/** ML API start **/
function MLAPI() { }

MLAPI.createMLElement = function (model_name, input_shape, input_type, output_shape, output_type) {
  if (model_name.indexOf(" ") >= 0) {
    console.error("Invalid model_name! " + model_name);
    return undefined;
  }
  if (input_shape.indexOf(" ") >= 0) {
    console.error("Invalid input_shape! " + input_shape);
    return undefined;
  }
  if (input_type.indexOf(" ") >= 0) {
    console.error("Invalid input_type! " + input_type);
    return undefined;
  }
  if (output_shape.indexOf(" ") >= 0) {
    console.error("Invalid output_shape! " + output_shape);
    return undefined;
  }
  if (output_type.indexOf(" ") >= 0) {
    console.error("Invalid output_type! " + output_type);
    return undefined;
  }
  if (!StreamAPI.isInitialized()) {
    console.error("ERROR: Stream API is not initialized");
    return;
  }

  function shape_array_to_str(shape_array) {
    var shape_str = "";
    for (var i in shape_array) {
      if (i == 0) {
        shape_str = shape_str + shape_array[i];
      } else {
        shape_str = shape_str + ":" + shape_array[i];
      }
    }
    return shape_str;
  }

  var input_shape_str = shape_array_to_str(input_shape);
  var output_shape_str = shape_array_to_str(output_shape);

  var tensor_filter = StreamAPI.createElement("tensor_filter");
  tensor_filter.setProperty("framework", "python3");
  tensor_filter.setProperty("model", "./ml/tvm_nnstreamer.py");
  tensor_filter.setProperty("input", input_shape_str);
  tensor_filter.setProperty("inputtype", input_type);
  tensor_filter.setProperty("output", output_shape_str);
  tensor_filter.setProperty("outputtype", output_type);
  var custom = model_name + " " + input_shape_str + " " + input_type
    + " " + output_shape_str + " " + output_type;
  tensor_filter.setProperty("custom", custom);
  console.log(custom);
  return tensor_filter;
};

MLAPI.connectCompressionServer = function (ipAddress) {
  return new CompressionServer(ipAddress);
};

function CompressionServer(ipAddress) {
  this.ipAddress = ipAddress;
}

CompressionServer.prototype.downloadModel = function (modelId) {
  if (this.ipAddress === undefined)
    return undefined;
  // TODO: implement details
};
CompressionServer.prototype.downloadKernel = function (kernelId) {
  if (this.ipAddress === undefined)
    return undefined;
  // TODO: implement details
};

/** NN API end */

/** Companion API start **/
function CompanionAPI() { }
CompanionAPI._mCompanionHost = undefined;
CompanionAPI._mCompanionPort = undefined;
CompanionAPI._mCompanionPath = undefined;
CompanionAPI._mHandlers = [];

CompanionAPI._setCompanionAddress = function (
  companionHost, companionPort, companionPath) {
  this._mCompanionHost = companionHost;
  this._mCompanionPort = companionPort;
  this._mCompanionPath = companionPath;
  return true;
};

CompanionAPI._onReceiveMessageFromCompanion = function (message) {
  for (var i in this._mHandlers) {
    this._mHandlers[i](message);
  }
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

CompanionAPI.registerOnReceiveMessage = function (handler) {
  this._mHandlers.push(handler);
};
CompanionAPI.unregisterOnReceiveMessage = function (handler) {
  if (handler === undefined) return false;
  for (var i in this._mHandlers) {
    if (this._mHandlers[i] === handler) {
      this._mHandlers.splice(i, 1);
      return true;
    }
  }
  return false;
};
/** Companion API end **/

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
ANT.prototype.stream = StreamAPI;
ANT.prototype.ml = MLAPI;
ANT.prototype.companion = CompanionAPI;
ANT.prototype.resource = ResourceAPI;
ANT.prototype.remoteui = RemoteUIAPI;

/** ANT main object end **/

module.exports = new ANT();
module.exports.ANT = ANT;