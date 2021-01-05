/* UI config area start */
var initialCode = '// Input your code here!!!';

var gANTClient = new ANTClient();

var gDashboard = new DashboardView();
var gCodeEditor = new CodeEditorView(initialCode);
var gConsole = undefined;

var gNavItems = [
  {
    id: 'navitem_dashboard',
    title: 'Dashboard',
    view: gDashboard
  },
  {
    id: 'navitem_codeeditor',
    title: 'Code Editor',
    view: gCodeEditor
  },
  {
    id: 'navitem_console',
    title: 'Console',
    view: gConsole
  }
];

/* UI config area end */

function updateForNavItem(navItem) {
  // Update header title
  var titleText = 'ANT Controlpad: ' + navItem.title;
  $('#header_title').html(titleText);

  // Update content view
  $('#content_root').html(navItem.view.getDom());
  navItem.view.onAddedDom();
}

function onClickNavItem(e) {
  var _nav_item = undefined;
  for (var j in gNavItems) {
    if (gNavItems[j].id == e.target.id) {
      _nav_item = gNavItems[j];
      break;
    }
  }
  if (_nav_item !== undefined) {
    updateForNavItem(_nav_item);
  }
}

function initializeNavItemCallbacks() {
  for (var i in gNavItems) {
    var nav_item = gNavItems[i];
    var nav_item_element = $('#' + nav_item.id);
    nav_item_element.click(onClickNavItem);
  }
}

function showInitialNavItem() {
  updateForNavItem(gNavItems[0]);
}

function onInitialize() {
  initializeNavItemCallbacks();
  showInitialNavItem();
}

$(document).ready(onInitialize);
