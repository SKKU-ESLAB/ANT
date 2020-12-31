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
  this._initTitle();
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
  this.mRootDom.append(childView);
};

CardView.prototype.enableListView = function () {
  this.mListView = document.createElement('ul');
  this.mListView.setAttribute('id', this.mId + '--list');
  this.mListView.setAttribute('class', 'demo-list-icon mdl-list');
  this.append(this.mListView);
};

/* iconType: https://material.io/resources/icons/?style=round */
var gNextListItemId = 0;
CardView.prototype.addListItem = function (iconType, text) {
  var listItem = document.createElement('li');
  listItem.setAttribute('id', this.mId + '--list-' + gNextListItemId++);
  listItem.setAttribute('class', 'mdl-list__item');
  {
    var listItemContent = document.createElement('span');
    listItemContent.setAttribute('class', 'mdl-list__item-primary-content');
    listItem.append(listItemContent);
    {
      var icon = document.createElement('i');
      icon.setAttribute('class', 'material-icons mdl-list__item-icon');
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
  this.mRootDom.append(childView);
};

function DashboardView() {
  this.mRootGrid = new CardGrid();

  this.mAppListCard = new CardView('app-list-card', 'Applications', 4);
  this.mAppListCard.enableListView();
  {
    this.mAppListCard.addListItem('pause', 'image-classification.js');
    this.mAppListCard.addListItem('pause', 'face-detection.js');
    this.mAppListCard.addListItem('pause', 'segmentation.js');
  }
  this.mRootGrid.append(this.mAppListCard.getDom());

  this.mDeviceStatusCard = new CardView('device-status-card', 'Devices', 8);
  this.mDeviceStatusCard.enableListView();
  {
    this.mDeviceStatusCard.addListItem(
      'memory',
      'Device Type: Nvidia Jetson TX2'
    );
    this.mDeviceStatusCard.addListItem('analytics', 'CPU Util: 51%');
    this.mDeviceStatusCard.addListItem('analytics', 'GPU Util: 20%');
  }
  this.mRootGrid.append(this.mDeviceStatusCard.getDom());

  this.mSensorListCard = new CardView('sensor-list-card', 'Sensors', 4);
  this.mSensorListCard.enableListView();
  {
    this.mSensorListCard.addListItem('linked_camera', '/gateway/camera');
    this.mSensorListCard.addListItem('image', '/gateway/imgcls');
    this.mSensorListCard.addListItem('face', '/gateway/facedet');
    this.mSensorListCard.addListItem('image_search', '/gateway/segm');
  }
  this.mRootGrid.append(this.mSensorListCard.getDom());
}
DashboardView.prototype.getDom = function () {
  return this.mRootGrid.getDom();
};
DashboardView.prototype.append = function (childView) {
  this.mRootGrid.append(childView);
};
