/* Copyright (c) 2017-2020 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

var console = require('console');

/**
 * ANT Stream API
 */
function ANTStream() {}
ANTStream.prototype._mIsInitialized = false;
ANTStream.prototype.pipelines = [];

ANTStream.prototype.callDbusMethod = function (message) {
  if (!this._mIsInitialized) {
    console.error('ERROR: Stream API is not initialized');
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
    var result = Boolean(ANTStream.callDbusMethod('streamapi_quitMainLoop'));
    if (result) {
      this._mIsInitialized = false;
    }
    return result;
  };
  for (var i = 0; i < this.pipelines.length; i++) {
    this.pipelines[i].setState(this.pipelines[i].STATE_NULL);
    this.pipelines[i].unref();
    this.pipelines.splice(i, 1);
  }
  console.log('quitMainLoop()');
  quitMainLoop();
  console.log('end');
};
ANTStream.prototype.createPipeline = function (pipelineName) {
  if (!this._mIsInitialized) {
    console.error('ERROR: Stream API is not initialized');
    return undefined;
  }
  var elementIndex = Number(
    this.callDbusMethod('streamapi_createPipeline\n' + pipelineName)
  );
  var pipeline = new Pipeline(pipelineName, elementIndex);
  this.pipelines.push(pipeline);
  return pipeline;
};
ANTStream.prototype.createElement = function (elementName) {
  if (!this._mIsInitialized) {
    console.error('ERROR: Stream API is not initialized');
    return undefined;
  }
  var elementIndex = Number(
    this.callDbusMethod('streamapi_createElement\n' + elementName)
  );
  // TODO: embedding Pipeline.bin_add()
  // console.log("Element: " + elementName + " / " + elementIndex);
  return new Element(elementName, elementIndex);
};

/**
 * Pipeline
 * @param {string} name the name of the pipeline
 * @param {int} elementIndex the element index of the pipeline that is
 * internally managed.
 */
function Pipeline(name, elementIndex) {
  this._elementIndex = elementIndex;

  this.name = name;
  this.elements = [];

  this.STATE_VOID_PENDING = 0;
  this.STATE_NULL = 1;
  this.STATE_READY = 2;
  this.STATE_PAUSED = 3;
  this.STATE_PLAYING = 4;
}
Pipeline.prototype.binAdd = function (elementOrElements) {
  var ANTStream = require('antstream');
  if (!ANTStream.isInitialized()) {
    console.error('ERROR: Stream API is not initialized');
    return false;
  }
  if (Array.isArray(elementOrElements)) {
    var elements = elementOrElements;
    for (var i = 0; i < elements.length; i++) {
      var result = this.binAdd(elements[i]);
      if (!result) {
        console.error('ERROR: Failed to add to bin at ' + i);
        return false;
      }
    }
    return true;
  } else if (elementOrElements instanceof Element) {
    var element = elementOrElements;
    this.elements.push(element);
    var result = Boolean(
      ANTStream.callDbusMethod(
        'pipeline_binAdd\n' + this._elementIndex + '\n' + element._elementIndex
      )
    );
    return result;
  } else {
    console.error('ERROR: Invalid element');
    return false;
  }
};
Pipeline.prototype.setState = function (state) {
  var ANTStream = require('antstream');
  if (!ANTStream.isInitialized()) {
    console.error('ERROR: Stream API is not initialized');
    return false;
  }
  if (typeof state !== 'number') {
    console.error('ERROR: Invalid state! ' + state);
    return false;
  }
  var result = Number(
    ANTStream.callDbusMethod(
      'pipeline_setState\n' + this._elementIndex + '\n' + state
    )
  );
  return result;
};
Pipeline.prototype.linkMany = function (elements) {
  var ANTStream = require('antstream');
  if (!ANTStream.isInitialized()) {
    console.error('ERROR: Stream API is not initialized');
    return false;
  }
  var prevElement = undefined;
  for (var i = 0; i < elements.length; i++) {
    if (prevElement !== undefined) {
      var result = prevElement.link(elements[i]);
      if (!result) {
        console.error(
          'ERROR: Failed to link element at ' + (i - 1) + ' with one at ' + i
        );
        return false;
      }
    }
    prevElement = elements[i];
  }
  return true;
};
Pipeline.prototype.unref = function () {
  var ANTStream = require('antstream');
  var result = Boolean(
    ANTStream.callDbusMethod('pipeline_unref\n' + this._elementIndex)
  );
  return result;
};

