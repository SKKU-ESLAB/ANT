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
var App = require('./app.js');
var Util = require('./util.js');

function AppManager() {
  this.mApps = [];
}
AppManager.prototype.initialize = function () {
  // Get file list
  var antAppsDir = path.join(Util.getAntRootDir(), 'apps');
  if (!fs.existsSync(antAppsDir)) {
    fs.mkdirSync(antAppsDir, {recursive: true});
  }
  var fileNames = fs.readdirSync(antAppsDir);

  // Add all the already-installed apps to the list
  for (var i in fileNames) {
    var fileName = fileNames[i];
    var filePath = path.join(antAppsDir, fileName);
    if (fileName.endsWith('.js')) {
      var appName = fileName.substring(0, fileName.indexOf('.js'));
      this._addApp(appName, filePath);
    }
  }
  console.log('Apps: ' + JSON.stringify(this.mApps));
};

AppManager.prototype.getAppList = function () {
  var appList = [];
  for (var i in this.mApps) {
    var app = this.mApps[i];
    var appListEntry = {name: app.getName(), state: app.getState()};
    appList.push(appListEntry);
  }
  return appList;
};

AppManager.prototype.getApp = function (appName) {
  for (var i in this.mApps) {
    var app = this.mApps[i];
    if (app.getName() == appName) {
      return app;
    }
  }
  return undefined;
};

AppManager.prototype._addApp = function (appName, appFilePath) {
  var app = new App(appName, appFilePath);
  this.mApps.append(app);
  return app;
};

AppManager.prototype.installApp = function (appName, appCode) {
  try {
    if (appName.endsWith('.js')) {
      appName = appName.substring(0, appName.indexOf('.js'));
    }
    var fileName = appName + '.js';

    var appFilePath = path.join(Util.getAntRootDir(), 'apps', fileName);
    var appFile = fs.openSync(appFilePath, 'w');
    fs.writeSync(appFile, appCode);
    fs.closeSync(appFile);

    var app = this._addApp(appName, appFilePath);
    return app;
  } catch (e) {
    throw 'Install app failure: ' + JSON.stringify(e);
  }
};

AppManager.prototype.removeApp = function (appName) {
  // Find app
  var app = this.getApp(appName);
  if (app === undefined) {
    throw 'Remove app failure: cannot find app ' + appName;
  }
  var index = this.mApps.indexOf(app);
  if (index < 0) {
    throw 'Remove app failure: cannot find app ' + appName;
  }

  // Check app state
  var appState = app.getState();
  if (appState !== 'Inactive') {
    throw 'Remove app failure: invalid app state ' + appState;
  }

  // Remove app metadata
  this.mApps.splice(index);

  // Remove app file
  try {
    var appFilePath = app.getFilePath();
    fs.unlinkSync(appFilePath);
  } catch (e) {
    throw 'Remove app failure: errno ' + JSON.stringify(e);
  }
};

AppManager.prototype.launchApp = function (appName) {
  // Find app
  var app = this.getApp(appName);
  if (app === undefined) {
    throw 'Launch app failure: cannot find app ' + appName;
  }

  // Check app state
  var appState = app.getState();
  if (appState !== 'Inactive') {
    throw 'Launch app failure: invalid app state ' + appState;
  }

  // Launch app
  try {
    app.launch();
  } catch (e) {
    throw 'Launch app failure: app.launch() error - ' + e;
  }
};

AppManager.prototype.terminateApp = function (appName) {
  // Find app
  var app = this.getApp(appName);
  if (app === undefined) {
    throw 'Terminate app failure: cannot find app ' + appName;
  }

  // Check app state
  var appState = app.getState();
  if (appState !== 'Active') {
    throw 'Terminate app failure: invalid app state ' + appState;
  }

  // Launch app
  try {
    app.terminate();
  } catch (e) {
    throw 'Terminate app failure: app.terminate() error - ' + e;
  }
};

AppManager.prototype.terminateAppInForce = function (appName) {
  // Find app
  var app = this.getApp(appName);
  if (app === undefined) {
    throw 'Terminate app in force failure: cannot find app ' + appName;
  }

  // Launch app
  try {
    app.terminateAppInForce();
  } catch (e) {
    throw 'Terminate app in force failure: app.terminate() error - ' + e;
  }
};

AppManager.module.exports = AppManager;
