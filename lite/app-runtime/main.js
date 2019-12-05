var console = require('console');
var http = require('http');
var fs = require('fs');

var ant = require('ant');

var RESULT_SUCCESS = 'Success';
var RESULT_FAILED = 'Failed';

var MAIN_LOOP_DIR_PATH = truncateFile(process.argv[1]);

/* App Main Config START */
var Config = function () {
  this.showHTTPRequestPath = true;
  this.showHTTPRequestData = false;
};
var gConfig = new Config();
/* App Main Config END */

/* App File Name START */
function AppFileName() {
  this.filename = undefined;
  this.fileindex = 0;
  var MAX_FILE_INDEX = 2;
  this.get = function () {
    return this.filename;
  };
  this.set_new = function () {
    this.fileindex = (this.fileindex + 1) % MAX_FILE_INDEX;
    this.filename = './app' + this.fileindex + '.js';
  };
  this.initialize = function () {
    var isFileExists = false;
    for (var i = 0; i < MAX_FILE_INDEX; i++) {
      this.set_new();
      if (fs.existsSync(this.get())) {
        isFileExists = true;
        break;
      }
    }
    if (!isFileExists) {
      this.filename = undefined;
    }
  };
  this.initialize();
}
var gAppFileName = new AppFileName();
/* App File Name END */

/* App Code Manager START */
function AppCodeManager() {
  this.current_app_object = undefined;
  this.load = function (appFileName) {
    this.current_app_object = require(appFileName);
  };
  this.install = function (appCodeBuffer) {
    // Write app code
    gAppFileName.set_new();
    var fd = fs.openSync(gAppFileName.get(), 'w');
    fs.writeSync(fd, appCodeBuffer, 0, appCodeBuffer.length);
    fs.closeSync(fd);

    // Load app code
    gAppCodeManager.load(gAppFileName.get());

    if (ant.runtime.getCurrentApp() !== undefined) {
      return true;
    } else {
      // IF failed, Remove app code
      this.remove(gAppFileName.get());
      return false;
    }
  };
  this.remove = function (appFileName) {
    // Remove app code
    ant.runtime._removeCurrentApp();
    if (fs.existsSync(appFileName)) {
      fs.unlinkSync(appFileName);
    }
    return true;
  };
}
var gAppCodeManager = new AppCodeManager();
/* App Code Manager END */


function truncateFile(path) {
  var tokens = path.split('/');
  var truncatedPath = '';
  for (var i = 0; i < tokens.length - 1; i++) {
    truncatedPath += tokens[i] + '/';
  }
  return truncatedPath;
}

function parseUrl(url) {
  var urlTokens = url.split('/');
  var uniqueTokens = [];
  for (i in urlTokens) {
    var token = urlTokens[i];
    if (token.length > 0) {
      uniqueTokens.push(token);
    }
  }
  return uniqueTokens;
}

function onAliveRequest(request, data) {
  var results = { message: 'Alive', code: 200 };
  return results;
}

function onAppCommand(request, data) {
  var results = { message: 'Invalid command', code: 500 };
  var command = data.toString();
  if (command == 'start') {
    results = onStartApp(request, data);
  } else if (command == 'stop') {
    results = onStopApp(request, data);
  }
  return results;
}

function onInstallApp(request, data) {
  var results = { message: RESULT_FAILED, code: 500 };

  if (ant.runtime.getCurrentApp() === undefined) {
    var appCodeBuffer = data;
    var isSuccess = gAppCodeManager.install(appCodeBuffer);
    if (isSuccess) {
      results.message = RESULT_SUCCESS;
      results.code = 200;
    }
  }
  return results;
}

function onRemoveApp(request, data) {
  var results = { message: RESULT_FAILED, code: 500 };

  if (ant.runtime.getCurrentApp() !== undefined) {
    var isSuccess = gAppCodeManager.remove(gAppFileName.get());
    if (isSuccess) {
      results.message = RESULT_SUCCESS;
      results.code = 200;
    }
  }
  return results;
}

