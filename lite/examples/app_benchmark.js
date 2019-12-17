// App Benchmark
// It requires Companion, Resource API. (option1)

var ant = require('ant');
var console = require('console');

var on_initialize = function () {
  console.log('on_initialize');
};

var on_start = function () {


  var func = function () {
    var startTime = '' + new Date().valueOf();
    ant.resource.requestPost("/tester", startTime, on_receive_message);
    if (totalCount < 100)
      setTimeout(func, 2000);
  };
  setTimeout(func, 2000);
};

var totalTimeMS = 0;
var totalCount = 0;
var on_receive_message = function (method, targetUri, message) {
  var startTime = parseInt(message);
  var endTime = new Date().valueOf();
  var timeMS = endTime - startTime;
  totalTimeMS += timeMS;
  totalCount++;
  console.log(
    '(' + totalCount + ')' +
    'Resource API elapsed time: ' + (timeMS) +
    'ms / average: ' + (totalTimeMS / totalCount) + 'ms');
};

var on_stop = function () {
  console.log('on_stop');
};

ant.runtime.setCurrentApp(on_initialize, on_start, on_stop);