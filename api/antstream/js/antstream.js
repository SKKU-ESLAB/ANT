var console = require('console');

var RESULT_SUCCESS = 'Success';
var RESULT_FAILED = 'Failed';

function ANTStream() { }
ANTStream.prototype._mIsInitialized = false;
ANTStream.prototype.pipelines = [];

ANTStream.prototype.callDbusMethod = function (message) {
  if (!this._mIsInitialized) {
    console.error("ERROR: Stream API is not initialized");
    return false;
  }
  return native.ant_stream_callDbusMethod(message);
};

ANTStream.prototype.isInitialized = function () {
  return this._mIsInitialized;
};

ANTStream.prototype.initialize = function () {
  this._mIsInitialized = true;
  native.ant_stream_initializeStream();
};
ANTStream.prototype.finalize = function () {
  var ANTStream = this;
  var quitMainLoop = function () {
    var result = Boolean(ANTStream.callDbusMethod("streamapi_quitMainLoop"));
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
ANTStream.prototype.createPipeline = function (pipeline_name) {
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
ANTStream.prototype.createElement = function (element_name) {
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
  var ANTStream = require('antstream');
  if (!ANTStream.isInitialized()) {
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
    var result = Boolean(ANTStream.callDbusMethod(
      "pipeline_binAdd\n" + this._element_index + "\n" + element._element_index));
    return result;
  } else {
    console.error("ERROR: Invalid element");
    return false;
  }
};
Pipeline.prototype.setState = function (state) {
  var ANTStream = require('antstream');
  if (!ANTStream.isInitialized()) {
    console.error("ERROR: Stream API is not initialized");
    return false;
  }
  if (typeof state !== "number") {
    console.error("ERROR: Invalid state! " + state);
    return false;
  }
  var result = Number(ANTStream.callDbusMethod(
    "pipeline_setState\n" + this._element_index + "\n" + state));
  return result;
};
Pipeline.prototype.linkMany = function (elements) {
  var ANTStream = require('antstream');
  if (!ANTStream.isInitialized()) {
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
  var ANTStream = require('antstream');
  var result = Boolean(ANTStream.callDbusMethod(
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
  var ANTStream = require('antstream');
  if (!ANTStream.isInitialized()) {
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
  var result = Boolean(ANTStream.callDbusMethod(
    "element_setProperty\n" + this._element_index + "\n" + key + "\n" + type + "\n" + value));
  this.properties[key] = value;
  return result;
};
Element.prototype.setCapsProperty = function (key, value) {
  var ANTStream = require('antstream');
  if (!ANTStream.isInitialized()) {
    console.error("ERROR: Stream API is not initialized");
    return false;
  } else if (typeof key !== "string") {
    console.error("ERROR: Invalid key: " + key);
    return false;
  } else if (typeof value !== "string") {
    console.error("ERROR: Invalid value: " + value);
    return false;
  }
  var result = Boolean(ANTStream.callDbusMethod(
    "element_setCapsProperty\n" + this._element_index + "\n" + key + "\n" + value));
  this.properties[key] = value;
  return result;
};
Element.prototype.connectSignal = function (detailedSignal, handler) {
  var ANTStream = require('antstream');
  if (!ANTStream.isInitialized()) {
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
  var result = native.ant_stream_elementConnectSignal(this._element_index, detailedSignal, handler);
  if (result) {
    this.handlers[detailedSignal] = handler;
  }
  return result;
};
Element.prototype.link = function (destElement) {
  var ANTStream = require('antstream');
  if (destElement === undefined || !(destElement instanceof Element)) {
    console.error("ERROR: Invalid sink element");
    return false;
  }
  this.sinkElement = destElement;
  destElement.srcElement = this;

  var result = Boolean(ANTStream.callDbusMethod(
    "element_link\n" + this._element_index + "\n" + destElement._element_index));
  return result;
};

module.exports = new ANTStream();
module.exports.ANTStream = ANTStream;