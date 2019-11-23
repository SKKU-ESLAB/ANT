var console = require('console');
var http = require('http');
var fs = require('fs');

var ant = require('ant');

var RESULT_SUCCESS = "Success";
var RESULT_FAILED = "Failed";

var MAIN_LOOP_DIR_PATH = truncateFile(process.argv[1]);

function AppFileName() {
  this.filename = undefined;
  this.fileindex = 0;
  this.get = function () {
    return this.filename;
  }
  this.set_new = function () {
    this.fileindex = (this.fileindex + 1) % 2;
    this.filename = "./app" + this.fileindex + ".js";
  }
}
var appFileName = new AppFileName();

function truncateFile(path) {
  var tokens = path.split("/");
  var truncatedPath = "";
  for (var i = 0; i < tokens.length - 1; i++) {
    truncatedPath += tokens[i] + "/";
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
  var results = {
    message: "Alive",
    code: 200
  };
  return results;
}

function onAppCommand(request, data) {
  var results = {
    message: "Invalid command",
    code: 500
  };
  var command = data.toString();
  if (command == "start") {
    results = onStartApp(request, data);
  } else if (command == "stop") {
    results = onStopApp(request, data);
  }
  return results;
}

var current_app_object = undefined;
function onInstallApp(request, data) {
  var results = {
    message: RESULT_FAILED,
    code: 500
  };

  if (ant.runtime.getCurrentApp() === undefined) {
    // Write app code
    appFileName.set_new();
    var fd = fs.openSync(appFileName.get(), 'w');
    var appCode = data;
    fs.writeSync(fd, appCode, 0, appCode.length);
    fs.closeSync(fd);

    // Execute app code with initializaiton function
    current_app_object = require(appFileName.get());

    if (ant.runtime.getCurrentApp() !== undefined) {
      results.message = RESULT_SUCCESS;
      results.code = 200;
    }
  }
  return results;
}

function onRemoveApp(request, data) {
  var results = {
    message: RESULT_FAILED,
    code: 500
  };

  if (ant.runtime.getCurrentApp() !== undefined) {
    ant.runtime._removeCurrentApp();
    if (fs.existsSync(appFileName.get())) {
      fs.unlinkSync(appFileName.get());
    }
    results.message = RESULT_SUCCESS;
    results.code = 200;
  }
  return results;
}

function onStartApp(request, data) {
  var results = {
    message: RESULT_FAILED,
    code: 500
  };
  var app = ant.runtime.getCurrentApp();
  if (app != undefined) {
    results.message = app.start();
    if (results.message == RESULT_SUCCESS) {
      results.code = 200;
    }
  }
  return results;
}

function onGetAppInfo(request, data) {
  var currentApp = ant.runtime.getCurrentApp();
  var results = {
    message: "No App Found",
    code: 500
  };

  if (currentApp != undefined) {
    var appInfo = currentApp.getInfo();
    results.message = JSON.stringify(appInfo);
    results.code = 200;
  }
  return results;
}

function onGetAppCode(request, data) {
  var results = {
    message: "No App Code Found",
    code: 500
  };

  if (fs.existsSync(appFileName.get())) {
    var appCode = fs.readFileSync(appFileName.get());
    results.message = appCode.toString();
    results.code = 200;
  }

  return results;
}

function onStopApp(request, data) {
  var results = {
    message: RESULT_FAILED,
    code: 500
  };
  var app = ant.runtime.getCurrentApp();
  if (app != undefined) {
    results.message = app.stop();
    if (results.message == RESULT_SUCCESS) {
      results.code = 200;
    }
  }
  return results;
}

function onGetAppEditorPage(request, data) {
  var results = {
    message: RESULT_FAILED,
    code: 500
  };

  var urlTokens = parseUrl(request.url);
  urlTokens.splice(0, 1);
  var path = MAIN_LOOP_DIR_PATH + "app-editor";
  if (urlTokens.length == 0) {
    path += "/index.html";
  } else {
    for (i in urlTokens) {
      path += "/" + urlTokens[i];
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

function _onHTTPRequest(request, response, data) {
  response.setHeader('Content-Type', 'text/html');
  var urlTokens = parseUrl(request.url);
  var results = {
    message: "Not Found Entry",
    code: 404
  };

  // console.log(urlTokens.toString());

  if (urlTokens.length == 0) {
    // "/"
    if (request.method == "GET") {
      // GET "/": Alive message
      results = onAliveRequest(request, data);
    }
  } else if (urlTokens[0] == "app-editor") {
    // "/alive"
    if (request.method == "GET") {
      // GET "/": Return app editor page
      results = onGetAppEditorPage(request, data);
    }
  } else if (urlTokens[0] == "runtime") {
    // "/runtime*"
    if (urlTokens.length == 1) {
      // "/runtime": Not found
    } else if (urlTokens[1] == "currentApp") {
      // "/runtime/currentApp*"
      if (urlTokens.length == 2) {
        // "/runtime/currentApp"
        if (request.method == "GET") {
          // GET "/runtime/currentApp"
          results = onGetAppInfo(request, data);
        } else if (request.method == "POST") {
          // POST "/runtime/currentApp"
          results = onInstallApp(request, data);
        } else if (request.method == "DELETE") {
          // DELETE "/runtime/currentApp"
          results = onRemoveApp(request, data);
        }
      } else if (urlTokens[2] == "command") {
        // "/runtime/currentApp/command"
        if (request.method == "POST") {
          // POST "/runtime/currentApp/command"
          results = onAppCommand(request, data);
        }
      } else if (urlTokens[2] == "code") {
        // "/runtime/currentApp/code"
        if (request.method == "GET") {
          // GET "/runtime/currentApp/code"
          results = onGetAppCode(request, data);
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
  console.log('Request for path: ' + request.url);

  var parts = [];
  request.on('data', function (chunk) {
    parts.push(chunk);
  });
  request.on('end', function () {
    var body = Buffer.concat(parts);
    console.log('Request data: ' + body.toString());
    _onHTTPRequest(request, response, body);
  });
}

function mainLoop() {
  console.log('main loop start');

  var server = http.createServer();
  server.on('request', onHTTPRequest);

  var port = 8001;
  server.listen(port, function () {
    console.log("ANT core listening on port: " + port);
  });
}

mainLoop();