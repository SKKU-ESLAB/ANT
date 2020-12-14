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

var StreamAPI = undefined;
try {
  StreamAPI = require('antstream');
} catch (e) {
  throw new Error('ML API Dependency Error: not found Stream API');
}

/**
 * ANT ML API
 */
function ANTML() {}

ANTML.prototype.createMLElement = function (
  modelName,
  inputShape,
  inputType,
  outputShape,
  outputType
) {
  if (modelName.indexOf(' ') >= 0) {
    console.error('Invalid modelName! ' + modelName);
    return undefined;
  }
  if (inputShape.indexOf(' ') >= 0) {
    console.error('Invalid inputShape! ' + inputShape);
    return undefined;
  }
  if (inputType.indexOf(' ') >= 0) {
    console.error('Invalid inputType! ' + inputType);
    return undefined;
  }
  if (outputShape.indexOf(' ') >= 0) {
    console.error('Invalid outputShape! ' + outputShape);
    return undefined;
  }
  if (outputType.indexOf(' ') >= 0) {
    console.error('Invalid outputType! ' + outputType);
    return undefined;
  }
  if (!StreamAPI.isInitialized()) {
    console.error('ERROR: Stream API is not initialized');
    return;
  }

  var shapeArrayToStr = function (shapeArray) {
    var shapeStr = '';
    for (var i = 0; i < shapeArray.length; i++) {
      if (i == 0) {
        shapeStr = shapeStr + shapeArray[i];
      } else {
        shapeStr = shapeStr + ':' + shapeArray[i];
      }
    }
    return shapeStr;
  };

  var inputShapeStr = shapeArrayToStr(inputShape);
  var outputShapeStr = shapeArrayToStr(outputShape);

  var tensorFilter = StreamAPI.createElement('tensor_filter');
  tensorFilter.setProperty('framework', 'python3');
  tensorFilter.setProperty('model', './ml/tvm_nnstreamer.py');
  tensorFilter.setProperty('input', inputShapeStr);
  tensorFilter.setProperty('inputtype', inputType);
  tensorFilter.setProperty('output', outputShapeStr);
  tensorFilter.setProperty('outputtype', outputType);
  var custom =
    modelName +
    ' ' +
    inputShapeStr +
    ' ' +
    inputType +
    ' ' +
    outputShapeStr +
    ' ' +
    outputType;
  tensorFilter.setProperty('custom', custom);
  return tensorFilter;
};

ANTML.prototype.getMaxOfBuffer = function (buffer, type) {
  return native.ant_ml_getMaxOfBuffer(buffer, type);
};

ANTML.prototype.connectCompressionServer = function (ipAddress) {
  return new CompressionServer(ipAddress);
};

/**
 * Compression server object
 * @param {string} ipAddress compression server's IP address
 */
function CompressionServer(ipAddress) {
  this.ipAddress = ipAddress;
}

CompressionServer.prototype.downloadModel = function (modelId) {
  if (this.ipAddress === undefined) return undefined;
  // TODO: implement details
};
CompressionServer.prototype.downloadKernel = function (kernelId) {
  if (this.ipAddress === undefined) return undefined;
  // TODO: implement details
};

module.exports = new ANTML();
module.exports.ANTML = ANTML;
