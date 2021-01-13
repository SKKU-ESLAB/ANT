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

// TODO: (resource server) start app
// TODO: (resource server) stop app

var console = require('console');
var net = require('net');

var ant = require('ant');

var RESULT_SUCCESS = 'Success';
var RESULT_FAILED = 'Failed';

/**
 * Application's main loop
 */
var gAppObject = undefined;
var gSocket = undefined;
var gPid = undefined;
function mainLoop() {
  ant.runtime.disableStdoutBuffering();

  // parse arguments
  if (process.argv.length !== 5) {
    console.error('Invalid arguements: ' + JSON.stringify(process.argv));
    return;
  }
  var appName = process.argv[2];
  var appFilePath = process.argv[3];
  var launcherPort = parseInt(process.argv[4]);

  // Load app code
  this.gAppObject = require(appFilePath);

  // Launch app
  var app = ant.runtime.getCurrentApp();
  if (app != undefined) {
    resultMessage = app.start();
    if (resultMessage == RESULT_SUCCESS) {
      gPid = '' + process.pid;
      console.log('App ' + appName + ' launched! (pid: ' + pid + ')');
    } else {
      console.error('App launch failure');
    }
  }

  // Connect to app launcher
  gSocket = net.connect({port: launcherPort, host: 'localhost'});
  gSocket.on('data', onAppCommandReceived);

  // Notify that the application is successfully launched
  gSocket.write(pid);
}

function onAppCommandReceived(data) {
  var message = data.toString();
  if (message.indexOf('Terminate') >= 0) {
    // Terminate-app command
    console.log('Received app terminatation command.');
    var app = ant.runtime.getCurrentApp();
    app.stop();

    console.log('App terminated successfully!');
    gSocket.close();
  }
}

mainLoop();
