/* Copyright (c) 2017-2021 SKKU ESLAB, and contributors. All rights reserved.
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

// Gateway Virtual Sensor API: gateway manager

var ant = require('ant');
// var console = require('console');
var gateway = ant.gateway;

// VSA: virtual sensor adapter
var gVSA = undefined;

/* ANT lifecycle handlers */
function onInitialize() {
  // Empty function
}

function onStart() {
  gVSA = gateway.getVSAdapter();
  gVSA.createGWManager(); // gateway manager
  gVSA.start();
}

function onStop() {
  gVSA.stop();
}

ant.runtime.setCurrentApp(onInitialize, onStart, onStop);
