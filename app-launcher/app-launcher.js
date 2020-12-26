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

function _getIPAddress(nameHead) {
  var networkInterfaces = os.networkInterfaces();
  for (var ni in networkInterfaces) {
    if (ni.startsWith(nameHead)) {
      return networkInterfaces[ni];
    }
  }
  return undefined;
}

// TODO: transform into HTTP request map

var _onHTTPRequest = function (request, response, data) {
  response.setHeader('Content-Type', 'text/html');
  var urlTokens = parseUrl(request.url);
  var results = {message: 'Not Found Entry', code: 404};

  if (urlTokens.length == 0 || urlTokens[0] == 'app-editor') {
    // "/" or "/app-editor"
    if (request.method == 'GET') {
      // GET "/" or "/app-editor": Return app editor page
      results = onGetAppEditorPage(request, data);
    }
  } else if (urlTokens[0] == 'alive') {
    // "/alive"
    if (request.method == 'GET') {
      // GET "/alive": Alive message
      results = onAliveRequest(request, data);
    }
  } else if (urlTokens[0] == 'runtime') {
    // "/runtime*"
    if (urlTokens.length == 1) {
      // "/runtime": Not found
    } else if (urlTokens[1] == 'currentApp') {
      // "/runtime/currentApp*"
      if (urlTokens.length == 2) {
        // "/runtime/currentApp"
        if (request.method == 'GET') {
          // GET "/runtime/currentApp"
          results = onGetAppInfo(request, data);
        } else if (request.method == 'POST') {
          // POST "/runtime/currentApp"
          results = onInstallApp(request, data);
        } else if (request.method == 'DELETE') {
          // DELETE "/runtime/currentApp"
          results = onRemoveApp(request, data);
        }
      } else if (urlTokens[2] == 'command') {
        // "/runtime/currentApp/command"
        if (request.method == 'POST') {
          // POST "/runtime/currentApp/command"
          results = onAppCommand(request, data);
        }
      } else if (urlTokens[2] == 'code') {
        // "/runtime/currentApp/code"
        if (request.method == 'GET') {
          // GET "/runtime/currentApp/code"
          results = onGetAppCode(request, data, false);
        }
      } else if (urlTokens[2] == 'codeInHtml') {
        // "/runtime/currentApp/code"
        if (request.method == 'GET') {
          // GET "/runtime/currentApp/code"
          results = onGetAppCode(request, data, true);
        }
      } else if (urlTokens[2] == 'state') {
        // "/runtime/currentApp/state"
        if (request.method == 'GET') {
          // GET "/runtime/currentApp/state"
          results = onGetAppState(request, data);
        }
      } else if (urlTokens[2] == 'companionAddress') {
        // "/runtime/currentApp/companionAddress"
        if (request.method == 'POST') {
          // POST "/runtime/currentApp/companionAddress"
          results = onSetCompanionAddress(request, data);
        }
      } else if (urlTokens[2] == 'companion') {
        // "/runtime/currentApp/companion"
        if (request.method == 'POST') {
          // POST "/runtime/currentApp/companion"
          results = onReceiveMessageFromCompanion(request, data);
        }
      }
    }
  }

  response.setHeader('Content-Length', results.message.length);
  response.writeHead(results.code);
  response.write(results.message);
  response.end();
};

/**
 * Event handler invoked when an HTTP request comes from companion device
 * @param {object} request request object sent from the companion
 * @param {object} response response object that will be sent to the companion
 */
function onHTTPRequest(request, response) {
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
}

/**
 * App launcher's main loop
 */
function mainLoop() {
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
}

mainLoop();