/**
 * Element
 * @param {string} name the name of the element
 * @param {int} elementIndex the index of the element that is
 * internally managed.
 */
function Element(name, elementIndex) {
  this._elementIndex = elementIndex;
  this.name = name; // copy of native
  this.properties = {}; // copy of native
  this.handlers = {};
  this.srcElement = undefined;
  this.sinkElement = undefined;
}
Element.prototype.setProperty = function (key, value) {
  var ANTStream = require('antstream');
  if (!ANTStream.isInitialized()) {
    console.error('ERROR: Stream API is not initialized');
    return false;
  } else if (typeof key !== 'string') {
    console.error('ERROR: Invalid key: ' + key);
    return false;
  } else if (
    typeof value !== 'string' &&
    typeof value !== 'number' &&
    typeof value !== 'boolean'
  ) {
    console.error('ERROR: Invalid value: ' + value);
    return false;
  }
  var type;
  if (typeof value === 'boolean') {
    type = 0;
  } else if (typeof value === 'string') {
    type = 1;
  } else if (value === parseInt(value, 10)) {
    type = 2;
  } else {
    type = 3;
  }
  var result = Boolean(
    ANTStream.callDbusMethod(
      'element_setProperty\n' +
        this._elementIndex +
        '\n' +
        key +
        '\n' +
        type +
        '\n' +
        value
    )
  );
  this.properties[key] = value;
  return result;
};
Element.prototype.setCapsProperty = function (key, value) {
  var ANTStream = require('antstream');
  if (!ANTStream.isInitialized()) {
    console.error('ERROR: Stream API is not initialized');
    return false;
  } else if (typeof key !== 'string') {
    console.error('ERROR: Invalid key: ' + key);
    return false;
  } else if (typeof value !== 'string') {
    console.error('ERROR: Invalid value: ' + value);
    return false;
  }
  var result = Boolean(
    ANTStream.callDbusMethod(
      'element_setCapsProperty\n' +
        this._elementIndex +
        '\n' +
        key +
        '\n' +
        value
    )
  );
  this.properties[key] = value;
  return result;
};
Element.prototype.connectSignal = function (detailedSignal, handler) {
  var ANTStream = require('antstream');
  if (!ANTStream.isInitialized()) {
    console.error('ERROR: Stream API is not initialized');
    return;
  } else if (typeof detailedSignal !== 'string') {
    console.error('ERROR: Invalid detailedSignal: ' + detailedSignal);
    return false;
  } else if (typeof handler !== 'function') {
    console.error('ERROR: Invalid handler for ' + detailedSignal);
    return false;
  } else if (this.handlers[detailedSignal] !== undefined) {
    console.error('ERROR: Handler already exists for ' + detailedSignal);
    return false;
  }
  var result = native.ant_stream_elementConnectSignal(
    this._elementIndex,
    detailedSignal,
    handler
  );
  if (result) {
    this.handlers[detailedSignal] = handler;
  }
  return result;
};
Element.prototype.link = function (destElement) {
  var ANTStream = require('antstream');
  if (destElement === undefined || !(destElement instanceof Element)) {
    console.error('ERROR: Invalid sink element');
    return false;
  }
  this.sinkElement = destElement;
  destElement.srcElement = this;

  var result = Boolean(
    ANTStream.callDbusMethod(
      'element_link\n' + this._elementIndex + '\n' + destElement._elementIndex
    )
  );
  return result;
};

module.exports = new ANTStream();
module.exports.ANTStream = ANTStream;
