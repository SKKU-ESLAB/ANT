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

function ButtonView(id, iconType, text) {
  // Setting attributes
  this.mId = id;
  this.mIconType = iconType;
  this.mText = text;

  this.mRootDom = document.createElement('button');
  this.mRootDom.setAttribute(
    'class',
    'mdl-button mdl-js-button mdl-button--raised mdl-js-ripple-effect mdl-button--accent'
  );
  this.mRootDom.setAttribute('style', 'margin-left:5px; margin-right:5px;');
  if (this.mIconType !== undefined) {
    this.mIcon = document.createElement('i');
    this.mIcon.setAttribute('class', 'material-icons');
    this.mIcon.setAttribute('style', 'margin-right:5px;');
    this.mIcon.innerHTML = this.mIconType;
    this.mRootDom.append(this.mIcon);
  }
  this.mTextSpan = document.createElement('span');
  this.mTextSpan.innerHTML = text;
  this.mRootDom.append(this.mTextSpan);
}

ButtonView.prototype.getId = function () {
  return this.mId;
};

ButtonView.prototype.getIconType = function () {
  return this.mIconType;
};

ButtonView.prototype.getText = function () {
  return this.mText;
};

ButtonView.prototype.setIconType = function (iconType) {
  this.mIconType = iconType;
  this.mIcon.innerHTML = this.mIconType;
};

ButtonView.prototype.setText = function (text) {
  this.mText = text;
  this.mTextSpan.innerHTML = text;
};

ButtonView.prototype.append = function (childView) {
  if (childView.getDom !== undefined) {
    this.mRootDom.append(childView.getDom());
  } else {
    this.mRootDom.append(childView);
  }
};

ButtonView.prototype.setOnClickHandler = function (onClickHandler) {
  this.mRootDom.onclick = onClickHandler;
};

ButtonView.prototype.getDom = function () {
  return this.mRootDom;
};

function CardView(
  id,
  titleText,
  widthColumns,
  contentsBgColor = 'white',
  titleBgColor = 'teal',
  titleColor = 'white'
) {
  // Check parameters
  if (widthColumns > 12 || widthColumns <= 0) {
    console.error("card's widthColumns must be 1~12: " + widthColumns);
    return undefined;
  }

  // Setting attributes
  this.mId = id;
  this.mTitleText = titleText;
  this.mWidthColumns = widthColumns;
  this.mContentsBgColor = contentsBgColor;
  this.mTitleBgColor = titleBgColor;
  this.mTitleColor = titleColor;

  // Initialize root DOM
  this.mRootDom = document.createElement('div');
  this.mRootDom.setAttribute('id', this.mId);
  this.mRootDom.setAttribute(
    'class',
    'mdl-card mdl-shadow--2dp mdl-cell ' +
      this._getColumnClass(this.mWidthColumns) +
      ' ' +
      this._getBgClass(this.mContentsBgColor)
  );

  // Initialize child views
  if (this.mTitleText !== undefined) {
    this._initTitle();
  }
  this.mListView = undefined;
}

CardView.prototype._initTitle = function () {
  this.mTitleView = document.createElement('div');
  this.mTitleView.setAttribute('id', this.mId + '--title');
  this.mTitleView.setAttribute(
    'class',
    'mdl-card__title mdl-card--expand ' + this._getBgClass(this.mTitleBgColor)
  );
  this.append(this.mTitleView);
  {
    var title_text = document.createElement('h2');
    title_text.setAttribute('class', 'mdl-card__title-text');
    title_text.setAttribute('style', 'color: ' + this.mTitleColor + ';');
    title_text.innerHTML = this.mTitleText;
    this.mTitleView.append(title_text);
  }
  this.append(this.mTitleView);
  return this.mTitleView;
};

CardView.prototype.getDom = function () {
  return this.mRootDom;
};

CardView.prototype.append = function (childView) {
  if (typeof childView === 'object' && childView.getDom !== undefined) {
    this.mRootDom.append(childView.getDom());
  } else {
    this.mRootDom.append(childView);
  }
};

CardView.prototype.enableListView = function () {
  this.mListView = document.createElement('ul');
  this.mListView.setAttribute('id', this.mId + '--list');
  this.mListView.setAttribute('class', 'demo-list-iconType mdl-list');
  this.append(this.mListView);
};

/* iconType: https://material.io/resources/icons/?style=round */
var gNextListItemId = 0;
CardView.prototype.addListItem = function (iconType, text) {
  var listItem = document.createElement('li');
  listItem.setAttribute('id', this.mId + '--list-' + gNextListItemId++);
  listItem.setAttribute('class', 'mdl-list__item');
  {
    var listItemContent = document.createElement('a');
    listItemContent.setAttribute('class', 'mdl-list__item-primary-content');
    listItemContent.setAttribute('href', '#');
    listItem.append(listItemContent);
    {
      var icon = document.createElement('i');
      icon.setAttribute('class', 'material-icons mdl-list__item-iconType');
      icon.setAttribute('style', 'margin-right:5px;');
      icon.innerHTML = iconType;
      listItemContent.append(icon);
      listItemContent.append(text);
    }
  }
  if (this.mListView !== undefined) {
    this.mListView.append(listItem);
    return listItem;
  } else {
    return undefined;
  }
};

CardView.prototype.clearListItems = function () {
  for (var i in this.mListView.children) {
    var childView = this.mListView.children[i];
    if (childView.id !== undefined && childView.id.indexOf('--list-') >= 0) {
      childView.remove();
    }
  }
};

CardView.prototype._getColumnClass = function (widthColumns) {
  return 'mdl-cell--' + widthColumns + '-col';
};

CardView.prototype._getBgClass = function (bgColor) {
  return 'mdl-color--' + bgColor;
};

function CardGrid(maxWidthPx = 1080) {
  this.mMaxWidthPx = maxWidthPx;

  this.mRootDom = document.createElement('div');
  this.mRootDom.setAttribute('class', 'mdl-grid');
  this.mRootDom.setAttribute('style', 'max-width: ' + this.mMaxWidthPx + 'px;');
}

CardGrid.prototype.getDom = function () {
  return this.mRootDom;
};

CardGrid.prototype.append = function (childView) {
  if (childView.getDom !== undefined) {
    this.mRootDom.append(childView.getDom());
  } else {
    this.mRootDom.append(childView);
  }
};
