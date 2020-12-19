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
var MLAPI = undefined;
try {
  StreamAPI = require('antstream');
} catch (e) {
  throw new Error('Gateway API Dependency Error: not found Stream API');
}

try {
  RuntimeAPI = require('antruntime');
} catch (e) {
  throw new Error('Gateway API Dependency Error: not found Runtime API');
}

try {
  MLAPI = require('antml');
} catch (e) {
  throw new Error('Gateway API Dependency Error: not found ML API');
}

/**
 * ANT Gateway API
 */
function ANTGateway() {}

ANTGateway.prototype.createImgClsImagenetElement = function (
    modelPath, numFragments, targetUri) {
  var mlFragmentElement = MLAPI.createMLFragmentElement(modelPath,
    [3, 224, 224, 1], 'uint8', 'input',
    'gateway_imgcls_imagenet', numFragments, targetUri);
  return mlFragmentElement;
};

module.exports = new ANTGateway();
module.exports.ANTGateway = ANTGateway;
