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

function ConsoleView(onRefresh, updatePeriodMS = 1000) {
  this.mOnRefresh = onRefresh;
  this.mCachedConsoleData = [];
  this.mCurrentAppName = undefined;
  this.mPeriodicUpdate = undefined;
  this.mUpdatePeriodMS = updatePeriodMS;

  this.mRootDom = document.createElement('div');
  this.mRootDom.setAttribute('class', 'console-text');
  // {
  //   this.mInfoCard = new CardView(
  //     'console-view-info-card',
  //     'Console output',
  //     12
  //   );
  //   this.mInfoCard.enableListView();
  //   this.append(this.mInfoCard);
  // }
}

ConsoleView.prototype.append = function (childView) {
  if (typeof childView === 'object' && childView.getDom !== undefined) {
    this.mRootDom.append(childView.getDom());
  } else {
    this.mRootDom.append(childView);
  }
};

ConsoleView.prototype.getDom = function () {
  return this.mRootDom;
};

ConsoleView.prototype.onAddedDom = function () {
  var self = this;
  var isValid = this.mOnRefresh();
  if (isValid) {
    this.mPeriodicUpdate = setInterval(function () {
      isValid = self.mOnRefresh();
    }, this.mUpdatePeriodMS);
  }
};

ConsoleView.prototype.onRemovedDom = function () {
  clearInterval(this.mPeriodicUpdate);
};

ConsoleView.prototype.updateOutputs = function (appName, outputBuffers) {
  // Get the cached console data or create a console data for the app
  var consoleData = this.mCachedConsoleData[appName];
  if (consoleData === undefined) {
    consoleData = {appName: appName, maxTs: -1, textLines: []};
    this.mCachedConsoleData[appName] = consoleData;
  }

  // Filter newly-added output buffers
  var maxTs = consoleData.maxTs;
  var newOutputBuffers = [];
  for (var i in outputBuffers) {
    var outputBuffer = outputBuffers[i];
    if (maxTs < outputBuffer.ts) {
      maxTs = outputBuffer.ts;
      newOutputBuffers.push(outputBuffer);
    }
  }
  outputBuffer = newOutputBuffers;

  // Set max timestamp of the app
  consoleData.maxTs = maxTs > 0 ? maxTs : -1;

  // Update text lines based on the output buffers
  var commands = [];
  for (var i in outputBuffers) {
    var outputBuffer = outputBuffers[i];
    var tokens = outputBuffer.d.split('\n');
    for (var j in tokens) {
      var token = tokens[j];
      var command;
      if (j == 0) {
        // append text
        commands.push({cmdType: 'A', dataType: outputBuffer.t, data: token});
      } else {
        // append text + newline
        commands.push({cmdType: 'N'});
        commands.push({cmdType: 'A', dataType: outputBuffer.t, data: token});
      }
    }
  }
  for (var i in commands) {
    var command = commands[i];
    if (command.cmdType == 'N') {
      var textLine = document.createElement('div');
      textLine.isDisplayed = false;

      consoleData.textLines.push(textLine);
    } else if (command.cmdType == 'A') {
      var textLine = consoleData.textLines.pop();
      if (textLine === undefined) {
        textLine = document.createElement('div');
        textLine.setAttribute('class', 'console-text-line');
        textLine.isDisplayed = false;
      }

      var textSpan = document.createElement('span');
      if (command.dataType == 'E')
        textSpan.setAttribute('class', 'console-text-span stderr');
      else textSpan.setAttribute('class', 'console-text-span stdout');
      textSpan.innerHTML = command.data;
      textLine.append(textSpan);

      consoleData.textLines.push(textLine);
    }
  }

  // Update UI
  if (this.mCurrentAppName != appName) {
    // Update current app name
    this.mCurrentAppName = appName;

    // Clear all the text spans
    this.removeTextLines();
  }
  for (var i in consoleData.textLines) {
    var textLine = consoleData.textLines[i];
    if (!textLine.isDisplayed) {
      textLine.isDisplayed = true;
      this.append(textLine);
    }
  }
};

ConsoleView.prototype.removeTextLines = function () {
  var childViews = this.mRootDom.children;
  for (var i = 0; i < childViews.length; i++) {
    var childView = childViews.item(i);
    if (childView.classList.contains('console-text-line')) {
      childView.isDisplayed = false;
      childView.remove();
    }
  }
};
