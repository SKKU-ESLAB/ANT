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
var initialCode =
  'var ant = require("ant"); \n\
var console = require("console"); \n\
\n\
var onInitialize = function () {\n\
  console.log("onInitialize");\n\
};\n\
\n\
var onStart = function () {\n\
  console.log("onStart");\n\
};\n\
\n\
var onStop = function () {\n\
  console.log("onStop");\n\
};\n\
\n\
ant.runtime.setCurrentApp(onInitialize, onStart, onStop);\n';
/* UI config area END */

/* Sub-controllers */
var gContext = {
  currentAppName: undefined,
  currentContentId: undefined
};
var gANTClient = new ANTClient();

/* Contents */
var gDashboard;
var gCodeEditor;
var gConsole;

/* Main UI components */
var gAppSelectorLabelView;
var gAppSelectorMenuView;
var gNavMenuView;
var gCreateAppDialogView;
var gCreateAppFabButtonView;

/* Control handlers */
function refreshAppList() {
  gANTClient.getAppList(function (isSuccess, appList) {
    if (isSuccess) {
      for (var i in appList) {
        var app = appList[i];
        _addAppFromUI(app.name);
      }

      if (appList.length > 0) {
        selectApp(appList[0].name);
      }
    } else {
      // TODO: toast
      console.warn('Get app list failed');
    }
  });

  // TODO: apply deleted app entry that is removed by another controlpad
}

function selectApp(appName) {
  console.log('on select app: ' + appName);
  gContext.currentAppName = appName;

  // Update app selector label
  gAppSelectorLabelView.setText(appName);

  // Update code editor when code editor is opened
  refreshCodeEditor();

  // TODO: Update console when console is opened
}

function tryToCreateApp() {
  // Show create app dialog
  gCreateAppDialogView.show(createApp);
}

function createApp(appName) {
  // Send "create app request"
  gANTClient.installApp(appName, initialCode, function (isSuccess, text) {
    if (isSuccess) {
      _addAppFromUI(appName);

      // Select this app
      selectApp(appName);
    } else {
      alert(text);
    }
  });
}

function _addAppFromUI(appName) {
  // Update app selector menu
  gAppSelectorMenuView.addApp(appName);

  // TODO: Update app list card when dashboard is opened
}

function refreshCodeEditor() {
  if (gContext.currentContentId === 'navitem-codeeditor') {
    var appName = gContext.currentAppName;
    if (appName !== undefined) {
      gANTClient.getAppCode(appName, function (isSuccess, appCode) {
        if (isSuccess) {
          gCodeEditor.setAppCode(appCode);
        } else {
          console.warn('Cannot get the code of app ' + appName);
        }
      });
    } else {
      alert('App has not been selected!');
      gNavMenuView.selectItem(gNavMenuView.getItem(0));
    }
  }
}

function updateCurrentAppCode(appCode) {
  // Send "create app request"
  var appName = gContext.currentAppName;
  gANTClient.installApp(appName, appCode, function (isSuccess, text) {
    if (isSuccess) {
      // TODO: toast UI
      alert(text);
    } else {
      alert(text);
    }
  });
}

function launchCurrentApp() {
  // Send "launch app request"
  var appName = gContext.currentAppName;
  gANTClient.launchApp(appName, function (isSuccess, text) {
    if (isSuccess) {
      // TODO: toast UI
      alert(text);

      if (gContext.currentContentId === 'navitem-codeeditor') {
        gCodeEditor.setRunButtonMode(false);
      }
    } else {
      alert(text);
    }
  });
}

function terminateCurrentApp() {
  // Send "terminate app request"
  var appName = gContext.currentAppName;
  gANTClient.terminateApp(appName, function (isSuccess, text) {
    if (isSuccess) {
      _removeAppFromUI(appName);

      // TODO: toast UI
      alert(text);

      if (gContext.currentContentId === 'navitem-codeeditor') {
        gCodeEditor.setRunButtonMode(true);
      }
    } else {
      alert(text);
    }
  });
}

function removeCurrentApp() {
  // Send "remove app request"
  var appName = gContext.currentAppName;
  gANTClient.removeApp(appName, function (isSuccess, text) {
    if (isSuccess) {
      // TODO: toast UI
      alert(text);
    } else {
      alert(text);
    }
  });
}

function _removeAppFromUI(appName) {
  // Update app selector menu
  gAppSelectorMenuView.removeApp(appName);

  // TODO: Update app list card when dashboard is opened
}

/* Initializers */
function onInitialize() {
  gDashboard = new DashboardView();
  gCodeEditor = new CodeEditorView(
    refreshCodeEditor,
    updateCurrentAppCode,
    launchCurrentApp,
    terminateCurrentApp,
    removeCurrentApp
  );
  gConsole = undefined;

  gAppSelectorLabelView = new AppSelectorLabelView();
  gAppSelectorMenuView = new AppSelectorMenuView(selectApp, tryToCreateApp);
  gNavMenuView = new NavMenuView(gContext);
  gCreateAppDialogView = new CreateAppDialogView();
  gCreateAppFabButtonView = new CreateAppFabButtonView();

  initializeAppSelectorView();
  initializeNavMenuView();
  initializeCreateAppFabButtonView();

  refreshAppList();
}

function initializeAppSelectorView() {
  gAppSelectorLabelView.setText('No app selected');
}

function initializeNavMenuView() {
  gNavMenuView.addItem('navitem-dashboard', 'home', 'Dashboard', gDashboard);
  gNavMenuView.addItem(
    'navitem-codeeditor',
    'inbox',
    'Code Editor',
    gCodeEditor
  );
  gNavMenuView.addItem('navitem-console', 'wysiwyg', 'Console', gConsole);

  gNavMenuView.selectItem(gNavMenuView.getItem(0));
}

function initializeCreateAppFabButtonView() {
  gCreateAppFabButtonView.setOnClickHandler(tryToCreateApp);
}

$(document).ready(onInitialize);
