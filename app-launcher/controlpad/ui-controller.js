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
  currentNavItem: undefined,
  currentConsoleAppName: undefined,
  consoleTsPointers: []
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
var gSnackbarView;

function showInfoMessage(text, useSnackbar = true) {
  if (useSnackbar) gSnackbarView.showShort(text);
  console.log(text);
}

function showErrorMessage(text, useSnackbar = true) {
  var errorMsg = 'Error: ' + text;
  if (useSnackbar) gSnackbarView.showLong(errorMsg);
  console.error(errorMsg);
}

/* Control handlers */
function refreshAppList(onFinish) {
  gANTClient.getAppList(function (isSuccess, appList) {
    if (isSuccess) {
      for (var i in appList) {
        var app = appList[i];
        _addAppFromUI(app.name);
      }
      onFinish(appList);
    } else {
      showErrorMessage('Get app list failed');
    }
  });
  // TODO: apply deleted app entry that is removed by another controlpad
}

function selectApp(appName) {
  gContext.currentAppName = appName;

  // Update app selector label
  var label = appName !== undefined ? appName : 'No app selected';
  gAppSelectorLabelView.setText(label);

  // Update code editor when code editor is opened
  refreshCodeEditor();

  // Update console when console is opened
  refreshConsole();
}

function tryToCreateApp() {
  // Show create app dialog
  gCreateAppDialogView.show(createApp);
}

function createApp(appName) {
  // Send "create app request"
  try {
    gANTClient.installApp(appName, initialCode, function (isSuccess, text) {
      if (isSuccess) {
        _addAppFromUI(appName);

        // Select this app
        selectApp(appName);
      } else {
        alert(text);
      }
    });
  } catch (e) {
    showErrorMessage(e);
  }
}

function _addAppFromUI(appName) {
  // Update app selector menu
  gAppSelectorMenuView.addApp(appName);

  // TODO: Update app list card when dashboard is opened
}

function refreshCodeEditor() {
  try {
    if (gContext.currentNavItem.getId() === 'navitem-codeeditor') {
      var appName = gContext.currentAppName;
      if (appName !== undefined) {
        gANTClient.getAppCode(appName, function (isSuccess, appCode) {
          if (isSuccess) {
            gCodeEditor.setAppCode(appCode);
          } else {
            showErrorMessage('Cannot get the code of app ' + appName);
          }
        });
      } else {
        gNavMenuView.selectItem(gNavMenuView.getItem(0));
      }
    }
  } catch (e) {
    showErrorMessage(e);
  }
}

function updateCurrentAppCode(appCode) {
  // Send "create app request"
  try {
    var appName = gContext.currentAppName;
    gANTClient.installApp(appName, appCode, function (isSuccess, text) {
      if (isSuccess) {
        showInfoMessage('App code loaded successfully');
      } else {
        showErrorMessage(text);
      }
    });
  } catch (e) {
    showErrorMessage(e);
  }
}

function launchCurrentApp() {
  // Send "launch app request"
  try {
    var appName = gContext.currentAppName;
    gANTClient.launchApp(appName, function (isSuccess, text) {
      if (isSuccess) {
        showInfoMessage('App ' + appName + ' launched successfully');

        if (gContext.currentNavItem.getId() === 'navitem-codeeditor') {
          gCodeEditor.setRunButtonMode(false);
        }
      } else {
        showErrorMessage(text);
      }
    });
  } catch (e) {
    showErrorMessage(e);
  }
}

function terminateCurrentApp() {
  // Send "terminate app request"
  try {
    var appName = gContext.currentAppName;
    gANTClient.terminateApp(appName, function (isSuccess, text) {
      if (isSuccess) {
        showInfoMessage('App ' + appName + ' terminated successfully');

        if (gContext.currentNavItem.getId() === 'navitem-codeeditor') {
          gCodeEditor.setRunButtonMode(true);
        }
      } else {
        showErrorMessage(text);
      }
    });
  } catch (e) {
    showErrorMessage(e);
  }
}

function removeCurrentApp() {
  // Send "remove app request"
  try {
    var appName = gContext.currentAppName;
    gANTClient.removeApp(appName, function (isSuccess, text) {
      if (isSuccess) {
        _removeAppFromUI(appName);

        showInfoMessage('App ' + appName + ' removed successfully');
      } else {
        showErrorMessage(text);
      }
    });
  } catch (e) {
    showErrorMessage(e);
  }
}

function _removeAppFromUI(appName) {
  // Update app selector menu (remove the app and select another one)
  var index = gAppSelectorLabelView.getIndex(appName);
  if (index < 0) {
    showErrorMessage('Invalid app index: ' + appName + ' / ' + index);
    return;
  }
  var appsCount = gAppSelectorLabelView.getAppsCount();
  var nextIndex = -1;
  if (appsCount > 1) {
    nextIndex = index > 0 ? index - 1 : 0;
  }
  gAppSelectorMenuView.removeApp(appName);
  selectApp(gAppSelectorMenuView.getAppNameAt(nextIndex));

  // TODO: Update app list card when dashboard is opened
}

function refreshConsole() {
  // Update console when console is opened
  if (gContext.currentNavItem.getId() === 'navitem-console') {
    var appName = gContext.currentAppName;

    var fromTs = gContext.consoleTsPointers[appName];
    if (fromTs === undefined) fromTs = -1;

    if (appName !== undefined) {
      // Send "get app output request"
      gANTClient.getOutput(appName, fromTs, function (isSuccess, outputs) {
        if (isSuccess) {
          // Update ts pointer of the app
          var maxTs = -1;
          for (var i in outputs) {
            var entry = outputs[i];
            if (maxTs < entry.ts) maxTs = entry.ts;
          }
          if (maxTs >= 0) {
            if (
              gContext.consoleTsPointers[appName] === undefined ||
              maxTs > gContext.consoleTsPointers[appName]
            ) {
              gContext.consoleTsPointers[appName] = maxTs + 1;
            }
          }

          // Update console
          gConsole.updateOutputs(appName, outputs);
        } else {
          showErrorMessage('Cannot get the outputs of app ' + appName, false);
        }
      });
    } else {
      gNavMenuView.selectItem(gNavMenuView.getItem(0));
    }
  }
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
  gConsole = new ConsoleView(refreshConsole);

  gAppSelectorLabelView = new AppSelectorLabelView();
  gAppSelectorMenuView = new AppSelectorMenuView(selectApp, tryToCreateApp);
  gNavMenuView = new NavMenuView(gContext);
  gCreateAppDialogView = new CreateAppDialogView();
  gCreateAppFabButtonView = new CreateAppFabButtonView();
  gSnackbarView = new SnackbarView();

  initializeNavMenuView();
  initializeCreateAppFabButtonView();

  refreshAppList(function (appList) {
    if (appList.length > 0) selectApp(appList[0].name);
    else selectApp(undefined);
  });
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
