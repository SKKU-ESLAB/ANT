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

/* AppSelectorView */
function AppSelectorView(onClick) {
  View.apply(this, ['app-selector']);
  this.mLabel = new AppSelectorLabelView(onClick);
  this.mSpacer = new AppSelectorSpacerView(onClick);
}

AppSelectorView.prototype = Object.create(View.prototype);
AppSelectorView.prototype.constructor = AppSelectorView;

AppSelectorView.prototype.setLabelText = function (text) {
  this.mLabel.setText(text);
};

/* AppSelectorLabelView */
function AppSelectorLabelView(onClick) {
  View.apply(this, ['app-selector-label']);
  this.mRootDom.onclick = onClick;
}
AppSelectorLabelView.prototype = Object.create(View.prototype);
AppSelectorLabelView.prototype.constructor = AppSelectorLabelView;

AppSelectorLabelView.prototype.setText = function (text) {
  this.mRootDom.innerHTML = text;
};

/* AppSelectorSpacerView */
function AppSelectorSpacerView(onClick) {
  View.apply(this, ['app-selector-spacer']);
  this.mRootDom.onclick = onClick;
}
AppSelectorSpacerView.prototype = Object.create(View.prototype);
AppSelectorSpacerView.prototype.constructor = AppSelectorSpacerView;
