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

var fs = require('fs');
var path = require('path');

var AppManager = require('./app-manager.js');
var CompanionAdapter = require('./companion-adapter.js');
var HTTPServer = require('./http-server.js');
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
  this.isVerbose = true;
  this.defaultInterfaceName = 'eth0';
  this.defaultPort = 8001;

  // TODO: hardcoded companion's port
  this.companionPort = 8002;
  this.companionPath = '/companion';
};
var gConfig = new Config();
/* Config END */

var onGetControlpadPage = function (request, data) {
  var _addTargetAddressScript = function (originalHtml) {
    var jQueryTagEndIdx =
      originalHtml.indexOf('</script>') + '</script>'.length;
    var ipAddress = Util.getIPAddress(gConfig.defaultInterfaceName);
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

  var results = {message: RESULT_FAILED, code: 404};

  var urlTokens = Util.parseUrl(request.url);
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

function onAliveRequest(request, data) {
  var results = {message: 'Alive', code: 200};
  return results;
}

function onSetCompanionAddress(request, data) {
  var results = {message: RESULT_FAILED, code: 500};
  var companionHost = data.toString();
  var companionPort = gConfig.companionPort;
  var companionPath = gConfig.companionPath;
  var result = gCompanionAdapter.setCompanionAddress(
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

function onGetAppList(request, data) {
  var results = {message: RESULT_FAILED, code: 500};
  try {
    var appList = gAppManager.getAppList();
    results.message = JSON.stringify(appList);
    results.code = 200;
    return results;
  } catch (e) {
    results.message = e;
    return results;
  }
}

function onLaunchApp(request, data) {
  var results = {message: RESULT_FAILED, code: 500};
  try {
    // Check arguments
    var urlTokens = Util.parseUrl(request.url);
    var appName = urlTokens[1];

    // Launch app
    gAppManager.launchApp(appName);

    results.message = RESULT_SUCCESS;
    results.code = 200;
    return results;
  } catch (e) {
    results.message = e;
    return results;
  }
}

function onTerminateApp(request, data) {
  var results = {message: RESULT_FAILED, code: 500};
  try {
    // Check arguments
    var urlTokens = Util.parseUrl(request.url);
    var appName = urlTokens[1];

    // Terminate app
    gAppManager.terminateApp(appName);

    results.message = RESULT_SUCCESS;
    results.code = 200;
    return results;
  } catch (e) {
    results.message = e;
    return results;
  }
}

function onTerminateAppInForce(request, data) {
  var results = {message: RESULT_FAILED, code: 500};
  try {
    // Check arguments
    var urlTokens = Util.parseUrl(request.url);
    var appName = urlTokens[1];

    // Terminate app in force
    gAppManager.terminateAppInForce(appName);

    results.message = RESULT_SUCCESS;
    results.code = 200;
    return results;
  } catch (e) {
    results.message = e;
    return results;
  }
}

var gInitialHTTPServerEntries = [
  {u: '/', m: 'GET', f: onGetControlpadPage},
  {u: '/app-editor', m: 'GET', f: onGetControlpadPage},
  {u: '/controlpad', m: 'GET', f: onGetControlpadPage},
  {u: '/alive', m: 'GET', f: onAliveRequest},
  {u: '/companionAddress', m: 'POST', f: onSetCompanionAddress},
  {u: '/apps', m: 'GET', f: onGetAppList}
];

/**
 * App launcher's main loop
 */
var gAppManager = undefined;
var gHTTPServer = undefined;
var gCompanionAdapter = undefined;

var mainLoop = function () {
  console.log('ANT app launcher daemon start');

  // Initialize app manager and HTTP server
  gHTTPServer = new HTTPServer(gInitialHTTPServerEntries, gConfig.isVerbose);
  gAppManager = new AppManager();
  gCompanionAdapter = new CompanionAdapter();

  // run HTTP server
  gHTTPServer.run(gConfig.defaultPort, gConfig.defaultInterfaceName);
};

mainLoop();
