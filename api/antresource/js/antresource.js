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

var CompanionAPI = undefined;
try {
  CompanionAPI = require('antcompanion');
} catch (e) {
  throw new Error('Resource API Dependency Error: not found Companion API');
}

/**
 * ANT Resource API
 */
function ANTResource() {}
ANTResource.prototype._mIsInitialized = false;
ANTResource.prototype._mRequestId = 0;
ANTResource.prototype._mOnResourceResponseDict = {};

ANTResource.prototype._initialize = function () {
  var ANTResource = require('antresource');
  ANTResource._mIsInitialized = true;
  CompanionAPI.registerOnReceiveMessage(ANTResource._onReceiveRawMessage);
};
ANTResource.prototype._onReceiveRawMessage = function (rawMessage) {
  var ANTResource = require('antresource');
  var firstLineEnd = rawMessage.indexOf('\n');
  var firstLine = rawMessage.substring(0, firstLineEnd);
  if (firstLine != 'ResourceResponse') {
    return;
  }
  var secondLineEnd = rawMessage.indexOf('\n', firstLineEnd + 1);
  var secondLine = rawMessage.substring(firstLineEnd + 1, secondLineEnd);
  var thirdLineEnd = rawMessage.indexOf('\n', secondLineEnd + 1);
  var thirdLine = rawMessage.substring(secondLineEnd + 1, thirdLineEnd);
  var fourthLineEnd = rawMessage.indexOf('\n', thirdLineEnd + 1);
  var fourthLine = rawMessage.substring(thirdLineEnd + 1, fourthLineEnd);
  var otherLines = rawMessage.substring(fourthLineEnd + 1);

  var requestId = Number(secondLine);
  var method = thirdLine;
  var targetUri = fourthLine;
  var message = otherLines;

  var onResourceResponse = ANTResource._mOnResourceResponseDict[requestId];
  if (onResourceResponse !== undefined) {
    onResourceResponse(method, targetUri, message);
    delete ANTResource._mOnResourceResponseDict[requestId];
  }
};

// ResourceHandler arguments: (String method, String targetUri, String message)
ANTResource.prototype._sendRequest = function (
  method,
  targetUri,
  message,
  onResourceResponse
) {
  var ANTResource = require('antresource');
  if (!ANTResource._mIsInitialized) {
    ANTResource._initialize();
  }
  var rawMessage =
    '' +
    'ResourceRequest\n' +
    ANTResource._mRequestId +
    '\n' +
    method +
    '\n' +
    targetUri +
    '\n' +
    message;
  ANTResource._mRequestId++;

  CompanionAPI.sendMessage(rawMessage);
  if (onResourceResponse !== undefined) {
    ANTResource._mOnResourceResponseDict[
      ANTResource._mRequestId
    ] = onResourceResponse;
  }
};

ANTResource.prototype.requestGet = function (
  targetUri,
  message,
  onResourceResponse
) {
  var ANTResource = require('antresource');
  ANTResource._sendRequest('GET', targetUri, message, onResourceResponse);
};
ANTResource.prototype.requestPost = function (
  targetUri,
  message,
  onResourceResponse
) {
  var ANTResource = require('antresource');
  ANTResource._sendRequest('POST', targetUri, message, onResourceResponse);
};
ANTResource.prototype.requestPut = function (
  targetUri,
  message,
  onResourceResponse
) {
  var ANTResource = require('antresource');
  ANTResource._sendRequest('PUT', targetUri, message, onResourceResponse);
};
ANTResource.prototype.requestDelete = function (
  targetUri,
  message,
  onResourceResponse
) {
  var ANTResource = require('antresource');
  ANTResource._sendRequest('DELETE', targetUri, message, onResourceResponse);
};

module.exports = new ANTResource();
module.exports.ANTResource = ANTResource;