function onStartApp(request, data) {
  var results = { message: RESULT_FAILED, code: 500 };
  var app = ant.runtime.getCurrentApp();
  if (app != undefined) {
    results.message = app.start();
    if (results.message == RESULT_SUCCESS) {
      results.code = 200;
    }
  }
  return results;
}

function onStopApp(request, data) {
  var results = { message: RESULT_FAILED, code: 500 };
  var app = ant.runtime.getCurrentApp();
  if (app != undefined) {
    results.message = app.stop();
    if (results.message == RESULT_SUCCESS) {
      results.code = 200;
    }
  }
  return results;
}

function onGetAppInfo(request, data) {
  var currentApp = ant.runtime.getCurrentApp();
  var results = { message: 'No App Found', code: 500 };

  if (currentApp != undefined) {
    var appInfo = currentApp.getInfo();
    results.message = JSON.stringify(appInfo);
    results.code = 200;
  }
  return results;
}

function onGetAppCode(request, data) {
  var results = { message: 'No App Code Found', code: 500 };

  if (fs.existsSync(gAppFileName.get())) {
    var appCode = fs.readFileSync(gAppFileName.get());
    results.message = appCode.toString();
    results.code = 200;
  }

  return results;
}

function onGetAppState(request, data) {
  var currentApp = ant.runtime.getCurrentApp();
  var results = { message: 'No App Found', code: 500 };

  if (currentApp != undefined) {
    results.message = currentApp.getState();
    results.code = 200;
  }
  return results;
}

function onGetAppEditorPage(request, data) {
  var results = { message: RESULT_FAILED, code: 500 };

  var urlTokens = parseUrl(request.url);
  urlTokens.splice(0, 1);
  var path = MAIN_LOOP_DIR_PATH + 'app-editor';
  if (urlTokens.length == 0) {
    path += '/index.html';
  } else {
    for (i in urlTokens) {
      path += '/' + urlTokens[i];
    }
  }
  // console.log(path);
  if (fs.existsSync(path)) {
    var indexHtml = fs.readFileSync(path);
    results.message = indexHtml;
    results.code = 200;
  }
  return results;
}

function onSetCompanionAddress(request, data) {
  var results = { message: RESULT_FAILED, code: 500 };
  // TODO: hardcoded port, path
  var companionHost = data.toString();
  var companionPort = 8002;
  var companionPath = '/companion';
  var result = ant.companion._setCompanionAddress(
    companionHost, companionPort, companionPath);
  if (result == true) {
    results.message = RESULT_SUCCESS;
    results.code = 200;
  }
  return results;
}

function onReceiveMessageFromCompanion(request, data) {
  var results = { message: RESULT_FAILED, code: 500 };
  if (data !== undefined && data !== null) {
    var message = data.toString();
    ant.companion._onReceiveMessageFromCompanion(message);
    results.message = RESULT_SUCCESS;
    results.code = 200;
  }
  return results;
}

