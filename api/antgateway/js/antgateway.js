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
var fs = require('fs');

var RuntimeAPI = undefined; 
var StreamAPI = undefined;
try {
  StreamAPI = require('antstream');
} catch (e) {
  throw new Error('ML API Dependency Error: not found Stream API');
}

try {
  RuntimeAPI = require('antruntime');
} catch (e) {
  throw new Error('ML API Dependency Error: not found Stream API');
}

/**
 * ANT Gateway API
 */
function ANTGateway() {}

ANTGateway.prototype.createImgClsImagenetElement = function (
    modelPath, numFragments, targetUri) {
  var mlFragmentElement = this.createMLFragmentElement(modelPath,
    [3, 224, 224, 1], 'uint8', 'input',
    'gateway_imgcls_imagenet', numFragments, targetUri);
  return mlFragmentElement;
};

ANTGateway.prototype.createMLFragmentElement = function (
  modelPath,
  inputShapes,
  inputTypes,
  inputNames,
  taskName,
  numFragments,
  targetUri
) {
  // Checking arguments
  if (modelPath.indexOf(' ') >= 0) {
    console.error('ERROR: Invalid modelPath! ' + modelPath);
    return undefined;
  }
  if (inputTypes.indexOf(' ') >= 0) {
    console.error('ERROR: Invalid inputTypes! ' + inputTypes);
    return undefined;
  }
  if (outputTypes.indexOf(' ') >= 0) {
    console.error('ERROR: Invalid outputTypes! ' + outputTypes);
    return undefined;
  }
  if (!StreamAPI.isInitialized()) {
    console.error('ERROR: Stream API is not initialized');
    return undefined;
  }
  if(taskName === undefined) {
    // Default task: imgcls_imagenet
    taskName = "imgcls_imagenet";
  }
  var taskPath = getTaskPath(taskName);
  if(taskPath === undefined) {
    console.error("ERROR: task does not exist: " + taskPath);
    return undefined;
  }

  var inputShapesStr = shapeArrayToStr(inputShapes);
  var outputShapesStr = shapeArrayToStr(outputShapes);
  var inputTypesStr = typeArrayToStr(inputTypes);
  var inputNamesStr = nameArrayToStr(inputNames);

  var tensorFilter = StreamAPI.createElement('tensor_filter');
  tensorFilter.setProperty('framework', 'python3');
  tensorFilter.setProperty('model', taskPath);
  tensorFilter.setProperty('input', inputShapesStr);
  tensorFilter.setProperty('inputtype', inputTypesStr);
  tensorFilter.setProperty('inputname', inputNamesStr);
  var custom =
    modelPath +
    ' ' +
    inputShapesStr +
    ' ' +
    inputTypesStr +
    ' ' +
    inputNamesStr +
    ' ' +
    numFragments +
    ' ' +
    targetUri;
  tensorFilter.setProperty('custom', custom);
  tensorFilter.modelPath = modelPath;
  return tensorFilter;
};

module.exports = new ANTGateway();
module.exports.ANTGateway = ANTGateway;
