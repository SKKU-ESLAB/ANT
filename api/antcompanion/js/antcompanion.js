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
var http = require('http');

/**
 * ANT Companion API
 */
function ANTCompanion() {}

ANTCompanion.prototype._mCompanionHost = undefined;
ANTCompanion.prototype._mCompanionPort = undefined;
ANTCompanion.prototype._mCompanionPath = undefined;
ANTCompanion.prototype._mHandlers = [];

ANTCompanion.prototype.getCompanionHost = function () {
  return this._mCompanionHost;
};

ANTCompanion.prototype._setCompanionAddress = function (
  companionHost,
  companionPort,
  companionPath
) {
  this._mCompanionHost = companionHost;
  this._mCompanionPort = companionPort;
  this._mCompanionPath = companionPath;
  return true;
};

ANTCompanion.prototype._onReceiveMessageFromCompanion = function (message) {
  for (var i = 0; i < this._mHandlers.length; i++) {
    this._mHandlers[i](message);
  }
};

ANTCompanion.prototype.sendMessage = function (message) {
  if (this._mCompanionPath === undefined) {
    console.log('Error: failed to send message due to no companion address');
    return false;
  }

  var options = {
    method: 'POST',
    host: this._mCompanionHost,
    port: this._mCompanionPort,
    path: this._mCompanionPath,
    headers: {'Content-Length': message.length}
  };
  var clientRequest = http.request(options);
  clientRequest.write(message);
  clientRequest.end();
  return true;
};

ANTCompanion.prototype.registerOnReceiveMessage = function (handler) {
  this._mHandlers.push(handler);
};
ANTCompanion.prototype.unregisterOnReceiveMessage = function (handler) {
  if (handler === undefined) return false;
  for (var i; i < this._mHandlers.length; i++) {
    if (this._mHandlers[i] === handler) {
      this._mHandlers.splice(i, 1);
      return true;
    }
  }
  return false;
};

ANTCompanion.prototype.getMyIPAddress = function (interfaceName) {
  return native.ant_companion_getMyIPAddress(interfaceName);
};

module.exports = new ANTCompanion();
module.exports.ANTCompanion = ANTCompanion;
