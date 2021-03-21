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

function AppManager(onAppAdded, onAppRemoved) {
  this.mApps = [];
  this.mOnAppAdded = onAppAdded;
  this.mOnAppRemoved = onAppRemoved;

  // Get file list
  var antAppsDir = path.join(Util.getAntRootDir(), 'apps');
  if (!fs.existsSync(antAppsDir)) {
    fs.mkdirSync(antAppsDir, {recursive: true});
  }
  var fileNames = fs.readdirSync(antAppsDir);

  // Add all the already-installed apps to the list
  for (var i = 0; i < fileNames.length; i++) {
    var fileName = fileNames[i];
    var filePath = path.join(antAppsDir, fileName);
    if (fileName.endsWith('.js')) {
      var appName = fileName.substring(0, fileName.indexOf('.js'));
      this._addApp(appName, filePath);
    }
  }
}

AppManager.prototype.getAppList = function () {
  var appList = [];
  for (var i = 0; i < this.mApps.length; i++) {
    var app = this.mApps[i];
    var appListEntry = {name: app.getName(), state: app.getState()};
    appList.push(appListEntry);
  }
  return appList;
};

AppManager.prototype.getApp = function (appName) {
  for (var i = 0; i < this.mApps.length; i++) {
    var app = this.mApps[i];
    if (app.getName() == appName) {
      return app;
    }
  }
  return undefined;
};

AppManager.prototype._addApp = function (appName, appFilePath) {
  // Add app metadata
  var app = new App(appName, appFilePath);
  this.mApps.push(app);

  // Call handler
  this.mOnAppAdded(app);
  return app;
};

AppManager.prototype.installApp = function (appName, appCode) {
  try {
    // Overwriting already-existing app is allowed.
    if (appName.endsWith('.js')) {
      appName = appName.substring(0, appName.indexOf('.js'));
    }
    var fileName = appName + '.js';

    var appFilePath = path.join(Util.getAntRootDir(), 'apps', fileName);
    var appFile = fs.openSync(appFilePath, 'w');
    fs.writeSync(appFile, appCode);
    fs.closeSync(appFile);

    var app = this.getApp(appName);
    if (app === undefined) {
      var app = this._addApp(appName, appFilePath);
    }
    return app;
  } catch (e) {
    console.error(e);
    throw JSON.stringify(e);
  }
};

AppManager.prototype.removeApp = function (appName) {
  // Find app
  var app = this.getApp(appName);
  if (app === undefined) {
    throw 'cannot find app ' + appName;
  }
  var index = this.mApps.indexOf(app);
  if (index < 0) {
    throw 'cannot find app ' + appName;
  }

  // Check app state
  var appState = app.getState();
  if (appState !== 'Inactive') {
    throw 'invalid app state ' + appState;
  }

  // Call handler
  this.mOnAppRemoved(app);

  // Remove app metadata
  this.mApps.splice(index, 1);

  // Remove app file
  try {
    var appFilePath = app.getFilePath();
    fs.unlinkSync(appFilePath);
  } catch (e) {
    throw 'errno ' + JSON.stringify(e);
  }
};

AppManager.prototype.launchApp = function (appName) {
  // Find app
  var app = this.getApp(appName);
  if (app === undefined) {
    throw 'cannot find app ' + appName;
  }

  // Check app state
  var appState = app.getState();
  if (appState !== 'Inactive') {
    throw 'invalid app state ' + appState;
  }

  // Launch app
  try {
    app.launch();
  } catch (e) {
    throw 'app.launch() error - ' + e;
  }
};

AppManager.prototype.terminateApp = function (appName) {
  // Find app
  var app = this.getApp(appName);
  if (app === undefined) {
    throw 'cannot find app ' + appName;
  }

  // Check app state
  var appState = app.getState();
  if (appState !== 'Active') {
    throw 'invalid app state ' + appState;
  }

  // Terminate app
  try {
    app.terminate();
  } catch (e) {
    throw 'app.terminate() error - ' + e;
  }
};

AppManager.prototype.terminateAppInForce = function (appName) {
  // Find app
  var app = this.getApp(appName);
  if (app === undefined) {
    throw 'cannot find app ' + appName;
  }

  // Terminate app in force
  try {
    app.terminateAppInForce();
  } catch (e) {
    throw 'app.terminate() error - ' + e;
  }
};

module.exports = AppManager;
