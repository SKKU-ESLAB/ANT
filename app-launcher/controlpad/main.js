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

/*ignore jslint start*/
var gANTClient = undefined;
var gUIController = undefined;
var gAppController = undefined;

var gContext = {
  currentAppName: undefined,
  currentNavItem: undefined,
  currentConsoleAppName: undefined,
  consoleTsPointers: []
};

function onReadyControlpad() {
  gANTClient = new ANTClient();
  gAppController = new AppController(gContext, gANTClient);
  gUIController = new UIController(gContext, gANTClient);
  gUIController.initialize();
}

$(document).ready(function () {
  setTimeout(onReadyControlpad, 100);
});
/*ignore jslint end*/
