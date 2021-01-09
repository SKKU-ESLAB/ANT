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

/* UI config area start */
var initialCode = '// Input your code here!!!';

/* ANT Client */
var gANTClient = new ANTClient();

/* Contents */
var gDashboard;
var gCodeEditor;
var gConsole;

/* Main UI components */
var gAppSelectorLabel;
var gAppSelectorMenu;
var gNavMenu;

function onSelectApp(appName) {
  // TODO:
  console.log('on select app: ' + appName);
}

function onCreateApp() {
  // TODO:
  console.log('on create app');
}

function onInitialize() {
  gDashboard = new DashboardView();
  gCodeEditor = new CodeEditorView(initialCode);
  gConsole = undefined;

  gAppSelectorLabel = new AppSelectorLabel();
  gAppSelectorMenu = new AppSelectorMenu(onSelectApp, onCreateApp);
  gNavMenu = new NavMenu();

  initializeAppSelector();
  initializeNavMenu();
}

function initializeAppSelector() {
  gAppSelectorLabel.setText('No app selected');
}

function initializeNavMenu() {
  gNavMenu.addItem('navitem-dashboard', 'home', 'Dashboard', gDashboard);
  gNavMenu.addItem('navitem-codeeditor', 'inbox', 'Code Editor', gCodeEditor);
  gNavMenu.addItem('navitem-console', 'wysiwyg', 'Console', gConsole);

  gNavMenu.updateForItem(gNavMenu.getItem(0));
}

$(document).ready(onInitialize);
