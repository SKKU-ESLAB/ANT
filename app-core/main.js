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
function mainLoop() {
  console.log('ANT app main loop start');

  // parse arguments
  if (process.argv.length !== 4) {
    console.error('Invalid arguements: ' + JSON.stringify(process.argv));
    return;
  }
  var appName = process.argv[2];
  var launcherPort = parseInt(process.argv[3]);

  // Load app code
  this.gAppObject = require(appFileName);

  // Launch app
  var app = ant.runtime.getCurrentApp();
  if (app != undefined) {
    resultMessage = app.start();
    if (resultMessage == RESULT_SUCCESS) {
      console.log('App ' + appName + ' launched!');
    } else {
      console.error('App launch failure');
    }
  }

  // Connect to app launcher
  gSocket = net.connect(launcherPort, 'localhost', function () {
    // Notify that the application is successfully launched
    var pid = '' + process.pid;
    gSocket.write(pid);
  });
  gSocket.on('data', function (data) {
    if (data.indexOf('Terminate') >= 0) {
      // Terminate app
      var app = ant.runtime.getCurrentApp();
      var text = app.stop();
      console.log('App terminate result: ' + text);
      gSocket.close();
    }
  });
}

mainLoop();
