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
var fs = require('fs');

var ant = require('ant');

var RESULT_SUCCESS = 'Success';
var RESULT_FAILED = 'Failed';

var MAIN_LOOP_DIR_PATH = truncateFile(process.argv[1]);

/* App Main Config START */
var Config = function () {
  this.showHTTPRequestPath = false;
  this.showHTTPRequestData = false;
  this.defaultInterfaceName = 'eth0';
  this.defaultPort = 8001;
};
var gConfig = new Config();
/* App Main Config END */

/**
 * Handler object that indicates application's code file.
 * The code files are managed by circular queue.
 */
function CodeFileHandler() {
  this.filename = undefined;
  this.fileindex = 0;
  var MAX_FILE_INDEX = 2;
  this.getName = function () {
    return this.filename;
  };
  this.setNew = function () {
    this.fileindex = (this.fileindex + 1) % MAX_FILE_INDEX;
    this.filename = './app' + this.fileindex + '.js';
  };
  this.initialize = function () {
    var isFileExists = false;
    for (var i = 0; i < MAX_FILE_INDEX; i++) {
      this.setNew();
      if (fs.existsSync(this.getName())) {
        isFileExists = true;
        break;
      }
    }
    if (!isFileExists) {
      this.filename = undefined;
    }
  };
  this.initialize();
}
var gCodeFileHandler = new CodeFileHandler();
/* App File Name END */

/**
 * Application code manager. It installs or removes application's code.
 */
function AppCodeManager() {
  this.current_app_object = undefined;
  this.load = function (appFileName) {
    this.current_app_object = require(appFileName);
  };
  this.install = function (appCodeBuffer) {
    // Write app code
    gCodeFileHandler.setNew();
    var fd = fs.openSync(gCodeFileHandler.getName(), 'w');
    fs.writeSync(fd, appCodeBuffer, 0, appCodeBuffer.length);
    fs.closeSync(fd);

    // Load app code
    gAppCodeManager.load(gCodeFileHandler.getName());

    if (ant.runtime.getCurrentApp() !== undefined) {
      return true;
    } else {
      // IF failed, Remove app code
      this.remove(gCodeFileHandler.getName());
      return false;
    }
  };
  this.remove = function (appFileName) {
    // Remove app code
    ant.runtime._removeCurrentApp();
    if (fs.existsSync(appFileName)) {
      fs.unlinkSync(appFileName);
    }
    return true;
  };
}
var gAppCodeManager = new AppCodeManager();
/* App Code Manager END */

var truncateFile = function (path) {
  var tokens = path.split('/');
  var truncatedPath = '';
  for (var i = 0; i < tokens.length - 1; i++) {
    truncatedPath += tokens[i] + '/';
  }
  return truncatedPath;
};

