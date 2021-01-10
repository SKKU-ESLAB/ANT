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
  var _addTargetAddressScript = function (originalCode) {
    var ipAddress = Util.getIPAddress(gConfig.defaultInterfaceName);
    var portNum = gConfig.defaultPort;
    var ipAddressSetting =
      'ANTClient.prototype.getTargetUri = function() {\n  return "' +
      ipAddress +
      ':' +
      portNum +
      '";\n' +
      '};';
    var html = originalCode + '\n' + ipAddressSetting;
    return html;
  };

  var results = {message: 'Not found ' + request.url, code: 404};

  var urlTokens = Util.parseUrl(request.url);
  var controlpadDir = path.join(
    Util.getAntRootDir(),
    'app-launcher',
    'controlpad'
  );
  if (urlTokens.length == 0) {
    urlTokens.push('index.html');
  }

  var filePath = controlpadDir;
  for (var i = 0; i < urlTokens.length; i++) {
    filePath = path.join(filePath, urlTokens[i]);
  }

  if (fs.existsSync(filePath)) {
    var fileContents = fs.readFileSync(filePath);
    if (filePath.indexOf('ant-client.js') >= 0) {
      fileContents = _addTargetAddressScript(fileContents);
    }
    results.message = fileContents;
    results.code = 200;
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

function onInstallApp(request, data) {
  var results = {message: RESULT_FAILED, code: 500};
  try {
    var newlineIndex = data.indexOf('\n');
    if (newlineIndex < 0 || newlineIndex + 1 == data.length) {
      throw 'invalid message\n' + data;
    }
    var appName = data.substring(0, newlineIndex);
    var appCode = data.substring(newlineIndex + 1);

    // Install app
    gAppManager.installApp(appName, appCode);

    results.message = RESULT_SUCCESS;
    results.code = 200;
    return results;
  } catch (e) {
    results.message = 'Install app failure: ' + e;
    return results;
  }
}

function onRemoveApp(request, data) {
  var results = {message: RESULT_FAILED, code: 500};
  try {
    // Check arguments
    var urlTokens = Util.parseUrl(request.url);
    var appName = urlTokens[1];
    if (appName === undefined) {
      throw 'invalid URL ' + request.url;
    }

    // Remove app
    gAppManager.removeApp(appName);

    results.message = RESULT_SUCCESS;
    results.code = 200;
    return results;
  } catch (e) {
    results.message = 'Remove app failure: ' + e;
    return results;
  }
}

function onLaunchApp(request, data) {
  var results = {message: RESULT_FAILED, code: 500};
  try {
    // Check arguments
    var urlTokens = Util.parseUrl(request.url);
    var appName = urlTokens[1];
    if (appName === undefined) {
      throw 'invalid URL ' + request.url;
    }

    // Launch app
    gAppManager.launchApp(appName);

    results.message = RESULT_SUCCESS;
    results.code = 200;
    return results;
  } catch (e) {
    results.message = 'Launch app failure: ' + e;
    return results;
  }
}

function onTerminateApp(request, data) {
  var results = {message: RESULT_FAILED, code: 500};
  try {
    // Check arguments
    var urlTokens = Util.parseUrl(request.url);
    var appName = urlTokens[1];
    if (appName === undefined) {
      throw 'invalid URL ' + request.url;
    }

    // Terminate app
    gAppManager.terminateApp(appName);

    results.message = RESULT_SUCCESS;
    results.code = 200;
    return results;
  } catch (e) {
    results.message = 'Terminate app failure: ' + e;
    return results;
  }
}

function onTerminateAppInForce(request, data) {
  var results = {message: RESULT_FAILED, code: 500};
  try {
    // Check arguments
    var urlTokens = Util.parseUrl(request.url);
    var appName = urlTokens[1];
    if (appName === undefined) {
      throw 'invalid URL ' + request.url;
    }

    // Terminate app in force
    gAppManager.terminateAppInForce(appName);

    results.message = RESULT_SUCCESS;
    results.code = 200;
    return results;
  } catch (e) {
    results.message = 'Terminate app in force failure: ' + e;
    return results;
  }
}

function onGetAppCode(request, data) {
  var results = {message: RESULT_FAILED, code: 500};
  try {
    // Check arguments
    var urlTokens = Util.parseUrl(request.url);
    var appName = urlTokens[1];
    if (appName === undefined) {
      throw 'invalid URL ' + request.url;
    }

    // Get app code
    var app = gAppManager.getApp(appName);
    if (app === undefined) {
      throw 'not found app ' + appName;
    }
    var appCode = app.readCode();

    results.message = appCode;
    results.code = 200;
    return results;
  } catch (e) {
    results.message = 'Get app code failure: ' + e;
    return results;
  }
}

function onGetAppState(request, data) {
  var results = {message: RESULT_FAILED, code: 500};
  try {
    // Check arguments
    var urlTokens = Util.parseUrl(request.url);
    var appName = urlTokens[1];
    if (appName === undefined) {
      throw 'invalid URL ' + request.url;
    }

    // Get app state
    var app = gAppManager.getApp(appName);
    if (app === undefined) {
      throw 'not found app ' + appName;
    }
    var appState = app.getState();

    results.message = appState;
    results.code = 200;
    return results;
  } catch (e) {
    results.message = 'Get app state failure: ' + e;
    return results;
  }
}

function onGetAppStdout(request, data) {
  var results = {message: RESULT_FAILED, code: 500};
  try {
    // Check arguments
    var urlTokens = Util.parseUrl(request.url);
    var appName = urlTokens[1];
    if (appName === undefined) {
      throw 'invalid URL ' + request.url;
    }
    var tsFrom = parseInt(data);
    if (tsFrom == NaN) {
      throw 'invalid tsFrom ' + data;
    }

    // Get app stdouts
    var app = gAppManager.getApp(appName);
    if (app === undefined) {
      throw 'not found app ' + appName;
    }
    var stdouts = app.onGetStdouts(tsFrom);

    results.message = JSON.stringify(stdouts);
    results.code = 200;
    return results;
  } catch (e) {
    results.message = 'Get app state failure: ' + e;
    return results;
  }
}

function onGetAppStderr(request, data) {
  var results = {message: RESULT_FAILED, code: 500};
  try {
    // Check arguments
    var urlTokens = Util.parseUrl(request.url);
    var appName = urlTokens[1];
    if (appName === undefined) {
      throw 'invalid URL ' + request.url;
    }
    var tsFrom = parseInt(data);
    if (tsFrom == NaN) {
      throw 'invalid tsFrom ' + data;
    }

    // Get app stderrs
    var app = gAppManager.getApp(appName);
    if (app === undefined) {
      throw 'not found app ' + appName;
    }
    var stderrs = app.onGetStderrs(tsFrom);

    results.message = JSON.stringify(stderrs);
    results.code = 200;
    return results;
  } catch (e) {
    results.message = 'Get app state failure: ' + e;
    return results;
  }
}

var gInitialHTTPServerEntries = [
  {u: '/', m: 'GET', f: onGetControlpadPage},
  {u: '/app-editor', m: 'GET', f: onGetControlpadPage},
  {u: '/controlpad', m: 'GET', f: onGetControlpadPage},
  {u: '/alive', m: 'GET', f: onAliveRequest},
  {u: '/companionAddress', m: 'POST', f: onSetCompanionAddress},
  {u: '/apps', m: 'GET', f: onGetAppList},
  {u: '/apps', m: 'POST', f: onInstallApp}
];

function onAppAdded(app) {
  var rootUrl = '/apps/' + app.getname();
  gHTTPServer.addEntry(rootUrl, 'DELETE', onRemoveApp);
  gHTTPServer.addEntry(rootUrl + '/launch', 'POST', onLaunchApp);
  gHTTPServer.addEntry(rootUrl + '/terminate', 'POST', onTerminateApp);
  gHTTPServer.addEntry(
    rootUrl + '/terminateInForce',
    'POST',
    onTerminateAppInForce
  );
  gHTTPServer.addEntry(rootUrl + '/code', 'GET', onGetAppCode);
  gHTTPServer.addEntry(rootUrl + '/state', 'GET', onGetAppState);
  gHTTPServer.addEntry(rootUrl + '/stdout', 'GET', onGetAppStdout);
  gHTTPServer.addEntry(rootUrl + '/stderr', 'GET', onGetAppStderr);
}

function onAppRemoved(app) {
  var rootUrl = '/apps/' + app.getname();
  gHTTPServer.removeEntry(rootUrl, 'DELETE');
  gHTTPServer.removeEntry(rootUrl + '/launch', 'POST');
  gHTTPServer.removeEntry(rootUrl + '/terminate', 'POST');
  gHTTPServer.removeEntry(rootUrl + '/terminateInForce', 'POST');
  gHTTPServer.removeEntry(rootUrl + '/code', 'GET');
  gHTTPServer.removeEntry(rootUrl + '/state', 'GET');
  gHTTPServer.removeEntry(rootUrl + '/stdout', 'GET');
  gHTTPServer.removeEntry(rootUrl + '/stderr', 'GET');
}

/**
 * App launcher's main loop
 */
var gAppManager = undefined;
var gHTTPServer = undefined;
var gCompanionAdapter = undefined;

var mainLoop = function () {
  console.log('ANT app launcher daemon start');

  // Initialize app manager and HTTP server
  gHTTPServer = new HTTPServer(
    gInitialHTTPServerEntries,
    onGetControlpadPage,
    gConfig.isVerbose
  );
  gAppManager = new AppManager(onAppAdded, onAppRemoved);
  gCompanionAdapter = new CompanionAdapter();

  // run HTTP server
  gHTTPServer.run(gConfig.defaultPort, gConfig.defaultInterfaceName);
};

mainLoop();
