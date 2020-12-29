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

var RESULT_SUCCESS = 'Success';
var RESULT_FAILED = 'Failed';

/**
 * App launcher
 *
 * It manages application list to install, uninstall, launch, terminate
 * applications.
 */

// TODO: (HTTP server) app editor file hosting
// TODO: (HTTP server) get app list
// TODO: (HTTP server, resource client) launch app -> start app
// TODO: (HTTP server, resource client) stop app -> terminate app
// TODO: (Web socket server, pipe) app's output

/* App Launcher Config START */
var Config = function () {
  this.showHTTPRequestPath = true;
  this.showHTTPRequestData = false;
  this.defaultInterfaceName = 'eth0';
  this.defaultPort = 8001;
};
var gConfig = new Config();

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

var _getAntRootDir = function () {
  var antRootDir = process.env['ANT_ROOT'];
  if (antRootDir.length == 0) {
    return undefined;
  }
  return antRootDir;
};

/**
 * Event handler invoked when a get app editor page request comes from
 * the companion
 */
var onGetControlpadPage = function (request, data) {
  var results = {message: RESULT_FAILED, code: 404};

  var urlTokens = _parseUrl(request.url);
  // urlTokens.splice(0, 1);
  var path = _getAntRootDir() + '/controlpad';
  if (urlTokens.length == 0) {
    path += '/index.html';

    if (fs.existsSync(path)) {
      var originalIndexHtml = fs.readFileSync(path).toString();
      var jQueryTagEndIdx =
        originalIndexHtml.indexOf('</script>') + '</script>'.length;
      var ipAddress = _getIPAddress(gConfig.defaultInterfaceName);
      var ipAddressSetting =
        '\n<script>$(document).ready(function () {\n' +
        '$("#targetAddressInput").val("' +
        ipAddress +
        ':' +
        gConfig.defaultPort +
        '");\n});</script>\n';
      var indexHtml =
        originalIndexHtml.slice(0, jQueryTagEndIdx) +
        ipAddressSetting +
        originalIndexHtml.slice(jQueryTagEndIdx + 1);
      results.message = indexHtml;
      results.code = 200;
    }
  } else {
    for (var i = 0; i < urlTokens.length; i++) {
      path += '/' + urlTokens[i];
    }
    console.log('read: ' + path);
    if (fs.existsSync(path)) {
      var originalIndexHtml = fs.readFileSync(path);
      results.message = originalIndexHtml;
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
  {u: '/alive', m: 'GET', f: onAliveRequest}
];
// {u: '/runtime/currentApp', m: 'GET', f: onGetAppInfo},
// {u: '/runtime/currentApp', m: 'POST', f: onInstallApp},
// {u: '/runtime/currentApp', m: 'DELETE', f: onRemoveApp},
// {u: '/runtime/currentApp/command', m: 'POST', f: onAppCommand},
// {u: '/runtime/currentApp/code', m: 'GET', f: onGetAppCode},
// {u: '/runtime/currentApp/state', m: 'GET', f: onGetAppState},
// {u: '/runtime/companionAddress', m: 'POST', f: onSetCompanionAddress},
// {u: '/runtime/companion', m: 'POST', f: onReceiveMessageFromCompanion}

var _onHTTPRequest = function (request, response, data) {
  response.setHeader('Content-Type', 'text/html');
  var responseParams = {message: 'Entry not found', code: 404};
  var entryFound = false;
  for (var i in gHttpEntries) {
    var entry = gHttpEntries[i];
    var urlRegExp = new RegExp(entry.u);

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
var mainLoop = function () {
  console.log('ANT app launcher daemon start');

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
