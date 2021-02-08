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

var http = require('http');
var Util = require('./util.js');

function HTTPServer(entries, onDefaultEntryHandler, isVerbose = false) {
  // Initialize properties
  this.mEntries = [];
  this.mOnDefaultEntryHandler = onDefaultEntryHandler;
  this.mServer = http.createServer();

  // Initial http server entries
  if (entries !== undefined) this.mEntries = entries;
  else this.mEntries = [];

  // Initialize HTTP server
  var self = this;
  this.mServer.on('request', function (request, response) {
    if (isVerbose) {
      console.log('Request for path: ' + request.url);
    }
    var parts = [];
    request.on('data', function (chunk) {
      parts.push(chunk);
    });
    request.on('end', function () {
      var body = Buffer.concat(parts).toString();
      self._onHTTPRequest(request, response, body);
    });
  });
}

HTTPServer.prototype._getContentType = function (url) {
  if (url.endsWith('.js')) {
    return 'text/javascript';
  } else if (url.endsWith('.css')) {
    return 'text/css';
  } else {
    return 'text/html';
  }
};

HTTPServer.prototype._onHTTPRequest = function (request, response, data) {
  response.setHeader('Content-Type', this._getContentType(request.url));
  var responseParams = {message: 'Entry not found', code: 404};
  var entryFound = false;
  for (var i in this.mEntries) {
    var entry = this.mEntries[i];

    if (entry.u == request.url && entry.m == request.method) {
      responseParams = entry.f(request, data);
      entryFound = true;
      break;
    }
  }
  if (!entryFound) {
    responseParams = this.mOnDefaultEntryHandler(request, data);
  }
  response.setHeader('Content-Length', responseParams.message.length);
  response.writeHead(responseParams.code);
  response.write(responseParams.message);
  response.end();
};

HTTPServer.prototype.run = function (portNum, interfaceName) {
  this.mServer.listen(portNum, function () {
    var ipAddress = Util.getIPAddress(interfaceName);
    console.log(
      'ANT app launcher listening: http://' + ipAddress + ':' + portNum
    );
  });
};

HTTPServer.prototype.addEntry = function (url, method, handlerFunc) {
  if (typeof url !== 'string') {
    throw 'Add entry failure: invalid url ' + url;
  } else if (typeof method !== 'string') {
    throw 'Add entry failure: invalid method ' + method;
  } else if (typeof handlerFunc !== 'function') {
    throw 'Add entry failure: invalid handler function' + handlerFunc;
  }

  var newEntry = {u: url, m: method, f: handlerFunc};
  this.mEntries.push(newEntry);
};

HTTPServer.prototype.removeEntry = function (url, method) {
  for (var i = this.mEntries.length - 1; i--; i >= 0) {
    var entry = this.mEntries[i];
    if (entry === undefined) continue;
    if (url !== undefined && url !== entry.u) continue;
    if (method !== undefined && method !== entry.m) continue;

    this.mEntries.splice(i, 1);
  }
};

module.exports = HTTPServer;
