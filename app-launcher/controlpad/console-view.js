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

function ConsoleView(onRefreshRequired, updatePeriodMS = 1000) {
  this.mOnRefreshRequired = onRefreshRequired;
  this.mCachedConsoleData = [];
  this.mCurrentAppName = undefined;
  this.mPeriodicUpdate = undefined;
  this.mUpdatePeriodMS = updatePeriodMS;

  this.mRootDom = document.createElement('div');

  {
    this.mInfoCard = new CardView(
      'console-view-info-card',
      'Console output',
      12
    );
    this.mInfoCard.enableListView();
    this.append(this.mInfoCard);
  }
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
  this.mOnRefreshRequired();
  this.mPeriodicUpdate = setInterval(function () {
    self.mOnRefreshRequired();
  }, this.mUpdatePeriodMS);
};

ConsoleView.prototype.onRemovedDom = function () {
  clearInterval(this.mPeriodicUpdate);
};

ConsoleView.prototype.updateOutputs = function (appName, givenOutputs) {
  // Get cached outputs
  var consoleData = this.mCachedConsoleData[appName];
  if (consoleData === undefined) {
    consoleData = {appName: appName, maxTs: -1, outputs: []};
    this.mCachedConsoleData[appName] = consoleData;
  }

  // Add given newly-added outputs to the consoleData
  var maxTs = consoleData.maxTs;
  var newOutputs = [];
  for (var i in givenOutputs) {
    var givenEntry = givenOutputs[i];
    if (maxTs < givenEntry.ts) {
      maxTs = givenEntry.ts;
      newOutputs.push(givenEntry);
    }
  }
  consoleData.maxTs = maxTs > 0 ? maxTs : undefined;
  for (var i in newOutputs) {
    var entry = newOutputs[i];
    consoleData.outputs.push(entry);
  }

  // Update UI
  if (this.mCurrentAppName == appName) {
    // Add only newly-added outputs
    for (var i in newOutputs) {
      var entry = newOutputs[i];
      this.addEntry(entry.t, entry.d);
    }
  } else {
    // Update current app name
    this.mCurrentAppName = appName;

    // Clear console view
    this.clear();

    // Add all the outputs
    for (var i in consoleData.outputs) {
      var entry = consoleData.outputs[i];
      var lines = entry.d.split('\n');
      for (var j in lines) {
        var line = lines[j];
        this.addEntry(entry.t, line);
      }
    }
  }
};

ConsoleView.prototype.addEntry = function (type, text) {
  var iconType = type === 'E' ? 'error' : 'arrow_right';
  this.mInfoCard.addListItem(iconType, text);
};

ConsoleView.prototype.clear = function () {
  this.mInfoCard.clearListItems();
};
