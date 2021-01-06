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
var fs = require('fs');
var os = require('os');
var path = require('path');

var AppManager = require('./app-manager.js');
var Util = require('./util.js');

var RESULT_SUCCESS = 'Success';
var RESULT_FAILED = 'Failed';

/**
 * App launcher
 *
 * It manages application list to install, uninstall, launch, terminate
 * applications.
 */

/* Config START */
var Config = function () {
  this.showHTTPRequestPath = true;
  this.showHTTPRequestData = false;
  this.defaultInterfaceName = 'eth0';
  this.defaultPort = 8001;
  this.maxStdoutBufferLength = 100;
  this.maxStderrBufferLength = 100;
};
var gConfig = new Config();
/* Config END */

var _getIPAddress = function (nameHead) {
  var networkInterfaces = os.networkInterfaces();
  for (var ni in networkInterfaces) {
    if (ni.startsWith(nameHead)) {
      return networkInterfaces[ni][0].address;
    }
  }
  return undefined;
};

/**
 * Event handler invoked when an alive request comes from the companion
 */
var onAliveRequest = function (request, data) {
  var results = {message: 'Alive', code: 200};
  return results;
};

var _parseUrl = function (url) {
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

var _addTargetAddressScript = function (originalHtml) {
  var jQueryTagEndIdx = originalHtml.indexOf('</script>') + '</script>'.length;
  var ipAddress = _getIPAddress(gConfig.defaultInterfaceName);
  var portNum = gConfig.defaultPort;
  var ipAddressSetting =
    '\n<script>$(document).ready(function () {\n' +
    '$("#targetAddressInput").val("' +
    ipAddress +
    ':' +
    portNum +
    '");\n});</script>\n';
  var html =
    originalHtml.slice(0, jQueryTagEndIdx) +
    ipAddressSetting +
    originalHtml.slice(jQueryTagEndIdx + 1);
  return html;
};

/**
 * Event handler invoked when a get app editor page request comes from
 * the companion
 */
var onGetControlpadPage = function (request, data) {
  var results = {message: RESULT_FAILED, code: 404};

  var urlTokens = _parseUrl(request.url);
  var controlpadDir = path.join(Util.getAntRootDir(), 'controlpad');
  if (urlTokens.length == 0) {
    var filePath = path.join(controlpadDir, '/index.html');

    if (fs.existsSync(filePath)) {
      var originalHtml = fs.readFileSync(filePath).toString();
      results.message = _addTargetAddressScript(originalHtml);
      results.code = 200;
    }
  } else {
    var filePath = controlpadDir;
    for (var i = 0; i < urlTokens.length; i++) {
      filePath = path.join(filePath, urlTokens[i]);
    }
    console.log('read: ' + filePath);
    if (fs.existsSync(filePath)) {
      results.message = fs.readFileSync(filePath);
      results.code = 200;
    }
  }
  return results;
};

/**
 * HTTP request entries
 */
var gHttpEntries = [
  {u: '/', m: 'GET', f: onGetControlpadPage},
  {u: '/app-editor', m: 'GET', f: onGetControlpadPage},
  {u: '/controlpad', m: 'GET', f: onGetControlpadPage},
  {u: '/alive', m: 'GET', f: onAliveRequest},
  {u: '/companionAddress', m: 'POST', f: onSetCompanionAddress},
  {u: '/apps', m: 'GET', f: onGetAppList}
];

function HTTPServer() {
  this.mEntries = [];
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
  for (var i in gHttpEntries) {
    var entry = gHttpEntries[i];

    if (entry.u == request.url) {
      responseParams = entry.f(request, data);
      entryFound = true;
      break;
    }
  }
  if (!entryFound) {
    responseParams = onGetControlpadPage(request, data);
  }
  response.setHeader('Content-Length', responseParams.message.length);
  response.writeHead(responseParams.code);
  response.write(responseParams.message);
  response.end();
};

/**
 * Event handler invoked when an HTTP request comes from companion device
 * @param {object} request request object sent from the companion
 * @param {object} response response object that will be sent to the companion
 */
var onHTTPRequest = function (request, response) {
  if (gConfig.showHTTPRequestPath) {
    console.log('Request for path: ' + request.url);
  }

  var parts = [];
  request.on('data', function (chunk) {
    parts.push(chunk);
  });
  request.on('end', function () {
    var body = Buffer.concat(parts);
    if (gConfig.showHTTPRequestData) {
      console.log('Request data: ' + body.toString());
    }
    _onHTTPRequest(request, response, body);
  });
};

/**
 * App launcher's main loop
 */
var gAppManager = undefined;
var mainLoop = function () {
  console.log('ANT app launcher daemon start');

  // Initialize app manager
  gAppManager = new AppManager();

  // Initialize and run HTTP server
  var server = http.createServer();
  server.on('request', onHTTPRequest);
  server.listen(gConfig.defaultPort, function () {
    var ipAddress = _getIPAddress(gConfig.defaultInterfaceName);
    console.log(
      'ANT app launcher listening: http://' +
        ipAddress +
        ':' +
        gConfig.defaultPort
    );
  });
};

mainLoop();
