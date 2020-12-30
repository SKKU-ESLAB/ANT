var gContentViews = {};

function getColumnClass(widthColumns) {
  return 'mdl-cell--' + widthColumns + '-col';
}

function getBgClass(bgColor) {
  return 'mdl-color--' + bgColor;
}

function makeCardView(
  titleText,
  widthColumns,
  contentsBgColor = 'white',
  titleBgColor = 'teal',
  titleColor = 'white'
) {
  if (widthColumns > 12 || widthColumns <= 0) {
    console.error("card's widthColumns must be 1~12: " + widthColumns);
    return undefined;
  }

  var card = document.createElement('div');
  card.setAttribute(
    'class',
    'mdl-card mdl-shadow--2dp mdl-cell ' +
      getColumnClass(widthColumns) +
      ' ' +
      getBgClass(contentsBgColor)
  );
  {
    var title = document.createElement('div');
    title.setAttribute(
      'class',
      'mdl-card__title mdl-card--expand ' + getBgClass(titleBgColor)
    );
    card.append(title);
    {
      var title_text = document.createElement('h2');
      title_text.setAttribute('class', 'mdl-card__title-text');
      title_text.setAttribute('style', 'color: ' + titleColor + ';');
      title_text.innerHTML = titleText;
      title.append(title_text);
    }
  }

  return card;
}

/* iconType: https://material.io/resources/icons/?style=round */
function addListItem(list, iconType, text) {
  var listItem;
  listItem = document.createElement('li');
  listItem.setAttribute('class', 'mdl-list__item');
  list.append(listItem);
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
  return listItem;
}

function makeListView() {
  var list = document.createElement('ul');
  list.setAttribute('class', 'demo-list-icon mdl-list');
  return list;
}

function getDashboardContentView() {
  if (gContentViews.dashboard === undefined) {
    var dashboard_root = document.createElement('div');
    dashboard_root.setAttribute('class', 'mdl-grid');
    dashboard_root.setAttribute('style', 'max-width: 1080px;');

    // Applications (App list)
    var app_list_card = makeCardView('Applications', 4);
    dashboard_root.append(app_list_card);
    {
      var list = makeListView();
      app_list_card.append(list);

      addListItem(list, 'description', 'image-classification.js');
      addListItem(list, 'description', 'face-detection.js');
      addListItem(list, 'description', 'segmentation.js');
    }

    // Device (Device status)
    var device_status_card = makeCardView('Device', 8);
    dashboard_root.append(device_status_card);
    {
      var list = makeListView();
      device_status_card.append(list);

      addListItem(list, 'memory', 'Device Type: Nvidia Jetson TX2');
      addListItem(list, 'analytics', 'CPU Util: 51%');
      addListItem(list, 'analytics', 'GPU Util: 20%');
    }

    // Sensors (Sensor list)
    var sensor_list_card = makeCardView('Sensors', 4);
    dashboard_root.append(sensor_list_card);
    {
      var list = makeListView();
      sensor_list_card.append(list);

      addListItem(list, 'linked_camera', '/gateway/camera');
      addListItem(list, 'image', '/gateway/imgcls');
      addListItem(list, 'face', '/gateway/facedet');
      addListItem(list, 'image_search', '/gateway/segm');
    }

    gContentViews.dashboard = dashboard_root;
  }
  return gContentViews.dashboard;
}

function getCodeEditorContentView() {
  if (gContentViews.codeEditor === undefined) {
    var code_editor = document.createElement('div');
    code_editor.setAttribute('id', 'code_editor');
    code_editor.setAttribute(
      'style',
      'width: 800px; height: 600px; border: 1px solid grey'
    );

    require.config({paths: {vs: './vs'}});
    require(['vs/editor/editor.main'], function () {
      var editor = monaco.editor.create(
        document.getElementById('code_editor'),
        {
          value: ['// Input your code here!!!'].join('\n'),
          language: 'javascript'
        }
      );
    });
    gContentViews.codeEditor = code_editor;
  }
  return gContentViews.codeEditor;
}

var nav_items = [
  {
    id: 'navitem_dashboard',
    title: 'Dashboard',
    getContentView: getDashboardContentView
  },
  {
    id: 'navitem_codeeditor',
    title: 'Code Editor',
    getContentView: getCodeEditorContentView
  }
];

function updateHeaderTitle(title) {
  var titleText = 'ANT Controlpad: ' + title;
  $('#header_title').html(titleText);
}
function updateContentView(contentView) {
  $('#content_root').html(contentView);
}

function initializeNavItemCallbacks() {
  for (var i in nav_items) {
    var nav_item = nav_items[i];
    var nav_item_element = $('#' + nav_item.id);
    nav_item_element.click(function (e) {
      var _nav_item = undefined;
      for (var j in nav_items) {
        if (nav_items[j].id == e.target.id) {
          _nav_item = nav_items[j];
          break;
        }
      }
      if (_nav_item !== undefined) {
        updateHeaderTitle(_nav_item.title);
        updateContentView(_nav_item.getContentView());
      }
    });
  }
}

function showInitialNavItem() {
  var nav_item = nav_items[0];
  updateHeaderTitle(nav_item.title);
  updateContentView(nav_item.getContentView());
}

function onInitialize() {
  initializeNavItemCallbacks();
  showInitialNavItem();
}

$(document).ready(onInitialize);
