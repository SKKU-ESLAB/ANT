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

var os = require('os');

function Util() {}

Util.prototype.getAntRootDir = function () {
  var antRootDir = process.env['ANT_ROOT'];
  if (antRootDir.length == 0) {
    return undefined;
  }
  return antRootDir;
};

Util.prototype.parseUrl = function (url) {
  var urlTokens = url.split('/');
  var uniqueTokens = [];
  for (var i = 0; i < urlTokens.length; i++) {
    var token = urlTokens[i];
    if (token.length > 0) {
      uniqueTokens.push(token);
    }
  }
  return uniqueTokens;
};

Util.prototype.getIPAddress = function (nameHead) {
  var networkInterfaces = os.networkInterfaces();
  for (var ni in networkInterfaces) {
    if (ni.startsWith(nameHead)) {
      return networkInterfaces[ni][0].address;
    }
  }
  return undefined;
};

module.exports = Util;