function _onHTTPRequest(request, response, data) {
  response.setHeader('Content-Type', 'text/html');
  var urlTokens = parseUrl(request.url);
  var results = { message: 'Not Found Entry', code: 404 };

  // console.log(urlTokens.toString());

  if (urlTokens.length == 0) {
    // "/"
    if (request.method == 'GET') {
      // GET "/": Alive message
      results = onAliveRequest(request, data);
    }
  } else if (urlTokens[0] == 'app-editor') {
    // "/alive"
    if (request.method == 'GET') {
      // GET "/": Return app editor page
      results = onGetAppEditorPage(request, data);
    }
  } else if (urlTokens[0] == 'runtime') {
    // "/runtime*"
    if (urlTokens.length == 1) {
      // "/runtime": Not found
    } else if (urlTokens[1] == 'currentApp') {
      // "/runtime/currentApp*"
      if (urlTokens.length == 2) {
        // "/runtime/currentApp"
        if (request.method == 'GET') {
          // GET "/runtime/currentApp"
          results = onGetAppInfo(request, data);
        } else if (request.method == 'POST') {
          // POST "/runtime/currentApp"
          results = onInstallApp(request, data);
        } else if (request.method == 'DELETE') {
          // DELETE "/runtime/currentApp"
          results = onRemoveApp(request, data);
        }
      } else if (urlTokens[2] == 'command') {
        // "/runtime/currentApp/command"
        if (request.method == 'POST') {
          // POST "/runtime/currentApp/command"
          results = onAppCommand(request, data);
        }
      } else if (urlTokens[2] == 'code') {
        // "/runtime/currentApp/code"
        if (request.method == 'GET') {
          // GET "/runtime/currentApp/code"
          results = onGetAppCode(request, data);
        }
      } else if (urlTokens[2] == 'state') {
        // "/runtime/currentApp/state"
        if (request.method == 'GET') {
          // GET "/runtime/currentApp/state"
          results = onGetAppState(request, data);
        }
      } else if (urlTokens[2] == 'companionAddress') {
        // "/runtime/currentApp/companionAddress"
        if (request.method == 'POST') {
          // POST "/runtime/currentApp/companionAddress"
          results = onSetCompanionAddress(request, data);
        }
      } else if (urlTokens[2] == 'companion') {
        // "/runtime/currentApp/companion"
        if (request.method == 'POST') {
          // POST "/runtime/currentApp/companion"
          results = onReceiveMessageFromCompanion(request, data);
        }
      }
    }
  }

  response.setHeader('Content-Length', results.message.length);
  response.writeHead(results.code);
  response.write(results.message);
  response.end();
}

function onHTTPRequest(request, response) {
  if (gConfig.showHTTPRequestPath) {
    console.log('Request for path: ' + request.url);
  }

  var parts = [];
  request.on('data', function (chunk) {
    parts.push(chunk);
  });
  request.on('end', function () {
    var body = Buffer.concat(parts);
    if (gConfig.showHTTPRequestData) {
      console.log('Request data: ' + body.toString());
    }
    _onHTTPRequest(request, response, body);
  });
}

function loadExistingAppCode() {
  if (gAppFileName.get() === undefined) {
    return false;
  }

  // Load app code
  console.log('Load existing app code');
  gAppCodeManager.load(gAppFileName.get());

  if (ant.runtime.getCurrentApp() === undefined) {
    return false;
  }

  return true;
}

function test_stream_api() {
  ant.stream.initialize();
  setTimeout(function () {
    var pipeline = ant.stream.createPipeline("test");

    var source = ant.stream.createElement("videotestsrc");
    source.setProperty("pattern", 1);
    var filter = ant.stream.createElement("capsfilter");
    filter.setCapsProperty("caps", "video/x-raw,width=480,height=360,framerate=30/1,format=RGB");
    var converter = ant.stream.createElement("videoconvert");
    var omxh264enc = ant.stream.createElement("omxh264enc");
    var rtph264pay = ant.stream.createElement("rtph264pay");
    rtph264pay.setProperty("pt", 06);
    rtph264pay.setProperty("config-interval", 1);
    var gdppay = ant.stream.createElement("gdppay");
    var sink = ant.stream.createElement("tcpserversink");
    sink.setProperty("sync", false);
    sink.setProperty("host", "192.168.0.33");
    sink.setProperty("port", 5000);

    pipeline.binAdd([source, filter, converter, omxh264enc, rtph264pay, gdppay, sink]);
    pipeline.linkMany([source, filter, converter, omxh264enc, rtph264pay, gdppay, sink]);
    pipeline.setState(pipeline.STATE_PLAYING);
  }, 5000);
}

function mainLoop() {
  console.log('ANT app runtime start');

  // TODO: temporary testing code
  test_stream_api();

  var isAppExists = loadExistingAppCode();
  if (isAppExists) {
    console.log('Existing app loaded!');
  }

  var server = http.createServer();
  server.on('request', onHTTPRequest);

  var port = 8001;
  server.listen(port, function () {
    console.log('ANT core listening on port: ' + port);
  });
}

mainLoop();