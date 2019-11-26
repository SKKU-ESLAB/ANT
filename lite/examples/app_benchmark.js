var ant = require('ant');
var console = require('console');

var on_initialize = function() {
  console.log('on_initialize');
};

var on_start = function() {
  ant.companion.setOnReceiveMessage(on_receive_message);

  var func = function() {
    var startTime = '' + new Date().valueOf();
    ant.companion.sendMessage(startTime);
    if(totalCount < 100)
      setTimeout(func, 2000);
  };
  setTimeout(func, 2000);
};

var totalTimeMS = 0;
var totalCount = 0;
var on_receive_message = function(message) {
  var startTime = parseInt(message);
  var endTime = new Date().valueOf();
  var timeMS = endTime - startTime;
  totalTimeMS += timeMS;
  totalCount++;
  console.log(
      '(' + totalCount + ')' +
      'Companion API elapsed time: ' + (timeMS) +
      'ms / average: ' + (totalTimeMS / totalCount) + 'ms');
};

var on_stop = function() {
  console.log('on_stop');
};

ant.runtime.setCurrentApp(on_initialize, on_start, on_stop);