var parseUrl = function (url) {
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

/**
 * Event handler invoked when an alive request comes from the companion
 * @param {object} request request object sent from the companion
 * @param {string} data data sent from the companion
 * @return {object} alive message
 */
function onAliveRequest(request, data) {
  var results = {message: 'Alive', code: 200};
  return results;
}

/**
 * Event handler invoked when an application command comes from the companion
 * @param {object} request request object sent from the companion
 * @param {string} data data sent from the companion
 * @return {object} result message
 */
function onAppCommand(request, data) {
  var results = {message: 'Invalid command', code: 500};
  var command = data.toString();
  if (command == 'start') {
    results = onStartApp(request, data);
  } else if (command == 'stop') {
    results = onStopApp(request, data);
  }
  return results;
}

/**
 * Event handler invoked when an install app command comes from the companion
 * @param {object} request request object sent from the companion
 * @param {string} data data sent from the companion
 * @return {object} result message
 */
function onInstallApp(request, data) {
  var results = {message: RESULT_FAILED, code: 500};

  if (ant.runtime.getCurrentApp() === undefined) {
    var appCodeBuffer = data;
    var isSuccess = gAppCodeManager.install(appCodeBuffer);
    if (isSuccess) {
      results.message = RESULT_SUCCESS;
      results.code = 200;
    }
  }
  return results;
}

/**
 * Event handler invoked when a remove app command comes from the companion
 * @param {object} request request object sent from the companion
 * @param {string} data data sent from the companion
 * @return {object} result message
 */
function onRemoveApp(request, data) {
  var results = {message: RESULT_FAILED, code: 500};

  if (ant.runtime.getCurrentApp() !== undefined) {
    var isSuccess = gAppCodeManager.remove(gCodeFileHandler.getName());
    if (isSuccess) {
      results.message = RESULT_SUCCESS;
      results.code = 200;
    }
  }
  return results;
}

/**
 * Event handler invoked when a start app command comes from the companion
 * @param {object} request request object sent from the companion
 * @param {string} data data sent from the companion
 * @return {object} result message
 */
function onStartApp(request, data) {
  var results = {message: RESULT_FAILED, code: 500};
  var app = ant.runtime.getCurrentApp();
  if (app != undefined) {
    results.message = app.start();
    if (results.message == RESULT_SUCCESS) {
      results.code = 200;
    }
  }
  return results;
}

/**
 * Event handler invoked when a stop app command comes from the companion
 * @param {object} request request object sent from the companion
 * @param {string} data data sent from the companion
 * @return {object} result message
 */
function onStopApp(request, data) {
  var results = {message: RESULT_FAILED, code: 500};
  var app = ant.runtime.getCurrentApp();
  if (app != undefined) {
    results.message = app.stop();
    if (results.message == RESULT_SUCCESS) {
      results.code = 200;
    }
  }
  return results;
}

/**
 * Event handler invoked when a get app info command comes from the companion
 * @param {object} request request object sent from the companion
 * @param {string} data data sent from the companion
 * @return {object} result message
 */
function onGetAppInfo(request, data) {
  var currentApp = ant.runtime.getCurrentApp();
  var results = {message: 'No App Found', code: 500};

  if (currentApp != undefined) {
    var appInfo = currentApp.getInfo();
    results.message = JSON.stringify(appInfo);
    results.code = 200;
  }
  return results;
}

/**
 * Event handler invoked when a get app code request comes from the companion
 * @param {object} request request object sent from the companion
 * @param {string} data data sent from the companion
 * @param {boolean} isInHtml set true if the companion wants html linebreaking
 * @return {object} result message
 */
function onGetAppCode(request, data, isInHtml) {
  var results = {message: 'No App Code Found', code: 500};

  if (fs.existsSync(gCodeFileHandler.getName())) {
    var appCode = fs.readFileSync(gCodeFileHandler.getName()).toString();
    if (isInHtml) {
      appCode.replace(/\n/gi, '<br />');
    }
    results.message = appCode.toString();
    results.code = 200;
  }

  return results;
}

/**
 * Event handler invoked when a get app state request comes from the companion
 * @param {object} request request object sent from the companion
 * @param {string} data data sent from the companion
 * @return {object} result message
 */
function onGetAppState(request, data) {
  var currentApp = ant.runtime.getCurrentApp();
  var results = {message: 'No App Found', code: 500};

  if (currentApp != undefined) {
    results.message = currentApp.getState();
    results.code = 200;
  }
  return results;
}

/**
 * Event handler invoked when a get app editor page request comes from
 * the companion
 * @param {object} request request object sent from the companion
 * @param {string} data data sent from the companion
 * @return {object} result message
 */
function onGetAppEditorPage(request, data) {
  var results = {message: RESULT_FAILED, code: 500};

  var urlTokens = parseUrl(request.url);
  urlTokens.splice(0, 1);
  var path = MAIN_LOOP_DIR_PATH + 'app-editor';
  if (urlTokens.length == 0) {
    path += '/index.html';

    if (fs.existsSync(path)) {
      var originalIndexHtml = fs.readFileSync(path).toString();
      var jQueryTagEndIdx =
        originalIndexHtml.indexOf('</script>') + '</script>'.length;
      var ipAddress = ant.companion.getMyIPAddress(
        gConfig.defaultInterfaceName
      );
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

    if (fs.existsSync(path)) {
      var originalIndexHtml = fs.readFileSync(path);
      results.message = originalIndexHtml;
      results.code = 200;
    }
  }

  return results;
}

/**
 * Event handler invoked when an set companion address command comes from
 * the companion
 * @param {object} request request object sent from the companion
 * @param {string} data data sent from the companion
 * @return {object} result message
 */
function onSetCompanionAddress(request, data) {
  var results = {message: RESULT_FAILED, code: 500};
  // TODO: hardcoded port, path
  var companionHost = data.toString();
  var companionPort = 8002;
  var companionPath = '/companion';
  var result = ant.companion._setCompanionAddress(
    companionHost,
    companionPort,
    companionPath
  );
  if (result == true) {
    results.message = RESULT_SUCCESS;
    results.code = 200;
  }
  return results;
}

/**
 * Event handler invoked when a custom message comes from the companion
 * @param {object} request request object sent from the companion
 * @param {string} data data sent from the companion
 * @return {object} result message
 */
function onReceiveMessageFromCompanion(request, data) {
  var results = {message: RESULT_FAILED, code: 500};
  if (data !== undefined && data !== null) {
    var message = data.toString();
    ant.companion._onReceiveMessageFromCompanion(message);
    results.message = RESULT_SUCCESS;
    results.code = 200;
  }
  return results;
}

var _onHTTPRequest = function (request, response, data) {
  response.setHeader('Content-Type', 'text/html');
  var urlTokens = parseUrl(request.url);
  var results = {message: 'Not Found Entry', code: 404};

  // console.log(urlTokens.toString());
  // TODO: move them to Resource API except app-editor
  if (urlTokens.length == 0) {
    // "/"
    if (request.method == 'GET') {
      // GET "/": Alive message
      results = onAliveRequest(request, data);
    }
  } else if (urlTokens[0] == 'app-editor') {
    // "/alive"
    if (request.method == 'GET') {
      // GET "/": Return app editor page
      results = onGetAppEditorPage(request, data);
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
 * Load existing app code
 * @return {boolean} it returns true if the loading is successful,
 * otherwise it returns false.
 */
function loadExistingAppCode() {
  if (gCodeFileHandler.getName() === undefined) {
    return false;
  }

  // Load app code
  console.log('Load existing app code');
  gAppCodeManager.load(gCodeFileHandler.getName());

  if (ant.runtime.getCurrentApp() === undefined) {
    return false;
  }

  return true;
}

/**
 * Application's main loop
 */
function mainLoop() {
  console.log('ANT app runtime start');

  var isAppExists = loadExistingAppCode();
  if (isAppExists) {
    console.log('Existing app loaded!');
  }

  var server = http.createServer();
  server.on('request', onHTTPRequest);

  server.listen(gConfig.defaultPort, function () {
    var ipAddress = ant.companion.getMyIPAddress(gConfig.defaultInterfaceName);
    console.log(
      'ANT core listening: http://' + ipAddress + ':' + gConfig.defaultPort
    );
  });
}

mainLoop();
