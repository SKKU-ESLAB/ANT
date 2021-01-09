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

/* UI config area START */
var initialCode = '// Input your code here!!!';
/* UI config area END */

/* Sub-controllers */
var gContext = {
  presentAppName: undefined
};
var gANTClient = new ANTClient();

/* Contents */
var gDashboard;
var gCodeEditor;
var gConsole;

/* Main UI components */
var gAppSelectorLabel;
var gAppSelectorMenu;
var gNavMenu;
var gCreateAppDialogView;

/* Control handlers */
function selectApp(appName) {
  console.log('on select app: ' + appName);
  gContext.presentAppName = appName;

  // Update app selector label
  gAppSelectorLabel.setText(appName);

  // TODO: Update code editor when code editor is opened
}

function tryToCreateApp() {
  console.log('on create app');

  // Show create app dialog
  gCreateAppDialogView.show(createApp);
}

function createApp(appName) {
  // Update app selector menu
  gAppSelectorMenu.addApp(appName);

  // TODO: Update app list card when dashboard is opened

  // Select this app
  selectApp(appName);
}

/* Initializers */
function onInitialize() {
  gDashboard = new DashboardView();
  gCodeEditor = new CodeEditorView(initialCode);
  gConsole = undefined;

  gAppSelectorLabel = new AppSelectorLabelView();
  gAppSelectorMenu = new AppSelectorMenuView(selectApp, tryToCreateApp);
  gNavMenu = new NavMenuView();
  gCreateAppDialogView = new CreateAppDialogView();

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
