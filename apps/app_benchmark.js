// App Benchmark
// It requires Companion, Resource API. (option1)

var ant = require('ant');
var console = require('console');

var onInitialize = function () {
  console.log('onInitialize');
};

var onStart = function () {
  var func = function () {
    var startTime = '' + new Date().valueOf();
    ant.resource.requestPost('/tester', startTime, onReceiveMessage);
    if (totalCount < 100) setTimeout(func, 2000);
  };
  setTimeout(func, 2000);
};

var totalTimeMS = 0;
var totalCount = 0;
var onReceiveMessage = function (method, targetUri, message) {
  var startTime = parseInt(message);
  var endTime = new Date().valueOf();
  var timeMS = endTime - startTime;
  totalTimeMS += timeMS;
  totalCount++;
  console.log(
    '(' +
      totalCount +
      ')' +
      'Resource API elapsed time: ' +
      timeMS +
      'ms / average: ' +
      totalTimeMS / totalCount +
      'ms'
  );
};

var on_stop = function () {
  console.log('on_stop');
};

ant.runtime.setCurrentApp(onInitialize, onStart, on_stop);
